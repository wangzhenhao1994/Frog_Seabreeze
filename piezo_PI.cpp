#include <cstdlib>
#include <iostream>
#include <string>
#include <PI_GCS2_DLL.h>

using namespace std;

Class PI_Stage(){
public:
  char* sz_buffer;
  const char* sz_description;
  const char* dev_id = "/dev/ttyUSB0";
  int controller_id=-1;
  const char* axes_id="1";
  BOOL* servo_mode=TRUE;
  int connection_flag=0;
  int ont_flag=-1;
  BOOL* ont_state=-1;
  double min_range;
  double max_range;
  void piezo_initializer(){
    controller_id=PI_ConnectRS232ByDevName(dev_id, 115200);
    connection_flag=PI_IsConnected(controller_id);
    PI_SVO(controller_id, axes_id, servo_mode);
    PI_qTMN(controller_id, axes_id, &min_range);
    PI_qTMX(controller_id, axes_id, &max_range);
    PI_MOV(controller_id, axes_id, &min_range);
    ont_flag=PI_qONT(controller_id, axes_id, &ont_state);

    if (ont_flag&&ont_state):{
      return;
    }
    else:{
      cout<<"Fail to initialize the stage!!!"<<endl;
      return;
    }
  }
  int move_onestep(int ID, const char* szAxes, double* step_length){
    PI_qMVR(ID, szAxes, step_length);
    ont_flag=PI_qONT(controller_id, axes_id, &ont_state);
    if (ont_flag&&ont_state):{
      return 0;
    }
    else:{
      cout<<"Fail to move the stage!!!"<<endl;
      return -1;
  }

  void exit(int ID){
    PI_CloseConnection (controller_id);
    return;
  }
}
int main(){
  PI_Stage* stage;
  stage->piezo_initializer();
  return 0;
}
}
