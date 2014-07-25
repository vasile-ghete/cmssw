import FWCore.ParameterSet.Config as cms

l1GtPsbToDigiColl = cms.EDProducer("L1GtPsbToDigiColl",
    
    # input tag for GT readout collection: 
    #     gtDigis = GT emulator
    InputTagGlobalTrigger = cms.InputTag("gtDigis"),
        
    # logical flags to produce the GCT collections, the external conditions and 
    # the technical trigger collections
    #     if flag is true, produce the corresponding collection
    #
    ProduceIsoEm = cms.bool(True),
    ProduceNonIsoEm = cms.bool(True),
    ProduceCenJets = cms.bool(True),
    ProduceTauJets = cms.bool(True),
    ProduceForJets = cms.bool(True),
    ProduceEtHad = cms.bool(True),
    ProduceEtMiss = cms.bool(True),
    ProduceEtTotal = cms.bool(True),
    ProduceHFRingEtSums = cms.bool(True),
    ProduceHFBitCounts = cms.bool(True),
    ProduceExternalConditions = cms.bool(True),
    ProduceTechnicalTriggers = cms.bool(True)   
)


