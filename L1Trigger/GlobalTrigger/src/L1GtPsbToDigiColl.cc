/**
 * \class L1GtPsbToDigiColl
 *
 *
 * Description: see header file.
 *
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 *
 *
 */

// this class header
#include "L1Trigger/GlobalTrigger/interface/L1GtPsbToDigiColl.h"

// system include files
#include <memory>

// user include files
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTrigger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTechnicalTriggerRecord.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

#include "CondFormats/L1TObjects/interface/L1GtFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtBoard.h"
#include "CondFormats/L1TObjects/interface/L1GtBoardMaps.h"
#include "CondFormats/DataRecord/interface/L1GtBoardMapsRcd.h"

// constructor(s)
L1GtPsbToDigiColl::L1GtPsbToDigiColl(const edm::ParameterSet& evSetup) :

        m_inputTagGlobalTrigger(
                evSetup.getParameter<edm::InputTag>("InputTagGlobalTrigger")),
        m_produceIsoEm(evSetup.getParameter<bool>("ProduceIsoEm")),
        m_produceNonIsoEm(evSetup.getParameter<bool>("ProduceNonIsoEm")),
        m_produceCenJets(evSetup.getParameter<bool>("ProduceCenJets")),
        m_produceTauJets(evSetup.getParameter<bool>("ProduceTauJets")),
        m_produceForJets(evSetup.getParameter<bool>("ProduceForJets")),
        m_produceEtHad(evSetup.getParameter<bool>("ProduceEtHad")),
        m_produceEtMiss(evSetup.getParameter<bool>("ProduceEtMiss")),
        m_produceEtTotal(evSetup.getParameter<bool>("ProduceEtTotal")),
        m_produceHFRingEtSums(evSetup.getParameter<bool>("ProduceHFRingEtSums")),
        m_produceHFBitCounts(evSetup.getParameter<bool>("ProduceHFBitCounts")),
        m_produceExternalConditions(evSetup.getParameter<bool>("ProduceExternalConditions")),
        m_produceTechnicalTriggers(evSetup.getParameter<bool>("ProduceTechnicalTriggers")) {

    // isolated and non-isolated electron gamma
    if (m_produceIsoEm) {
        produces<L1GctEmCandCollection>("isoEm");
    }

    if (m_produceNonIsoEm) {
        produces<L1GctEmCandCollection>("nonIsoEm");
    }

    // jets
    if (m_produceCenJets) {
        produces<L1GctJetCandCollection>("cenJets");
    }

    if (m_produceTauJets) {
        produces<L1GctJetCandCollection>("tauJets");
    }

    if (m_produceForJets) {
        produces<L1GctJetCandCollection>("forJets");
    }

    // energy sums
    if (m_produceEtHad) {
        produces<L1GctEtHadCollection>();
    }

    if (m_produceEtMiss) {
        produces<L1GctEtMissCollection>();
    }

    if (m_produceEtTotal) {
        produces<L1GctEtTotalCollection>();
    }

    // HF ring sums and HF bit counts
    if (m_produceHFRingEtSums) {
        produces<L1GctHFRingEtSumsCollection>();
    }

    if (m_produceHFBitCounts) {
        produces<L1GctHFBitCountsCollection>();
    }

    // external conditions
    if (m_produceExternalConditions) {
        produces<L1GtTechnicalTriggerRecord>("externalConditions");
    }

    // technical triggers
    if (m_produceTechnicalTriggers) {
        produces<L1GtTechnicalTriggerRecord>("technicalTriggers");
    }

}

// destructor
L1GtPsbToDigiColl::~L1GtPsbToDigiColl() {
    // empty

}

void L1GtPsbToDigiColl::beginJob() {
    // empty
}

void L1GtPsbToDigiColl::beginRun(edm::Run& iRun,
        const edm::EventSetup& evSetup) {
    // empty
}

void L1GtPsbToDigiColl::beginLuminosityBlock(edm::LuminosityBlock& iLumBlock,
        const edm::EventSetup& evSetup) {
    // empty
}

