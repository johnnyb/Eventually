#include "EvtListener.h"

void EvtListener::reset()
{
}

bool EvtListener::isEventTriggered()
{
  return _enabled;
}

bool EvtListener::performTriggerAction(IEvtContext *ctx)
{
  return (*_triggerAction)(this, ctx);
}

void EvtListener::disable()
{
  _enabled = false;
}

void EvtListener::enable()
{
  _enabled = true;
  reset();
}

EvtListener::~EvtListener()
{
}