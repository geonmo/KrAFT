import FWCore.ParameterSet.Config as cms

flatCATDummy = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag(""),
    variables = cms.PSet(),
    selections = cms.PSet(),
)

flatCATMuons = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("catMuons"),
    variables = cms.PSet(
        relIso = cms.string("relIso()"),
        dxy = cms.string("dxy"),
        dz = cms.string("dz"),
    ),
    selections = cms.PSet(
        isTight = cms.string("isTightMuon"),
        isLoose = cms.string("isLooseMuon"),
        isSoft = cms.string("isSoftMuon"),
    ),
)
flatCATElectrons = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("catElectrons"),
    variables = cms.PSet(
        mva = cms.string("mva"),
        #relIso = cms.string("userIso(2)"),
        scEta = cms.string("scEta"),
        dxy = cms.string("dxy"),
        dz = cms.string("dz"),
    ),
    selections = cms.PSet(
        conversionVeto = cms.string("conversionVeto"),
        chargeIDFull = cms.string("chargeIDFull"),
        isPF = cms.string("isPF"),
    ),
)
"""
flatJets = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("goodJets"),
    variables = cms.PSet(
        bTagCSV = cms.string("bDiscriminator('combinedSecondaryVertexBJetTags')"),
    ),
    selections = cms.PSet(),
)

flatMETs = flatDummy.clone(src = cms.InputTag("patMETsPFlow"))

flatJpsiMuMu = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("jpsiToMuMu"),
    variables = cms.PSet(
      lxy = cms.InputTag("jpsiToMuMu", "lxy"),
      l3D = cms.InputTag("jpsiToMuMu", "l3D"),
      jetDR = cms.InputTag("jpsiToMuMu", "jetDR"),
      vProb = cms.InputTag("jpsiToMuMu", "vProb"),
    ),
    selections = cms.PSet(),
)
flatJpsiElEl = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("jpsiToElEl"),
    variables = cms.PSet(
      lxy = cms.InputTag("jpsiToElEl", "lxy"),
      l3D = cms.InputTag("jpsiToElEl", "l3D"),
      jetDR = cms.InputTag("jpsiToElEl", "jetDR"),
      vProb = cms.InputTag("jpsiToElEl", "vProb"),
    ),
    selections = cms.PSet(),
)

flatPseudoTopLepton = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("pseudoTop", "leptons"),
    variables = cms.PSet(),
    selections = cms.PSet(),
)

flatPseudoTopNu = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("pseudoTop", "neutrinos"),
    variables = cms.PSet(),
    selections = cms.PSet(),
)

flatPseudoTopJet = cms.EDProducer("FlatCandProducer",
    src = cms.InputTag("pseudoTop", "jets"),
    variables = cms.PSet(),
    selections = cms.PSet(),
)
"""
