#ifndef TRK_TRACKANALYSIS_CXX
#define TRK_TRACKANALYSIS_CXX

#include "TrackAnalysis.hh"

#include "Geometry/GeometryCore.h"
#include <iostream>

trk::TrackAnalysis::TrackAnalysis(){}

void trk::TrackAnalysis::SetupOutputTree(TTree* tfs_tree_trk){
  fTrackTree = tfs_tree_trk;
  fTrackTree->SetObject(fTrackTree->GetName(),"Track Tree");
  fTrackTree->Branch("run",&fRun);
  fTrackTree->Branch("event",&fEvent);
  fTrackTree->Branch("trk",&fTrackTreeObj,fTrackTreeObj.Leaflist().c_str());
  fTrackTree->Branch("trk_col",&fCollection);
  fTrackTree->Branch("trk_id",&fTrkID);
  fTrackTree->Branch("trk_mindist",&fDistance);
  fTrackTree->Branch("trk_containment",&fContainment);
}

void trk::TrackAnalysis::Configure(fhicl::ParameterSet const& p)
{
  fZBuffer = p.get<double>("ZBuffer");
  fYBuffer = p.get<double>("YBuffer");
  fXBuffer = p.get<double>("XBuffer");
  fIsolation = p.get<double>("Isolation");
}

bool trk::TrackAnalysis::IsContained(recob::Track const& track, geo::GeometryCore const& geo)
{
  if( track.Vertex().Z() < (0+fZBuffer) || track.Vertex().Z() > (geo.DetLength()-fZBuffer) )
    return false;
  if( track.End().Z() < (0+fZBuffer) || track.End().Z() > (geo.DetLength()-fZBuffer) )
    return false;
  if( track.Vertex().Y() < (-1*geo.DetHalfHeight()+fYBuffer) || track.Vertex().Y() > (geo.DetHalfHeight()-fYBuffer) )
    return false;
  if( track.End().Y() < (-1*geo.DetHalfHeight()+fYBuffer) || track.End().Y() > (geo.DetHalfHeight()-fYBuffer) )
    return false;
  if( track.Vertex().X() < (0+fXBuffer) || track.Vertex().X() > (2*geo.DetHalfWidth()-fXBuffer) )
    return false;
  if( track.End().X() < (0+fXBuffer) || track.End().X() > (2*geo.DetHalfWidth()-fXBuffer) )
    return false;

  return true;
}

double trk::TrackAnalysis::MinDistanceStartPt(recob::Track const& t_probe, recob::Track const& t_ref)
{
  double min_distance = 9e12;
  double tmp;
  for(size_t i_p=0; i_p<t_ref.NumberTrajectoryPoints(); ++i_p){
    tmp =
      (t_probe.Vertex().X()-t_ref.LocationAtPoint(i_p).X())*(t_probe.Vertex().X()-t_ref.LocationAtPoint(i_p).X()) +
      (t_probe.Vertex().Y()-t_ref.LocationAtPoint(i_p).Y())*(t_probe.Vertex().Y()-t_ref.LocationAtPoint(i_p).Y()) +
      (t_probe.Vertex().Z()-t_ref.LocationAtPoint(i_p).Z())*(t_probe.Vertex().Z()-t_ref.LocationAtPoint(i_p).Z());
    //std::cout << "\t\t\ttmp=" << tmp << std::endl;
    if(tmp<min_distance)
      min_distance=tmp;
  }
  return std::sqrt(min_distance);
}

double trk::TrackAnalysis::MinDistanceEndPt(recob::Track const& t_probe, recob::Track const& t_ref)
{
  double min_distance = 9e12;
  double tmp;
  for(size_t i_p=0; i_p<t_ref.NumberTrajectoryPoints(); ++i_p){
    tmp =
      (t_probe.End().X()-t_ref.LocationAtPoint(i_p).X())*(t_probe.End().X()-t_ref.LocationAtPoint(i_p).X()) +
      (t_probe.End().Y()-t_ref.LocationAtPoint(i_p).Y())*(t_probe.End().Y()-t_ref.LocationAtPoint(i_p).Y()) +
      (t_probe.End().Z()-t_ref.LocationAtPoint(i_p).Z())*(t_probe.End().Z()-t_ref.LocationAtPoint(i_p).Z());
    if(tmp<min_distance)
      min_distance=tmp;
  }
  return std::sqrt(min_distance);
}

void trk::TrackAnalysis::SetRunEvent(unsigned int const& run, unsigned int const& event)
{
  fRun = run;
  fEvent = event;
}

