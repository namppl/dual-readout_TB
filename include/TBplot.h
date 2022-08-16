#ifndef TBplot_h
#define TBplot_h 1

#include <vector>

#include "TString.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

#include "TBdetector.h"
#include "TBmid.h"

class TBplotbase {
public:
  enum kind {
    hitmap = 0,
    distribution,
    waveform,
    sipmHitMap
  };

  TBplotbase(int ww, int wh, TString canvasname, TBplotbase::kind plotkind);
  TBplotbase(int ww, int wh, const std::string& canvasname, const std::string& plotkind);
  ~TBplotbase() {}

  void init();
  void setSaveName(TString name) { savename_ = name; }
  void SavePng() { c_->SaveAs(savename_+".png"); }
  TString getSaveName() const { return savename_; }
  TPad* aPad(int idx) const { return pads_.at(idx); }
  TCanvas* c_;
  std::vector<TPad*> pads_;

  std::vector< std::vector<double> > xlow = {
    {0.92 , 0.45 , 0.675, 0.45 , 0.675, 0.01 , 0.156, 0.303, 0.01 , 0.156, 0.303, 0.01 , 0.156, 0.303, 0.45 , 0.675, 0.45 , 0.675, 0.01 , 0.156, 0.303, 0.01 , 0.156, 0.303, 0.01 , 0.156, 0.303},
    {0.50, 0.74, 0.50, 0.74, 0.02, 0.18, 0.34, 0.02, 0.18, 0.34, 0.02, 0.18, 0.34},
    {0.50, 0.74, 0.50, 0.74, 0.02, 0.18, 0.34, 0.02, 0.18, 0.34, 0.02, 0.18, 0.34},
    {0.01, 0.01}
  };

  std::vector< std::vector<double> > xup = {
    {0.99 , 0.675, 0.90 , 0.675, 0.90 , 0.156, 0.303, 0.450, 0.156, 0.303, 0.450, 0.156, 0.303, 0.450, 0.675, 0.90 , 0.675, 0.90 , 0.156, 0.303, 0.450, 0.156, 0.303, 0.450, 0.156, 0.303, 0.450},
    {0.74 ,0.98 ,0.74 ,0.98 ,0.18 ,0.34 ,0.50 ,0.18 ,0.34 ,0.50 ,0.18 ,0.34 ,0.50},
    {0.74 ,0.98 ,0.74 ,0.98 ,0.18 ,0.34 ,0.50 ,0.18 ,0.34 ,0.50 ,0.18 ,0.34 ,0.50},
    {0.99, 0.99}
	};

  std::vector< std::vector<double> > ylow = {
    {0.01 , 0.25 , 0.25 , 0.01 , 0.01 , 0.33 , 0.33 , 0.33 , 0.17 , 0.17 , 0.17 , 0.01 , 0.01 , 0.01 , 0.75 , 0.75 , 0.51 , 0.51 , 0.83 , 0.83 , 0.83 , 0.67 , 0.67 , 0.67 , 0.51 , 0.51 , 0.51 },
    {0.50, 0.50, 0.02, 0.02, 0.66, 0.66, 0.66, 0.34, 0.34, 0.34, 0.02, 0.02, 0.02},
    {0.50, 0.50, 0.02, 0.02, 0.66, 0.66, 0.66, 0.34, 0.34, 0.34, 0.02, 0.02, 0.02},
    {0.01, 0.51}
  };

  std::vector< std::vector<double> > yup = {
    {0.99 , 0.49 , 0.49 , 0.25 , 0.25 , 0.49 , 0.49 , 0.49 , 0.33 , 0.33 , 0.33 , 0.17 , 0.17 , 0.17 , 0.99 , 0.99 , 0.75 , 0.75 , 0.99 , 0.99 , 0.99 , 0.83 , 0.83 , 0.83 , 0.67 , 0.67 , 0.67 },
    {0.98, 0.98, 0.50, 0.50, 0.98, 0.98, 0.98, 0.66, 0.66, 0.66, 0.34, 0.34, 0.34},
    {0.98, 0.98, 0.50, 0.50, 0.98, 0.98, 0.98, 0.66, 0.66, 0.66, 0.34, 0.34, 0.34},
    {0.49, 0.99}
  };

private:
  TString canvasname_;
  TString savename_;
  kind plotkind_;

  void padSet (TPad* tPad, double margin) {
    tPad->Draw();
    tPad->cd();

    tPad->SetTopMargin(margin);
    tPad->SetLeftMargin(margin);
    tPad->SetRightMargin(margin);
    tPad->SetBottomMargin(margin);
  }
};

class TBplot : public TBplotbase {
public:
  TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind);
  TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH1D*> plot1D);
  TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH2D*> plot2D);
  TBplot(int ww, int wh, const std::string& plotname, const std::string& plotkind);
  ~TBplot() {}

  void init_plots();
  TH1D* aPlot1D(int idx) { return plots1D_.at(idx); }
  TH2D* aPlot2D(int idx) { return plots2D_.at(idx); }
  int getPlotSize1D() const { return plots1D_.size(); }
  int getPlotSize2D() const { return plots2D_.size(); }

  void openFile(const std::string& name);
  void closeFile();

  void loadTH1D(const std::string& name, int num);
  void loadTH2D(const std::string& name, int num);

  void fillWaveform(TBdetector detid, std::vector<short> awave);
  void fillADC(TBdetector detid, float adc);

  void Draw();
  void Reset();
  void print();

private:
  double hitmapMax = 1500.;

  int distBin = 100;
  double distMax = 5000.;

  TString plotname_;
  TFile* dqmFile_;

  kind plotkind_;
  std::vector<TH1D*> plots1D_;
  std::vector<TH2D*> plots2D_;
};

#endif
