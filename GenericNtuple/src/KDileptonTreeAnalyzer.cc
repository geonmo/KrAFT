#include "KrAFT/GenericNtuple/interface/KDileptonTreeAnalyzer.h"
#include<boost/iterator/counting_iterator.hpp>
#include<boost/range/algorithm/sort.hpp>

using namespace std;

struct GreaterByBtag
{
  GreaterByBtag(std::vector<double>* x):x_(x) {};
  bool operator()(size_t i, size_t j) { return x_->at(i) > x_->at(j); }
  std::vector<double>* x_;
};

KDileptonTreeAnalyzer::KDileptonTreeAnalyzer(const std::string modeName,
                                           const std::string inputFileName,
                                           const std::string outputFileName):
  KFlatTreeAnalyzerBase(modeName, inputFileName, outputFileName)
{
  if ( !event_ ) return;

  outTree_->Branch("nVertex", &nVertex_, "nVertex/I");

  if ( modeName_ == "MuMu" )
  {
    mode_ = 1;
    leptons1_pt_  = leptons2_pt_  = event_->fVars_["muons_pt" ];
    leptons1_eta_ = leptons2_eta_ = event_->fVars_["muons_eta"];
    leptons1_phi_ = leptons2_phi_ = event_->fVars_["muons_phi"];
    leptons1_m_   = leptons2_m_   = event_->fVars_["muons_m"  ];
    leptons1_Q_   = leptons2_Q_   = event_->iVars_["muons_Q"  ];
    leptons1_iso_ = leptons2_iso_ = event_->fVars_["muons_relIso"];
  }
  else if ( modeName_ == "ElEl")
  {
    mode_ = 2;
    leptons1_pt_  = leptons2_pt_  = event_->fVars_["electrons_pt" ];
    leptons1_eta_ = leptons2_eta_ = event_->fVars_["electrons_eta"];
    leptons1_phi_ = leptons2_phi_ = event_->fVars_["electrons_phi"];
    leptons1_m_   = leptons2_m_   = event_->fVars_["electrons_m"  ];
    leptons1_Q_   = leptons2_Q_   = event_->iVars_["electrons_Q"  ];
    leptons1_iso_ = leptons2_iso_ = event_->fVars_["electrons_relIso"];
  }
  else
  {
    mode_ = 3;

    leptons1_pt_  = event_->fVars_["muons_pt" ];
    leptons1_eta_ = event_->fVars_["muons_eta"];
    leptons1_phi_ = event_->fVars_["muons_phi"];
    leptons1_m_   = event_->fVars_["muons_m"  ];
    leptons1_Q_   = event_->iVars_["muons_Q"  ];
    leptons1_iso_ = event_->fVars_["muons_relIso"];

    leptons2_pt_  = event_->fVars_["electrons_pt" ];
    leptons2_eta_ = event_->fVars_["electrons_eta"];
    leptons2_phi_ = event_->fVars_["electrons_phi"];
    leptons2_m_   = event_->fVars_["electrons_m"  ];
    leptons2_Q_   = event_->iVars_["electrons_Q"  ];
    leptons2_iso_ = event_->fVars_["electrons_relIso"];
  }

  outTree_->Branch("lepton1_pt" , &lepton1_pt_ , "lepton1_pt/D" );
  //outTree_->Branch("lepton1_eta", &lepton1_eta_, "lepton1_eta/D");
  //outTree_->Branch("lepton1_phi", &lepton1_phi_, "lepton1_phi/D");
  outTree_->Branch("lepton1_iso", &lepton1_iso_, "lepton1_iso/D");

  outTree_->Branch("lepton2_pt" , &lepton2_pt_ , "lepton2_pt/D" );
  //outTree_->Branch("lepton2_eta", &lepton2_eta_, "lepton2_eta/D");
  //outTree_->Branch("lepton2_phi", &lepton2_phi_, "lepton2_phi/D");
  outTree_->Branch("lepton2_iso", &lepton2_iso_, "lepton2_iso/D");

  outTree_->Branch("z_m" , &z_m_ , "z_m/D" );
  outTree_->Branch("z_pt", &z_pt_, "z_pt/D");
  outTree_->Branch("z_Q" , &z_Q_ , "z_Q/I" );
  outTree_->Branch("z_dphi", &z_dphi_, "z_dphi/D");

  outTree_->Branch("met_pt" , &met_pt_ , "met_pt/D" );
  outTree_->Branch("met_phi", &met_phi_, "met_phi/D");
  outTree_->Branch("metJESUp_pt" , &metJESUp_pt_ , "metJESUp_pt/D" );
  outTree_->Branch("metJESUp_phi", &metJESUp_phi_, "metJESUp_phi/D");
  outTree_->Branch("metJESDn_pt" , &metJESDn_pt_ , "metJESDn_pt/D" );
  outTree_->Branch("metJESDn_phi", &metJESDn_phi_, "metJESDn_phi/D");

  jets_pt_   = new doubles;
  jetsUp_pt_ = new doubles;
  jetsDn_pt_ = new doubles;
  outTree_->Branch("jets_pt"  , &jets_pt_   );
  outTree_->Branch("jetsUp_pt", &jetsUp_pt_ );
  outTree_->Branch("jetsDn_pt", &jetsDn_pt_ );

  jets_bTag_   = new doubles;
  jetsUp_bTag_ = new doubles;
  jetsDn_bTag_ = new doubles;
  outTree_->Branch("jets_bTag"  , &jets_bTag_  );
  outTree_->Branch("jetsUp_bTag", &jetsUp_bTag_);
  outTree_->Branch("jetsDn_bTag", &jetsDn_bTag_);

  outTree_->Branch("bjets_n"  , &bjets_n_  , "bjets_n/i"  );
  outTree_->Branch("bjetsUp_n", &bjetsUp_n_, "bjestUp_n/i");
  outTree_->Branch("bjetsDn_n", &bjetsDn_n_, "bjetsDn_n/i");

  outTree_->Branch("st", &st_, "st/D");
  outTree_->Branch("lb1_m", &lb1_m_, "lb1_m/D");
  outTree_->Branch("lb2_m", &lb2_m_, "lb2_m/D");
  outTree_->Branch("ttbar_vsumM", &ttbar_vsumM_, "ttbar_vsumM/D");

  if ( isMC_ )
  {
    outTree_->Branch("decayMode", &decayMode_, "decayMode/I");

    outTree_->Branch("puWeight", &puWeight_, "puWeight/D");
    outTree_->Branch("puWeightUp", &puWeightUp_, "puWeightUp/D");
    outTree_->Branch("puWeightDn", &puWeightDn_, "puWeightDn/D");

    outTree_->Branch("metJER_pt" , &metJER_pt_ , "metJER_pt/D" );
    outTree_->Branch("metJER_phi", &metJER_phi_, "metJER_phi/D");
    outTree_->Branch("metJERUp_pt" , &metJERUp_pt_ , "metJERUp_pt/D" );
    outTree_->Branch("metJERUp_phi", &metJERUp_phi_, "metJERUp_phi/D");
    outTree_->Branch("metJERDn_pt" , &metJERDn_pt_ , "metJERDn_pt/D" );
    outTree_->Branch("metJERDn_phi", &metJERDn_phi_, "metJERDn_phi/D");

    jetsResUp_pt_ = new doubles;
    jetsResDn_pt_ = new doubles;
    outTree_->Branch("jetsResUp_pt", &jetsUp_pt_);
    outTree_->Branch("jetsResDn_pt", &jetsUp_pt_);

    jetsResUp_bTag_ = new doubles;
    jetsResDn_bTag_ = new doubles;
    outTree_->Branch("jetsResUp_bTag", &jetsUp_bTag_);
    outTree_->Branch("jetsResDn_bTag", &jetsUp_bTag_);

    outTree_->Branch("bjetsResUp_n", &bjetsResUp_n_, "bjetsResUp_n/i");
    outTree_->Branch("bjetsResDn_n", &bjetsResDn_n_, "bjetsResDn_n/i");
  }
}

