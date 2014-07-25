#
# cfg file to compute the L1T rates
# 
# V M Ghete 2014-04-30

import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("L1GtRatesReport")

# choose number of events to run on, sample and global tag in UserOptions_cff
#
print '\n'
from L1Trigger.GlobalTriggerAnalyzer.UserOptions_cff import *
if errorUserOptions == True :
    print '\nError returned by UserOptions_cff\n'
    sys.exit()

# flag to run the L1T-GT emulator before the rate report
# if False, the emulator is not run
# if True, the emulator is run, either with the same L1 menu or with a new L1 menu
RunL1TGtEmulator = False
#RunL1TGtEmulator = True


if RunL1TGtEmulator == True :
    # L1 menu selection via L1Trigger_custom - expert choice, do it only if you know what you do
    # if True, modify correspondingly L1Trigger_custom

    #customL1Menu = True
    customL1Menu = False

    if customL1Menu == True :
        from L1Trigger.Configuration.L1Trigger_custom import customiseL1Menu
        process=customiseL1Menu(process)

        # reset all prescale factors and masks
        #from L1Trigger.Configuration.L1Trigger_custom import customiseResetPrescalesAndMasks
        #process = customiseResetPrescalesAndMasks(process)

# source according to data type
if dataType == 'StreamFile' :
    process.source = cms.Source("NewEventStreamFileReader", fileNames=readFiles)
else :        
    process.source = cms.Source ('PoolSource', 
                                 fileNames=readFiles, 
                                 secondaryFileNames=secFiles,
                                 lumisToProcess = selectedLumis,
                                 eventsToProcess = selectedEvents
                                 )


# number of events to be processed and source file
process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(maxNumberEvents)
)

#
# load and configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = useGlobalTag

# processes to be run

