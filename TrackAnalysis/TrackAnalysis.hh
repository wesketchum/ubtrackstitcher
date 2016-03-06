/**
 * \file TrackAnalysis.h
 *
 * 
 * \brief Little sample program for establishing a user analysis space.
 *
 * @author wketchum
*/

#ifndef TEST_USERANALYSIS_H
#define TEST_USERANALYSIS_H

#include <string>

#include "TTree.h"

namespace trk{
  class TrackAnalysis;
}

class trk::TrackAnalysis{
  
public:
  
  /// Default constructor
  TrackAnalysis();

  /// Default destructor
  virtual ~TrackAnalysis(){};

  void RunAnalysis();
  void SetupOutputTree(TTree*);
  
 private:

  std::string fAlgName;
  TTree*      fTree;
  
  void PrintInfo();

  
};

#endif
