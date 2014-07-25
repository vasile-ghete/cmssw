/**
 * \class L1GtTrigRatesReport
 *
 *
 * Description: L1 Trigger rates report.
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 *
 *
 */

// this class header
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtTrigRatesReport.h"

// system include files
#include <memory>

#include <iostream>
#include <iomanip>

#include<map>
#include<set>
#include <cmath>
#include <string>

#include "boost/lexical_cast.hpp"

// user include files
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/L1TObjects/interface/L1GtStableParameters.h"
#include "CondFormats/DataRecord/interface/L1GtStableParametersRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMaskVetoAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskVetoTechTrigRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtTrigRatesReportEntry.h"

// constructor(s)
L1GtTrigRatesReport::L1GtTrigRatesReport(const edm::ParameterSet& pSet) :

        m_useL1GlobalTriggerRecord(
                pSet.getParameter<bool>("UseL1GlobalTriggerRecord")),

        m_l1GtRecordInputTag(
                pSet.getParameter<edm::InputTag>("L1GtRecordInputTag")),

        //
        m_numberBunches(pSet.getParameter<int>("NumberBunches")),
        // Sample parameter set is read in the body of the constructor
        m_corr4Deadtime(pSet.getParameter<bool>("Corr4Deadtime")),
        //

        m_printLsReports(pSet.getUntrackedParameter<bool>("PrintLsReports")),

        m_printRunReports(pSet.getUntrackedParameter<bool>("PrintRunReports")),

        m_printJobReports(pSet.getUntrackedParameter<bool>("PrintJobReports")),

        m_printVerbosity(pSet.getUntrackedParameter<int>("PrintVerbosity", 2)),

        m_printOutput(pSet.getUntrackedParameter<int>("PrintOutput", 3)),

        //

        m_l1GtStableParCacheID(0ULL),

        m_numberAlgoTriggers(0), m_numberTechnicalTriggers(0),

        //

        m_l1GtPfAlgoCacheID(0ULL), m_l1GtPfTechCacheID(0ULL),

        m_l1GtTmAlgoCacheID(0ULL), m_l1GtTmTechCacheID(0ULL),

        m_l1GtTmVetoAlgoCacheID(0ULL), m_l1GtTmVetoTechCacheID(0ULL),

        m_l1GtMenuCacheID(0ULL),

        //
        m_rotationFrequency(0.),
        m_lsLengthSec(0.),

        //

        m_runTotalNrLS(0),

        m_jobTotalNrEvents(0), m_runTotalNrEvents(0), m_lsTotalNrEvents(0),

        m_jobNrL1Errors(0), m_runNrL1Errors(0), m_lsNrL1Errors(0),

        m_jobNrL1Accepts(0), m_runNrL1Accepts(0), m_lsNrL1Accepts(0),

        //

        m_physicsDaqPartition(0) {

    //
    m_lsEntryListAlgoTrig.clear();
    m_lsEntryListTechTrig.clear();
    m_lsEntryListGlobal.clear();

    m_runEntryListAlgoTrig.clear();
    m_runEntryListTechTrig.clear();

    // LS time length
    const unsigned int lsLengthOrbits = pSet.getParameter<int>(
            "LsLengthOrbits");

    // LHC circumference [m]
    const double lhcLength = 26659.;

    // speed of light [m/s]
    const double lightSpeed = 299792458.;

    // LHC rotation frequency ~11246 Hz
    m_rotationFrequency = lightSpeed / lhcLength;

    // for LsLengthOrbits 18, one gets LS = (2^18)/11246 = 23.31 sec
    m_lsLengthSec = pow(2.0, static_cast<double>(lsLengthOrbits))
            / m_rotationFrequency;

    std::cout << "m_lsLengthSec = " << m_lsLengthSec << std::endl;

    //

    const edm::ParameterSet& sample = pSet.getParameter<edm::ParameterSet>(
            "Sample");
    std::string sampleIdentifier = sample.getParameter<std::string>(
            "SampleIdentifier");

    m_sampleIsData = sample.getParameter<bool>("SampleIsData");
    m_sampleCrossSection = sample.getParameter<double>("SampleCrossSection");
    m_samplePU = sample.getParameter<double>("SamplePU");



    LogDebug("L1GtTrigRatesReport") << "\n  Use L1GlobalTriggerRecord:   "
            << m_useL1GlobalTriggerRecord
            << "\n   (if false: L1GtTrigRatesReport uses L1GlobalTriggerReadoutRecord.)"
            << "\n  Input tag for L1 GT record:  "
            << m_l1GtRecordInputTag.label()
            << "\n\n  Print verbosity level:           " << m_printVerbosity
            << "\n\n  Print output:                    " << m_printOutput
            << " \n" << std::endl;

}

// destructor
L1GtTrigRatesReport::~L1GtTrigRatesReport() {

    // clean LS entries

    for (ItEntry itEntry = m_lsEntryListAlgoTrig.begin();
            itEntry != m_lsEntryListAlgoTrig.end(); itEntry++) {
        if (*itEntry != 0) {
            delete *itEntry;
            *itEntry = 0;
        }
    }

    m_lsEntryListAlgoTrig.clear();

    for (ItEntry itEntry = m_lsEntryListTechTrig.begin();
            itEntry != m_lsEntryListTechTrig.end(); itEntry++) {
        if (*itEntry != 0) {
            delete *itEntry;
            *itEntry = 0;
        }
    }

    m_lsEntryListTechTrig.clear();

    for (ItEntry itEntry = m_lsEntryListGlobal.begin();
            itEntry != m_lsEntryListGlobal.end(); itEntry++) {
        if (*itEntry != 0) {
            delete *itEntry;
            *itEntry = 0;
        }
    }

    m_lsEntryListGlobal.clear();

    // clean run entries

    for (ItEntry itEntry = m_runEntryListAlgoTrig.begin();
            itEntry != m_runEntryListAlgoTrig.end(); itEntry++) {
        if (*itEntry != 0) {
            delete *itEntry;
            *itEntry = 0;
        }
    }

    m_runEntryListAlgoTrig.clear();

    for (ItEntry itEntry = m_runEntryListTechTrig.begin();
            itEntry != m_runEntryListTechTrig.end(); itEntry++) {
        if (*itEntry != 0) {
            delete *itEntry;
            *itEntry = 0;
        }
    }

    m_runEntryListTechTrig.clear();

}

