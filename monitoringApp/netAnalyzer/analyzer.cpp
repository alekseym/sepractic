#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <iphlpapi.h>


using namespace std;

void WriteErrorAndExit(DWORD err) {
    std::cout << "Error" << std::endl;
    exit(0);
}

// https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getiftable
// https://docs.microsoft.com/en-us/windows/win32/api/ifmib/ns-ifmib-mib_ifrow

struct NetInterfaceInfo {
    string _name;
    unsigned int _sendSize;
    unsigned int _recvSize;

    NetInterfaceInfo(string name, unsigned int sendSize, unsigned int recvSize):_name(name), _sendSize(sendSize), 
         _recvSize(recvSize){}
};


class TraficInfoCollector {
    MIB_IFTABLE* pIfTable;
    //PMIB_IFTABLE *pIfTable;
    MIB_IFROW ifRow;
    PMIB_IFROW pIfRow = &ifRow;
    DWORD dwSize = sizeof(MIB_IFTABLE);
public:
    TraficInfoCollector() {
        pIfTable = (MIB_IFTABLE*)malloc(dwSize);

        memset(&ifRow, sizeof(ifRow), 0);

        // first call returns the buffer size needed
        DWORD retv = GetIfTable(pIfTable, &dwSize, true);
        if (retv != ERROR_INSUFFICIENT_BUFFER)
            WriteErrorAndExit(retv);

        free(pIfTable);
        pIfTable = (MIB_IFTABLE*)malloc(dwSize);
    }
    ~TraficInfoCollector() {
        free(pIfTable);
    }


    vector<NetInterfaceInfo> getStatisticForAllAdapters() {
        vector<NetInterfaceInfo> statisticForAllAdapters;
        DWORD retv;

        memset(pIfTable, dwSize, 0);

        retv = GetIfTable(pIfTable, &dwSize, true);
        if (retv != NO_ERROR)
            WriteErrorAndExit(retv);
        // Get index
        int i, j;

        for (i = 0; i < (int)pIfTable->dwNumEntries; i++)
        {
            pIfRow = (MIB_IFROW*)&pIfTable->table[i];
            if ((pIfRow->dwInOctets == 0) && (pIfRow->dwOutOctets == 0))
                continue;
            //        printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);

            // your new String
            string strname;
            for (j = 0; j < (int)pIfRow->dwDescrLen; j++) {
                if(pIfRow->bDescr[j])
                    strname.push_back(pIfRow->bDescr[j]);
            }

            // NetInterfaceInfo(string name, unsigned int sendSize, unsigned int recvSize)
            NetInterfaceInfo interfaceStatistic (strname, pIfRow->dwOutOctets, pIfRow->dwInOctets);

            statisticForAllAdapters.push_back(interfaceStatistic);
        }
        return statisticForAllAdapters;
    }

    void showStatistic(vector<NetInterfaceInfo>& statisticForAllAdapters) {
        for (NetInterfaceInfo& val : statisticForAllAdapters) {
            showStatistic(val);
        }
    }

    void showStatistic(NetInterfaceInfo& statisticForAdapter) {
        std::cout << "name " << statisticForAdapter._name << " " << "send " << statisticForAdapter._sendSize << " "
           << "recv " << statisticForAdapter._recvSize << std::endl;
    }

    NetInterfaceInfo getStatisticForOneAdapter(std::string adapterName, vector<NetInterfaceInfo>& statisticForAllAdapters) {
        std::vector<NetInterfaceInfo>::iterator it;

        it = std::find_if(statisticForAllAdapters.begin(), statisticForAllAdapters.end(), 
              [adapterName](NetInterfaceInfo val) {
                return val._name == adapterName; 
            });

        if (it != statisticForAllAdapters.end())
            return *it;
        else
            return NetInterfaceInfo("empty", 0, 0);
    }
};



NetInterfaceInfo calculateTraficConsumption(const NetInterfaceInfo &first, const NetInterfaceInfo& second) {
    NetInterfaceInfo rezult(first);

    rezult._recvSize = second._recvSize - first._recvSize;
    rezult._sendSize = second._sendSize - first._sendSize;

    return rezult;
}






vector<size_t> inputTrafic;
vector<size_t> outputTrafic;


vector<size_t> inputTraficConsumption;
vector<size_t> outputTraficConsumption;

