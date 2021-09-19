#include <unity.h>
#include <Arduino.h>

#include "EvtByteListener.h"

bool triggered = false;

bool trigger()
{
    triggered = true;
    return true;
}

byte value = 10;
byte *pValue = &value;
EvtByteListener target(pValue, (byte)10, ALWAYS, (EvtAction)trigger);

void setUp(void)
{
    triggered = false;
    target.enable();
    value = 10;
}

void test_does_not_trigger_when_disabled(void)
{
    target.disable();
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_does_not_trigger_when_not_target_value(void)
{
    value = 99;
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_triggers_when_target_value(void)
{
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_does_not_trigger_second_time(void)
{
    TEST_ASSERT_TRUE(target.isEventTriggered());
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_does_not_trigger_third_time(void)
{
    TEST_ASSERT_TRUE(target.isEventTriggered());
    TEST_ASSERT_FALSE(target.isEventTriggered());
    TEST_ASSERT_FALSE(target.isEventTriggered());
}

void test_retriggers_after_change(void)
{
    TEST_ASSERT_TRUE(target.isEventTriggered());
    value = 15;
    TEST_ASSERT_FALSE(target.isEventTriggered());
    value = 10;
    TEST_ASSERT_TRUE(target.isEventTriggered());
}

void test_retriggers_always(void)
{
    // target.triggerMode = ALWAYS;
    // TEST_ASSERT_TRUE(target.isEventTriggered());
    // TEST_ASSERT_TRUE(target.isEventTriggered());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_does_not_trigger_when_disabled);
    RUN_TEST(test_does_not_trigger_when_not_target_value);
    RUN_TEST(test_triggers_when_target_value);
    RUN_TEST(test_does_not_trigger_second_time);
    RUN_TEST(test_does_not_trigger_third_time);
    RUN_TEST(test_retriggers_after_change);
    RUN_TEST(test_retriggers_always);
    UNITY_END();

    return 0;
}