void trk::TrackAnalysis::ProcessTracks(std::vector< std::vector<recob::Track> > const& tracksVec,
				       geo::GeometryCore const& geo)
{

  //std::cout << "Geometry:" << std::endl;
  //std::cout << "\t" << geo.DetHalfWidth() << " " << geo.DetHalfHeight() << " " << geo.DetLength() << std::endl;
  //std::cout << "\t z:(" << fZBuffer << "," << geo.DetLength()-fZBuffer << ")"
  //	    << "\t y:(" << -1.*geo.DetHalfHeight()+fYBuffer << "," << geo.DetHalfHeight()-fYBuffer << ")"
  //	    << "\t x:(" << 0+fXBuffer << "," << 2*geo.DetHalfWidth()-fXBuffer << ")" << std::endl;
  int containment_level=0;
  bool track_linked = false;
  std::size_t n_tracks=0;

  fTrackContainmentLevel.clear();
  fTrackContainmentLevel.resize(tracksVec.size());
  fMinDistances.clear();
  fMinDistances.resize(tracksVec.size());

  fTrackContainmentIndices.clear();
  fTrackContainmentIndices.push_back( std::vector< std::pair<int,int> >() );

  //std::cout << "Made it here with tracks of size " << tracksVec.size() << std::endl;

  for(size_t i_tc=0; i_tc<tracksVec.size(); ++i_tc){
    fTrackContainmentLevel[i_tc].resize(tracksVec[i_tc].size(),-1);
    fMinDistances[i_tc].resize(tracksVec[i_tc].size(),9e12);
    n_tracks += tracksVec[i_tc].size();
    for(size_t i_t=0; i_t<tracksVec[i_tc].size(); ++i_t){
      //std::cout << "\tprocessing " << i_tc << " " << i_t << std::endl;
      if(!IsContained(tracksVec[i_tc][i_t],geo)){
	if(!track_linked) track_linked=true;
	fTrackContainmentLevel[i_tc][i_t] = 0;
	fTrackContainmentIndices.back().emplace_back(i_tc,i_t);
	//std::cout << "\tTrack (" << i_tc << "," << i_t << ")" 
	//	  << " " << containment_level << std::endl;

      }
    }

  }
  
  while(track_linked){

    track_linked=false;
    ++containment_level;
    fTrackContainmentIndices.push_back( std::vector< std::pair<int,int> >() );
    
    for(size_t i_tc=0; i_tc<tracksVec.size(); ++i_tc){
      for(size_t i_t=0; i_t<tracksVec[i_tc].size(); ++i_t){
	if(fTrackContainmentLevel[i_tc][i_t]>=0)
	  continue;
	else
	  {
	    for(auto const& i_tr : fTrackContainmentIndices[containment_level-1]){
	      //std::cout << "\t\t" << MinDistanceStartPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second]) << std::endl;
	      //std::cout << "\t\t" << MinDistanceEndPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second]) << std::endl;

	      if(MinDistanceStartPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second])<fMinDistances[i_tc][i_t])
		fMinDistances[i_tc][i_t] = MinDistanceStartPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second]);
	      if(MinDistanceEndPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second])<fMinDistances[i_tc][i_t])
		fMinDistances[i_tc][i_t] = MinDistanceEndPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second]);
	      
	      if(MinDistanceStartPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second])<fIsolation ||
		 MinDistanceEndPt(tracksVec[i_tc][i_t],tracksVec[i_tr.first][i_tr.second])<fIsolation){
		if(!track_linked) track_linked=true;
		fTrackContainmentLevel[i_tc][i_t] = containment_level;
		fTrackContainmentIndices.back().emplace_back(i_tc,i_t);
		//std::cout << "\tTrackPair (" << i_tc << "," << i_t << ") and (" << i_tr.first << "," << i_tr.second << ")"
		//	  << " " << containment_level << std::endl;
	      }
	    }
	  }
	
      }
    }
  }

  for(size_t i_tc=0; i_tc<tracksVec.size(); ++i_tc){
    for(size_t i_t=0; i_t<tracksVec[i_tc].size(); ++i_t){
      fTrackTreeObj = TrackTree_t(tracksVec[i_tc][i_t]);
      fDistance = fMinDistances[i_tc][i_t];
      fCollection = i_tc;
      fTrkID = i_t;
      fContainment = fTrackContainmentLevel[i_tc][i_t];

      fTrackTree->Fill();
      
      if(fTrackContainmentLevel[i_tc][i_t]<0){
	//std::cout << "Track (" << i_tc << "," << i_t << ")" 
	//	  << " " << fTrackContainmentLevel[i_tc][i_t]
	//	  << " " << minDistances[i_tc][i_t] << std::endl;

	//std::cout << "\tS_(X,Y,Z) = ("
	//	  << tracksVec[i_tc][i_t].Vertex().X() << ","
	//	  << tracksVec[i_tc][i_t].Vertex().Y() << ","
	//	  << tracksVec[i_tc][i_t].Vertex().Z() << ")" << std::endl;
	//std::cout << "\tNearest wire ..." << std::endl;
	//for(size_t i_p=0; i_p<geo.Nplanes(); ++i_p)
	//std::cout << "\t\tPlane " << i_p << " " << geo.NearestWireID(tracksVec[i_tc][i_t].Vertex(),i_p).Wire << std::endl;
	//std::cout << "\tE_(X,Y,Z) = ("
	//	  << tracksVec[i_tc][i_t].End().X() << ","
	//	  << tracksVec[i_tc][i_t].End().Y() << ","
	//	  << tracksVec[i_tc][i_t].End().Z() << ")" << std::endl;
	//std::cout << "\tNearest wire ..." << std::endl;
	//for(size_t i_p=0; i_p<geo.Nplanes(); ++i_p)
	//std::cout << "\t\tPlane " << i_p << " " << geo.NearestWireID(tracksVec[i_tc][i_t].End(),i_p).Wire << std::endl;
	//std::cout << "\tLength=" << tracksVec[i_tc][i_t].Length() << std::endl;
	//std::cout << "\tSimple_length=" << (tracksVec[i_tc][i_t].End()-tracksVec[i_tc][i_t].Vertex()).Mag() << std::endl;
      }
    }
  }
}
#endif
