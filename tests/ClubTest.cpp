#include "Club.h"
#include "Event.h"
#include <gtest/gtest.h>

class ClubTest : public ::testing::Test
{
protected:
  Club* club;

  void SetUp() override { club = new Club(5, "08:00", "22:00", 100); }

  void TearDown() override { delete club; }
};

TEST_F(ClubTest, HandleClientArrival)
{
  Event event("08:15", 1, "client_1");
  club->handleClientArrival(event);
  ASSERT_EQ(club->clients.size(), 1);
}

TEST_F(ClubTest, HandleClientSit)
{
  Event arrival("08:15", 1, "client_1");
  club->handleClientArrival(arrival);

  Event sit("08:20", 2, "client_1 1");
  club->handleClientSit(sit);

  ASSERT_EQ(club->clients["client_1"].table, 1);
}

TEST_F(ClubTest, HandleClientLeave)
{
  Event arrival("08:15", 1, "client_1");
  club->handleClientArrival(arrival);

  Event sit("08:20", 2, "client_1 1");
  club->handleClientSit(sit);

  Event leave("09:00", 4, "client_1");
  club->handleClientLeave(leave);

  ASSERT_EQ(club->clients["client_1"].table, -1);
}
