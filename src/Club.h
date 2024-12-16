#ifndef CLUB_H
#define CLUB_H

#include "Client.h"
#include "Event.h"
#include <fstream>
#include <gtest/gtest.h>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Club
{
private:
  int numTables;
  std::string openTime;
  std::string closeTime;
  int costPerHour;
  std::vector<Event> events;
  std::map<std::string, Client> clients;
  std::vector<int> tables;
  std::vector<int> tableOccupiedTime;
  std::queue<std::string> waitingQueue;
  std::vector<Event> outputEvents;

public:
  Club(int nt, const std::string& ot, const std::string& ct, int cph);

  void readEvents(std::ifstream& infile);
  void handleEvents();
  void handleEndOfDay();
  void printResults() const;

private:
  void handleClientArrival(const Event& event);
  void handleClientSit(const Event& event);
  void handleClientWait(const Event& event);
  void handleClientLeave(const Event& event);
  void printErrorAndExit(const std::string& line) const;
  bool freeTables();

  FRIEND_TEST(ClubTest, HandleClientArrival);
  FRIEND_TEST(ClubTest, HandleClientSit);
  FRIEND_TEST(ClubTest, HandleClientLeave);
};

#endif
