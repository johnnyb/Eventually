#ifndef IEvtContext_h
#define IEvtContext_h

#include "IEvtListener.h"
#include <Arduino.h>

class IEvtContext
{
public:
  IEvtContext();

  /// @brief Resets the context to its initial state, removing all listeners.
  /// Potentially introduces fragmentation of the heap so use sparingly.
  /// Consider disabling the listener instead.
  virtual void reset() = 0;

  /// @brief Executes all listeners in this cycle.
  virtual void loopIteration() = 0;

  /// @brief Adds a listener to the list of listeners.
  /// @param lstn
  virtual void addListener(IEvtListener *lstn) = 0;

  /// @brief Removes the listener and frees memory (using delete).
  /// Potentially introduces fragmentation of the heap so use sparingly.
  /// Consider disabling the listener instead.
  /// @param lstn
  virtual void removeListener(IEvtListener *lstn) = 0;

  /// @brief Determines whether memory is controlled by
  /// the context or caller
  /// @param manage
  virtual void manageListeners(bool manage) = 0;

  /// @brief The count of listeners registered with the context
  /// @return
  virtual byte listenerCount() = 0;
};

#endif