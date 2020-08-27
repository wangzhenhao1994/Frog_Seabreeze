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
// #include "tool.h"
#include "spectrometer.cpp"
#include "piezo_PI_linux.h"
//using namespace TMath;

double fs2um(double fs_step){
  return roundf(TMath::C()*fs_step*pow(10,-15)*pow(10,6)*100)/100; //round a number to 2 places after decimal
}
double step_length=fs2um(1.0);

Spectrometer spec;
PI_Stage stage(&step_length);

Int_t no_step=0;
Int_t nsteps = round(fs2um(5)/step_length);
Int_t pixelNum=spec.pixel_num;
TMatrixD frog_trace(nsteps, pixelNum);

void frog(){
  spec.spec_initializer();
  stage.piezo_initializer();
  double trace_center=50;
  cout<<"Move "<<nsteps<<" steps and step length is "<<step_length<<" um."<<endl;

    /////////////////////
  //   PI_Stage stage(&step_length);
  //   stage.piezo_initializer();
  //   stage.move_onestep();
  //   stage.exit();
  //   Spectrometer spec;
  //   spec.spec_initializer();
  //   spec.readSpec();
  //   spec.spec_destructor();
    ////////////////////////
//////////////////////////////////
  TCanvas *c1 = new TCanvas("FROG Trace", "FROG Trace", 900,900);
  //h->SetDirectory(0);


/////////////////////////////////
  long waiting_for = spec.integration_time*spec.averaged_n+300;
  TTimer *timer = new TTimer(waiting_for);
  timer->SetCommand("Animate()");
  timer->TurnOn();
}

void Animate()
{  if(no_step>=nsteps){return;}
   TMatrixD S(1, pixelNum);
   S=spec.readSpec();
   TMatrixDRow(frog_trace,no_step)=TMatrixDRow(S,0);
   //gPad->GetListOfPrimitives()->ls();
   if(no_step==0){auto spec_histo = static_cast<TH2D*>(gPad->GetPrimitive("TMatrixDBase"));}
   spec_histo->
   stage.move_onestep();
   cout<<"Success!"<<endl;
   frog_trace.Draw("COLZ");
   gPad->Modified();
   gPad->Update();
   no_step +=1;
}
