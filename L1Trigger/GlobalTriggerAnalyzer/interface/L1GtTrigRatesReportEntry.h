#ifndef GlobalTriggerAnalyzer_L1GtTrigRatesReportEntry_h
#define GlobalTriggerAnalyzer_L1GtTrigRatesReportEntry_h

/**
 * \class L1GtTrigRatesReportEntry
 * 
 * 
 * Description: an individual L1 GT report entry.
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 *
 */

// system include files
#include <string>
#include <iosfwd>

// class declaration

class L1GtTrigRatesReportEntry {

public:

    /// constructor(s)
    explicit L1GtTrigRatesReportEntry();

    explicit L1GtTrigRatesReportEntry(const int runNumber, const int lsNumber,
            const int lsPileup, const std::string& menuName,
            const std::string& triggerName, const int prescaleFactor);

    /// destructor
    virtual ~L1GtTrigRatesReportEntry();

public:

    /// assignment operator
    L1GtTrigRatesReportEntry& operator=(const L1GtTrigRatesReportEntry&);

    /// equal operator
    bool operator==(const L1GtTrigRatesReportEntry&) const;

    /// unequal operator
    bool operator!=(const L1GtTrigRatesReportEntry&) const;

public:

    ///  get the run number
    inline const int gtRunNumber() const {
        return m_runNumber;
    }

    ///  get /set the luminosity segment number
    inline const int gtLsNumber() const {
        return m_lsNumber;
    }

    inline void setLsNumber(const int lsNumber) {
        m_lsNumber = lsNumber;
    }

    ///  get / set the pileup value for the luminosity segment
    inline const int gtLsPileup() const {
        return m_lsPileup;
    }

    inline void setLsPileup(const int lsPileup) {
        m_lsPileup = lsPileup;
    }

    /// get the trigger menu name
    inline const std::string gtTriggerMenuName() const {
        return m_triggerMenuName;
    }

    ///  get the algorithm / technical trigger name
    inline const std::string gtTriggerName() const {
        return m_triggerName;
    }

    ///  get the prescale factor
    inline const int gtPrescaleFactor() const {
        return m_prescaleFactor;
    }

    ///  get / set the number of events accepted for this entry
    inline const int gtNrEventsAccept() const {
        return m_nrEventsAccept;
    }

    inline void setNrEventsAccept(const float nrEvents) {
        m_nrEventsAccept = nrEvents;
    }

    /// get / set the number of events rejected for this entry
    inline const int gtNrEventsReject() const {
        return m_nrEventsReject;
    }

    inline void setNrEventsReject(const float nrEvents) {
        m_nrEventsReject = nrEvents;
    }

    /// get / set the number of events with error for this entry
    inline const int gtNrEventsError() const {
        return m_nrEventsError;
    }

    inline void setNrEventsError(const float nrEvents) {
        m_nrEventsError = nrEvents;
    }

    /// get / set the average trigger rate
    inline const float gtTriggerRate() const {
        return m_triggerRate;
    }

    inline void setTriggerRate(const float triggerRate) {
        m_triggerRate = triggerRate;
    }

    /// get / set the statistical error on the trigger rate
    inline const float gtTriggerRateStatErr() const {
        return m_triggerRateStatErr;
    }

    inline void setTriggerRateStatErr(const float triggerRateStatErr) {
        m_triggerRateStatErr = triggerRateStatErr;
    }


public:

    /// increase # of events accepted/rejected for this entry
    void addValidEntry(const bool triggerResultAfterMask);

    /// increase # of events with error 
    void addErrorEntry();

    /// add two entries
    void sumEventsEntries(const L1GtTrigRatesReportEntry*);

    /// isEqual operator - partial comparison of two entries
    bool isEqual(const L1GtTrigRatesReportEntry& repEntry,
            const int compareMode) const;

    /// output stream operator
    friend std::ostream& operator<<(std::ostream&,
            const L1GtTrigRatesReportEntry&);

private:

    /// run number
    int m_runNumber;

    /// luminosity block (segment) number
    int m_lsNumber;

    /// pile-up for a given LS
    int m_lsPileup;

    /// menu name 
    std::string m_triggerMenuName;

    /// algorithm / technical trigger name
    std::string m_triggerName;

    /// prescale factor
    int m_prescaleFactor;

    /// counters

    /// number of events accepted for this entry
    int m_nrEventsAccept;

    /// number of events rejected for this entry
    int m_nrEventsReject;

    /// number of events with error
    int m_nrEventsError;

    /// average rate (Hz)
    float m_triggerRate;

    /// statistical error on the trigger rate
    float m_triggerRateStatErr;

};

#endif /*GlobalTriggerAnalyzer_L1GtTrigRatesReportEntry_h*/
