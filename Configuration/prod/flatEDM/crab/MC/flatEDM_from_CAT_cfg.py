import FWCore.ParameterSet.Config as cms

runOnMC = True

from KrAFT.Configuration.customise_cff import *
process = initialize(runOnMC)
#customisePAT(process, runOnMC=runOnMC, outputModules=[])

process.source.fileNames = [
    'file:/pnfs/user/jlee/data/ttbar_catTuple.root'
]

process.load("KrAFT.GenericNtuple.flatCATCands_cfi")
#process.GEN = cms.Path(
  #  process.pseudoTop
  #+ process.partons
  #* process.flatPseudoTopLepton + process.flatPseudoTopNu + process.flatPseudoTopJet
#)


process.CANDSEL = cms.Path(
  process.flatCATMuons
  #process.flatEventInfo+flatMuons
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.output = cms.EndPath(process.out)




