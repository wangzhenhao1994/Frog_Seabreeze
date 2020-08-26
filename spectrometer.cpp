#include "api/seabreezeapi/SeaBreezeAPI.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "TVectorD.h"
#include "TMatrixD.h"
//#define _WINDOWS

#ifndef __unix__
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

class Spectrometer{
public:
  Spectrometer();
  void spec_initializer();
  void spec_destructor();
  TMatrixD readSpec();
  int pixel_num;
  unsigned long integration_time=100;
  int averaged_n=30;
private:
  long device_id;
  long feature_id;
  int errorcode=0;
  SeaBreezeAPI* API = SeaBreezeAPI::getInstance();
  unsigned long number_of_devices;
  long *device_ids;
  char *nameBuffer;
  int flag;
  int status=0;
  double* spectra;
};

Spectrometer::Spectrometer(){};

void Spectrometer::spec_initializer(){

  API->probeDevices();
  number_of_devices = API->getNumberOfDeviceIDs();
  device_ids = (long *)calloc(number_of_devices, sizeof(long));
  number_of_devices=API->getDeviceIDs(device_ids, number_of_devices);
  device_id=device_ids[0];
  cout<<device_id<<endl;
  //printf("%ld\n",device_id);
  nameBuffer=(char *)calloc(80, sizeof(char));
  flag=API->getDeviceType(device_id, &errorcode, nameBuffer, 79);
  if(flag > 0) {
      cout<<"The device type is: "<<nameBuffer<<endl;
      //printf("The device type is%s\n", nameBuffer);
  }
  status=API->openDevice(device_id, &errorcode);

  if (status) {
    cout<<"Can't open this spectrometer!"<<endl;
    //printf("Can't open this spectrometer!");
  }

  //API->getNumberOfSpectrometerFeatures(device_id, &errorcode);
  //feature_id=(long *)calloc(number_of_devices, sizeof(long));
  API->getSpectrometerFeatures(device_id, &errorcode, &feature_id, number_of_devices);
  pixel_num=API->spectrometerGetFormattedSpectrumLength(device_id, feature_id, &errorcode);
  cout<<pixel_num<<endl;
  API->spectrometerSetIntegrationTimeMicros(device_id, feature_id, &errorcode, integration_time);
  //printf("%d\n", pixel_num);
}

void Spectrometer::spec_destructor(){
  API->closeDevice(device_id, &errorcode);
  API->shutdown();
  return;
}

TMatrixD Spectrometer::readSpec() {
  spectra=(double *)calloc(pixel_num, sizeof(double));
  TMatrixD S=THaarMatrixD(1, pixel_num);
  //S-=S;
  //ofstream myfile;
  //myfile.open ("example.txt");
  //myfile.close();
  for (size_t i=0; i < averaged_n; i++) {
    API->spectrometerGetFormattedSpectrum(device_id, feature_id, &errorcode, spectra, pixel_num);
    for (size_t j=0; j<pixel_num; j++){
      S(0,j)+=spectra[j];
    }
  }
  S=S*(double)averaged_n;
  cout<<"Success!"<<endl;
  //printf("Success!");
  return S;
}

void spectrometer(){
  Spectrometer spec;
  spec.spec_initializer();
  for (size_t i = 0; i < 5; i++) {
  spec.readSpec().Draw();
  spec.readSpec()+=spec.readSpec();
  }

  spec.spec_destructor();
}
