#ifndef TEST_USERANALYSIS_CXX
#define TEST_USERANALYSIS_CXX

#include "TrackAnalysis.hh"
#include <iostream>

trk::TrackAnalysis::TrackAnalysis()
  : fAlgName("TrackAnalysis")
{}

void trk::TrackAnalysis::SetupOutputTree(TTree* tfs_tree){
  fTree = tfs_tree;

  std::string title = fAlgName + " Tree";
  fTree->SetObject(fTree->GetName(),title.c_str());
}

void trk::TrackAnalysis::RunAnalysis(){
  PrintInfo();
}

void trk::TrackAnalysis::PrintInfo(){
  std::cout << "\n================================== TrackAnalysis ==========================" << std::endl;
  std::cout << "This is a ub_TrackAnalysis class called " << fAlgName << std::endl;
  std::cout << "\tThere is an output tree called "
	    << fTree->GetName() << " (" << fTree->GetTitle() << ")" << std::endl;
  std::cout << "==========================================================================\n" << std::endl;
}

#endif
