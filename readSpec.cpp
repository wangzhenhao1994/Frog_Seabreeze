/* Includes */
#include <iostream>
#include <cstdlib>
#include <string>
#include "api/seabreezeapi/SeaBreezeAPI.h"

#ifndef _WINDOWS
#include <unistd.h>
#else
#include <Windows.h>
#endif

using namespace std;

int readSpec() {
  SeaBreezeAPI::shutdown();
  SeaBreezeAPI* API = SeaBreezeAPI::getInstance();

  unsigned long number_of_devices;
  long *device_ids;
  long device_id;
  int *error_code=0;
  char *nameBuffer;
  int flag;
  long *buffer;
  long *buffer_s;
  long length;

  API->probeDevices();
  number_of_devices = API->getNumberOfDeviceIDs();
  cout<<number_of_devices<<endl;
  device_ids = (long *)calloc(number_of_devices, sizeof(long));
  number_of_devices=API->getDeviceIDs(device_ids, number_of_devices);

  nameBuffer=(char *)calloc(80, sizeof(char));
  flag=API->getDeviceType(device_id, error_code, nameBuffer, 79);
  if(flag > 0) {
      cout<<"The device type is: "<<nameBuffer<<endl;
  }
  int status=API->openDevice(device_id, error_code);

  if (status) {
    cout<<"Can't open this spectrometer!"<<endl;
  }

  length=API->getNumberOfRawUSBBusAccessFeatures(device_id, error_code);
  cout<<"Length is: "<<length<<endl;
  buffer_s=(long *)calloc(20, sizeof(long));
  API->getRawUSBBusAccessFeatures(device_id, error_code, buffer_s, 1);
  cout<<"beffer_s is  "<<buffer_s[0]<<endl;
  //API->rawUSBBusAccessRead(device_id, 0, error_code, buffer, unsigned int bufferLength, unsigned char endpoint);
  return 0;
}
//
