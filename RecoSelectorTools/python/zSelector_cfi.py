import FWCore.ParameterSet.Config as cms

ZToMuMuCandidates = cms.EDProducer("CandViewCombiner",
    decay = cms.string("flatMuons@+ flatMuons@-"),
    cut = cms.string("86.0 < mass < 96.0"),
    name = cms.string("ZToMuMuCandidates"),
    roles = cms.vstring('muon1','muon2'),
)
ZToElElCandidates = cms.EDProducer("CandViewCombiner",
    decay = cms.string("flatElectrons@+ flatElectrons@-"),
    cut = cms.string("86.0 < mass < 96.0"),
    name = cms.string("ZToElElCandidates"),
    roles = cms.vstring('electron1','electron2'),
)



