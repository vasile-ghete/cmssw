#ifndef GlobalTriggerAnalyzer_L1GtTrigRatesReport_h
#define GlobalTriggerAnalyzer_L1GtTrigRatesReport_h

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

// system include files
#include <memory>
#include <string>
#include <vector>
#include <list>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtTrigRatesReportEntry.h"

// forward declarations
class L1GtStableParameters;
class L1GtPrescaleFactors;
class L1GtTriggerMask;
class L1GtTriggerMenu;

// class declaration

class L1GtTrigRatesReport: public edm::EDAnalyzer {

public:

    /// constructor
    explicit L1GtTrigRatesReport(const edm::ParameterSet&);

    /// destructor
    virtual ~L1GtTrigRatesReport();

private:

    /// private methods

    /// compute the algorithm or technical trigger rate given the number of accepted
    /// events, total number of events, the type of the sample (data: isData = true, or MC: isData = false),
    /// corrected for deadtime (corr4Deadtime = true)
    void const computeTrigRates(const int nrAcceptedEvts, const int totalNrEvts,
            bool sampleIsData, bool corr4Deadtime, float& rate,
            float& rateStatErr) const;

    /// compute the algorithm or technical trigger rates in a LS in a run
    void computeTrigRatesLs(const int runNumber, const int lsNumber,
            std::list<L1GtTrigRatesReportEntry*>& listTrig, float& lsTotalRate,
            float& lsTotalRateStatErr);

    /// print trigger rates
    void printL1TrigRates(const int runNumber, const int lsNumber,
            const int lsPileup, const std::string& menuName,
            const std::string& triggerName, const int prescaleFactor,
            const std::list<L1GtTrigRatesReportEntry*> listAlgoTrig,
            const std::list<L1GtTrigRatesReportEntry*> listTechTrig);

private:

    /// standard CMSSW edm::EDAnalyzer methods

    virtual void beginJob();
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void beginLuminosityBlock(const edm::LuminosityBlock&,
            const edm::EventSetup&);

    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    virtual void endLuminosityBlock(const edm::LuminosityBlock&,
            const edm::EventSetup&);
    virtual void endRun(const edm::Run&, const edm::EventSetup&);

    virtual void endJob();

private:

    /// input parameters

    /// boolean flag to select the input record
    bool m_useL1GlobalTriggerRecord;

    /// input tag for GT record (L1 GT DAQ record or L1 GT "lite" record):
    edm::InputTag m_l1GtRecordInputTag;

    /// number of bunch crosses
    int m_numberBunches;

    /// type of the event sample: true for data, false for MC
    bool m_sampleIsData;

    /// sample cross section
    double m_sampleCrossSection;

    /// sample cross section
    int m_samplePU;

    /// correction for deadtime required
    bool m_corr4Deadtime;


    /// print settings

    /// print detailed LS reports
    bool m_printLsReports;

    /// print detailed run reports
    bool m_printRunReports;

    /// print detailed job reports
    bool m_printJobReports;

    /// print verbosity
    int m_printVerbosity;

    /// print output
    int m_printOutput;

private:

    /// cached stuff

    /// stable parameters
    const L1GtStableParameters* m_l1GtStablePar;
    unsigned long long m_l1GtStableParCacheID;

    /// number of algorithm triggers
    unsigned int m_numberAlgoTriggers;

    /// number of technical triggers
    unsigned int m_numberTechnicalTriggers;

    /// prescale factors
    const L1GtPrescaleFactors* m_l1GtPfAlgo;
    unsigned long long m_l1GtPfAlgoCacheID;

    const L1GtPrescaleFactors* m_l1GtPfTech;
    unsigned long long m_l1GtPfTechCacheID;

    const std::vector<std::vector<int> >* m_prescaleFactorsAlgoTrig;
    const std::vector<std::vector<int> >* m_prescaleFactorsTechTrig;

    /// trigger masks & veto masks
    const L1GtTriggerMask* m_l1GtTmAlgo;
    unsigned long long m_l1GtTmAlgoCacheID;

    const L1GtTriggerMask* m_l1GtTmTech;
    unsigned long long m_l1GtTmTechCacheID;

    const L1GtTriggerMask* m_l1GtTmVetoAlgo;
    unsigned long long m_l1GtTmVetoAlgoCacheID;

    const L1GtTriggerMask* m_l1GtTmVetoTech;
    unsigned long long m_l1GtTmVetoTechCacheID;

    std::vector<unsigned int> m_triggerMaskAlgoTrig;
    std::vector<unsigned int> m_triggerMaskTechTrig;

    std::vector<unsigned int> m_triggerMaskVetoAlgoTrig;
    std::vector<unsigned int> m_triggerMaskVetoTechTrig;

    // trigger menu
    const L1GtTriggerMenu* m_l1GtMenu;
    unsigned long long m_l1GtMenuCacheID;


private:

    /// internal members

    // LHC rotation frequency
    double m_rotationFrequency;

    /// LS time length [sec]
    double m_lsLengthSec;

    /// counters

    /// total number of luminosity blocks processed per run
    int m_runTotalNrLS;

    /// total number of events processed per job / run / LS
    int m_jobTotalNrEvents;
    int m_runTotalNrEvents;
    int m_lsTotalNrEvents;

    /// total number of events with error (EDProduct[s] not found) per job / run / LS
    int m_jobNrL1Errors;
    int m_runNrL1Errors;
    int m_lsNrL1Errors;

    /// total number of events accepted by any of the L1 trigger in any menu per job / run / LS
    int m_jobNrL1Accepts;
    int m_runNrL1Accepts;
    int m_lsNrL1Accepts;

    /// list of individual entries in the report for each algorithm / technical triggers per LS
    std::list<L1GtTrigRatesReportEntry*> m_lsEntryListAlgoTrig;
    std::list<L1GtTrigRatesReportEntry*> m_lsEntryListTechTrig;
    ///
    /// and for global decision
    std::list<L1GtTrigRatesReportEntry*> m_lsEntryListGlobal;

    /// list of individual entries in the report for each algorithm / technical triggers
    /// per run, summed over LS
    std::list<L1GtTrigRatesReportEntry*> m_runEntryListAlgoTrig;
    std::list<L1GtTrigRatesReportEntry*> m_runEntryListTechTrig;

    typedef std::list<L1GtTrigRatesReportEntry*>::const_iterator CItEntry;
    typedef std::list<L1GtTrigRatesReportEntry*>::iterator ItEntry;

    /// index of physics DAQ partition
    unsigned int m_physicsDaqPartition;

};

#endif /*GlobalTriggerAnalyzer_L1GtTrigRatesReport_h*/
