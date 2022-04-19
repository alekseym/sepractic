
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


std::vector<int> dotaSend {
72179,74875,81695,63434,81637,80929,61801,72019,60275,70537,
54500,60572,75338,76644,52484,72773,74882,89956,48197,70918,
82748,81682,54934,59504,63993,70056,54891,85802,84880,87771
};


std::vector<int> zoomSend {
120583,104209,160822,197686,174649,163307,122332,148945,176874,161047
,98038,87391,163888,119147,184423,100544,150125,142389,203395
,256534,204126,213953,172011,155783,193952,107647,132439,134220
,161393,160147
};

int calcAverage(std::vector<int> arr) {
  int sum = 0;    

  for(int i=0;i<arr.size();i++) {
    sum=sum+arr[i];
  }
  if(arr.size()==0)
    return 0;
  else
    return sum/arr.size();  
}

float calcStandardDeviation(std::vector<int> arr) {
  return 0;
}

int calcMedian(std::vector<int> arr) {
  return 0;
}

int calcMax(std::vector<int> arr) {
  return 0;
}

int calcMin(std::vector<int> arr) {
  return 0;
}

void testCalcAverage(void) {
  std::vector<int> dotaSend {2,3,2,1};

  if (calcAverage(zoomSend)!=2)
  {
    std::cout <<" testCalcAverage FAIL" << std::endl;
    return;
  }

  std::vector<int> emptySend;
  if (calcAverage(zoomSend)!=0)
  {
    std::cout <<" testCalcAverage FAIL" << std::endl;
    return;
  }

  std::cout <<" testCalcAverage PASS" << std::endl;
}

void testCalcStandardDeviation () {
}

void testCalcMedian(void) {
} 

void testCalcMax(void) {
}
void testCalcMin(void) {
}

#define RUNTEST 1

int main() {
  int dotaSendAvrg = 0;
  int zoomSendAvrg = 0;
  dotaSendAvrg = calcAverage(dotaSend);
  zoomSendAvrg = calcAverage(zoomSend);

  std::cout <<"dota " << dotaSendAvrg << std::endl;
  std::cout <<"zoom " << zoomSendAvrg << std::endl;

#if RUNTEST
  testCalcAverage();
  testCalcStandardDeviation ();
  testCalcMedian(); 
  testCalcMax();
  testCalcMin();
#endif
}