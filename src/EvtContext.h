/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtContext_h
#define EvtContext_h

#define EVENTUALLY_MAX_CONTEXTS 10
#define EVENTUALLY_MAX_LISTENERS 20

#include "EvtListener.h"

// Note - should probably expand the number of available listeners by chaining contexts
class EvtContext
{
public:
  void *data = 0;

  EvtContext();
  void setupContext();
  void loopIteration();
  void addListener(EvtListener *lstn);
  void removeListener(EvtListener *lstn);

private:
  EvtListener **listeners = 0;
  int listenerCount;
};

#endif