// member functions

// method called once each job just before starting event loop
void L1GtTrigRatesReport::beginJob() {

    m_jobNrL1Accepts = 0;
    m_runNrL1Accepts = 0;
    m_lsNrL1Accepts = 0;

}


void L1GtTrigRatesReport::beginRun(const edm::Run& iRun,
        const edm::EventSetup& evSetup) {

    LogDebug("L1GtTrigRatesReport") << "\nRun " << iRun.run() << std::endl;

    // get / update the stable parameters from the EventSetup
    // local cache & check on cacheIdentifier

    // stable parameters

    unsigned long long l1GtStableParCacheID = evSetup.get<
            L1GtStableParametersRcd>().cacheIdentifier();

    if (m_l1GtStableParCacheID != l1GtStableParCacheID) {

        edm::ESHandle<L1GtStableParameters> l1GtStablePar;
        evSetup.get<L1GtStableParametersRcd>().get(l1GtStablePar);
        m_l1GtStablePar = l1GtStablePar.product();

        // number of algorithm triggers
        m_numberAlgoTriggers = m_l1GtStablePar->gtNumberPhysTriggers();

        // number of technical triggers
        m_numberTechnicalTriggers =
                m_l1GtStablePar->gtNumberTechnicalTriggers();

        //
        m_l1GtStableParCacheID = l1GtStableParCacheID;

    }

    // get / update the prescale factors from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtPfAlgoCacheID = evSetup.get<
            L1GtPrescaleFactorsAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtPfAlgoCacheID != l1GtPfAlgoCacheID) {

        edm::ESHandle<L1GtPrescaleFactors> l1GtPfAlgo;
        evSetup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(l1GtPfAlgo);
        m_l1GtPfAlgo = l1GtPfAlgo.product();

        m_prescaleFactorsAlgoTrig = &(m_l1GtPfAlgo->gtPrescaleFactors());

        m_l1GtPfAlgoCacheID = l1GtPfAlgoCacheID;

    }

    unsigned long long l1GtPfTechCacheID = evSetup.get<
            L1GtPrescaleFactorsTechTrigRcd>().cacheIdentifier();

    if (m_l1GtPfTechCacheID != l1GtPfTechCacheID) {

        edm::ESHandle<L1GtPrescaleFactors> l1GtPfTech;
        evSetup.get<L1GtPrescaleFactorsTechTrigRcd>().get(l1GtPfTech);
        m_l1GtPfTech = l1GtPfTech.product();

        m_prescaleFactorsTechTrig = &(m_l1GtPfTech->gtPrescaleFactors());

        m_l1GtPfTechCacheID = l1GtPfTechCacheID;

    }

    // get / update the trigger mask from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtTmAlgoCacheID = evSetup.get<
            L1GtTriggerMaskAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtTmAlgoCacheID != l1GtTmAlgoCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmAlgo;
        evSetup.get<L1GtTriggerMaskAlgoTrigRcd>().get(l1GtTmAlgo);
        m_l1GtTmAlgo = l1GtTmAlgo.product();

        m_triggerMaskAlgoTrig = m_l1GtTmAlgo->gtTriggerMask();

        m_l1GtTmAlgoCacheID = l1GtTmAlgoCacheID;

    }

    unsigned long long l1GtTmTechCacheID = evSetup.get<
            L1GtTriggerMaskTechTrigRcd>().cacheIdentifier();

    if (m_l1GtTmTechCacheID != l1GtTmTechCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmTech;
        evSetup.get<L1GtTriggerMaskTechTrigRcd>().get(l1GtTmTech);
        m_l1GtTmTech = l1GtTmTech.product();

        m_triggerMaskTechTrig = m_l1GtTmTech->gtTriggerMask();

        m_l1GtTmTechCacheID = l1GtTmTechCacheID;

    }

    unsigned long long l1GtTmVetoAlgoCacheID = evSetup.get<
            L1GtTriggerMaskVetoAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtTmVetoAlgoCacheID != l1GtTmVetoAlgoCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmVetoAlgo;
        evSetup.get<L1GtTriggerMaskVetoAlgoTrigRcd>().get(l1GtTmVetoAlgo);
        m_l1GtTmVetoAlgo = l1GtTmVetoAlgo.product();

        m_triggerMaskVetoAlgoTrig = m_l1GtTmVetoAlgo->gtTriggerMask();

        m_l1GtTmVetoAlgoCacheID = l1GtTmVetoAlgoCacheID;

    }

    unsigned long long l1GtTmVetoTechCacheID = evSetup.get<
            L1GtTriggerMaskVetoTechTrigRcd>().cacheIdentifier();

    if (m_l1GtTmVetoTechCacheID != l1GtTmVetoTechCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmVetoTech;
        evSetup.get<L1GtTriggerMaskVetoTechTrigRcd>().get(l1GtTmVetoTech);
        m_l1GtTmVetoTech = l1GtTmVetoTech.product();

        m_triggerMaskVetoTechTrig = m_l1GtTmVetoTech->gtTriggerMask();

        m_l1GtTmVetoTechCacheID = l1GtTmVetoTechCacheID;

    }

    // get / update the trigger menu from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtMenuCacheID =
            evSetup.get<L1GtTriggerMenuRcd>().cacheIdentifier();

    if (m_l1GtMenuCacheID != l1GtMenuCacheID) {

        edm::ESHandle<L1GtTriggerMenu> l1GtMenu;
        evSetup.get<L1GtTriggerMenuRcd>().get(l1GtMenu);
        m_l1GtMenu = l1GtMenu.product();

        m_l1GtMenuCacheID = l1GtMenuCacheID;

        LogTrace("L1GtTrigRatesReport") << "\n  Changing L1 menu to : \n"
                << m_l1GtMenu->gtTriggerMenuName() << "\n\n" << std::endl;

    }

    // ... end EventSetup

    // reset the counters

    m_runTotalNrEvents = 0;
    m_runNrL1Errors = 0;
    m_runNrL1Accepts = 0;

    m_runTotalNrLS = 0;


}