if RunL1TGtEmulator == True :

    # Global Trigger emulator
    import L1Trigger.GlobalTrigger.gtDigis_cfi
    process.l1GtEmulDigis = L1Trigger.GlobalTrigger.gtDigis_cfi.gtDigis.clone()


    # input tag for GMT readout collection: 
    #     gmtDigis = GMT emulator (default)
    #     l1GtUnpack     = GT unpacker (common GT/GMT unpacker)
    if useRelValSample == True :
        process.l1GtEmulDigis.GmtInputTag = 'simGmtDigis'
    else :
        process.l1GtEmulDigis.GmtInputTag = 'gtDigis'
   

    # input tag for GCT readout collections: 
    #     gctDigis = GCT emulator (default) 
    if useRelValSample == True :
        process.l1GtEmulDigis.GctInputTag = 'simGctDigis'
    else :
        process.l1GtEmulDigis.GctInputTag = 'gctDigis'

    # external conditions: a vector of input tags, one tag per each external condition producer 
    # 
    # by default: empty vector FIXME: update the emulator to take ExternalConditionsInputTags
        
    # Example:
    # ExternalConditionsInputTags = cms.VInputTag(cms.InputTag('anExternalConditionDigis'), 
    #                                            cms.InputTag('anotherExternalConditionDigis'))
    #if useRelValSample == True :
    #    process.l1GtEmulDigis.ExternalConditionsInputTags = cms.VInputTag(
    #                                        cms.InputTag( 'bptxDigis' )
    #                                        )
    #else :
    #    process.l1GtEmulDigis.ExternalConditionsInputTags = cms.VInputTag()
        
    # technical triggers: a vector of input tags, one tag per each technical 
    # trigger producer 
    # 
    # by default: empty vector
        
    # Example:
    # TechnicalTriggersInputTags = cms.VInputTag(cms.InputTag('aTechTrigDigis'), 
    #                                            cms.InputTag('anotherTechTriggerDigis'))
    if useRelValSample == True :
        process.l1GtEmulDigis.TechnicalTriggersInputTags = cms.VInputTag(
                                            cms.InputTag( 'simBscDigis' ), 
                                            cms.InputTag( 'simRpcTechTrigDigis' ),
                                            cms.InputTag( 'simHcalTechTrigDigis' )
                                            )
    else :
        process.l1GtEmulDigis.TechnicalTriggersInputTags = cms.VInputTag()
        
    
    # logical flag to produce the L1 GT DAQ readout record
    #     if true, produce the record (default)
    #process.l1GtEmulDigis.ProduceL1GtDaqRecord = False
        
    # logical flag to produce the L1 GT EVM readout record
    #     if true, produce the record (default)
    #process.l1GtEmulDigis.ProduceL1GtEvmRecord = False
    
    # logical flag to produce the L1 GT object map record
    #     if true, produce the record (default)
    #process.l1GtEmulDigis.ProduceL1GtObjectMapRecord = False
    
    # logical flag to write the PSB content in the  L1 GT DAQ record
    #     if true, write the PSB content in the record (default)
    #process.l1GtEmulDigis.WritePsbL1GtDaqRecord = False
    
    # logical flag to read the technical trigger records
    #     if true, it will read via getMany the available records (default)
    #process.l1GtEmulDigis.ReadTechnicalTriggerRecords = False
    
    # number of "bunch crossing in the event" (BxInEvent) to be emulated
    # symmetric around L1Accept (BxInEvent = 0):
    #    1 (BxInEvent = 0); 3 (F 0 1) (standard record); 5 (E F 0 1 2) (debug record)
    # even numbers (except 0) "rounded" to the nearest lower odd number
    # negative value: emulate TotalBxInEvent as given in EventSetup  
    process.l1GtEmulDigis.EmulateBxInEvent = 1
     
    # number of BXs in the event corresponding to alternative 0 and 1 in altNrBxBoard()
    # EmulateBxInEvent >= max(RecordLength[0], RecordLength[1])
    # negative values: take the numbers from event setup, from L1GtParameters
    process.l1GtEmulDigis.RecordLength = cms.vint32(3, 5)
    
    # alternative for number of BX per active board in GT DAQ record: 0 or 1
    # the position is identical with the active board bit
    process.l1GtEmulDigis.AlternativeNrBxBoardDaq = 0x101 
    
    # alternative for number of BX per active board in GT EVM record: 0 or 1
    # the position is identical with the active board bit
    process.l1GtEmulDigis.AlternativeNrBxBoardEvm = 0x2 
    
    # length of BST record (in bytes) from parameter set
    # negative value: take the value from EventSetup      
    #process.l1GtEmulDigis.BstLengthBytes = 52
    
    # run algorithm triggers 
    #     if true, unprescaled (all prescale factors 1)
    #     will overwrite the event setup
    process.l1GtEmulDigis.AlgorithmTriggersUnprescaled = True
        
    #     if true, unmasked - all enabled (all trigger masks set to 0)
    #     will overwrite the event setup
    process.l1GtEmulDigis.AlgorithmTriggersUnmasked = True
    
    # run technical triggers
    #     if true, unprescaled (all prescale factors 1)
    #     will overwrite the event setup
    process.l1GtEmulDigis.TechnicalTriggersUnprescaled = True
        
    #     if true, unmasked - all enabled (all trigger masks set to 0)
    #     will overwrite the event setup
    process.l1GtEmulDigis.TechnicalTriggersUnmasked = True
    
    #     if true, veto unmasked - all enabled (all trigger veto masks set to 0)
    #     will overwrite the event setup
    process.l1GtEmulDigis.TechnicalTriggersVetoUnmasked = True
    
    
    # 
    process.l1GtEmulDigis.Verbosity = cms.untracked.int32(1)

#
# l1GtTrigRatesReport module
#

process.load("L1Trigger.GlobalTriggerAnalyzer.l1GtTrigRatesReport_cfi")
 
# boolean flag to select the input record
# if true, it will use L1GlobalTriggerRecord 
#process.l1GtTrigRatesReport.UseL1GlobalTriggerRecord = True

