#include <cstdlib>
#include <iostream>
#include <string>
#include <Windows.h>
#include <PI_GCS2_DLL.h>

using namespace std;

class PI_Stage{
public:
  PI_Stage(double* step, const char* id = "/dev/ttyUSB0");
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
  double* step_length;
  char model="E-709";
};

PI_Stage::PI_Stage(double* step, const char* id = "/dev/ttyUSB0"):dev_id (id), step_length (step){}

void PI_Stage::piezo_initializer(){
  PI_TryConnectUSB(&model);
}

int PI_Stage::move_onestep(){
  PI_MVR(controller_id, axes_id, step_length);
  ont_flag=PI_qONT(controller_id, axes_id, &ont_state);
  if (ont_flag&&ont_state){
    return 0;
  }
  else{
    cout<<"error_code"<<(ont_flag&&ont_state)<<"Fail to move the stage!!!"<<endl;
    return -1;
  }
}

void PI_Stage::exit(){
  PI_CloseConnection (controller_id);
  return;
}

int main(){

  char model = "E-709";
  PI_TryConnectUSB(&model);
}