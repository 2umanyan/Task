#include "Event.h"
#include <gtest/gtest.h>

TEST(EventTest, TimeValidation)
{
  ASSERT_TRUE(Event::isTimeValid("12:30"));
  ASSERT_TRUE(Event::isTimeValid("00:00"));
  ASSERT_FALSE(Event::isTimeValid("24:00"));
  ASSERT_FALSE(Event::isTimeValid("12:60"));
  ASSERT_FALSE(Event::isTimeValid("12:5x"));
}

TEST(EventTest, TimeConversion)
{
  int minutes = Event::timeToMinutes("12:30");
  ASSERT_EQ(minutes, 750);
}

TEST(EventTest, InvalidTime)
{
  std::string invalidTime = "25:99";
  ASSERT_FALSE(Event::isTimeValid(invalidTime));
}
