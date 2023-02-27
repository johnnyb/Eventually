/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtContextManager_h
#define EvtContextManager_h

#include "Common.h"
#include "EvtContext.h"
#include "EvtListener.h"

class EvtContextManager : public IEvtContext
{
public:
  EvtContextManager();
  void pushContext(IEvtContext *context);
  void resetContext();
  IEvtContext *popContext();
  IEvtContext *currentContext();

  virtual void reset() override;
  virtual void loopIteration() override;
  virtual void addListener(IEvtListener *lstn) override;
  virtual void removeListener(IEvtListener *lstn) override;
  virtual void manageListeners(bool manage) override;
  virtual byte listenerCount() override;

private:
  IEvtContext *_contextStack[EVENTUALLY_MAX_CONTEXTS];
  byte _contextOffset = 0;
  EvtContext _defaultContext;
};

#endif