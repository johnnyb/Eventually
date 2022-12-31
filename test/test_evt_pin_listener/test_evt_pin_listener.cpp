#include <ArduinoFake.h>
#include <unity.h>

#include "EvtPinListener.h"

using namespace fakeit;

bool triggered = false;
bool trigger()
{
    triggered = true;
    return true;
}

EvtPinListener target(2, 200, (EvtAction)trigger);
EvtPinListener targetWithoutDebounce(2, 0, (EvtAction)trigger);

void setUp(void)
{
    triggered = false;
    ArduinoFakeReset();
    When(Method(ArduinoFake(), millis)).AlwaysReturn(10);
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(LOW);
    target.enable();
    targetWithoutDebounce.enable();
}

void tearDown(void) {}

void test_does_not_trigger_when_disabled(void)
{
    target.disable();
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_does_not_trigger_when_enabled_before_debounce_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(20);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), millis)).AlwaysReturn(220);
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_does_not_trigger_when_enabled_without_debounce_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(20);
    TEST_ASSERT_TRUE(targetWithoutDebounce.isEventTriggered());
}

void test_triggers_when_enabled_after_debounce_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(20);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), millis)).AlwaysReturn(221);
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_does_not_change_when_value_changes_before_debounce_delay(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);

    When(Method(ArduinoFake(), millis)).AlwaysReturn(20);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(LOW);
    When(Method(ArduinoFake(), millis)).AlwaysReturn(221);
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_wait_for_change_if_start_state_matches_target_value(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);
    target.enable();

    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(LOW);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), millis)).AlwaysReturn(221);
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_wait_for_change_holds_until_opposite_to_start_state(void)
{
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);
    target.enable();

    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH);
    TEST_ASSERT_FALSE(target.isEventTriggered());

    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(LOW);
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_does_not_trigger_when_disabled);
    RUN_TEST(test_does_not_trigger_when_enabled_before_debounce_delay);
    RUN_TEST(test_does_not_trigger_when_enabled_without_debounce_delay);
    RUN_TEST(test_triggers_when_enabled_after_debounce_delay);
    RUN_TEST(test_does_not_change_when_value_changes_before_debounce_delay);
    RUN_TEST(test_wait_for_change_if_start_state_matches_target_value);
    RUN_TEST(test_wait_for_change_holds_until_opposite_to_start_state);
    UNITY_END();

    return 0;
}