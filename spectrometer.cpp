#pragma once
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "TH2F.h"
//#define _WINDOWS

#ifndef __unix__
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
class Spectrometer{
public:
  Spectrometer(int n=30, unsigned long t=10);
  void spec_initializer();
  void spec_destructor();
  void readSpec(int no_step, double xticks[], TH2F* spec_hist);
  int pixel_num=2048;
  double* wavelengths;
  //TH2F* spec_hist;
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
  unsigned long integration_time;
  int averaged_n;
  double* spectra;
};

Spectrometer::Spectrometer(int n=30, unsigned long t=10):averaged_n(n), integration_time(t){};

void Spectrometer::spec_initializer(){

  API->probeDevices();
  number_of_devices = API->getNumberOfDeviceIDs();
  device_ids = (long *)calloc(number_of_devices, sizeof(long));
  number_of_devices=API->getDeviceIDs(device_ids, number_of_devices);
  device_id=device_ids[0];
  //cout<<device_id<<endl;
  nameBuffer=(char *)calloc(80, sizeof(char));
  flag=API->getDeviceType(device_id, &errorcode, nameBuffer, 79);
  if(flag > 0) {
      cout<<"The device type is: "<<nameBuffer<<endl;
  }
  status=API->openDevice(device_id, &errorcode);

  if (status) {
    cout<<"Can't open this spectrometer!"<<endl;
  }

  //API->getNumberOfSpectrometerFeatures(device_id, &errorcode);
  //feature_id=(long *)calloc(number_of_devices, sizeof(long));
  API->getSpectrometerFeatures(device_id, &errorcode, &feature_id, number_of_devices);
  pixel_num=API->spectrometerGetFormattedSpectrumLength(device_id, feature_id, &errorcode);
  wavelengths=(double *)calloc(pixel_num, sizeof(double));
  API->spectrometerGetWavelengths(device_id, feature_id, &errorcode, wavelengths, pixel_num);
  //cout<<pixel_num<<endl;
  API->spectrometerSetIntegrationTimeMicros(device_id, feature_id, &errorcode, integration_time);
}

void Spectrometer::spec_destructor(){
  API->closeDevice(device_id, &errorcode);
  API->shutdown();
  return;
}

void Spectrometer::readSpec(int no_step, double xticks[], TH2F* spec_hist) {
  spectra=(double *)calloc(pixel_num, sizeof(double));
  for (size_t i = 0; i < pixel_num; i++) {
    spectra[i]=0;
  }

  //ofstream myfile;
  //myfile.open ("example.txt");
  //myfile.close();
  for (size_t i=0; i < averaged_n; i++) {
    API->spectrometerGetFormattedSpectrum(device_id, feature_id, &errorcode, spectra, pixel_num);
    for (size_t j=0; j<pixel_num; j++){
      spectra[j]+=spectra[j];
    }
  }

  for (size_t i = 0; i < pixel_num; i++) {
    spectra[i]=spectra[i]*(double)(1.00/averaged_n);
    spec_hist->Fill(xticks[no_step], wavelengths[i], spectra[i]);
  }
  return;
}

//void spectrometer()
//{
//  double *xticks;
//  xticks = (double *)calloc(5, sizeof(double));
//  for (size_t i = 0; i < 5; i++)
//  {
//    xticks[i] = i + 1;
//    cout << xticks[i] << endl;
//  }
//  Spectrometer spec;
//  spec.spec_initializer();
//  // for (size_t i = 0; i < spec.pixel_num; i++) {
//  //   cout<<spec.wavelengths[i]<<endl;
//  // }
//
//  TH2F *h2 = new TH2F("h2", "FROG Trace", 4, xticks, 4, xticks);
//  // spec.readSpec(1,xticks,h2);
//  h2->Draw("COLZ");
//  spec.spec_destructor();
//}
