#include <ArduinoFake.h>
#include <unity.h>

#include "EvtTimeListener.h"
#include "EvtContext.h"

using namespace fakeit;

bool triggered = false;
bool trigger()
{
    triggered = true;
    return true;
}

EvtContext context;
EvtTimeListener target(200, true, (EvtAction)trigger);
EvtTimeListener singleFireTarget(200, false, (EvtAction)trigger);

void setUp(void)
{
    triggered = false;
    ArduinoFakeReset();
    When(Method(ArduinoFake(), millis)).AlwaysReturn(10);
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(LOW);
    target.enable();
}

void test_does_not_trigger_when_disabled(void)
{
    target.disable();
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_does_not_trigger_before_time_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(210);
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_triggers_after_time_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(211);
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_triggers_after_wrap_around(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(5);
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_multifire_always_returns_false(void)
{
    TEST_ASSERT_FALSE(target.performTriggerAction(&context));
}

void test_multifire_resets_delay_check(void)
{
    When(Method(ArduinoFake(), millis)).AlwaysReturn(20);
    TEST_ASSERT_FALSE(target.isEventTriggered());
    target.performTriggerAction(&context);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(221);
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_single_fire_returns_action_result(void)
{
    TEST_ASSERT_TRUE(singleFireTarget.performTriggerAction(&context));
}

void test_single_fire_only_runs_once(void)
{
    TEST_ASSERT_TRUE(singleFireTarget.performTriggerAction(&context));

    When(Method(ArduinoFake(), millis)).AlwaysReturn(211);
    TEST_ASSERT_FALSE(singleFireTarget.isEventTriggered());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_does_not_trigger_when_disabled);
    RUN_TEST(test_does_not_trigger_before_time_delay);
    RUN_TEST(test_triggers_after_time_delay);
    RUN_TEST(test_triggers_after_wrap_around);
    RUN_TEST(test_multifire_always_returns_false);
    RUN_TEST(test_multifire_resets_delay_check);
    RUN_TEST(test_single_fire_returns_action_result);
    RUN_TEST(test_single_fire_only_runs_once);
    UNITY_END();

    return 0;
}