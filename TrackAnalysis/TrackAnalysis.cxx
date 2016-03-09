#ifndef TRK_TRACKANALYSIS_CXX
#define TRK_TRACKANALYSIS_CXX

#include "TrackAnalysis.hh"
#include <iostream>

trk::TrackAnalysis::TrackAnalysis()
  : fAlgName("TrackAnalysis")
{}

void trk::TrackAnalysis::SetupOutputTrees(TTree* tfs_tree_trk){
  fTrackTree = tfs_tree_trk;
  fTrackTree->SetObject(fTrackTree->GetName(),"Track Tree");
}

void trk::TrackAnalysis::RunAnalysis(std::vector<recob::Track> const& trkVector)
{
}


#endif
