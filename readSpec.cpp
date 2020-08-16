/* Includes */
#include <iostream>
#include <cstdlib>
#include <string>
#include "SeaBreeze/include/api/seabreezeapi/SeaBreezeAPI.h"

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
  long *device_id;
  int *error_code=0;

  if (API->probeDevices()){
    cout<<"No spectrometer is detected!"<<endl;
  }
  number_of_devices = API->getNumberOfDeviceIDs();
  device_ids = (long *)calloc(number_of_devices, sizeof(long));
  API->getDeviceIDs(device_ids, number_of_devices);
  API->openDevice(device_ids[0], error_code);
  API->spectrometerGetWavelengths(long deviceID, long spectrometerFeatureID, int *errorCode, double *wavelengths, int length);
  return 0;
}
//
