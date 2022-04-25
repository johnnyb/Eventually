#include <unity.h>
#include <Arduino.h>

#include "EvtContext.h"
#include "StateMachineListener.h"

bool triggered = false;

bool trigger()
{
    triggered = true;
    return true;
}

bool failedTrigger()
{
    return false;
}

StateMachineListener target;
EvtContext ctx;

void setUp(void)
{
    triggered = false;
    target.enable();
    target.setState(3);
}

void test_does_not_trigger_when_disabled(void)
{
    target.disable();
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_triggers_when_enabled(void)
{
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_triggers_when_state(void)
{
    target.addState(3, (EvtAction)trigger, 4);
    target.performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(triggered);
}

void test_sets_next_state(void)
{
    target.addState(3, (EvtAction)trigger, 4);
    target.performTriggerAction(&ctx);
    TEST_ASSERT_EQUAL(4, target.currentState());
}

void test_sets_next_failed_state(void)
{
    target.addState(3, (EvtAction)failedTrigger, 4, 5);
    target.performTriggerAction(&ctx);
    TEST_ASSERT_EQUAL(5, target.currentState());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_does_not_trigger_when_disabled);
    RUN_TEST(test_triggers_when_enabled);
    RUN_TEST(test_triggers_when_state);
    RUN_TEST(test_sets_next_state);
    RUN_TEST(test_sets_next_failed_state);
    UNITY_END();

    return 0;
}