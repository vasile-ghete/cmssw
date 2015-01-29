import FWCore.ParameterSet.Config as cms

#Full Event content
RecoBTagFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_impactParameterTagInfos_*_*',
        'keep *_trackCountingHighEffBJetTags_*_*',
        'keep *_trackCountingHighPurBJetTags_*_*',
        'keep *_jetProbabilityBJetTags_*_*',
        'keep *_jetBProbabilityBJetTags_*_*',
        'keep *_secondaryVertexTagInfos_*_*',
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*',
        'keep *_ghostTrackVertexTagInfos_*_*',
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_combinedSecondaryVertexBJetTags_*_*',
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_ghostTrackBJetTags_*_*',
        'keep *_softPFMuonsTagInfos_*_*',
        'keep *_softPFElectronsTagInfos_*_*',
        'keep *_softPFElectronBJetTags_*_*',
        'keep *_softPFMuonBJetTags_*_*',
        'keep *_softMuonTagInfos_*_*',
        'keep *_softMuonBJetTags_*_*',
        'keep *_softMuonByIP3dBJetTags_*_*',
        'keep *_softMuonByPtBJetTags_*_*',
        'keep *_combinedMVABJetTags_*_*',
        'keep *_pfImpactParameterTagInfos_*_*',
        'keep *_pfTrackCountingHighEffBJetTags_*_*',
        'keep *_pfTrackCountingHighPurBJetTags_*_*',
        'keep *_pfJetProbabilityBJetTags_*_*',
        'keep *_pfJetBProbabilityBJetTags_*_*',
        'keep *_pfSecondaryVertexTagInfos_*_*',
        'keep *_pfInclusiveSecondaryVertexFinderTagInfos_*_*',
        'keep *_pfSimpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_pfSimpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*',
        'keep *_pfCombinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_pfCombinedMVABJetTags_*_*',
        'keep *_inclusiveCandidateSecondaryVertices_*_*'
    )
)
#RECO content
RecoBTagRECO = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_impactParameterTagInfos_*_*',
        'keep *_trackCountingHighEffBJetTags_*_*',
        'keep *_trackCountingHighPurBJetTags_*_*',
        'keep *_jetProbabilityBJetTags_*_*',
        'keep *_jetBProbabilityBJetTags_*_*',
        'keep *_secondaryVertexTagInfos_*_*',
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*',
        'keep *_ghostTrackVertexTagInfos_*_*',
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_combinedSecondaryVertexBJetTags_*_*',
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_ghostTrackBJetTags_*_*',
        'keep *_softPFMuonsTagInfos_*_*',
        'keep *_softPFElectronsTagInfos_*_*',
        'keep *_softPFElectronBJetTags_*_*',
        'keep *_softPFMuonBJetTags_*_*',
        'keep *_softMuonTagInfos_*_*',
        'keep *_softMuonBJetTags_*_*',
        'keep *_softMuonByIP3dBJetTags_*_*',
        'keep *_softMuonByPtBJetTags_*_*',
        'keep *_combinedMVABJetTags_*_*',
        'keep *_pfImpactParameterTagInfos_*_*',
        'keep *_pfTrackCountingHighEffBJetTags_*_*',
        'keep *_pfTrackCountingHighPurBJetTags_*_*',
        'keep *_pfJetProbabilityBJetTags_*_*',
        'keep *_pfJetBProbabilityBJetTags_*_*',
        'keep *_pfSecondaryVertexTagInfos_*_*',
        'keep *_pfInclusiveSecondaryVertexFinderTagInfos_*_*',
        'keep *_pfSimpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_pfSimpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*',
        'keep *_pfCombinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_pfCombinedMVABJetTags_*_*',
        'keep *_inclusiveCandidateSecondaryVertices_*_*'
    )
)
#AOD content
RecoBTagAOD = cms.PSet(
    outputCommands = cms.untracked.vstring(
#        'keep *_impactParameterTagInfos_*_*',
        'keep *_trackCountingHighEffBJetTags_*_*',
        'keep *_trackCountingHighPurBJetTags_*_*',
        'keep *_jetProbabilityBJetTags_*_*',
        'keep *_jetBProbabilityBJetTags_*_*',
#        'keep *_secondaryVertexTagInfos_*_*',
#        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*',
#        'keep *_ghostTrackVertexTagInfos_*_*',
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_combinedSecondaryVertexBJetTags_*_*',
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_ghostTrackBJetTags_*_*',
#        'keep *_softPFMuonsTagInfos_*_*',
#        'keep *_softPFElectronsTagInfos_*_*',
        'keep *_softPFElectronBJetTags_*_*',
        'keep *_softPFMuonBJetTags_*_*',
#        'keep *_softMuonTagInfos_*_*',
        'keep *_softMuonBJetTags_*_*',
        'keep *_softMuonByIP3dBJetTags_*_*',
        'keep *_softMuonByPtBJetTags_*_*',
        'keep *_combinedMVABJetTags_*_*',
#        'keep *_pfImpactParameterTagInfos_*_*',
        'keep *_pfTrackCountingHighEffBJetTags_*_*',
        'keep *_pfTrackCountingHighPurBJetTags_*_*',
        'keep *_pfJetProbabilityBJetTags_*_*',
        'keep *_pfJetBProbabilityBJetTags_*_*',
#        'keep *_pfSecondaryVertexTagInfos_*_*',
#        'keep *_pfInclusiveSecondaryVertexFinderTagInfos_*_*',
        'keep *_pfSimpleSecondaryVertexHighEffBJetTags_*_*',
        'keep *_pfSimpleSecondaryVertexHighPurBJetTags_*_*',
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*',
        'keep *_pfCombinedInclusiveSecondaryVertexV2BJetTags_*_*',
        'keep *_pfCombinedMVABJetTags_*_*',
        'keep *_inclusiveCandidateSecondaryVertices_*_*'
    )
)
