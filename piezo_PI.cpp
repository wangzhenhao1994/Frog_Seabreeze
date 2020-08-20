#include <PI_GCS2_DLL.h>
#include <cstdlib>
#include <iostream>
#include <string>

char* sz_buffer;
const char* sz_description;
int i_buffersize=1;
const char* sz_filter="E-709";
int enumerate_flag;
int connection_flag;


int piezo_PI(){
  sz_buffer=(char *)calloc(i_buffersize, sizeof(char));
  enumerate_flag=PI_EnumerateUSB(sz_buffer, i_buffersize, sz_filter);
  sz_description=&sz_buffer[0];
  connection_flag = PI_ConnectUSB(sz_description);
  cout<<enumerate_flag<<"!!!"<<connection_flag<<"!!!"<<+sz_buffer[0]<<"!!!"<<endl;

  return 0;
}
