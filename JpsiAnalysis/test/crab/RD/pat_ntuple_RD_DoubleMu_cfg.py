import FWCore.ParameterSet.Config as cms
runOnMC = False

from KrAFT.Configuration.customise_cff import *
process = initialise(decayMode="MuMu", runOnMC=runOnMC)
addNtupleStep(process, runOnMC=runOnMC)
#process.event.muon.src=cms.InputTag("goodMuonsForJpsi")
#process.event.muon.src=cms.InputTag("goodElectronsForJpsi")

process.maxEvents.input = -1
