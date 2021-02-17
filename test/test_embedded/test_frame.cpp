#include <unity.h>

#include <widgetlib.hpp>

void test_simple_test() { TEST_ASSERT_EQUAL(2, 2); }


void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    UNITY_BEGIN();
  RUN_TEST(test_simple_test);
  UNITY_END();
}


void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}
