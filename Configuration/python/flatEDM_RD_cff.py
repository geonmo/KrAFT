import FWCore.ParameterSet.Config as cms

from KrAFT.RecoSelectorTools.leptonSelector_cfi import *
from KrAFT.RecoSelectorTools.jetSelector_cfi import *
from KrAFT.RecoSelectorTools.jpsiSelector_cfi import *
from KrAFT.RecoSelectorTools.zSelector_cfi import *
from KrAFT.GenericNtuple.flatEventInfo_cfi import *
from KrAFT.GenericNtuple.flatCands_cfi import *

analysisObjectSequence = cms.Sequence(
    goodMuons + goodElectrons * goodJets
  * jpsiToMuMu + jpsiToElEl
  * ZToMuMuCandidates +ZToElElCandidates

  + flatEventInfo
  * flatMuons + flatElectrons + flatJets
  + flatMETs + flatMETsUp + flatMETsDn
  + flatJpsiMuMu + flatJpsiElEl
  + flatZMuMu + flatZElEl
)

goodJets.isMC = False
delattr(flatJets.variables, "res")
delattr(flatJets.variables, "resUp")
delattr(flatJets.variables, "resDn")
