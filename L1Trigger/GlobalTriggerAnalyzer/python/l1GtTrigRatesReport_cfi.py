import FWCore.ParameterSet.Config as cms

l1GtTrigRatesReport = cms.EDAnalyzer("L1GtTrigRatesReport",
                                
    # boolean flag to select the input record
    # if true, it will use L1GlobalTriggerRecord 
    UseL1GlobalTriggerRecord = cms.bool(False),

    # input tag for L1T-GT record used for the rate report
    #   GT emulator from MC:    gtDigis (DAQ record)
    #   GT unpacker:            gtDigis (DAQ record)
    #   GT lite record:         l1GtRecord 
    #   GT emulator run in job: rateL1TGtEmulator
    # 
    L1GtRecordInputTag = cms.InputTag("gtDigis"),
    
    # flag to run the L1T-GT emulator before the rate report
    # if False, the emulator is not run
    # if True, the emulator is run
    RunL1TGtEmulator = cms.bool(False),


    # length of LS in number of 2^{LsLengthOrbits}, e.g. 2^{18} orbits
    # time lengths computed using LHC rotation frequency f_R = c/l_LHC
    #
    # TODO number of orbits in the event setup?
    # 
    LsLengthOrbits = cms.int32(18),
    
    # beam characteristics
    # 25 ns: 2508 bunches
    # 50 ns: 1368 bunches
    #
    # if negative, read it from data
    NumberBunches = cms.int32(2508),   
    
    # Sample characteristics
    #
    # sample identifier
    #
    # sample type: data or MC
    # for data, one LS has fixed length given above (in orbits)
    # for MC, one has to convert from cross section to equivalent of LS
    #
    # cross section [mb]
    #
    # samplePU - the pile-up used to generate the sample. 
    # If negative, one computes the pile-up from the sample
    Sample=cms.PSet(
        SampleIdentifier = cms.string("207454_RAW"),
        SampleIsData = cms.bool(True),
        SampleCrossSection = cms.double(0.),
        SamplePU = cms.double(0.)
    ),

    # correct for deadtime using luminosity values for data
    # 
    Corr4Deadtime = cms.bool(False),

    # print settings
    #
    # print detailed LS reports
    PrintLsReports =  cms.untracked.bool(False),
    #
    # print detailed run reports
    PrintRunReports =  cms.untracked.bool(True),
    #
    # print detailed job reports
    PrintJobReports =  cms.untracked.bool(False),
    
   
    # print verbosity
    #   Level 0 Physics Partition:  TriggerKey, AlgorithmKey, Passed, Rejected, Error
    #   Level 1 Physics Partition:  Level 0 + PrescaleFactors + Mask
    #   Level 2 Physics Partition:  Level 0, Efficiency
    #
    #   Level 10 Physics Partition: TriggerKey, AlgorithmKey, PrescaleFactors
    #                               Before masks: Passed, Rejected, 
    #                               Mask
    #                               After masks:  Passed, Rejected,
    #                               Error
    #
    #   ... 
    PrintVerbosity = cms.untracked.int32(1),
    
    # print output
    #   0 std::cout
    #   1 LogTrace
    #   2 LogVerbatim
    #   3 LogInfo
    PrintOutput = cms.untracked.int32(2)
    
)


