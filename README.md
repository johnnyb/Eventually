# Eventually

[![PlatformIO CI](https://github.com/matthewturner/Eventually/actions/workflows/platformio.yml/badge.svg)](https://github.com/matthewturner/Eventually/actions/workflows/platformio.yml)

## An Arduino Event-based Programming Library

Based on the excellent [Eventually](https://github.com/johnnyb/Eventually) library, the goal is to make a more event-oriented environment for Arduino programming. To make it *actually* easy to use to build worthwhile projects.

See the extension libraries for more capabilities:

* [State machine](https://github.com/matthewturner/EventuallyStateMachine)
* [Command Processor](https://github.com/matthewturner/EventuallyCommand)

### Writing a Simple Eventually Program

To show the program in action, we will write a program that blinks a light, when a button is pushed.

To use Eventually, you need to download the library and then include it in your code (Sketch -> Include Library).  That should put the following line at the top of your program:

```c++
#include <Eventually.h>
```

Next, you need to include a global variable for the event manager. eg `mgr`:

```c++
EvtManager mgr;
```

Eventually works seamlessly with other code in your sketches. Call the `mgr.loopIteration();` on each loop:

```c++
void loop()
{
  // other code
  mgr.loopIteration();
}
```

Now, during our setup() function, instead of just doing pinModes, you also need to setup one or more listeners. The two primary types of listeners for Eventually are EvtPinListener and EvtTimeListener. We are going to use both. We will use the pin listener to tell us when the button is pushed and the time listener to blink the light. Therefore, our setup function will look like this:

```c++
#define LIGHT_PIN 5
#define BUTTON_PIN 3

void setup() {
  /* Set the pin modes */
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  /* Add a button listener */
  mgr.addListener(new EvtPinListener(BUTTON_PIN, (EvtAction)startBlinking));
}
```

Alternatively, if our BUTTON_PIN uses INPUT_PULLUP:

```c++
void setup() {
  // ...

  /* Button is LOW when pushed, so use INPUT_PULLUP */
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  /* Add a button listener, listening for LOW */
  /* (To use this constructor, we also have to specify the debounce delay) */
  mgr.addListener(new EvtPinListener(BUTTON_PIN, 40, LOW, (EvtAction)startBlinking));
}
```

This creates a listener for the given button pin, and will call startBlinking when the button is pushed. Note that by default, EvtPinListeners will automatically debounce the pin, and also make sure that it starts in the opposite state before beginning listening (though both of these are configurable).

Also note that we put (EvtAction) in front of our function name. This makes sure the compiler knows that this will be used as an event action. It may not compile without this marking.

Now, when the button is pushed, we need startBlinking to install a new time-based listener to blink the light, and another button listener to stop the blinking. So the code will look like this:

```c++
bool startBlinking()
{
  mgr.resetContext(); 
  mgr.addListener(new EvtTimeListener(500, true, (EvtAction)blinkPin));
  mgr.addListener(new EvtPinListener(BUTTON_PIN, (EvtAction)stopBlinking));
  return true;
}
```

This starts by calling resetContext(), which will uninstall all current listeners to prepare for a new set. Then, it adds a time listener. The first parameter to the EvtTimeListener constructor is the number of milliseconds to wait until firing. In this case, 500 (half a second). The second parameter is whether or not to continually fire.  If it is set to false, it will fire once and then be done. The last parameter is the function to call.

Then it adds a pin listener to call the stopBlinking function when pushed. All triggered actions need to return true or false. "true" stops the current action chain and is usually what you want.

Now, let's look at the blinkPin function:

```c++
bool blinkState = LOW;
bool blinkPin()
{
  blinkState = !blinkState;
  digitalWrite(LIGHT_PIN, blinkState);
  return false;
}
```

This creates a global variable that tells us the blink state, and whenever this is called, it alternates the blink state and writes that to the pin. This one returns false because, since the blinking doesn't affect the action of other pins, the event chain should continue.

Finally, we need a stopBlinking() function. All this will do is create a new listener to wait for it to start again:

```c++
bool stopBlinking()
{
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTON_PIN, (EvtAction)startBlinking));
  return true;
}
```

Now you are ready to go!

### C++ Issues

One of the goals of this library was to make it use the flexibility of C++ without the user having to know a lot of C++. If you don't know C++, just use the listeners in the basic way mentioned. However, if you want to do more advanced things, this section covers the C++ issues.  

One main goal is to take advantage of the flexibility available with C++ pointers but without exposing the user to them. Therefore, listeners are always created with the "new" keyword, but they are immediately added to the manager (technically, the context) which then manages it. Once the listener is added to the manager/context, then the responsibility for its memory management goes to the manager/context. Note that at this time, there is no ability for user code to detect when a listener gets destroyed, so if you add data to a listener (see section below), then it should not rely on the listener to destroy the data at the right time.

### Heap fragmentation/Memory allocation

While the above approach works, it may:

1. lead to heap fragmentation and you may run out of memory
2. prevent Arduino/PlatformIO from estimating memory usage

Instead, consider enabling/disabling listeners:

```c++
EvtManager mgr;
EvtPinListener startBlinkListener(BUTTON_PIN, (EvtAction)startBlinking);
EvtPinListener stopBlinkListener(BUTTON_PIN, (EvtAction)stopBlinking);

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  mgr.addListener(&startBlinkListener);
  mgr.addListener(&stopBlinkListener);
  
  stopBlinkListener.disable();
}

bool startBlinking()
{
  startBlinkListener.disable();
  stopBlinkListener.enable();
  return true;
}

bool stopBlinking()
{
  stopBlinkListener.disable();
  return true;
}

void loop()
{
  mgr.loopIteration();
}
```

### Writing Your Own Listeners

If you need a more advanced listener, you can write your own fairly easily:

* Implement the IEvtListener interface or create a subclass of EvtListener
* Add a constructor/destructor
* Add a `reset()` method
* Add an `isEventTriggered()` method

The constructor simply takes whatever arguments control the listener's behavior. The reset() method gets called when the listener is added to the chain. This should handle doing things such as sampling the current time or pin states that you will need to determine if the event is triggered.

The `isEventTriggered()` method returns true if the event is triggered.

Below is a simple class that simply always fires:

```c++
class EvtAlwaysFiresListener : public EvtListener
{
  public:
  EvtAlwaysFiresListener();
  ~EvtAlwaysFiresListener();
  void reset();
  bool isEventTriggered();
}

EvtAlwaysFiresListener::EvtAlwaysFiresListener()
{
  // Nothing to construct
}

EvtAlwaysFiresListener::~EvtAlwaysFiresListener()
{
  // Nothing to destruct
}

void EvtAlwaysFiresListener::reset()
{
  // Nothing to setup
}

bool EvtAlwaysFiresListener::isEventTriggered()
{
  return true;
}
```

## Differences with the original

Some of the capabilities were not being used and have been removed to conserve memory. Some other benefits over the original:

* Reduced memory consumption with simplified EvtManager
* Control the number of listeners (default: 10) and contexts (default: 1)
* Bug fixes
* Unit tests
* Ability to enable/disable listeners
* Remove compiler warnings in derived projects
* Reduced reliance on heap allocation
* Work with Arduino/PlatformIO memory tools
* Extract/implement interfaces
* Favour explicitly calling the `mgr.loopIteration()`
