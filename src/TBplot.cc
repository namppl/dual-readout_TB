#include <iostream>
#include <vector>
#include <stdexcept>

#include "TCanvas.h"
#include "TPad.h"

#include "TBplot.h"
#include "TPaletteAxis.h"

TBplotbase::TBplotbase(int ww, int wh, TString canvasname, TBplotbase::kind plotkind)
: plotkind_(plotkind), pads_(0), savename_(0), canvasname_(canvasname) {
  c_ = new TCanvas((TString)(canvasname_+std::to_string(plotkind_)), (TString)(canvasname_+std::to_string(plotkind_)), ww, wh);

  init();
}

TBplotbase::TBplotbase(int ww, int wh, const std::string& canvasname, const std::string& plotkind)
: canvasname_(canvasname), savename_("") {
  if (plotkind=="hitmap")
    plotkind_ = kind::hitmap;
  else if (plotkind=="distribution")
    plotkind_ = kind::distribution;
  else if (plotkind=="waveform")
    plotkind_ = kind::waveform;
  else if (plotkind=="sipmHitMap")
    plotkind_ = kind::sipmHitMap;
  else
    throw std::runtime_error("TBplotbase - please check TBplotbase::kind!");

  c_ = new TCanvas((TString)(canvasname_+std::to_string(plotkind_)), (TString)(canvasname_+std::to_string(plotkind_)), ww, wh);

  init();
}

void TBplotbase::init() {
  for (int i = 0; i < xlow.at(plotkind_).size(); i++) {

    c_->cd(); TPad* tmpPad = new TPad((TString)(canvasname_+std::to_string(plotkind_)+""+std::to_string(i)),
                                      (TString)(canvasname_+std::to_string(plotkind_)+""+std::to_string(i)),
                                      xlow.at(plotkind_).at(i),
                                      ylow.at(plotkind_).at(i),
                                      xup.at(plotkind_).at(i),
                                      yup.at(plotkind_).at(i));

  pads_.push_back(tmpPad);
  padSet(pads_.at(i), 0.);
  }
}

TBplot::TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind)
: TBplotbase(ww, wh, plotname, plotkind), plots1D_(0), plots2D_(0), plotkind_(plotkind), plotname_(plotname) {
  init_plots();
}

TBplot::TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH1D*> plot1D)
: TBplotbase(ww, wh, plotname, plotkind), plots1D_(plot1D), plots2D_(0), plotkind_(plotkind), plotname_(plotname) {}

TBplot::TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH2D*> plot2D)
: TBplotbase(ww, wh, plotname, plotkind), plots1D_(0), plots2D_(plot2D), plotkind_(plotkind), plotname_(plotname) {}

TBplot::TBplot(int ww, int wh, const std::string& plotname, const std::string& plotkind)
: TBplotbase(ww, wh, plotname, plotkind), plots1D_(0), plots2D_(0) {}

