#include "gtest/gtest.h"

#include "../StreamBroker.h"

using namespace man::stream;

TEST(StreamBrokerTest, CanStartAndStopBroker) {
  StreamBroker s;
  ASSERT_NO_THROW(s.start());

  ASSERT_NO_THROW(int result = s.stop());
}

TEST(StreamBrokerTest, CannotStartTwoBrokersOnSamePort) {
  StreamBroker s;
  ASSERT_NO_THROW(s.start());

  StreamBroker evil_s;
  ASSERT_ANY_THROW(evil_s.start());
  ASSERT_NO_THROW(s.stop());
}

TEST(StreamBrokerTest, CanRestartTheSameBroker) {
  StreamBroker s;
  ASSERT_NO_THROW(s.start());
  ASSERT_NO_THROW(s.stop());

  ASSERT_NO_THROW(s.start());
  ASSERT_NO_THROW(s.stop());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
