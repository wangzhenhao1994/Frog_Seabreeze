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


TH2F *h2;
TH1D * projh2X;
TH1D * projh2Y;
TPad *right_pad, *top_pad;
Double_t pi;

double fs2um(double fs_step){
  return roundf(TMath::C()*fs_step*pow(10,-15)*pow(10,6)*100)/100; //round a number to 2 places after decimal
}
double step_length=fs2um(1.0);

Spectrometer spec;
PI_Stage stage(&step_length);


Int_t no_step=0;
Int_t nsteps = round(fs2um(20)/step_length);
Int_t pixelNum=spec.pixel_num;
TMatrixD frog_trace(nsteps, pixelNum);

void frog(){
  spec.spec_initializer();
  stage.piezo_initializer();

  double trace_center=50;
  cout<<nsteps<<'\n'<<step_length<<endl;

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
  gStyle->SetCanvasPreferGL(true);
  TCanvas *c1 = new TCanvas("c1", "c1",900,900);
  gStyle->SetOptStat(0);
  TLatex *t = new TLatex();
  t->SetTextFont(42);
  t->SetTextSize(0.02);
  t->DrawLatex(0.6,0.88,"This example demonstrates how to display");
  t->DrawLatex(0.6,0.85,"a histogram and its two projections.");
/////////////////////////////////
  long waiting_for = spec.integration_time*spec.averaged_n+300;
  TTimer *timer = new TTimer(waiting_for);
  timer->SetCommand("Animate()");
  timer->TurnOn();
}

void Animate()
{
   //just in case the canvas has been deleted
   if (!gROOT->GetListOfCanvases()->FindObject("c1")) return;
   TMatrixD S(1, pixelNum);
   S=spec.readSpec();
   TMatrixDRow(frog_trace,no_step)=TMatrixDRow(S,0);
   frog_trace.Draw();
   stage.move_onestep();
   //h2->Fill(frog_trace);
   projh2X = h2->ProjectionX();
   projh2Y = h2->ProjectionY();
   gPad->cd(0);
   gPad->Modified();
   gPad->Update();
   gPad->cd(1);
   gPad->Modified();
   gPad->Update();
   gPad->cd(2);
   gPad->Modified();
   gPad->Update();
   no_step +=1;
   cout<<no_step<<endl;
}