void TBplot::init_plots() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap ) {
    for( int i = 0; i < xlow.at(plotkind_).size(); i++ ) {
      TH2D* tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 20, 0, 20, 20, 0, 20);
      tmpHist->SetStats(0);
      tmpHist->SetTitle("");
      tmpHist->GetXaxis()->SetTickLength(0.);
      tmpHist->GetYaxis()->SetTickLength(0.);
      tmpHist->GetZaxis()->SetRangeUser(0., hitmapMax);
      tmpHist->Sumw2();
      plots2D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::hitmap ) {
    TH2D* colPal = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), 1, 0, 1, 10000, 0, 1);
    colPal->SetStats(0);
    colPal->SetTitle("");
    colPal->GetXaxis()->SetTickLength(0.);
    colPal->GetYaxis()->SetTickLength(0.);
    colPal->GetZaxis()->SetRangeUser(0., hitmapMax);
    colPal->Sumw2();

    for (int i = 1; i <= 10000; i++)
      colPal->SetBinContent(1, i, (hitmapMax/9999.)*(i-1));

    plots2D_.push_back(colPal);

    for( int i = 1; i < xlow.at(plotkind_).size(); i++ ) {
      TH2D* tmpHist;
      if( i != 9 && i != 22 ) {
        tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 1, 0, 1, 1, 0, 1);
        tmpHist->SetStats(0); tmpHist->SetTitle("");
        tmpHist->GetXaxis()->SetTickLength(0.);
        tmpHist->GetYaxis()->SetTickLength(0.);
        tmpHist->GetZaxis()->SetRangeUser(0., hitmapMax);
        tmpHist->Sumw2();
      } else {
        tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 20, 0, 20, 20, 0, 20);
        tmpHist->SetStats(0);
        tmpHist->SetTitle("");
        tmpHist->GetXaxis()->SetTickLength(0.);
        tmpHist->GetYaxis()->SetTickLength(0.);
        tmpHist->GetZaxis()->SetRangeUser(0., hitmapMax);
        tmpHist->Sumw2();
      }

      plots2D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution ) {
    for( int i = 0; i < 2*xlow.at(plotkind_).size(); i++ ) {
      TH1D* tmpHist = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), distBin, -1500, distMax);
      tmpHist->SetStats(1);
      tmpHist->SetLineWidth(1);
      tmpHist->SetTitle("");
      tmpHist->Sumw2();

      if( i < xlow.at(plotkind_).size() )
        tmpHist->SetLineColor(kBlue);
      else
        tmpHist->SetLineColor(kRed);

      plots1D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::waveform ) {
    for( int i = 0; i < 2*xlow.at(plotkind_).size(); i++ ) {
      TH1D* tmpHist = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 1000, 0, 1000);
      tmpHist->SetTitle("");
      tmpHist->SetStats(0);
      tmpHist->SetLineWidth(1);
      tmpHist->GetYaxis()->SetRangeUser(0., 4100.);
      tmpHist->Sumw2();
      if( i < xlow.at(plotkind_).size() )
        tmpHist->SetLineColor(kBlue);
      else
        tmpHist->SetLineColor(kRed);

      plots1D_.push_back(tmpHist);
    }
  } else {
    throw std::runtime_error("Available plot : Hitmap, ADC distribution, Waveform");
  }
}

void TBplot::openFile(const std::string& name) {
  dqmFile_ = TFile::Open(name.c_str(),"READ");
}

void TBplot::closeFile() {
  dqmFile_->Close();
}

void TBplot::loadTH1D(const std::string& name, int num) {
  for(int i = 0; i < num; i++)
    plots1D_.push_back((TH1D*)dqmFile_->Get((TString)(name+std::to_string(i))));
}

void TBplot::loadTH2D(const std::string& name, int num) {
  for(int i = 0; i < num; i++)
    plots2D_.push_back((TH2D*)dqmFile_->Get((TString)(name+std::to_string(i))));
}

void TBplot::fillWaveform( TBdetector detid, std::vector<short> awave ) {
  if ( plotkind_ != TBplotbase::kind::waveform )
    throw std::runtime_error("TBplot::fillWaveform is only available with waveform plot");

  int Nmodule  = detid.module();
  int Ntower   = detid.tower();
  bool isCeren = detid.isCeren();
  int plate    = detid.plate();
  int column   = detid.column();

  if ( isCeren ) {
    if ( Nmodule == 1 ) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(0)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(1)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(2)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(3)->Fill(i+0.5, awave.at(i+1));
    } else if ( Nmodule == 2) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(4)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(5)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(6)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(7)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 6 ) for(int i = 0; i < 1000; i++) plots1D_.at(9)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 7 ) for(int i = 0; i < 1000; i++) plots1D_.at(10)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 8 ) for(int i = 0; i < 1000; i++) plots1D_.at(11)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 9 ) for(int i = 0; i < 1000; i++) plots1D_.at(12)->Fill(i+0.5, awave.at(i+1));
    }

  } else {
    if ( Nmodule == 1 ) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(13)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(14)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(15)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(16)->Fill(i+0.5, awave.at(i+1));
    } else if ( Nmodule == 2) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(17)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(18)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(19)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(20)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 6 ) for(int i = 0; i < 1000; i++) plots1D_.at(22)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 7 ) for(int i = 0; i < 1000; i++) plots1D_.at(23)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 8 ) for(int i = 0; i < 1000; i++) plots1D_.at(24)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 9 ) for(int i = 0; i < 1000; i++) plots1D_.at(25)->Fill(i+0.5, awave.at(i+1));
    }
  }
}