bool KDileptonTreeAnalyzer::analyze()
{
  // Initialize tree
  z_Q_ = -999;
  jets_pt_  ->clear(); jets_bTag_  ->clear();
  jetsUp_pt_->clear(); jetsUp_bTag_->clear();
  jetsDn_pt_->clear(); jetsDn_bTag_->clear();
  bjets_n_ = 0;
  bjetsUp_n_ = 0;
  bjetsDn_n_ = 0;
  st_ = 0;
  if ( isMC_ )
  {
    decayMode_ = 0;
    jetsResUp_pt_->clear(); jetsResUp_bTag_->clear();
    jetsResDn_pt_->clear(); jetsResDn_bTag_->clear();
    bjetsResUp_n_ = 0;
    bjetsResDn_n_ = 0;
  }

  // Fill common variables
  nVertex_ = event_->nVertex_;

  // Select leptons
  LorentzVector lepton1P4, lepton2P4, zP4;
  for ( int i=0, n=leptons1_pt_->size(); i<n; ++i )
  {
    if ( mode_ != 2 )
    {
      if ( event_->fVars_["muons_type"]->at(i) == 0 ) continue; // MuMu or MuEl
    }
    else if ( event_->iVars_["electrons_type"]->at(i) < 100 ) continue; // ElEl mode

    lepton1_pt_  = leptons1_pt_ ->at(i);
    lepton1_eta_ = leptons1_eta_->at(i);
    if ( lepton1_pt_ < 20 or std::abs(lepton1_eta_) > 2.5 ) continue;

    lepton1_phi_ = leptons1_phi_->at(i);
    const double lepton1M = leptons1_m_->at(i);
    const int lepton1Q = leptons1_Q_->at(i);

    lepton1P4.SetPtEtaPhiM(lepton1_pt_, lepton1_eta_, lepton1_phi_, lepton1M);
    lepton1_iso_ = leptons1_iso_->at(i);

    for ( int j=((mode_ == 3) ? 0 : i+1), m=leptons2_pt_->size(); j<m; ++j )
    {
      if ( mode_ == 1 )
      {
        if ( event_->fVars_["muons_type"]->at(j) == 0 ) continue; // MuMu mode
      }
      else if ( event_->iVars_["electrons_type"]->at(j) < 100 ) continue; // ElEl and MuEl

      lepton2_pt_  = leptons2_pt_->at(j);
      lepton2_eta_ = leptons2_eta_->at(j);
      if ( lepton2_pt_ < 20 or std::abs(lepton2_eta_) > 2.5 ) continue;

      lepton2_phi_ = leptons2_phi_->at(j);
      const double lepton2M = leptons2_m_->at(j);
      const int lepton2Q = leptons2_Q_->at(j);

      lepton2P4.SetPtEtaPhiM(lepton2_pt_, lepton2_eta_, lepton2_phi_, lepton2M);
      lepton2_iso_ = leptons2_iso_->at(j);

      zP4 = lepton1P4 + lepton2P4;
      z_Q_ = lepton1Q + lepton2Q;

      break;
    }
    break;
  }
  if ( z_Q_ == -999 ) return false;
  z_pt_ = zP4.Pt();
  z_m_  = zP4.M();
  z_dphi_ = lepton1P4.DeltaPhi(lepton2P4);

  // Run lepton loop again for st calculation
  for ( int i=0, n=event_->fVars_["muons_pt"]->size(); i<n; ++i )
  {
    const double muonPt = event_->fVars_["muons_pt"]->at(i);
    const double muonEta = abs(event_->fVars_["muons_eta"]->at(i));
    if ( muonPt < 20 or muonEta > 2.5 ) continue;
    st_ += muonPt;
  }
  for ( int i=0, n=event_->fVars_["electrons_pt"]->size(); i<n; ++i )
  {
    const double electronPt = event_->fVars_["electrons_pt"]->at(i);
    const double electronEta = abs(event_->fVars_["electrons_eta"]->at(i));
    if ( electronPt < 20 or electronEta > 2.5 ) continue;
    st_ += event_->fVars_["electrons_pt"]->at(i);
  }

  met_pt_  = event_->met_pt_ ;
  met_phi_ = event_->met_phi_;
  metJESUp_pt_  = event_->metJESUp_pt_ ;
  metJESUp_phi_ = event_->metJESUp_phi_;
  metJESDn_pt_  = event_->metJESDn_pt_ ;
  metJESDn_phi_ = event_->metJESDn_phi_;
  if ( isMC_ )
  {
    metJER_pt_  = event_->metJER_pt_ ;
    metJER_phi_ = event_->metJER_phi_;
    metJERUp_pt_  = event_->metJERUp_pt_ ;
    metJERUp_phi_ = event_->metJERUp_phi_;
    metJERDn_pt_  = event_->metJERDn_pt_ ;
    metJERDn_phi_ = event_->metJERDn_phi_;
  }
  TLorentzVector metP4;
  metP4.SetPtEtaPhiM(met_pt_, 0, met_phi_, 0);
  st_ += met_pt_;

  if ( isMC_ )
  {
    // Decay mode
    unsigned int nTop = 0, nMuon = 0, nElectron = 0, nTau = 0;
    for ( int i=0, n=event_->iVars_["genParticles_pdgId"]->size(); i<n; ++i )
    {
      const int pdgId = abs(event_->iVars_["genParticles_pdgId"]->at(i));
      if ( pdgId == 6 ) ++nTop;
      else if ( pdgId == 11 ) ++nElectron;
      else if ( pdgId == 13 ) ++nMuon;
      else if ( pdgId == 15 ) ++nTau;
    }
    if ( nTop == 2 )
    {
      if ( nMuon == 2 ) decayMode_ = 1;
      else if ( nElectron == 2 ) decayMode_ = 2;
      else if ( nMuon == 1 and nElectron == 1 ) decayMode_ = 3;
      else decayMode_ = 4;
    }

    // Weights
    puWeight_ = event_->puWeight_;
    puWeightUp_ = event_->puWeightUp_;
    puWeightDn_ = event_->puWeightDn_;
  }

  // Get jet indices by bTag
  const int nJets = event_->fVars_["jets_pt"]->size();
  std::vector<int> jetIndices(nJets);
  std::copy(boost::counting_iterator<int>(0),
            boost::counting_iterator<int>(nJets), jetIndices.begin());
  GreaterByBtag greaterByBtag(event_->fVars_["jets_bTag"]);
  boost::sort(jetIndices, greaterByBtag);

  // Make jets four vector, insert in bTag-order
  std::vector<TLorentzVector> jets;
  for ( int i=0; i<nJets; ++i )
  {
    const int j = jetIndices[i];

    const double pt   = event_->fVars_["jets_pt"]->at(j);
    const double eta  = event_->fVars_["jets_eta"]->at(j);
    const double phi  = event_->fVars_["jets_phi"]->at(j);
    const double m    = event_->fVars_["jets_m"]->at(j);
    const double bTag = event_->fVars_["jets_bTag"]->at(j);

    jets_pt_->push_back(pt);
    jets_bTag_->push_back(bTag);

    st_ += pt;

    jets.push_back(LorentzVector());
    jets.back().SetPtEtaPhiM(pt, eta, phi, m);

  }

  ttbar_vsumM_ = -1;
  if ( jets.size() >= 2 )
  {
    const double lb1122_dphi = abs(lepton1P4.DeltaPhi(jets[0]))+abs(lepton2P4.DeltaPhi(jets[1]));
    const double lb1221_dphi = abs(lepton1P4.DeltaPhi(jets[1]))+abs(lepton2P4.DeltaPhi(jets[0]));
    if ( lb1122_dphi < lb1221_dphi )
    {
      lb1_m_ = (lepton1P4+jets[0]).M();
      lb2_m_ = (lepton2P4+jets[1]).M();
    }
    else
    {
      lb1_m_ = (lepton1P4+jets[1]).M();
      lb2_m_ = (lepton2P4+jets[0]).M();
    }
    ttbar_vsumM_ = (zP4+metP4+jets[0]+jets[1]).M();
  }

  return true;
}

