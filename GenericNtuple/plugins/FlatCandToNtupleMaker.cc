#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "TTree.h"
#include "TH1F.h"

#include <memory>
#include <vector>
#include <string>

using namespace std;
using namespace edm;

class FlatCandToNtupleMaker : public edm::EDAnalyzer
{
public:
  FlatCandToNtupleMaker(const edm::ParameterSet& pset);
  ~FlatCandToNtupleMaker() {};

  void analyze(const edm::Event& event, const edm::EventSetup& eventSetup);
  void endLuminosityBlock(const edm::LuminosityBlock& lumi, const edm::EventSetup& eventSetup);

private:
  typedef edm::ParameterSet PSet;
  typedef std::vector<double> doubles;
  typedef std::vector<std::string> strings;
  typedef std::vector<edm::InputTag> VInputTag;
  typedef StringObjectFunction<reco::Candidate,true> CandFtn;
  typedef StringCutObjectSelector<reco::Candidate,true> CandSel;

  std::vector<edm::InputTag> eventCounterLabels_;
  std::vector<edm::InputTag> intLabels_;
  std::vector<edm::InputTag> weightLabels_;
  std::vector<edm::InputTag> vWeightLabels_;
  std::vector<edm::InputTag> candLabels_;
  std::vector<std::vector<CandFtn> > exprs_;
  std::vector<std::vector<CandSel> > selectors_;
  std::vector<VInputTag> vmapLabels_;

  TH1F* hNEvent_;

  TTree* tree_;
  int runNumber_, lumiNumber_, eventNumber_;
  std::vector<int*> ints_;
  std::vector<double*> weights_;
  std::vector<doubles*> vWeights_;
  std::vector<std::vector<doubles*> > candVars_;

  struct FAILUREMODE
  {
    enum { KEEP, SKIP, ERROR };
  };
  int failureMode_;
};

FlatCandToNtupleMaker::FlatCandToNtupleMaker(const edm::ParameterSet& pset)
{
  std::string failureMode = pset.getUntrackedParameter<std::string>("failureMode", "keep");
  std::transform(failureMode.begin(), failureMode.end(), failureMode.begin(), ::tolower);
  if ( failureMode == "keep" ) failureMode_ = FAILUREMODE::KEEP;
  else if ( failureMode == "skip" ) failureMode_ = FAILUREMODE::SKIP;
  else if ( failureMode == "error" ) failureMode_ = FAILUREMODE::ERROR;
  else throw cms::Exception("ConfigError") << "select one from \"keep\", \"skip\", \"error\"\n";

  // Output histograms and tree
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("event", "event");

  tree_->Branch("run"  , &runNumber_  , "run/I"  );
  tree_->Branch("lumi" , &lumiNumber_ , "lumi/I" );
  tree_->Branch("event", &eventNumber_, "event/I");

  PSet intPSets = pset.getParameter<PSet>("int");
  const strings intNames = intPSets.getParameterNamesForType<PSet>();
  for ( auto& intName : intNames )
  {
    PSet intPSet = intPSets.getParameter<PSet>(intName);
    intLabels_.push_back(intPSet.getParameter<edm::InputTag>("src"));

    ints_.push_back(new int);
    tree_->Branch(intName.c_str(), ints_.back(), (intName+"/I").c_str());
  }

  PSet weightPSets = pset.getParameter<PSet>("weight");
  const strings weightNames = weightPSets.getParameterNamesForType<PSet>();
  for ( auto& weightName : weightNames )
  {
    PSet weightPSet = weightPSets.getParameter<PSet>(weightName);
    weightLabels_.push_back(weightPSet.getParameter<edm::InputTag>("src"));

    weights_.push_back(new double);
    tree_->Branch(weightName.c_str(), weights_.back(), (weightName+"/D").c_str());
  }

  PSet vWeightPSets = pset.getParameter<PSet>("vWeight");
  const strings vWeightNames = vWeightPSets.getParameterNamesForType<PSet>();
  for ( auto& vWeightName : vWeightNames )
  {
    PSet vWeightPSet = vWeightPSets.getParameter<PSet>(vWeightName);
    vWeightLabels_.push_back(vWeightPSet.getParameter<edm::InputTag>("src"));

    vWeights_.push_back(new doubles);
    tree_->Branch(vWeightName.c_str(), vWeights_.back());
  }

  PSet candPSets = pset.getParameter<PSet>("cands");
  const strings candNames = candPSets.getParameterNamesForType<PSet>();
  for ( auto& candName : candNames )
  {
    PSet candPSet = candPSets.getParameter<PSet>(candName);

    candLabels_.push_back(candPSet.getParameter<edm::InputTag>("src"));

    exprs_.push_back(std::vector<CandFtn>());
    selectors_.push_back(std::vector<CandSel>());
    vmapLabels_.push_back(VInputTag());
    candVars_.push_back(std::vector<doubles*>());
    const string candLabelName = candLabels_.back().label();
    const PSet exprSets = candPSet.getUntrackedParameter<PSet>("exprs", PSet());
    for ( auto& exprName : exprSets.getParameterNamesForType<string>() )
    {
      const string expr = exprSets.getParameter<string>(exprName);
      candVars_.back().push_back(new doubles);
      exprs_.back().push_back(CandFtn(expr));

      tree_->Branch((candName+"_"+exprName).c_str(), candVars_.back().back());
    }
    const PSet selectionSets = candPSet.getUntrackedParameter<PSet>("seletions", PSet());
    for ( auto& selectionName : selectionSets.getParameterNamesForType<string>() )
    {
      const string selection = selectionSets.getParameter<string>(selectionName);
      candVars_.back().push_back(new doubles);
      selectors_.back().push_back(CandSel(selection));

      tree_->Branch((candName+"_"+selectionName).c_str(), candVars_.back().back());
    }
    const strings vmapNames = candPSet.getUntrackedParameter<strings>("vmaps", strings());
    for ( auto& vmapName : vmapNames )
    {
      candVars_.back().push_back(new doubles);
      vmapLabels_.back().push_back(edm::InputTag(candLabelName, vmapName));

      tree_->Branch((candName+"_"+vmapName).c_str(), candVars_.back().back());
    }
  }

  const strings eventCounters = pset.getParameter<strings>("eventCounters");
  const size_t nEventCounter = eventCounters.size();
  hNEvent_ = fs->make<TH1F>("hNEvent", "NEvent", nEventCounter, 0, nEventCounter);
  for ( size_t i=0; i<nEventCounter; ++i )
  {
    hNEvent_->GetXaxis()->SetBinLabel(i+1, eventCounters[i].c_str());
    eventCounterLabels_.push_back(edm::InputTag(eventCounters[i]));
  }

}