void L1GtTrigRatesReport::beginLuminosityBlock(
        const edm::LuminosityBlock& iLumiBlock,
        const edm::EventSetup& evSetup) {

    const int lsNumber = static_cast<int>(iLumiBlock.luminosityBlock());

    LogTrace("L1GtTrigRatesReport") << "\n  Luminosity block "
            << lsNumber << std::endl;

    // increase the number of processed LS per run
    m_runTotalNrLS++;

    // reset the counters
    m_lsTotalNrEvents = 0;
    m_lsNrL1Errors = 0;
    m_lsNrL1Accepts = 0;

}

// analyze each event
void L1GtTrigRatesReport::analyze(const edm::Event& iEvent,
        const edm::EventSetup& evSetup) {

    // increase the number of processed events
    m_jobTotalNrEvents++;
    m_runTotalNrEvents++;
    m_lsTotalNrEvents++;

    //
    const int runNumber = static_cast<int>(iEvent.run());
    const int lsNumber = static_cast<int>(iEvent.luminosityBlock());
    const int lsPileup = 0;

    //
    const AlgorithmMap& algorithmMap = m_l1GtMenu->gtAlgorithmMap();
    const AlgorithmMap& technicalTriggerMap =
            m_l1GtMenu->gtTechnicalTriggerMap();

    const std::string& menuName = m_l1GtMenu->gtTriggerMenuName();

    // get L1GlobalTriggerReadoutRecord or L1GlobalTriggerRecord
    // in L1GlobalTriggerRecord, only the physics partition is available
    edm::Handle < L1GlobalTriggerReadoutRecord > gtReadoutRecord;
    edm::Handle < L1GlobalTriggerRecord > gtRecord;

    if (m_useL1GlobalTriggerRecord) {
        iEvent.getByLabel(m_l1GtRecordInputTag, gtRecord);
    } else {
        iEvent.getByLabel(m_l1GtRecordInputTag, gtReadoutRecord);
    }

    bool validRecord = false;

    unsigned int pfIndexAlgo = 0; // get them later from the record
    unsigned int pfIndexTech = 0;

    DecisionWord gtDecisionWordBeforeMask;
    DecisionWord gtDecisionWordAfterMask;

    TechnicalTriggerWord technicalTriggerWordBeforeMask;
    TechnicalTriggerWord technicalTriggerWordAfterMask;

    if (m_useL1GlobalTriggerRecord) {

        if (gtRecord.isValid()) {

            // get Global Trigger decision and the decision word
            bool gtDecision = gtRecord->decision();

            gtDecisionWordBeforeMask = gtRecord->decisionWordBeforeMask();
            gtDecisionWordAfterMask = gtRecord->decisionWord();

            technicalTriggerWordBeforeMask =
                    gtRecord->technicalTriggerWordBeforeMask();
            technicalTriggerWordAfterMask = gtRecord->technicalTriggerWord();

            if (gtDecision) {
                m_jobNrL1Accepts++;
                m_runNrL1Accepts++;
                m_lsNrL1Accepts++;
            }

            pfIndexAlgo = gtRecord->gtPrescaleFactorIndexAlgo();
            pfIndexTech = gtRecord->gtPrescaleFactorIndexTech();

            validRecord = true;

        } else {

            m_jobNrL1Errors++;
            m_runNrL1Errors++;
            m_lsNrL1Errors++;

            edm::LogWarning("L1GtTrigRatesReport")
                    << "\n  L1GlobalTriggerRecord with input tag "
                    << m_l1GtRecordInputTag.label() << " not found."
                    << "\n  Event classified as Error\n\n" << std::endl;

        }

    } else {
        if (gtReadoutRecord.isValid()) {

            // check if the readout record has size greater than zero, then proceeds
            const std::vector<L1GtFdlWord>& fdlVec =
                    gtReadoutRecord->gtFdlVector();
            size_t fdlVecSize = fdlVec.size();

            if (fdlVecSize > 0) {

//                LogDebug("L1GtTrigRatesReport")
//                        << "\n  L1GlobalTriggerReadoutRecord with input tag "
//                        << m_l1GtRecordInputTag.label()
//                        << " has gtFdlVector of size " << fdlVecSize
//                        << std::endl;

                // get Global Trigger finalOR and the decision word
                boost::uint16_t gtFinalOR = gtReadoutRecord->finalOR();

                gtDecisionWordBeforeMask = gtReadoutRecord->decisionWord();
                technicalTriggerWordBeforeMask =
                        gtReadoutRecord->technicalTriggerWord();

                bool gtDecision = static_cast<bool>(gtFinalOR
                        & (1 << m_physicsDaqPartition));
                if (gtDecision) {
                    m_jobNrL1Accepts++;
                    m_runNrL1Accepts++;
                    m_lsNrL1Accepts++;
                }

                pfIndexAlgo =
                        static_cast<unsigned int>((gtReadoutRecord->gtFdlWord()).gtPrescaleFactorIndexAlgo());
                pfIndexTech =
                        static_cast<unsigned int>((gtReadoutRecord->gtFdlWord()).gtPrescaleFactorIndexTech());

                validRecord = true;

            } else {

                m_jobNrL1Errors++;
                m_runNrL1Errors++;
                m_lsNrL1Errors++;

                edm::LogWarning("L1GtTrigRatesReport")
                        << "\n  L1GlobalTriggerReadoutRecord with input tag "
                        << m_l1GtRecordInputTag.label()
                        << " has gtFdlVector of size " << fdlVecSize
                        << "\n  Invalid L1GlobalTriggerReadoutRecord!"
                        << "\n  Event classified as Error\n\n" << std::endl;

                validRecord = false;

            }

        } else {

            m_jobNrL1Errors++;
            m_runNrL1Errors++;
            m_lsNrL1Errors++;

            edm::LogWarning("L1GtTrigRatesReport")
                    << "\n  L1GlobalTriggerReadoutRecord with input tag "
                    << m_l1GtRecordInputTag.label() << " not found."
                    << "\n  Event classified as Error\n\n" << std::endl;

        }

    }

    // get the prescale factor set used in the actual luminosity segment
    const std::vector<int>& prescaleFactorsAlgoTrig =
            (*m_prescaleFactorsAlgoTrig).at(pfIndexAlgo);

    const std::vector<int>& prescaleFactorsTechTrig =
            (*m_prescaleFactorsTechTrig).at(pfIndexTech);

    if (validRecord) {

        // loop over algorithms and increase the corresponding counters
        for (CItAlgo itAlgo = algorithmMap.begin();
                itAlgo != algorithmMap.end(); itAlgo++) {

            std::string triggerName = itAlgo->first;
            int triggerBitNumber = (itAlgo->second).algoBitNumber();

            // the result before applying the trigger masks is available
            // in both L1GlobalTriggerReadoutRecord or L1GlobalTriggerRecord
            bool triggerResultBeforeMask = gtDecisionWordBeforeMask[triggerBitNumber];

            int prescaleFactor = prescaleFactorsAlgoTrig.at(triggerBitNumber);

            unsigned int triggerMask = (m_triggerMaskAlgoTrig.at(triggerBitNumber))
                    & (1 << m_physicsDaqPartition);

            bool triggerResultAfterMask = false;

            if (m_useL1GlobalTriggerRecord) {
                triggerResultAfterMask = gtDecisionWordAfterMask[triggerBitNumber];
            } else {
                // apply the masks for L1GlobalTriggerReadoutRecord
                triggerResultAfterMask = triggerResultBeforeMask;

                if (triggerMask) {
                    triggerResultAfterMask = false;
                }
            }

            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry(
                    runNumber, lsNumber, lsPileup, menuName, triggerName,
                    prescaleFactor);

            int iCount = 0;

            for (CItEntry itEntry = m_lsEntryListAlgoTrig.begin();
                    itEntry != m_lsEntryListAlgoTrig.end(); itEntry++) {
                if ((*itEntry)->isEqual((*entryRep), 0)) {
                    iCount++;
                    // increase the corresponding counter in the list entry
                    (*itEntry)->addValidEntry(triggerResultAfterMask);
                }
            }

            if (iCount == 0) {
                // if entry not in the list, increase the corresponding counter
                // and push the entry in the list
                entryRep->addValidEntry(triggerResultAfterMask);
                m_lsEntryListAlgoTrig.push_back(entryRep);
            } else {
                delete entryRep;
            }

        }

        // loop over technical triggers and increase the corresponding counters
        for (CItAlgo itAlgo = technicalTriggerMap.begin();
                itAlgo != technicalTriggerMap.end(); itAlgo++) {

            std::string triggerName = itAlgo->first;
            int triggerBitNumber = (itAlgo->second).algoBitNumber();

            // the result before applying the trigger masks is available
            // in both L1GlobalTriggerReadoutRecord or L1GlobalTriggerRecord
            bool ttResultBeforeMask =
                    technicalTriggerWordBeforeMask[triggerBitNumber];

            int prescaleFactor = prescaleFactorsTechTrig.at(triggerBitNumber);

            unsigned int triggerMask = (m_triggerMaskTechTrig.at(triggerBitNumber))
                    & (1 << m_physicsDaqPartition);

            bool ttResultAfterMask = false;

            if (m_useL1GlobalTriggerRecord) {
                ttResultAfterMask = technicalTriggerWordAfterMask[triggerBitNumber];
            } else {
                // apply the masks for L1GlobalTriggerReadoutRecord
                ttResultAfterMask = ttResultBeforeMask;

                if (triggerMask) {
                    ttResultAfterMask = false;
                }
            }

            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry(
                    runNumber, lsNumber, lsPileup, menuName, triggerName,
                    prescaleFactor);

            int iCount = 0;

            for (CItEntry itEntry = m_lsEntryListTechTrig.begin();
                    itEntry != m_lsEntryListTechTrig.end(); itEntry++) {
                if ((*itEntry)->isEqual((*entryRep), 0)) {
                    iCount++;
                    // increase the corresponding counter in the list entry
                    (*itEntry)->addValidEntry(ttResultAfterMask);
                }
            }

            if (iCount == 0) {
                // if entry not in the list, increase the corresponding counter
                // and push the entry in the list
                entryRep->addValidEntry(ttResultAfterMask);
                m_lsEntryListTechTrig.push_back(entryRep);
            } else {
                delete entryRep;
            }

        }

    } else {

        // loop over algorithm triggers and increase the error counters
        for (CItAlgo itAlgo = algorithmMap.begin();
                itAlgo != algorithmMap.end(); itAlgo++) {

            std::string triggerName = itAlgo->first;
            int triggerBitNumber = (itAlgo->second).algoBitNumber();

            int prescaleFactor = prescaleFactorsAlgoTrig.at(triggerBitNumber);

            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry(
                    runNumber, lsNumber, lsPileup, menuName, triggerName,
                    prescaleFactor);

            int iCount = 0;

            for (CItEntry itEntry = m_lsEntryListAlgoTrig.begin();
                    itEntry != m_lsEntryListAlgoTrig.end(); itEntry++) {

                if ((*itEntry)->isEqual((*entryRep), 0)) {
                    iCount++;
                    // increase the corresponding counter in the list entry
                    (*itEntry)->addErrorEntry();
                }
            }

            if (iCount == 0) {
                // if entry not in the list, increase the corresponding counter
                // and push the entry in the list
                entryRep->addErrorEntry();
                m_lsEntryListAlgoTrig.push_back(entryRep);
            } else {
                delete entryRep;
            }

        }

        // loop over technical triggers and increase the error counters
        for (CItAlgo itAlgo = technicalTriggerMap.begin();
                itAlgo != technicalTriggerMap.end(); itAlgo++) {

            std::string triggerName = itAlgo->first;
            int triggerBitNumber = (itAlgo->second).algoBitNumber();

            int prescaleFactor = prescaleFactorsTechTrig.at(triggerBitNumber);

            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry(
                    runNumber, lsNumber, lsPileup, menuName, triggerName,
                    prescaleFactor);

            int iCount = 0;

            for (CItEntry itEntry = m_lsEntryListTechTrig.begin();
                    itEntry != m_lsEntryListTechTrig.end(); itEntry++) {

                if ((*itEntry)->isEqual((*entryRep), 0)) {
                    iCount++;
                    // increase the corresponding counter in the list entry
                    (*itEntry)->addErrorEntry();
                }
            }

            if (iCount == 0) {
                // if entry not in the list, increase the corresponding counter
                // and push the entry in the list
                entryRep->addErrorEntry();
                m_lsEntryListTechTrig.push_back(entryRep);
            } else {
                delete entryRep;
            }

        }

    }

}


