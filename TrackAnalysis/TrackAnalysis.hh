/**
 * \file TrackAnalysis.h
 *
 * 
 * @author wketchum
*/

#ifndef TRK_TRACKANALYSIS_H
#define TRK_TRACKANALYSIS_H

#include <string>
#include <vector>

#include "RecoBase/Track.h"

#include "TTree.h"
#include "fhiclcpp/ParameterSet.h"

namespace geo{ class GeometryCore; }

namespace trk{
  class TrackAnalysis;

  typedef struct TrackTree{

    TrackTree(){}
    
    TrackTree(recob::Track const& t):
      start_x(t.Vertex().X()),start_y(t.Vertex().Y()),start_z(t.Vertex().Z()),
      start_theta(t.VertexDirection().Theta()),start_phi(t.VertexDirection().Phi()),
      end_x(t.End().X()),end_y(t.End().Y()),end_z(t.End().Z()),
      end_theta(t.EndDirection().Theta()),end_phi(t.EndDirection().Phi()),
      length(t.Length()),length_simple( (t.End()-t.Vertex()).Mag() ){}

    double start_x;
    double start_y;
    double start_z;
    double start_theta;
    double start_phi;
    double end_x;
    double end_y;
    double end_z;
    double end_theta;
    double end_phi;
    double length;
    double length_simple;
    
  } TrackTree_t;
  
}


class trk::TrackAnalysis{
  
public:
  
  /// Default constructor
  TrackAnalysis();

  /// Default destructor
  virtual ~TrackAnalysis(){};

  void SetupOutputTree(TTree*);

  void Configure(fhicl::ParameterSet const&);
  
  void ProcessTracks(std::vector< std::vector<recob::Track> > const&,
		     geo::GeometryCore const& );
  
 private:

  double      fZBuffer;
  double      fYBuffer;
  double      fXBuffer;
  double      fIsolation;
  
  TTree*      fTrackTree;
  TrackTree_t fTrackTreeObj;
  std::vector< std::vector<int> > fTrackContainmentLevel;
  std::vector< std::vector< std::pair<int,int> > > fTrackContainmentIndices;

  bool IsContained(recob::Track const&, geo::GeometryCore const&);

  double MinDistanceStartPt(recob::Track const&, recob::Track const&);
  double MinDistanceEndPt(recob::Track const&, recob::Track const&);
  
};

#endif