void L1GtPsbToDigiColl::produce(edm::Event& iEvent,
        const edm::EventSetup& evSetup) {

    // GT readout record
    edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
    iEvent.getByLabel(m_inputTagGlobalTrigger, gtReadoutRecord);
    if (!gtReadoutRecord.isValid()) {
        throw cms::Exception("ProductNotFound")
                << "\nFailed to get L1GlobalTriggerReadoutRecord handle "
                << " with input tag " << m_inputTagGlobalTrigger << std::endl;
    }

    // Get the board maps
    edm::ESHandle<L1GtBoardMaps> l1GtBM;
    evSetup.get<L1GtBoardMapsRcd>().get(l1GtBM);
    if (!l1GtBM.isValid()) {
        throw cms::Exception("Configuration")
                << "\nFailed to get L1GtBoardMapsRcd handle " << std::endl;
    }

    const std::vector<L1GtBoard>& boardMaps = l1GtBM.product()->gtBoardMaps();

    const std::vector<L1GtPsbWord>& psbWords = gtReadoutRecord->gtPsbVector();

    // Loop over all words in the GT readout record
    // for every board id -> search the corresponding board in the board map
    // and then loop over the PSB quadruple assigned to that board
    // create all the required GCT objects

    std::auto_ptr<L1GctEmCandCollection> gctNonIsoEmCands(
            new L1GctEmCandCollection);
    std::auto_ptr<L1GctEmCandCollection> gctIsoEmCands(
            new L1GctEmCandCollection());

    std::auto_ptr<L1GctJetCandCollection> gctForJetCands(
            new L1GctJetCandCollection);
    std::auto_ptr<L1GctJetCandCollection> gctCenJetCands(
            new L1GctJetCandCollection);
    std::auto_ptr<L1GctJetCandCollection> gctTauJetCands(
            new L1GctJetCandCollection);

    std::auto_ptr<L1GctEtTotalCollection> gctEtTotCol(
            new L1GctEtTotalCollection);
    std::auto_ptr<L1GctEtHadCollection> gctEtHadCol(new L1GctEtHadCollection);
    std::auto_ptr<L1GctEtMissCollection> gctEtMissCol(
            new L1GctEtMissCollection);

    std::auto_ptr<L1GctHFRingEtSumsCollection> gctHFRingEtSumsCol(
            new L1GctHFRingEtSumsCollection);
    std::auto_ptr<L1GctHFBitCountsCollection> gctHFBitCountsCol(
            new L1GctHFBitCountsCollection);

    std::auto_ptr<L1GtTechnicalTriggerRecord> externalConditionsCol(
            new L1GtTechnicalTriggerRecord);

    std::auto_ptr<L1GtTechnicalTriggerRecord> technicalTriggersCol(
            new L1GtTechnicalTriggerRecord);


    for (std::vector<L1GtPsbWord>::const_iterator iWord = psbWords.begin();
            iWord != psbWords.end(); ++iWord) {

        const L1GtBoard *board = 0;

        for (std::vector<L1GtBoard>::const_iterator itBoard = boardMaps.begin();
                itBoard != boardMaps.end(); ++itBoard) {

            if (iWord->boardId() == itBoard->gtBoardId()) {
                board = &*itBoard;

                break;
            }
        }

        if (board == 0) {
            edm::LogError("L1GtPsbToDigiColl") << "\nCould not match boardId "
                    << iWord->boardId() << "\n" << std::endl;
            break;
        }

        std::vector<L1GtPsbQuad> quadInPsb = board->gtQuadInPsb();

        int iCount = 0;

        for (std::vector<L1GtPsbQuad>::const_iterator itQuad =
                quadInPsb.begin(); itQuad != quadInPsb.end(); ++itQuad) {

            int channelUpper = (MaxChannel - ChPerQuad * iCount);
            int channelLower = channelUpper - ChPerQuad + 1;
            iCount++;

            switch (*itQuad) {

                case NoIsoEGQ:
                    if (m_produceNonIsoEm) {
                        for (int iAB = 0; iAB <= 1; iAB++) {
                            for (int iChan = channelLower;
                                    iChan <= channelUpper; iChan++) {

                                uint16_t rawData;
                                if (iAB == 0) {
                                    rawData = iWord->aData(iChan);
                                } else {
                                    rawData = iWord->bData(iChan);
                                }

                                // Non-Isolated = False
                                L1GctEmCand tmpEmCand(rawData, 0, 0, 0,
                                        iWord->bxInEvent());
                                gctNonIsoEmCands->push_back(tmpEmCand);
                            }
                        }
                    }
                    break;

                case IsoEGQ:
                    if (m_produceIsoEm) {
                        for (int iAB = 0; iAB <= 1; iAB++) {
                            for (int iChan = channelLower;
                                    iChan <= channelUpper; iChan++) {

                                uint16_t rawData;
                                if (iAB == 0) {
                                    rawData = iWord->aData(iChan);
                                } else {
                                    rawData = iWord->bData(iChan);
                                }

                                // Isolated = True
                                L1GctEmCand tmpEmCand(rawData, 1, 0, 0,
                                        iWord->bxInEvent());
                                gctIsoEmCands->push_back(tmpEmCand);
                            }
                        }
                    }
                    break;

                case ForJetQ:
                    if (m_produceForJets) {

                        // iAB -> A or B, A=0
                        for (int iAB = 0; iAB <= 1; iAB++) {
                            for (int iChan = channelLower;
                                    iChan <= channelUpper; iChan++) {

                                uint16_t rawData;
                                if (iAB == 0) {
                                    rawData = iWord->aData(iChan);
                                } else {
                                    rawData = iWord->bData(iChan);
                                }

                                // construct jet from raw data
                                // L1GctJetCand(uint16_t rawData, bool isTau, bool isFor,
                                // uint16_t block, uint16_t index, int16_t bx);

                                L1GctJetCand tmpJetCand(rawData, 0, 1, 0, 0,
                                        iWord->bxInEvent());
                                gctForJetCands->push_back(tmpJetCand);
                            }
                        }

                    }
                    break;

                case CenJetQ:
                    if (m_produceCenJets) {

                        // iAB -> A or B, A=0
                        for (int iAB = 0; iAB <= 1; iAB++) {
                            for (int iChan = channelLower;
                                    iChan <= channelUpper; iChan++) {

                                uint16_t rawData;
                                if (iAB == 0) {
                                    rawData = iWord->aData(iChan);
                                } else {
                                    rawData = iWord->bData(iChan);
                                }

                                // construct jet from raw data
                                // L1GctJetCand(uint16_t rawData, bool isTau, bool isFor,
                                // uint16_t block, uint16_t index, int16_t bx);

                                L1GctJetCand tmpJetCand(rawData, 0, 0, 0, 0,
                                        iWord->bxInEvent());
                                gctCenJetCands->push_back(tmpJetCand);
                            }
                        }
                    }
                    break;

                case TauJetQ:
                    if (m_produceTauJets) {

                        for (int iAB = 0; iAB <= 1; iAB++) {
                            for (int iChan = channelLower;
                                    iChan <= channelUpper; iChan++) {

                                uint16_t rawData;
                                if (iAB == 0) {
                                    rawData = iWord->aData(iChan);
                                } else {
                                    rawData = iWord->bData(iChan);
                                }

                                // construct jet from raw data
                                // L1GctJetCand(uint16_t rawData, bool isTau, bool isFor,
                                // uint16_t block, uint16_t index, int16_t bx);

                                L1GctJetCand tmpJetCand(rawData, 1, 0, 0, 0,
                                        iWord->bxInEvent());
                                gctTauJetCands->push_back(tmpJetCand);
                            }
                        }
                    }
                    break;

                case ESumsQ: {
                    if (m_produceEtTotal) {
                        uint16_t rawEtTot = iWord->aData(channelLower);
                        L1GctEtTotal tmpEtTotal(rawEtTot, iWord->bxInEvent());
                        gctEtTotCol->push_back(tmpEtTotal);
                    }

                    if (m_produceEtHad) {
                        uint16_t rawEtHad = iWord->bData(channelLower);
                        L1GctEtHad tmpEtHad(rawEtHad, iWord->bxInEvent());
                        gctEtHadCol->push_back(tmpEtHad);
                    }

                    if (m_produceEtMiss) {

                        // ETM rawData is 32 bit !!
                        int wordLen = 16;
                        uint32_t rawData = (iWord->bData(channelUpper)
                                << wordLen) | iWord->aData(channelUpper);

                        L1GctEtMiss tmpEtMiss(rawData, iWord->bxInEvent());

                        gctEtMissCol->push_back(tmpEtMiss);

                    }

                }
                    break;

                case HfQ: {

                    if (m_produceHFRingEtSums) {

                        L1GctHFRingEtSums tmpHFR;
                        uint32_t rawData = 0;

                        uint32_t datA, datB;
                        int aMask = 0x7;
                        int bMask = 0x1FF;
                        int lengthOfBitCounts = 12;
                        int lengthOfHFRinA = 3;

                        datA = (iWord->aData(channelLower) >> lengthOfBitCounts)
                                & aMask;
                        datB = (iWord->bData(channelLower) & bMask);
                        rawData = (datB << lengthOfHFRinA) | datA;

                        tmpHFR.setBx(iWord->bxInEvent());
                        tmpHFR.setData(rawData);

                        gctHFRingEtSumsCol->push_back(tmpHFR);

                    }
                    if (m_produceHFBitCounts) {

                        uint32_t rawData = 0;

                        // lowest 12-bits of A are the HFBitCounts
                        int mask = 0xFFF;
                        rawData = (iWord->aData(channelLower) & mask);

                        L1GctHFBitCounts tmpHFB;
                        tmpHFB.setBx(iWord->bxInEvent());
                        tmpHFB.setData(rawData);
                        gctHFBitCountsCol->push_back(tmpHFB);

                    }

                }
                    break;

                case TechTr: {
                    // TODO add technical triggers
                }
                    break;

                case GtExternalQ: {
                    // TODO add external conditions
                }
                    break;

                case Free:
                case JetCountsQ:
                case MQB1:
                case MQB2:
                case MQF3:
                case MQF4:
                case MQB5:
                case MQB6:
                case MQF7:
                case MQF8:
                case MQB9:
                case MQB10:
                case MQF11:
                case MQF12:
                case CastorQ:
                case BptxQ:
                case PsbQuadNull: {
                    // do nothing
                }
                    break;

                default: {
                    // should not arrive here
                    throw cms::Exception("Configuration")
                            << "\n Invalid object quadruple " << " labeled "
                            << (*itQuad) << std::endl;

                }
                    break;

            }
        }

    }

    if (m_produceNonIsoEm) {
        iEvent.put(gctNonIsoEmCands, "nonIsoEm");
    }

    if (m_produceIsoEm) {
        iEvent.put(gctIsoEmCands, "isoEm");
    }

    if (m_produceCenJets) {
        iEvent.put(gctCenJetCands, "cenJets");
    }

    if (m_produceTauJets) {
        iEvent.put(gctTauJetCands, "tauJets");
    }

    if (m_produceForJets) {
        iEvent.put(gctForJetCands, "forJets");
    }

    if (m_produceEtTotal) {
        iEvent.put(gctEtTotCol);
    }

    if (m_produceEtHad) {
        iEvent.put(gctEtHadCol);
    }

    if (m_produceEtMiss) {
        iEvent.put(gctEtMissCol);
    }

    if (m_produceHFRingEtSums) {
        iEvent.put(gctHFRingEtSumsCol);
    }

    if (m_produceHFBitCounts) {
        iEvent.put(gctHFBitCountsCol);
    }

    if (m_produceExternalConditions) {
        iEvent.put(externalConditionsCol, "externalConditions");
    }

    if (m_produceTechnicalTriggers) {
        iEvent.put(technicalTriggersCol, "technicalTriggers");
    }

}

void L1GtPsbToDigiColl::endLuminosityBlock(edm::LuminosityBlock& iLumBlock,
        const edm::EventSetup& evSetup) {
}

void L1GtPsbToDigiColl::endRun(edm::Run& iRun, const edm::EventSetup& evSetup) {
}

void L1GtPsbToDigiColl::endJob() {
}

// static data members

const int L1GtPsbToDigiColl::MaxChannel = 7;
const int L1GtPsbToDigiColl::ChPerQuad = 2;