void L1GtTrigRatesReport::endLuminosityBlock(
        const edm::LuminosityBlock& iLumiBlock,
        const edm::EventSetup& evSetup) {

    const int runNumber = static_cast<int>(iLumiBlock.run());
    const int lsNumber = static_cast<int>(iLumiBlock.luminosityBlock());

    const int lsPileup = 0;
    const std::string& menuName = m_l1GtMenu->gtTriggerMenuName();
    const std::string triggerName = "";
    const int prescaleFactor = 0;

    // compute the average total trigger rate for the corresponding LS
    // and fill the corresponding entry

    float rate = 0.;
    float rateStatErr = 0.;

    computeTrigRates(m_lsNrL1Accepts, m_lsTotalNrEvents, m_sampleIsData,
            m_corr4Deadtime, rate, rateStatErr);

    L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry(runNumber,
            lsNumber, lsPileup, menuName, triggerName, prescaleFactor);

    entryRep->setNrEventsAccept(m_lsNrL1Accepts);
    entryRep->setNrEventsReject(
            m_lsTotalNrEvents - m_lsNrL1Errors - m_lsNrL1Accepts);
    entryRep->setNrEventsError(m_lsNrL1Errors);

    entryRep->setTriggerRate(rate);
    entryRep->setTriggerRateStatErr(rateStatErr);

    m_lsEntryListGlobal.push_back(entryRep);

    // compute the pure average rate for each algorithm and technical trigger
    // for the corresponding LS

    float lsTotalPureRateAlgoTrig = 0.;
    float lsTotalPureRateAlgoTrigStatErr = 0.;

    float lsTotalPureRateTechTrig = 0.;
    float lsTotalPureRateTechTrigStatErr = 0.;

    float lsTotalPureRate = 0.;
    float lsTotalPureRateStatErr = 0.;

    computeTrigRatesLs(runNumber, lsNumber, m_lsEntryListAlgoTrig,
            lsTotalPureRateAlgoTrig, lsTotalPureRateAlgoTrigStatErr);

    computeTrigRatesLs(runNumber, lsNumber, m_lsEntryListTechTrig,
            lsTotalPureRateTechTrig, lsTotalPureRateTechTrigStatErr);

    lsTotalPureRate = lsTotalPureRateAlgoTrig + lsTotalPureRateTechTrig;
    lsTotalPureRateStatErr = sqrt(
            pow(lsTotalPureRateAlgoTrig, 2) + pow(lsTotalPureRateTechTrig, 2));

    edm::LogVerbatim("L1GtTrigRatesReport") << "\nRun " << runNumber
            << "  Luminosity block " << lsNumber << "\n  L1 menu: "
            << m_l1GtMenu->gtTriggerMenuName()
            << "\n  Number of events processed per luminosity block: "
            << m_lsTotalNrEvents
            << "\n  Total algorithm trigger pure rate per luminosity block: "
            << lsTotalPureRateAlgoTrig << " +- "
            << lsTotalPureRateAlgoTrigStatErr
            << "\n  Total technical trigger pure rate per luminosity block: "
            << lsTotalPureRateTechTrig << " +- "
            << lsTotalPureRateTechTrigStatErr
            << "\n  Total trigger pure rate per luminosity block:           "
            << lsTotalPureRate << " +- " << lsTotalPureRateStatErr
            << "\n  Average total trigger rate per luminosity block:        "
            << rate << " +- " << rateStatErr << "\n" << std::endl;

    // print the pure average rate for each algorithm and technical trigger
    // for the corresponding LS

    printL1TrigRates(runNumber, lsNumber, lsPileup, menuName, triggerName,
            prescaleFactor, m_lsEntryListAlgoTrig, m_lsEntryListTechTrig);

}

