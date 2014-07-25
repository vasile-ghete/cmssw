#ifndef L1Trigger_GlobalTrigger_L1GtPsbToDigiColl_h
#define L1Trigger_GlobalTrigger_L1GtPsbToDigiColl_h

/**
 * \class L1GtPsbToDigiColl
 *
 *
 * Description: Create GCT collections from GT PSB.
 *
 * Implementation:
 *    TODO: enter details
 *
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 *
 *
 */

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

// forward declarations

// class declaration
#endif
class L1GtPsbToDigiColl: public edm::EDProducer {

public:

    /// constructor(s)
    explicit L1GtPsbToDigiColl(const edm::ParameterSet&);

    /// destructor
    virtual ~L1GtPsbToDigiColl();

private:

    /// private methods

private:

    /// standard CMSSW edm::EDAnalyzer methods

    virtual void beginJob();
    virtual void beginRun(edm::Run&, const edm::EventSetup&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock&,
            const edm::EventSetup&);

    virtual void produce(edm::Event&, const edm::EventSetup&);

    virtual void endLuminosityBlock(edm::LuminosityBlock&,
            const edm::EventSetup&);
    virtual void endRun(edm::Run&, const edm::EventSetup&);

    virtual void endJob();

private:

    /// private members - input

    /// input tag for LHEEventProduct
    edm::InputTag m_inputTagGlobalTrigger;

    /// enable / disable the production of a collection
    bool m_produceIsoEm;
    bool m_produceNonIsoEm;

    bool m_produceCenJets;
    bool m_produceTauJets;
    bool m_produceForJets;

    bool m_produceEtHad;
    bool m_produceEtMiss;
    bool m_produceEtTotal;

    bool m_produceHFRingEtSums;
    bool m_produceHFBitCounts;

    bool m_produceExternalConditions;
    bool m_produceTechnicalTriggers;

private:

    static const int MaxChannel;
    static const int ChPerQuad;

};

#endif /*L1Trigger_GlobalTrigger_L1GtPsbToDigiColl_h*/