void TBplot::fillADC( TBdetector detid, float adc ) {
  if ( plotkind_ == TBplotbase::kind::waveform )
    throw std::runtime_error("TBplot::fillADC is only available with ADC distribution and Hitmap plot");

  int Nmodule  = detid.module();
  int Ntower   = detid.tower();
  bool isCeren = detid.isCeren();
  int plate    = detid.plate() - 20;
  int column   = detid.column() - 20;

  if ( Ntower == 255 )
    return;

  if ( plotkind_ == TBplotbase::kind::hitmap ) {
    if ( isCeren ) {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots2D_.at(1)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(2)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(3)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(4)->Fill(1 - 0.5, 1 - 0.5, adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots2D_.at(5)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(6)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(7)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(8)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 5 ) plots2D_.at(9)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
        if ( Ntower == 6 ) plots2D_.at(10)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 7 ) plots2D_.at(11)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 8 ) plots2D_.at(12)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 9 ) plots2D_.at(13)->Fill(1 - 0.5, 1 - 0.5, adc);
      }

    } else {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots2D_.at(14)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(15)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(16)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(17)->Fill(1 - 0.5, 1 - 0.5, adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots2D_.at(18)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(19)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(20)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(21)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 5 ) plots2D_.at(22)->Fill(column- 0.5, plate - 0.5, adc); // SiPM
        if ( Ntower == 6 ) plots2D_.at(23)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 7 ) plots2D_.at(24)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 8 ) plots2D_.at(25)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 9 ) plots2D_.at(26)->Fill(1 - 0.5, 1 - 0.5, adc);
      }
    }
  } else if ( plotkind_ == TBplotbase::kind::sipmHitMap ) {
    if ( isCeren ) {
      if ( Nmodule == 2 && Ntower == 5)
        plots2D_.at(0)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
    } else {
      if ( Nmodule == 2 && Ntower == 5)
        plots2D_.at(1)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution ) {
    if ( isCeren ) {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots1D_.at(0)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(1)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(2)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(3)->Fill(adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots1D_.at(4)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(5)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(6)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(7)->Fill(adc);
        if ( Ntower == 6 ) plots1D_.at(9)->Fill(adc);
        if ( Ntower == 7 ) plots1D_.at(10)->Fill(adc);
        if ( Ntower == 8 ) plots1D_.at(11)->Fill(adc);
        if ( Ntower == 9 ) plots1D_.at(12)->Fill(adc);
      }
    } else {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots1D_.at(13)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(14)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(15)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(16)->Fill(adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots1D_.at(17)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(18)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(19)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(20)->Fill(adc);
        if ( Ntower == 6 ) plots1D_.at(22)->Fill(adc);
        if ( Ntower == 7 ) plots1D_.at(23)->Fill(adc);
        if ( Ntower == 8 ) plots1D_.at(24)->Fill(adc);
        if ( Ntower == 9 ) plots1D_.at(25)->Fill(adc);
      }
    }
  }
}

void TBplot::print() {
  for (int i = 0; i < plots2D_.size(); i++) {
    std::cout << plots2D_.at(i)->GetNbinsX() << " " << plots2D_.at(i)->GetNbinsY() << std::endl;
    for (int j = 1; j <= plots2D_.at(i)->GetNbinsX(); j++) {
      for (int t = 1; t <= plots2D_.at(i)->GetNbinsY(); t++)
        std::cout << "plots2D_.at(" << i << ").[" << j << "][" << t << "] : " << plots2D_.at(i)->GetBinContent(j,t) << std::endl;
    }
  }
}

void TBplot::Reset() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap || plotkind_ == TBplotbase::kind::hitmap ) {
    for(int i = 0; i < plots2D_.size(); i++)
      plots2D_.at(i)->Reset();
  } else if ( plotkind_ == TBplotbase::kind::distribution || plotkind_ == TBplotbase::kind::waveform ) {
    for(int i = 0; i < plots2D_.size(); i++)
      plots1D_.at(i)->Reset();
  }
}

void TBplot::Draw() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap ) {
    for (int i = 0; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots2D_.at(i)->Draw("col");
    }
  } else if ( plotkind_ == TBplotbase::kind::hitmap ) {
    c_->cd(); pads_.at(0)->cd(); plots2D_.at(0)->Draw("col");

    for (int i = 1; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots2D_.at(i)->Draw("col");
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution || plotkind_ == TBplotbase::kind::waveform ) {
    for (int i = 0; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots1D_.at(i+xlow.at(plotkind_).size())->Draw("Hist");
      plots1D_.at(i)->Draw("Hist&sames");
    }
  }
}