void L1GtTrigRatesReport::endRun(const edm::Run& iRun,
        const edm::EventSetup& evSetup) {

    const int runNumber = static_cast<int>(iRun.run());

//    // compute the average total trigger rate for the corresponding run
//    // and fill the corresponding entry
//    for (CItEntry itLsEntry = m_lsEntryListGlobal.begin();
//            itLsEntry != m_lsEntryListGlobal.end(); itLsEntry++) {
//
//        if ((*itLsEntry)->gtRunNumber() == runNumber) {
//
//
//        }
//
//
//    }

    edm::LogVerbatim("L1GtTrigRatesReport") << "\nRun " << runNumber
            << "\n  L1 menu: " << m_l1GtMenu->gtTriggerMenuName()
            << "\n  Number of luminosity blocks processed per run: "
            << m_runTotalNrLS << "\n  Number of events processed per run: "
            << m_runTotalNrEvents << "\n" << std::endl;

    // for each algorithm and technical trigger, sum over LS from a run

    for (CItEntry itLsEntry = m_lsEntryListAlgoTrig.begin();
            itLsEntry != m_lsEntryListAlgoTrig.end(); itLsEntry++) {

        int iCount = 0;

        for (ItEntry itRunEntry = m_runEntryListAlgoTrig.begin();
                itRunEntry != m_runEntryListAlgoTrig.end(); itRunEntry++) {

            // comparison on runNumber, lsPileup, menuName, triggerName, prescaleFactor
            if ((*itRunEntry)->isEqual((*(*itLsEntry)), 2)) {
                // add the LS entry to the corresponding run entry
                (*itRunEntry)->sumEventsEntries((*itLsEntry));
                iCount++;

                break;
            }
        }

        if (iCount == 0) {
            // create the corresponding run entry
            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry();
            *entryRep = *(*itLsEntry);
            entryRep->setLsNumber(0);

            m_runEntryListAlgoTrig.push_back(entryRep);

        }

    }

    for (CItEntry itLsEntry = m_lsEntryListTechTrig.begin();
            itLsEntry != m_lsEntryListTechTrig.end(); itLsEntry++) {

        int iCount = 0;

        for (ItEntry itRunEntry = m_runEntryListTechTrig.begin();
                itRunEntry != m_runEntryListTechTrig.end(); itRunEntry++) {

            // comparison on runNumber, lsPileup, menuName, triggerName, prescaleFactor
            if ((*itRunEntry)->isEqual((*(*itLsEntry)), 2)) {
                // add the LS entry to the corresponding run entry
                (*itRunEntry)->sumEventsEntries((*itLsEntry));
                iCount++;

                break;
            }
        }

        if (iCount == 0) {
            // create the corresponding run entry
            L1GtTrigRatesReportEntry* entryRep = new L1GtTrigRatesReportEntry();
            *entryRep = *(*itLsEntry);
            entryRep->setLsNumber(0);

            m_runEntryListTechTrig.push_back(entryRep);

        }

    }

    // for printing, do not check lsNumber, lsPileup, menuName, triggerName, prescaleFactor
    const int lsNumber = 0;
    const int lsPileup = 0;
    const std::string& menuName = "";
    const std::string triggerName = "";
    const int prescaleFactor = 0;

    printL1TrigRates(runNumber, lsNumber, lsPileup, menuName, triggerName,
            prescaleFactor, m_runEntryListAlgoTrig, m_runEntryListTechTrig);

}

