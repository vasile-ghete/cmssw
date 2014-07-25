#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTrigger.h"
#include "L1Trigger/GlobalTrigger/interface/ConvertObjectMapRecord.h"
#include "L1Trigger/GlobalTrigger/interface/CompareToObjectMapRecord.h"
#include "L1Trigger/GlobalTrigger/interface/L1GtPsbToGctColl.h"
  
DEFINE_FWK_MODULE(L1GlobalTrigger);
DEFINE_FWK_MODULE(ConvertObjectMapRecord);
DEFINE_FWK_MODULE(CompareToObjectMapRecord);
DEFINE_FWK_MODULE(L1GtPsbToGctColl);
