#ifndef TBmonit_h
#define TBmonit_h 1

#include <string>
#include <iostream>

#include <boost/python.hpp>

#include "TFile.h"
#include "TPad.h"

class TBmonit {
public:
  TBmonit();
  ~TBmonit() {}

  void setFastmodeFiles(const boost::python::list&);
  void setWaveformFiles(const boost::python::list&);

  void MonitPlots();
  void padSet (TPad* tPad, double margin);

  std::string mappingpath() const { return mappingpath_; }
  void setMappingPath(const std::string& path) { mappingpath_ = path; }
  void printMappingPath() { std::cout << mappingpath_ << std::endl; }

  std::string pedestalpath() const { return pedestalpath_; }
  void setPedestalPath(const std::string& path) { pedestalpath_ = path; }
  void printPedestalPath() { std::cout << pedestalpath_ << std::endl; }

  void SetOutputName(std::string outputName) { outputName_ = outputName; }

  void SetADCmax(double DistMaxADC) { DistMaxADC_ = DistMaxADC; }
  void SetADCbin(double DistBinADC) { DistBinADC_ = DistBinADC; }

private:
  TFile* outputFile;

  std::vector<std::string> fastList_;
  std::vector<std::string> waveList_;

  double DistMaxADC_;
  double DistBinADC_;

  std::string outputName_;
  std::string mappingpath_;
  std::string pedestalpath_;
};

#endif
