#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


vector<int> inputTrafic;
vector<int> outputTrafic;


vector<int> inputTraficConsumption;
vector<int> outputTraficConsumption;

void calculateTraficConsumption (vector<int> &traficStatistic, vector<int> &traficConsumption) {
  if (traficStatistic.size()==0)
    return;
  int prev = traficStatistic[0];
  for(int i=1;i<traficStatistic.size();i++) {
    traficConsumption.push_back(traficStatistic[i]-prev);
    prev = traficStatistic[i]; 
  } 
}

void addStatisticItem (string line, vector<int> &toutputTrafic, vector<int> &tinputTrafic) {
  int invalue, outvalue;

  std::istringstream strStream(line);
  strStream >> invalue >> outvalue;

  tinputTrafic.push_back(invalue);
  toutputTrafic.push_back(outvalue);
}

int calulateAverage(vector<int> &traficData) {
  int sum = 0;
  if(traficData.size()==0)
    return 0;
  for(int i=0;i<traficData.size();i++) {
    sum=sum + traficData[i];
  }
  return sum/traficData.size();
}


int main(int argc,char**argv){
   fstream newfile;

   if(argc<2) {
     cout << "analyzer filename"<< endl;
     return 0;
   }
   newfile.open(argv[1],ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
      string tp;
      while(getline(newfile, tp)){ //read data from file object and put it into string.
         cout << tp << "\n"; //print the data of the string
         addStatisticItem (tp,outputTrafic,inputTrafic);
      }
      newfile.close(); //close the file object.
   }

   calculateTraficConsumption(inputTrafic, inputTraficConsumption);
   calculateTraficConsumption(outputTrafic, outputTraficConsumption);

   cout << "average "<< calulateAverage(outputTraficConsumption) << " " <<  calulateAverage(inputTraficConsumption) << std::endl;
}