// method called once each job just after ending the event loop
void L1GtTrigRatesReport::endJob() {

    edm::LogVerbatim("L1GtTrigRatesReport")
            << "\nTotal number of events processed in this job: " << m_jobTotalNrEvents
            << "\n";

}

void const L1GtTrigRatesReport::computeTrigRates(const int nrAcceptedEvts,
        const int totalNrEvts, bool sampleIsData, bool corr4Deadtime,
        float& rate, float& rateStatErr) const {

    float rateValue = 0.0;
    float rateStatErrValue = 0.0;

    if (sampleIsData) {
        rateValue = static_cast<float>(nrAcceptedEvts) / m_lsLengthSec;

        rateStatErrValue = sqrt(static_cast<float>(nrAcceptedEvts))
                / m_lsLengthSec;

    } else {
        float averageLuminosity = m_rotationFrequency * m_samplePU
                * m_numberBunches / (1E7 * m_sampleCrossSection);

        std::cout << "averageLuminosity = " << averageLuminosity << std::endl;
    }

    rate = rateValue;
    rateStatErr = rateStatErrValue;

}

void L1GtTrigRatesReport::computeTrigRatesLs(const int runNumber,
        const int lsNumber, std::list<L1GtTrigRatesReportEntry*>& listTrig,
        float& lsTotalPureTrigRate, float& lsTotalPureTrigRateStatErr) {

    float sumPureTrigRateLs = 0.;
    float sumStatErr = 0.;

    for (ItEntry itLsEntry = listTrig.begin(); itLsEntry != listTrig.end();
            itLsEntry++) {

        if ((runNumber == (*itLsEntry)->gtRunNumber())
                && (lsNumber == (*itLsEntry)->gtLsNumber())) {

            int nrAcceptedEvts = (*itLsEntry)->gtNrEventsAccept();

            float rate = 0.;
            float rateStatErr = 0.;
            computeTrigRates(nrAcceptedEvts, m_lsTotalNrEvents, m_sampleIsData,
                        m_corr4Deadtime, rate, rateStatErr);

            (*itLsEntry)->setTriggerRate(rate);
            (*itLsEntry)->setTriggerRateStatErr(rateStatErr);

            sumPureTrigRateLs += rate;
            sumStatErr += pow(nrAcceptedEvts, 2);
        }

    }

    // total pure trigger rate per LS
    lsTotalPureTrigRate = sumPureTrigRateLs;
    lsTotalPureTrigRateStatErr = sqrt(static_cast<float>(sumStatErr))
            / m_lsLengthSec;

}

