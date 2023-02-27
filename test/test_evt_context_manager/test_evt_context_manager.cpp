#include <ArduinoFake.h>
#include <unity.h>

#include "EvtContextManager.h"

using namespace fakeit;

EvtContextManager target;
Mock<IEvtListener> listenerMock;
Mock<IEvtContext> contextMock;

void setUp(void)
{
    target.manageListeners(false);
    When(Method(listenerMock, reset)).AlwaysReturn();
    When(Method(listenerMock, isEventTriggered)).AlwaysReturn(true);
    When(Method(listenerMock, performTriggerAction)).AlwaysReturn(true);
    Fake(Dtor(listenerMock));

    When(Method(contextMock, reset)).AlwaysReturn();
}

void tearDown(void)
{
    listenerMock.Reset();
    contextMock.Reset();
    target.reset();
}

void test_default_to_one_context(void)
{
    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);
    target.loopIteration();
    Verify(Method(listenerMock, performTriggerAction)).Once();
}

void test_push_sets_new_context(void)
{
    IEvtListener &listener = listenerMock.get();
    target.addListener(&listener);
    EvtContext context;
    target.pushContext(&context);
    target.loopIteration();
    Verify(Method(listenerMock, performTriggerAction)).Never();
}

void test_push_context_resets_context(void)
{
    IEvtContext &context = contextMock.get();
    target.pushContext(&context);

    Verify(Method(contextMock, reset)).Once();
}

void test_reset_context_resets_current_context(void)
{
    IEvtContext &context = contextMock.get();
    target.pushContext(&context);
    target.resetContext();

    Verify(Method(contextMock, reset)).Exactly(2);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_to_one_context);
    RUN_TEST(test_push_sets_new_context);
    RUN_TEST(test_push_context_resets_context);
    RUN_TEST(test_reset_context_resets_current_context);
    UNITY_END();

    return 0;
}