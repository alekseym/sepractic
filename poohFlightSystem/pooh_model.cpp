#include <iostream>
#include <thread>
#include <cmath>
// #include "pid.h"

using namespace std;


class PID
{
    public:
        PID( double dt, double max, double min, double Kp, double Kd, double Ki );
        ~PID();
        double calculate( double setpoint, double pv );

    private:
        double _dt;
        double _max;
        double _min;
        double _Kp;
        double _Kd;
        double _Ki;
        double _pre_error;
        double _integral;
};


PID::PID( double dt, double max, double min, double Kp, double Kd, double Ki ) :
    _dt(dt),
    _max(max),
    _min(min),
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0)
{
}

double PID::calculate( double setpoint, double pv ) {
    
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * _dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / _dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > _max )
        output = _max;
    else if( output < _min )
        output = _min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

PID::~PID()
{
}


enum ECmd {
   ENone,
   EExit
};

int counter = 0;

ECmd getCmd (){
    counter ++;
    if (counter>=100){
        return EExit; 
    } 
    else
        return ENone;
}

float getHZadFromUser () {
    return 10.;
}

int main() {
  PID pid (0.1, 0, 100, 0.1, 0.01, 0.5);

  double t = 0;                   // Modeling time
  double enginePower =0;          // n    0..500 n
  double enginePowerPercent = 0;  // 0..100
  double deltaEnginePowerPercent = 0;
  double v = 0;          // m/s
  double dt = 0.1;       // s
  double a = 0;          //  m/s2
  double m  = 10;        // kg
  double h = 0;          // Current height
  double dh =0 ;         // Height change

  float hZad =0 ;       // Height setpoint

  ECmd cmd = ENone;

  
  while(cmd != EExit) {
    cmd = getCmd (); 

    enginePower = enginePowerPercent * (500./100.);

    h = h + dh;
    dh = v * dt;
    v = v + a * dt;
    a = (enginePower - 9.8*m)/m;

    hZad = getHZadFromUser ();                // Get height setpoint from user
    // m    = m + eat();

    deltaEnginePowerPercent = pid.calculate(hZad, h);
    enginePowerPercent = enginePowerPercent + deltaEnginePowerPercent;

    t = t + dt;
    
    std::cout << "h= "<< h << "power = "<< enginePower <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
  }

  return 0;
}