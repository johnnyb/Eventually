#include "EvtListener.h"

void EvtListener::setupListener()
{
}

bool EvtListener::isEventTriggered()
{
  return enabled;
}

bool EvtListener::performTriggerAction(EvtContext *ctx)
{
  return (*triggerAction)(this, ctx);
}

void EvtListener::disable()
{
  this->enabled = false;
}
  
void EvtListener::enable()
{
  this->enabled = true;
  this->setupListener();
}