#include "TStyle.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TTimer.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TArrayF.h"
#include <cstdlib>
#include <cmath>
//////////////////////////////
#include "spectrometer.cpp"
#include "piezo_PI_linux.cpp"
using namespace std;

double round2(double x){
  return roundf(x*100)/100;
}
double fs2um(double step_time){
  return round2(TMath::C()*step_time*pow(10,-15)*pow(10,6)); //round a number to 2 places after decimal
}
double step_time=1;
double range_time=30;

int integration_time = 1000;
int averaged_n = 30;

double step_length=fs2um(step_time);
double range_length=fs2um(range_time);
Int_t nsteps = round(range_length/step_length);
Int_t no_step=0;
double trace_center=50;
double* xticks=(double *)calloc(nsteps, sizeof(double));
double start_point=round2(trace_center-range_length/2);

TH2F* h1;
Spectrometer spec(averaged_n, integration_time);
PI_Stage stage(step_length, trace_center, start_point);
void frog(){
  spec.spec_initializer();
  stage.piezo_initializer();
  for(size_t i = 0; i <= nsteps; i++) {
    xticks[i]=i*step_time-range_time/2;
  }
  cout<<"Move "<<nsteps<<" steps and step length is "<<step_length<<" um."<<endl;

  TCanvas *c1 = new TCanvas("c1", "FROG Trace", 900,900);
  h1 = new TH2F("h1","FROG Trace",nsteps,xticks,spec.pixel_num-1,spec.wavelengths);
  h1->Draw("COLZ");
/////////////////////////////////
  long waiting_for = 500;
  TTimer *timer = new TTimer(waiting_for);
  timer->SetCommand("Animate()");
  timer->TurnOn();
}

void Animate(){
  if (!gROOT->GetListOfCanvases()->FindObject("c1")) return;
  if(no_step>=nsteps){return;}
  spec.readSpec(no_step, xticks, h1);
  stage.move_onestep();
  cout<<"Success!"<<endl;
  gPad->Modified();
  gPad->Update();
  no_step +=1;
}
