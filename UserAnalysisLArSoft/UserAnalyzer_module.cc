////////////////////////////////////////////////////////////////////////
// Class:       UserAnalyzer
// Module Type: analyzer
// File:        UserAnalyzer_module.cc
//
// Generated at Tue Nov 10 13:06:09 2015 by Wesley Ketchum using artmod
// from cetpkgsupport v1_08_07.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


#include "UserAnalysis/UserAnalysis.hh"

namespace test {
  class UserAnalyzer;
}

class test::UserAnalyzer : public art::EDAnalyzer {
public:
  explicit UserAnalyzer(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  UserAnalyzer(UserAnalyzer const &) = delete;
  UserAnalyzer(UserAnalyzer &&) = delete;
  UserAnalyzer & operator = (UserAnalyzer const &) = delete;
  UserAnalyzer & operator = (UserAnalyzer &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;

  // Selected optional functions.
  void reconfigure(fhicl::ParameterSet const & p) override;

private:

  // Declare member data here.
  test::UserAnalysis fMyAnalysisObj;
  
};


test::UserAnalyzer::UserAnalyzer(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{}

void test::UserAnalyzer::analyze(art::Event const & e)
{
  fMyAnalysisObj.RunAnalysis();
}

void test::UserAnalyzer::reconfigure(fhicl::ParameterSet const & p)
{
  // Implementation of optional member function here.
}

DEFINE_ART_MODULE(test::UserAnalyzer)
