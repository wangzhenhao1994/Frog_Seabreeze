#pragma once
/* Includes */
#include <iostream>
#include <cstdlib>
#include <string>
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include <fstream>

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
  long *feature_id;
  int status=0;
  int pixel_num;
  double *spectra=0;

  API->probeDevices();
  number_of_devices = API->getNumberOfDeviceIDs();
  cout<<number_of_devices<<endl;
  device_ids = (long *)calloc(number_of_devices, sizeof(long));

  number_of_devices=API->getDeviceIDs(device_ids, number_of_devices);
  device_id=device_ids[0];

  nameBuffer=(char *)calloc(80, sizeof(char));
  flag=API->getDeviceType(device_id, error_code, nameBuffer, 79);
  if(flag > 0) {
      cout<<"The device type is: "<<nameBuffer<<endl;
  }
  status=API->openDevice(device_id, error_code);

  if (status) {
    cout<<"Can't open this spectrometer!"<<endl;
  }

  API->getNumberOfSpectrometerFeatures(device_id, error_code);
  feature_id=(long *)calloc(number_of_devices, sizeof(long));
  API->getSpectrometerFeatures(device_id, error_code, feature_id, number_of_devices);
  pixel_num=API->spectrometerGetFormattedSpectrumLength(device_id, feature_id[0], error_code);
  cout<<pixel_num<<endl;
  spectra=(double *)calloc(pixel_num, sizeof(double));
  pixel_num=API->spectrometerGetFormattedSpectrum(device_id, feature_id[0], error_code, spectra, pixel_num);

  while (1){
    ofstream myfile;
    myfile.open ("example.txt");
    for (size_t i = 0; i < pixel_num; i++) {
    myfile << spectra[i]<<"\n";
  }
  myfile.close();
  cout<<"Success!"<<endl;
  }
  API->closeDevice(device_id, error_code);
  return 0;
}
//
