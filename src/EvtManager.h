#ifndef EvtManager_h
#define EvtManager_h

#include "Common.h"
#include "EvtContext.h"
#include "EvtListener.h"

/// @brief Simple event manager derived directly
/// from context to indicate intent and provide
/// for future expansion of the manager
class EvtManager : public EvtContext
{
public:
  EvtManager();
};

#endif