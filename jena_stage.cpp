#include <cstdlib>
#include <iostream>
// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
 
void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}
serial::Serial my_serial("/dev/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));
class Stage{
public:
  Stage(serial::Serial *my_serial, double step, double center, double start_point);
  void piezo_initializer();
  int move_onestep();
  void exit();

private:
  serial::Serial* my_serial;
  char* sz_buffer;
  const char* sz_description;
  int controller_id=-1;
  const char* axes_id="1";
  const double* pdLowvoltageArray;
  bool servo_mode=true;
  bool atz_flag=false;
  int connection_flag=-1;
  int servo_flag=false;
  bool ont_flag=false;
  bool ont_state=false;
  double min_range;
  double max_range;
  double step_length;
  double trace_center;
  double start_point;
};

Stage::Stage(double step, double center, double start_point):my_serial(my_serial),step_length (step), trace_center(center), start_point(start_point){}

void Stage::piezo_initializer(){
  controller_id=PI_ConnectRS232ByDevName(dev_id, 115200);
  connection_flag=PI_IsConnected(controller_id);
  if (connection_flag){
    cout<<"Connect Successfully!"<<endl;
  }else{
    cout<<"error_code: "<<connection_flag<<"->:Something is wrong when trying to connect!"<<endl;
  }

  servo_flag=PI_SVO(controller_id, axes_id, &servo_mode);
  if (servo_flag){
    cout<<"Stage is in Closed-Loop mode!"<<endl;
  }else{
    cout<<"Something is wrong when trying to set closed-loop!"<<endl;
  }

  atz_flag=PI_ATZ(controller_id, axes_id, pdLowvoltageArray, &servo_mode);
  if (atz_flag){
    cout<<"Auto zero Successfully!"<<endl;
  }else{
    cout<<"Something is wrong when trying to autozero!"<<endl;
  }

  //PI_qTMN(controller_id, axes_id, &min_range);
  //PI_qTMX(controller_id, axes_id, &max_range);
  PI_MOV(controller_id, axes_id, &start_point);
  //if (!(ont_flag&&ont_state)){
  while (!(ont_flag&&ont_state)){
    ont_flag=PI_qONT(controller_id, axes_id, &ont_state);
  }
  if (ont_flag&&ont_state){
    cout<<"Successfully initialize the stage!!!"<<endl;
    return;
  }
  else{
    cout<<"Fail to initialize the stage!!!"<<endl;
    return;
  }
}

int Stage::move_onestep(){
  PI_MVR(controller_id, axes_id, &step_length);
  ont_flag=PI_qONT(controller_id, axes_id, &ont_state);
  // while (!(ont_flag&&ont_state)){
  //   ont_flag=PI_qONT(controller_id, axes_id, &ont_state);
  // }
  // if (ont_flag&&ont_state){
  //   return 0;
  // }
  // else{
  //   cout<<"error_code"<<(ont_flag&&ont_state)<<"Fail to move the stage!!!"<<endl;
  //   return -1;
  // }
  return 0;
}

void Stage::exit(){
  PI_MOV(controller_id, axes_id, &trace_center);
  PI_CloseConnection (controller_id);
  return;
}

extern Stage stage;
