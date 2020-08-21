#include <cstdlib>
#include <iostream>
#include <string>
#include <PI_GCS2_DLL.h>

using namespace std;


int piezo_PI(){
  char* sz_buffer;
  const char* sz_description;
  const char* dev_id = "/dev/ttyUSB0";
  int controller_id=-1;
  const char* axes_id="1";
  int connection_flag=0;
  double min_range;
  double max_range;

  controller_id=PI_ConnectRS232ByDevName(dev_id, 115200);
  connection_flag=PI_IsConnected(controller_id);

  const double pos=15.0;
  PI_MOV(controller_id, axes_id, &pos);
  PI_qTMN(controller_id, axes_id, &min_range);
  PI_qTMX(controller_id, axes_id, &max_range);
  qONT qMVR
  cout<<min_range<<"!!!"<<max_range<<endl;
  PI_CloseConnection (controller_id);

  return 0;

}
