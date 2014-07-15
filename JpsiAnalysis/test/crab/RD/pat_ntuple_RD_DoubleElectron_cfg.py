import FWCore.ParameterSet.Config as cms
runOnMC = False

from KrAFT.Configuration.customise_cff import *
process = initialise(decayMode="ElEl", runOnMC=runOnMC)
addNtupleStep(process, runOnMC=runOnMC)


process.maxEvents.input = -1