void FlatCandToNtupleMaker::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
  typedef edm::View<reco::LeafCandidate> Cands;

  int nFailure = 0;

  runNumber_   = event.run();
  lumiNumber_  = event.luminosityBlock();
  eventNumber_ = event.id().event();

  for ( size_t i=0, n=intLabels_.size(); i<n; ++i )
  {
    edm::Handle<int> intHandle;
    event.getByLabel(intLabels_[i], intHandle);
    if ( intHandle.isValid() ) *ints_[i] = *intHandle;
    else
    {
      *ints_[i] = 0;
      ++nFailure;
    }
  }

  for ( size_t i=0, n=weightLabels_.size(); i<n; ++i )
  {
    edm::Handle<double> weightHandle;
    event.getByLabel(weightLabels_[i], weightHandle);
    if ( weightHandle.isValid() ) *weights_[i] = *weightHandle;
    else
    {
      *weights_[i] = 0;
      ++nFailure;
    }
  }

  for ( size_t i=0, n=vWeightLabels_.size(); i<n; ++i )
  {
    edm::Handle<doubles> vWeightHandle;
    event.getByLabel(vWeightLabels_[i], vWeightHandle);
    if ( vWeightHandle.isValid() )
    {
      vWeights_[i]->insert(vWeights_[i]->begin(), vWeightHandle->begin(), vWeightHandle->end());
    }
    else
    {
      ++nFailure;
    }
  }

  const size_t nCand = candLabels_.size();
  for ( size_t iCand=0; iCand < nCand; ++iCand )
  {
    edm::Handle<Cands> srcHandle;
    event.getByLabel(candLabels_[iCand], srcHandle);
    if ( !srcHandle.isValid() )
    {
      ++nFailure;
      continue;
    }

    const std::vector<CandFtn>& exprs = exprs_[iCand];
    const std::vector<CandSel>& selectors = selectors_[iCand];
    VInputTag& vmapLabels = vmapLabels_[iCand];
    const size_t nExpr = exprs.size();
    const size_t nSels = selectors.size();
    const size_t nVmap = vmapLabels.size();
    std::vector<edm::Handle<edm::ValueMap<double> > > vmapHandles(nVmap);
    for ( size_t iVar=0; iVar<nVmap; ++iVar )
    {
      event.getByLabel(vmapLabels[iVar], vmapHandles[iVar]);
    }

    for ( size_t i=0, n=srcHandle->size(); i<n; ++i )
    {
      edm::Ref<Cands> candRef(srcHandle, i);

      for ( size_t j=0; j<nExpr; ++j )
      {
        const double val = exprs[j](*candRef);
        candVars_[iCand][j]->push_back(val);
      }
      for ( size_t j=0; j<nSels; ++j )
      {
        const double val = selectors[j](*candRef);
        candVars_[iCand][j+nExpr]->push_back(val);
      }
      for ( size_t j=0; j<nVmap; ++j )
      {
        double val = 0;
        if ( vmapHandles[j].isValid() ) val = (*vmapHandles[j])[candRef];
        candVars_[iCand][j+nExpr+nSels]->push_back(val);
      }
    }
  }

  if ( failureMode_ == FAILUREMODE::KEEP ) tree_->Fill();
  //else if ( failureMode_ == FAILUREMODE::SKIP ); // don't fill and continue memory cleanup
  else if ( nFailure > 0 and failureMode_ == FAILUREMODE::ERROR )
  {
    edm::LogError("FlatCandToNtupleMaker") << "Failed to get " << nFailure << " items";
    throw cms::Exception("DataError") << "Cannot get object from data";
  }

  for ( auto& vWeight : vWeights_ )
  {
    vWeight->clear();
  }

  for ( size_t iCand=0; iCand<nCand; ++iCand )
  {
    const size_t nVar = candVars_[iCand].size();
    for ( size_t iVar=0; iVar<nVar; ++iVar )
    {
      candVars_[iCand][iVar]->clear();
    }
  }
}

void FlatCandToNtupleMaker::endLuminosityBlock(const edm::LuminosityBlock& lumi, const edm::EventSetup& eventSetup)
{
  for ( size_t i=0, n=eventCounterLabels_.size(); i<n; ++i )
  {
    edm::Handle<edm::MergeableCounter> eventCounterHandle;
    if ( lumi.getByLabel(eventCounterLabels_[i], eventCounterHandle) )
    {
      hNEvent_->Fill(i, double(eventCounterHandle->value));
    }
else cout << "CANNOT LOAD" << eventCounterLabels_[i] << endl;
  }
}


DEFINE_FWK_MODULE(FlatCandToNtupleMaker);

