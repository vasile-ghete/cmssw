/**
 * \class L1GtTrigRatesReportEntry
 *
 *
 * Description: L1 Trigger report.
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 *
 *
 */

// this class header
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtTrigRatesReportEntry.h"

// system include files
#include <string>
#include <iostream>
#include <iomanip>

// user include files

// constructor(s)
L1GtTrigRatesReportEntry::L1GtTrigRatesReportEntry() {

    m_runNumber = 0;
    m_lsNumber = 0;
    m_lsPileup = 0;

    m_triggerMenuName = "";
    m_triggerName = "";
    m_prescaleFactor = 0;

    // counters

    m_nrEventsAccept = 0;
    m_nrEventsReject = 0;
    m_nrEventsError = 0;

    // rates
    m_triggerRate = 0.;
    m_triggerRateStatErr = 0.;

}

L1GtTrigRatesReportEntry::L1GtTrigRatesReportEntry(const int runNumber,
        const int lsNumber, const int lsPileup, const std::string& menuName,
        const std::string& triggerName, const int prescaleFactor) {

    m_runNumber = runNumber;
    m_lsNumber = lsNumber;
    m_lsPileup = lsPileup;

    m_triggerMenuName = menuName;
    m_triggerName = triggerName;
    m_prescaleFactor = prescaleFactor;

    // counters

    m_nrEventsAccept = 0;
    m_nrEventsReject = 0;
    m_nrEventsError = 0;

    // rates
    m_triggerRate = 0.;
    m_triggerRateStatErr = 0.;

}

// destructor
L1GtTrigRatesReportEntry::~L1GtTrigRatesReportEntry() {

    //empty

}

// assignment operator
L1GtTrigRatesReportEntry& L1GtTrigRatesReportEntry::operator=(
        const L1GtTrigRatesReportEntry& repEntry) {

    if (this != &repEntry) {

        m_runNumber = repEntry.m_runNumber;
        m_lsNumber = repEntry.m_lsNumber;
        m_lsPileup = repEntry.m_lsPileup;

        m_triggerMenuName = repEntry.m_triggerMenuName;
        m_triggerName = repEntry.m_triggerName;
        m_prescaleFactor = repEntry.m_prescaleFactor;

        m_nrEventsAccept = repEntry.m_nrEventsAccept;
        m_nrEventsReject = repEntry.m_nrEventsReject;
        m_nrEventsError = repEntry.m_nrEventsError;

        m_triggerRate = repEntry.m_triggerRate;
        m_triggerRateStatErr = repEntry.m_triggerRateStatErr;

    }

    return *this;

}

// equal operator
bool L1GtTrigRatesReportEntry::operator==(
        const L1GtTrigRatesReportEntry& repEntry) const {

    // faster comparisons first

    if (m_runNumber != repEntry.m_runNumber) {
        return false;
    }

    if (m_lsNumber != repEntry.m_lsNumber) {
        return false;
    }

    if (m_lsPileup != repEntry.m_lsPileup) {
        return false;
    }

    if (m_prescaleFactor != repEntry.m_prescaleFactor) {
        return false;
    }

    if (m_triggerName != repEntry.m_triggerName) {
        return false;
    }

    if (m_triggerMenuName != repEntry.m_triggerMenuName) {
        return false;
    }

    // all selected members identical
    return true;

}

// unequal operator
bool L1GtTrigRatesReportEntry::operator!=(
        const L1GtTrigRatesReportEntry& result) const {

    return !(result == *this);

}

// member functions

/// increase # of events accepted/rejected for this entry
void L1GtTrigRatesReportEntry::addValidEntry(const bool triggerResult) {

    if (triggerResult) {
        m_nrEventsAccept++;
    } else {
        m_nrEventsReject++;
    }

}

/// increase # of events with error
void L1GtTrigRatesReportEntry::addErrorEntry() {

    m_nrEventsError++;
}

void L1GtTrigRatesReportEntry::sumEventsEntries(
        const L1GtTrigRatesReportEntry* entryRep) {

    m_nrEventsAccept += entryRep->m_nrEventsAccept;
    m_nrEventsReject += entryRep->m_nrEventsReject;
    m_nrEventsError += entryRep->m_nrEventsError;

}

// isEqual operator - partial comparison of two entries
bool L1GtTrigRatesReportEntry::isEqual(const L1GtTrigRatesReportEntry& repEntry,
        const int compareMode) const {

    switch (compareMode) {
        case 0: {

            // full comparison for all comparable quantities

            if (*this != repEntry) {
                return false;
            }
        }

            break;
        case 1: {

            // no comparison on m_runNumber

            if (m_lsNumber != repEntry.m_lsNumber) {
                return false;
            }

            if (m_lsPileup != repEntry.m_lsPileup) {
                return false;
            }

            if (m_prescaleFactor != repEntry.m_prescaleFactor) {
                return false;
            }

            if (m_triggerName != repEntry.m_triggerName) {
                return false;
            }

            if (m_triggerMenuName != repEntry.m_triggerMenuName) {
                return false;
            }
        }

            break;
        case 2: {

            if (m_runNumber != repEntry.m_runNumber) {
                return false;
            }

            // no comparison on m_lsNumber

            if (m_lsPileup != repEntry.m_lsPileup) {
                return false;
            }

            if (m_prescaleFactor != repEntry.m_prescaleFactor) {
                return false;
            }

            if (m_triggerName != repEntry.m_triggerName) {
                return false;
            }

            if (m_triggerMenuName != repEntry.m_triggerMenuName) {
                return false;
            }
        }

            break;
        case 3: {

            if (m_runNumber != repEntry.m_runNumber) {
                return false;
            }

            if (m_lsNumber != repEntry.m_lsNumber) {
                return false;
            }

            // no comparison on m_lsPileup

            if (m_prescaleFactor != repEntry.m_prescaleFactor) {
                return false;
            }

            if (m_triggerName != repEntry.m_triggerName) {
                return false;
            }

            if (m_triggerMenuName != repEntry.m_triggerMenuName) {
                return false;
            }
        }

            break;
        default: {
            return false;
        }

            break;
    }

    // all selected members identical
    return true;

}

std::ostream& operator<<(std::ostream& streamRec,
        const L1GtTrigRatesReportEntry& result) {

    streamRec << "\n " << std::endl;
    streamRec << "Run number: " << result.gtRunNumber() << std::endl;
    streamRec << "LS number:  " << result.gtLsNumber() << std::endl;
    streamRec << "LS pileup:  " << result.gtLsPileup() << std::endl;
    streamRec << "L1 trigger menu:  " << result.gtTriggerMenuName()
            << std::endl;
    streamRec << "L1 trigger name:  " << result.gtTriggerName() << std::endl;

    streamRec << "Number of accepted events:   " << result.gtNrEventsAccept()
            << std::endl;
    streamRec << "Number of rejected events:   " << result.gtNrEventsReject()
            << std::endl;
    streamRec << "Number of events with error: " << result.gtNrEventsError()
            << std::endl;

    streamRec << "Average trigger rate [Hz]: " << result.gtTriggerRate()
            << std::endl;
    streamRec << "Average trigger rate statistical error [Hz]: "
            << result.gtTriggerRateStatErr() << std::endl;

    streamRec << "\n " << std::endl;

    return streamRec;

}