# input tag for GT record: 
#   l1GtEmulDigis: GT emulator, run in this job
#   gtDigis      : GT unpacker (data) or GT emulator (MC sample) - default
#   hltGtDigis   : GT unpacker (L1Accept)
#   l1GtRecord   : GT lite record: l1GtRecord 
#process.l1GtTrigRatesReport.L1GtRecordInputTag = "l1GtEmulDigis"

#process.l1GtTrigRatesReport.PrintVerbosity = 10
#process.l1GtTrigRatesReport.PrintOutput = 1

# path to be run

# for RAW data, run first the RAWTODIGI 
if (dataType == 'RAW') and not (useRelValSample) :
    
    if dataset.count('L1Accept') :
        
        # unpack only GT and GMT
        print "\n   Unpacking only L1T-GT data for dataset ", dataset, "\n"
        import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
        process.gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
        process.gtDigis.DaqGtInputTag = 'rawDataCollector'
        
        if RunL1TGtEmulator == True :
            #
            process.p = cms.Path(process.gtDigis+process.l1GtEmulDigis*process.l1GtTrigRatesReport)
        else :
            process.p = cms.Path(process.gtDigis*process.l1GtTrigRatesReport)

    else :
        
        # standard unpacking
        process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
        # unpack five BX for GCT
        process.gctDigis.numberOfGctSamplesToUnpack = cms.uint32(5)
    
        if RunL1TGtEmulator == True :
            #
            process.p = cms.Path(process.RawToDigi+process.l1GtEmulDigis*process.l1GtTrigRatesReport)
        else :
            process.p = cms.Path(process.RawToDigi*process.l1GtTrigRatesReport)

elif (dataType == 'RAW') and (useRelValSample) :
    
    process.load('Configuration/StandardSequences/RawToDigi_cff')
    process.p = cms.Path(process.RawToDigi+process.l1GtEmulDigis*process.l1GtTrigRatesReport)    
else :        
    # path to be run for RECO
    process.p = cms.Path(process.l1GtEmulDigis*process.l1GtTrigRatesReport)


# Message Logger
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = ['l1GtEmulDigis', 'l1GtTrigRatesReport']
process.MessageLogger.categories.append('L1GlobalTrigger')
process.MessageLogger.categories.append('L1GtTrigRatesReport')
process.MessageLogger.destinations = ['L1GtTrigRatesReport_errors', 
                                      'L1GtTrigRatesReport_warnings', 
                                      'L1GtTrigRatesReport_info', 
                                      'L1GtTrigRatesReport_debug'
                                      ]
process.MessageLogger.statistics = []
process.MessageLogger.fwkJobReports = []

process.MessageLogger.L1GtTrigRatesReport_errors = cms.untracked.PSet( 
        threshold = cms.untracked.string('ERROR'),
        ERROR = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
        L1GlobalTrigger = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
        L1GtTrigRatesReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ) 
       )

process.MessageLogger.L1GtTrigRatesReport_warnings = cms.untracked.PSet( 
        threshold = cms.untracked.string('WARNING'),
        WARNING = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        L1GlobalTrigger = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
        L1GtTrigRatesReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) )  
        )

process.MessageLogger.L1GtTrigRatesReport_info = cms.untracked.PSet( 
        threshold = cms.untracked.string('INFO'),
        INFO = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        WARNING = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        L1GlobalTrigger = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
        L1GtTrigRatesReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) )  
        )

process.MessageLogger.L1GtTrigRatesReport_debug = cms.untracked.PSet( 
        threshold = cms.untracked.string('DEBUG'),
        DEBUG = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        INFO = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        WARNING = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        L1GlobalTrigger = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
        L1GtTrigRatesReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) )  
        )

# output 

# keep GMT, GCT and GT output records
# update labels if InputTags are replaced above

process.outputL1GlobalTrigger = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('L1GtTrigRatesReport.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_simGmtDigis_*_*', 
        'keep *_simGctDigis_*_*', 
        'keep *_l1GtEmulDigis_*_*')
)

#process.outpath = cms.EndPath(process.outputL1GlobalTrigger)
