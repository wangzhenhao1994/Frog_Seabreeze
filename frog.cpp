#include "TStyle.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TTimer.h"
#include "spectrometer.cpp"
#include "piezo_PI.cpp"
#include "TMath.h"
#include <cstdlib>
#include <cmath>
using namespace std;
//using namespace TMath;

TH2F *h2;
TH1D * projh2X;
TH1D * projh2Y;
TPad *right_pad, *top_pad;
Double_t pi;
Float_t tt = 0;

double fs2um(double fs_step){
  return roundf(TMath::C()*fs_step*pow(10,-15)*pow(10,6)*100)/100; //round a number to 2 places after decimal
}

void frog(){
  double trace_center=50;
  double step_length=fs2um(1.0);
  const int nsteps = round(fs2um(20)/step_length);
  cout<<nsteps<<'\n'<<step_length<<endl;
  double* ndelay_bins[nsteps];
//////////////////////////////////
  auto c1 = new TCanvas("c1", "c1",900,900);
  gStyle->SetOptStat(0);

  TPad *center_pad = new TPad("center_pad", "center_pad",0.0,0.0,0.6,0.6);
  center_pad->Draw();

  right_pad = new TPad("right_pad", "right_pad",0.55,0.0,1.0,0.6);
  right_pad->Draw();

  top_pad = new TPad("top_pad", "top_pad",0.0,0.55,0.6,1.0);
  top_pad->Draw(); 

  h2 = new TH2F("h2","",40,-4,4,40,-20,20);

  Float_t px, py;
  for (Int_t i = 0; i < 25000; i++) {
   gRandom->Rannor(px,py);
   h2->Fill(px,5*py);
  }
  projh2X = h2->ProjectionX();
  projh2Y = h2->ProjectionY();

  center_pad->cd();
  gStyle->SetPalette(1);
  h2->Draw("COL");

  top_pad->cd();
  projh2X->SetFillColor(kBlue+1);
  projh2X->Draw("bar");

  right_pad->cd();
  projh2Y->SetFillColor(kBlue-2);
  projh2Y->Draw("hbar");

  c1->cd();
  TLatex *t = new TLatex();
  t->SetTextFont(42);
  t->SetTextSize(0.02);
  t->DrawLatex(0.6,0.88,"This example demonstrates how to display");
  t->DrawLatex(0.6,0.85,"a histogram and its two projections.");
/////////////////////////////////
  TTimer *timer = new TTimer(200);
  timer->SetCommand("Animate()");
  timer->TurnOn();
}

void ZoomExec()
{
   int xfirst = h2->GetXaxis()->GetFirst();
   int xlast = h2->GetXaxis()->GetLast();
   double xmin = h2->GetXaxis()->GetBinLowEdge(xfirst);
   double xmax = h2->GetXaxis()->GetBinUpEdge(xlast);
   projh2X->GetXaxis()->SetRangeUser(xmin, xmax);
   top_pad->Modified();

   int yfirst = h2->GetYaxis()->GetFirst();
   int ylast = h2->GetYaxis()->GetLast();
   double ymin = h2->GetYaxis()->GetBinLowEdge(yfirst);
   double ymax = h2->GetYaxis()->GetBinUpEdge(ylast);
   projh2Y->GetXaxis()->SetRangeUser(ymin, ymax);
   right_pad->Modified();
}

void Animate()
{
   //just in case the canvas has been deleted
   if (!gROOT->GetListOfCanvases()->FindObject("c1")) return;
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
   tt +=10;
   Float_t px, py;
   for (Int_t i = 0; i < 25000; i++) {
      gRandom->Rannor(px,py);
      h2->Fill(px,5*py*10);
   }
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
}