void L1GtTrigRatesReport::printL1TrigRates(const int runNumber,
        const int lsNumber, const int lsPileup, const std::string& menuName,
        const std::string& triggerName, const int prescaleFactor,
        const std::list<L1GtTrigRatesReportEntry*> listAlgoTrig,
        const std::list<L1GtTrigRatesReportEntry*> listTechTrig) {

    // check which comparison must be done

    bool compareRunNumber = false;
    if (runNumber > 0) {
        compareRunNumber = true;
    }

    bool compareLsNumber = false;
    if (lsNumber > 0) {
        compareLsNumber = true;
    }

    bool compareLsPileup = false;
    if (lsPileup > 0) {
        compareLsPileup = true;
    }

    bool compareMenuName = false;
    if (!menuName.empty()) {
        compareMenuName = true;
    }

    bool compareTriggerName = false;
    if (!triggerName.empty()) {
        compareTriggerName = true;
    }

    bool comparePrescaleFactor = false;
    if (prescaleFactor > 0) {
        comparePrescaleFactor = true;
    }

    // define an output stream to print into
    // it can then be directed to whatever log level is desired
    std::ostringstream myCout;

//    // get the list of menus for the sample analyzed
//    //
//    std::set<std::string> menuList;
//    typedef std::set<std::string>::const_iterator CItL1Menu;
//
//    for (CItEntry itEntry = listAlgoTrig.begin(); itEntry != listAlgoTrig.end();
//            itEntry++) {
//        menuList.insert((*itEntry)->gtTriggerMenuName());
//    }
//
//    myCout << "\nThe following L1 menus were used for this sample: "
//            << std::endl;
//    for (CItL1Menu itMenu = menuList.begin(); itMenu != menuList.end();
//            itMenu++) {
//        myCout << "  " << (*itMenu) << std::endl;
//    }
//    myCout << "\n" << std::endl;

    switch (m_printVerbosity) {
        case 1: {

            myCout << std::right << std::setw(45) << "Algorithm Key" << " "
                    << std::right << std::setw(10) << "Prescale" << " "
                    << std::right << std::setw(10) << "Passed" << " "
                    << std::right << std::setw(10) << "Rejected" << " "
                    << std::right << std::setw(10) << "Error"
                    << std::right << std::setw(30) << "Trigger rate [Hz]      "
                    << std::setw(2)
                    << " " << "\n";

            for (CItEntry itEntry = listAlgoTrig.begin();
                    itEntry != listAlgoTrig.end(); itEntry++) {

                if (compareRunNumber) {
                    if ((*itEntry)->gtRunNumber() != runNumber) {
                        continue;
                    }
                }

                if (compareLsNumber) {
                    if ((*itEntry)->gtLsNumber() != lsNumber) {
                        continue;
                    }
                }

                if (compareLsPileup) {
                    if ((*itEntry)->gtLsPileup() != lsPileup) {
                        continue;
                    }
                }

                if (compareMenuName) {
                    if ((*itEntry)->gtTriggerMenuName() != menuName) {
                        continue;
                    }
                }

                if (compareTriggerName) {
                    if ((*itEntry)->gtTriggerName() != triggerName) {
                        continue;
                    }
                }

                if (comparePrescaleFactor) {
                    if ((*itEntry)->gtPrescaleFactor() != prescaleFactor) {
                        continue;
                    }
                }

                myCout
                    << std::right << std::setw(45) << (*itEntry)->gtTriggerName() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtPrescaleFactor() << " "
                    << std::dec
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsAccept() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsReject() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsError()
                    << std::right << std::setw(12) << std::fixed << std::setprecision(2)
                    << (*itEntry)->gtTriggerRate() << " +- "
                    << std::right << std::setw(12) << std::setprecision(2)
                    << (*itEntry)->gtTriggerRateStatErr()
                    << std::setw(2) << " "
                    << "\n";
            }

            myCout << "\n\n"
                    << std::right << std::setw(45) << "Technical Trigger Key" << " "
                    << std::right << std::setw(10) << "Prescale" << " "
                    << std::right << std::setw(10) << "Passed" << " "
                    << std::right << std::setw(10) << "Rejected" << " "
                    << std::right << std::setw(10) << "Error"
                    << std::right << std::setw(30) << "Trigger rate [Hz]      "
                    << std::setw(2) << " "
                    << "\n";

            for (CItEntry itEntry = listTechTrig.begin(); itEntry != listTechTrig.end(); itEntry++) {

                if (compareRunNumber) {
                    if ((*itEntry)->gtRunNumber() != runNumber) {
                        continue;
                    }
                }

                if (compareLsNumber) {
                    if ((*itEntry)->gtLsNumber() != lsNumber) {
                        continue;
                    }
                }

                if (compareLsPileup) {
                    if ((*itEntry)->gtLsPileup() != lsPileup) {
                        continue;
                    }
                }

                if (compareMenuName) {
                    if ((*itEntry)->gtTriggerMenuName() != menuName) {
                        continue;
                    }
                }

                if (compareTriggerName) {
                    if ((*itEntry)->gtTriggerName() != triggerName) {
                        continue;
                    }
                }

                if (comparePrescaleFactor) {
                    if ((*itEntry)->gtPrescaleFactor() != prescaleFactor) {
                        continue;
                    }
                }


                myCout
                    << std::right << std::setw(45) << (*itEntry)->gtTriggerName() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtPrescaleFactor() << " "
                    << std::dec
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsAccept() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsReject() << " "
                    << std::right << std::setw(10) << (*itEntry)->gtNrEventsError()
                    << std::right << std::setw(12) << std::fixed << std::setprecision(2)
                    << (*itEntry)->gtTriggerRate() << " +- "
                    << std::right << std::setw(12) << std::setprecision(2)
                    << (*itEntry)->gtTriggerRateStatErr()
                    << std::setw(2) << " "
                    << "\n";
            }


        }

            break;
        case 2: {


//            for (CItL1Menu itMenu = menuList.begin(); itMenu != menuList.end(); itMenu++) {
//
//                myCout << "\nReport for L1 menu:  " << ( *itMenu ) << "\n"
//                        << std::endl;
//
//                myCout
//                    << std::right << std::setw(45) << "Algorithm Key" << " "
//                    << std::right << std::setw(10) << "Passed" << " "
//                    << std::right << std::setw(10) << "Rejected" << " "
//                    << std::right << std::setw(10) << "Error" << "\n";
//
//                for (CItEntry itEntry = listAlgoTrig.begin(); itEntry != listAlgoTrig.end(); itEntry++) {
//
//                    if ( ( ( *itEntry )->gtDaqPartition() == m_physicsDaqPartition )
//                            && ( ( *itEntry )->gtTriggerMenuName() == *itMenu )) {
//
//                        int nrEventsAccept = ( *itEntry )->gtNrEventsAccept();
//                        int nrEventsReject = ( *itEntry )->gtNrEventsReject();
//                        int nrEventsError = ( *itEntry )->gtNrEventsError();
//
//                        myCout
//                            << std::right << std::setw(45) << (( *itEntry )->gtTriggerName()) << " "
//                            << std::right << std::setw(10) << nrEventsAccept << " "
//                            << std::right << std::setw(10) << nrEventsReject << " "
//                            << std::right << std::setw(10) << nrEventsError << "\n";
//
//                    }
//                }
//
//                // efficiency and its statistical error
//
//                myCout << "\n\n"
//                    << std::right << std::setw(45) << "Algorithm Key" << "    "
//                    << std::right << std::setw(10) << "Efficiency " << " "
//                    << std::right << std::setw(10) << "Stat error (%)" << "\n";
//
//                for (CItEntry itEntry = listAlgoTrig.begin(); itEntry != listAlgoTrig.end(); itEntry++) {
//
//                    if ( ( ( *itEntry )->gtDaqPartition() == 0 )
//                            && ( ( *itEntry )->gtTriggerMenuName() == *itMenu )) {
//
//                        int nrEventsAccept = ( *itEntry )->gtNrEventsAccept();
//                        int nrEventsReject = ( *itEntry )->gtNrEventsReject();
//                        int nrEventsError = ( *itEntry )->gtNrEventsError();
//
//                        int totalEvents = nrEventsAccept + nrEventsReject + nrEventsError;
//
//                        // efficiency and their statistical error
//                        float eff = 0.;
//                        float statErrEff = 0.;
//
//                        if (totalEvents != 0) {
//                            eff = static_cast<float> (nrEventsAccept)
//                                    / static_cast<float> (totalEvents);
//                            statErrEff = sqrt(eff * ( 1.0 - eff )
//                                    / static_cast<float> (totalEvents));
//
//                        }
//
//                        myCout
//                            << std::right << std::setw(45) << (( *itEntry )->gtTriggerName()) << " "
//                            << std::right << std::setw(10) << std::fixed << std::setprecision(2)
//                            << 100.*eff << " +- "
//                            << std::right << std::setw(10) << std::setprecision(2)
//                            << 100.*statErrEff << "\n";
//
//
//                    }
//
//                }
//
//                myCout
//                    << "\n\n"
//                    << std::right << std::setw(45) << "Technical Trigger Key" << " "
//                    << std::right << std::setw(10) << "Passed" << " "
//                    << std::right << std::setw(10) << "Rejected" << " "
//                    << std::right << std::setw(10) << "Error" << "\n";
//
//                for (CItEntry itEntry = listTechTrig.begin(); itEntry
//                        != listTechTrig.end(); itEntry++) {
//
//                    if ( ( ( *itEntry )->gtDaqPartition() == m_physicsDaqPartition )
//                            && ( ( *itEntry )->gtTriggerMenuName() == *itMenu )) {
//
//                        int nrEventsAccept = ( *itEntry )->gtNrEventsAccept();
//                        int nrEventsReject = ( *itEntry )->gtNrEventsReject();
//                        int nrEventsError = ( *itEntry )->gtNrEventsError();
//
//                        myCout
//                            << std::right << std::setw(45) << (( *itEntry )->gtTriggerName()) << " "
//                            << std::right << std::setw(10) << nrEventsAccept << " "
//                            << std::right << std::setw(10) << nrEventsReject << " "
//                            << std::right << std::setw(10) << nrEventsError << "\n";
//
//                    }
//                }
//
//                // efficiency and its statistical error
//
//                myCout << "\n\n"
//                    << std::right << std::setw(45) << "Technical Trigger Key" << "    "
//                    << std::right << std::setw(10) << "Efficiency " << " "
//                    << std::right << std::setw(10) << "Stat error (%)" << "\n";
//
//                for (CItEntry itEntry = listTechTrig.begin(); itEntry
//                        != listTechTrig.end(); itEntry++) {
//
//                    if ( ( ( *itEntry )->gtDaqPartition() == 0 )
//                            && ( ( *itEntry )->gtTriggerMenuName() == *itMenu )) {
//
//                        int nrEventsAccept = ( *itEntry )->gtNrEventsAccept();
//                        int nrEventsReject = ( *itEntry )->gtNrEventsReject();
//                        int nrEventsError = ( *itEntry )->gtNrEventsError();
//
//                        int totalEvents = nrEventsAccept + nrEventsReject + nrEventsError;
//
//                        // efficiency and their statistical error
//                        float eff = 0.;
//                        float statErrEff = 0.;
//
//                        if (totalEvents != 0) {
//                            eff = static_cast<float> (nrEventsAccept)
//                                    / static_cast<float> (totalEvents);
//                            statErrEff = sqrt(eff * ( 1.0 - eff )
//                                    / static_cast<float> (totalEvents));
//
//                        }
//
//                        myCout
//                            << std::right << std::setw(45) << (( *itEntry )->gtTriggerName()) << " "
//                            << std::right << std::setw(10) << std::fixed << std::setprecision(2)
//                            << 100.*eff << " +- "
//                            << std::right << std::setw(10) << std::setprecision(2)
//                            << 100.*statErrEff << "\n";
//
//
//                    }
//
//                }
//
//            }
//

        }
            break;
        default: {
            myCout
                << "\n\nL1GtTrigRatesReport: Error - no print verbosity level = " << m_printVerbosity
                << " defined! \nCheck available values in the cfi file." << "\n";
        }

            break;
    }

    // TODO for other verbosity levels
    // print the trigger menu, the prescale factors and the trigger mask, etc


    myCout << std::endl;
    myCout << std::endl;

    switch (m_printOutput) {
        case 0: {

            std::cout << myCout.str() << std::endl;

        }

            break;
        case 1: {

            LogTrace("L1GtTrigRatesReport") << myCout.str() << std::endl;

        }
            break;

        case 2: {

            edm::LogVerbatim("L1GtTrigRatesReport") << myCout.str() << std::endl;

        }

            break;
        case 3: {

            edm::LogInfo("L1GtTrigRatesReport") << myCout.str();

        }

            break;
        default: {
            std::cout
                << "\n\n  L1GtTrigRatesReport: Error - no print output = " << m_printOutput
                << " defined! \n  Check available values in the cfi file." << "\n" << std::endl;

        }
            break;
    }

}