void calculateTraficConsumption (vector<size_t> &traficStatistic, vector<size_t> &traficConsumption) {
  if (traficStatistic.size()==0)
    return;
  size_t prev = traficStatistic[0];
  for(size_t i=1;i<traficStatistic.size();i++) {
    traficConsumption.push_back(traficStatistic[i]-prev);
    prev = traficStatistic[i]; 
  } 
}

void addStatisticItem (string line, vector<size_t> &toutputTrafic, vector<size_t> &tinputTrafic) {
  size_t invalue, outvalue;

  std::istringstream strStream(line);
  strStream >> invalue >> outvalue;

  tinputTrafic.push_back(invalue);
  toutputTrafic.push_back(outvalue);
}

size_t calulateAverage(vector<int> &traficData) {
  size_t sum = 0;
  if(traficData.size()==0)
    return 0;
  for(size_t i=0;i<traficData.size();i++) {
    sum=sum + traficData[i];
  }
  return sum/traficData.size();
}

class StatisticWriter {
    std::ofstream outfile;
public:
    StatisticWriter() {

        std::time_t t = std::time(nullptr);
        std::string datetime(100, 0);
        datetime.resize(std::strftime(&datetime[0], datetime.size(),
            "%a-%d0%b-%Y_%I-%M-%S%p", std::localtime(&t)));

        std::string fname = "stat_" + datetime + ".txt";
        outfile.open(fname,ios::out);    
    }

    ~StatisticWriter() {
        outfile.close();
    }

    void write(NetInterfaceInfo& val) {
        std::time_t t = std::time(nullptr);
        std::string datetime(100, 0);
        datetime.resize(std::strftime(&datetime[0], datetime.size(),
             "%a-%b-%Y-%M-%S%p", std::localtime(&t)));
        outfile << datetime << " ,"<< val._sendSize<<","<< val._recvSize <<std::endl;
    }
};

std::string getAdapterName(int argc,char**argv) {
  std::string adapterName;
  for(int i=1;i<argc;i++) {
      int offset = 0;
      std::cout << argv[i] << std::endl;

      for(int j=offset;j<std::strlen(argv[i]);j++) {
//        std::cout <<std::endl<< argv[i][j];
        if(argv[i][j]=='"') {
          return adapterName;
        }
        adapterName.push_back(argv[i][j]);
      }
  }   
  
  std::cout << "Try to find name "<< adapterName << std::endl;

  return adapterName;
}

void showInvitation () {
  std::cout << "usage: gameUsageAnalizaer.exe [showall] [\"adapter name\"]" << std::endl;

}


int main(int argc,char**argv){
   fstream newfile;
   std::string adapterName = "Qualcomm Atheros QCA9377 Wireless Network Adapter";
   TraficInfoCollector collector;

   int collectIntervalInSecond = 10;
   int numberOfIntervalsInFile = 30;  // 5 min


   if (argc > 1) {
       std::string adapterName = getAdapterName(argc,argv);
       vector<NetInterfaceInfo> statistic = collector.getStatisticForAllAdapters();
       NetInterfaceInfo oneStatistic = collector.getStatisticForOneAdapter(adapterName, statistic);

       if (adapterName == "showall") {                           // Get list of all adapter
           collector.showStatistic(statistic);
           return 0;
       }

       if (oneStatistic._name == "empty") {
           std::cout << "Adapter name not found" << std::endl;
           return 0;
       }
   }
   else {
     showInvitation ();
   }

   vector<NetInterfaceInfo> statistic = collector.getStatisticForAllAdapters();
   //collector.showStatistic(statistic);

   NetInterfaceInfo oneStatistic = collector.getStatisticForOneAdapter(adapterName, statistic);
   NetInterfaceInfo previosOneStatistic(oneStatistic);
   collector.showStatistic(oneStatistic);


   while (1) {
       int i = 0;
       StatisticWriter writer;
       while(i< numberOfIntervalsInFile) // 5 минут == 30 
       {
           std::this_thread::sleep_for(std::chrono::seconds(collectIntervalInSecond));

           vector<NetInterfaceInfo> statistic = collector.getStatisticForAllAdapters();

           NetInterfaceInfo oneStatistic = collector.getStatisticForOneAdapter(adapterName, statistic);

           NetInterfaceInfo consumeStatistic = calculateTraficConsumption(previosOneStatistic, oneStatistic);
           writer.write(consumeStatistic);
           previosOneStatistic = oneStatistic;

           i++;
       }
   }

}