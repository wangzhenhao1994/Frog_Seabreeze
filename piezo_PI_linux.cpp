#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <PI_GCS2_DLL.h>

using namespace std;

class Stage{
public:
  Stage(double step, double center, double start_point, const char* id = "/dev/ttyUSB0");
  void piezo_initializer();
  int move_onestep();
  void exit();

private:
  char* sz_buffer;
  const char* sz_description;
  const char* dev_id;
  int controller_id=-1;
  const char* axes_id="1";
  const double* pdLowvoltageArray;
  BOOL servo_mode=TRUE;
  BOOL atz_flag=FALSE;
  int connection_flag=-1;
  int servo_flag=FALSE;
  BOOL ont_flag=FALSE;
  BOOL ont_state=FALSE;
  double min_range;
  double max_range;
  double step_length;
  double trace_center;
  double start_point;
};

Stage::PI_Stage(double step, double center, double start_point, const char* id = "/dev/ttyUSB0"):dev_id (id), step_length (step), trace_center(center), start_point(start_point){}

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

extern PI_Stage stage;
