import FWCore.ParameterSet.Config as cms

p4Set = cms.untracked.PSet(
    pt = cms.string("pt"),
    eta = cms.string("eta"),
    phi = cms.string("phi"),
    m = cms.string("mass"),
    q = cms.string("charge"),
    pdgId = cms.string("pdgId"),
)


zMuMu = cms.PSet(
    src = cms.InputTag("flatZMuMu"),
    exprs = p4Set.clone(),
    vmaps = cms.untracked.vstring("lep1Pt","lep2Pt","lep1Eta","lep2Eta","lep1Phi","lep2Phi"),
)
zElEl = cms.PSet(
    src = cms.InputTag("flatZElEl"),
    exprs = p4Set.clone(),
    vmaps = cms.untracked.vstring("lep1Pt","lep2Pt","lep1Eta","lep2Eta","lep1Phi","lep2Phi"),
)

