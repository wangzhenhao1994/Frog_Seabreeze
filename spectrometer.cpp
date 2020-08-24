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



class Spectrometer{
public:
  Spectrometer();
  int spec_initializer();
  int spec_destructor();
  int readSpec();
private:
  long device_id;
  long *feature_id;
  int errorcode=0;
  int pixel_num;
  double *spectra=0;
  SeaBreezeAPI* API = SeaBreezeAPI::getInstance();

  unsigned long number_of_devices;
  long *device_ids;
  char *nameBuffer;
  int flag;
  int status=0;
};

Spectrometer::Spectrometer(){};

int Spectrometer::spec_initializer(){

  API->probeDevices();
  number_of_devices = API->getNumberOfDeviceIDs();
  cout<<number_of_devices<<endl;
  device_ids = (long *)calloc(number_of_devices, sizeof(long));

  number_of_devices=API->getDeviceIDs(device_ids, number_of_devices);
  device_id=device_ids[0];

  nameBuffer=(char *)calloc(80, sizeof(char));
  flag=API->getDeviceType(device_id, &errorcode, nameBuffer, 79);
  if(flag > 0) {
      cout<<"The device type is: "<<nameBuffer<<endl;
  }
  status=API->openDevice(device_id, &errorcode);

  if (status) {
    cout<<"Can't open this spectrometer!"<<endl;
  }

  API->getNumberOfSpectrometerFeatures(device_id, &errorcode);
  feature_id=(long *)calloc(number_of_devices, sizeof(long));
  API->getSpectrometerFeatures(device_id, &errorcode, feature_id, number_of_devices);
  pixel_num=API->spectrometerGetFormattedSpectrumLength(device_id, feature_id[0], &errorcode);
  cout<<pixel_num<<endl;
  return 0;
}

int Spectrometer::spec_destructor(){
  API->closeDevice(device_id, &errorcode);
  API->shutdown();
  return 0;
}

int Spectrometer::readSpec() {
  spectra=(double *)calloc(pixel_num, sizeof(double));

  ofstream myfile;
  myfile.open ("example.txt");
  for (size_t i=0; i<100; i++){
    API->spectrometerGetFormattedSpectrum(device_id, feature_id[0], &errorcode, spectra, pixel_num);
    for (size_t j = 0; j < pixel_num; j++) {
    myfile << spectra[j]<<"\n";
    }
  }
  myfile.close();
  cout<<"Success!"<<endl;
  return 0;
}
