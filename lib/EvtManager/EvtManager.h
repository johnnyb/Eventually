#ifndef EvtManager_h
#define EvtManager_h

#include "EvtContext.h"
#include "EvtListener.h"

class EvtManager
{
public:
  EvtManager();
  void loopIteration();
  EvtContext *pushContext();
  EvtContext *resetContext();
  EvtContext *popContext();
  EvtContext *currentContext();
  void addListener(EvtListener *lstn);
  void removeListener(EvtListener *lstn);

private:
  EvtContext *contextStack = 0;
  int contextOffset = 0;
  int contextDepth = 0;
};

#endif