#include <ArduinoFake.h>
#include <unity.h>

#include "EvtContext.h"
#include "MockEvtListener.h"

using namespace fakeit;

EvtContext target;
Mock<IEvtListener> listenerMock;
Mock<IEvtListener> nonBlockingListenerMock;

void setUp(void)
{
    target.manageListeners(false);
    When(Method(listenerMock, reset)).AlwaysReturn();
    When(Method(listenerMock, isEventTriggered)).AlwaysReturn(true);
    When(Method(listenerMock, performTriggerAction)).AlwaysReturn(true);
    Fake(Dtor(listenerMock));

    When(Method(nonBlockingListenerMock, reset)).AlwaysReturn();
    When(Method(nonBlockingListenerMock, isEventTriggered)).AlwaysReturn(true);
    When(Method(nonBlockingListenerMock, performTriggerAction)).AlwaysReturn(false);
    Fake(Dtor(nonBlockingListenerMock));
}

void tearDown(void)
{
    listenerMock.Reset();
    nonBlockingListenerMock.Reset();
    target.reset();
}

void test_add_listener_resets_listener(void)
{
    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);
    Verify(Method(listenerMock, reset)).Once();
}

void test_add_listener_in_middle_slot_resets_listener(void)
{
    MockEvtListener *listenerToRemove = new MockEvtListener();
    target.addListener(listenerToRemove);
    target.addListener(new MockEvtListener());
    target.removeListener(listenerToRemove);

    IEvtListener &listener = listenerMock.get();
    // add in the newly vacated slot
    target.addListener(&listener);

    Verify(Method(listenerMock, reset)).Once();
}

void test_listener_count_does_not_include_empty_slots(void)
{
    MockEvtListener *listenerToRemove = new MockEvtListener();
    target.addListener(listenerToRemove);
    target.addListener(new MockEvtListener());
    target.removeListener(listenerToRemove);

    TEST_ASSERT_EQUAL(1, target.listenerCount());
}

void test_loop_iteration_calls_listener(void)
{
    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);
    target.loopIteration();
    Verify(Method(listenerMock, performTriggerAction)).Once();
}

void test_loop_iteration_calls_all_listeners(void)
{
    IEvtListener &nonBlockingListener = nonBlockingListenerMock.get();
    target.addListener(&nonBlockingListener);

    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);

    target.loopIteration();

    Verify(Method(nonBlockingListenerMock, performTriggerAction)).Once();
    Verify(Method(listenerMock, performTriggerAction)).Once();
}

void test_loop_iteration_stops_after_first_blocking_listener(void)
{
    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);

    IEvtListener &nonBlockingListener = nonBlockingListenerMock.get();
    target.addListener(&nonBlockingListener);

    target.loopIteration();

    Verify(Method(listenerMock, performTriggerAction)).Once();
    Verify(Method(nonBlockingListenerMock, performTriggerAction)).Never();
}

void test_reset_clears_listeners(void)
{
    target.manageListeners(true);
    IEvtListener *listener = new MockEvtListener();
    target.addListener(listener);
    target.reset();

    TEST_ASSERT_EQUAL(0, target.listenerCount());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_listener_resets_listener);
    RUN_TEST(test_add_listener_in_middle_slot_resets_listener);
    RUN_TEST(test_listener_count_does_not_include_empty_slots);
    RUN_TEST(test_loop_iteration_calls_listener);
    RUN_TEST(test_loop_iteration_calls_all_listeners);
    RUN_TEST(test_loop_iteration_stops_after_first_blocking_listener);
    RUN_TEST(test_reset_clears_listeners);
    UNITY_END();

    return 0;
}