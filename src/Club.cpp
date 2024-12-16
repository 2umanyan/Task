#include "Club.h"
#include <algorithm>
#include <iostream>
#include <sstream>

void
printTime(int time)
{
  if (time / 60 < 10)
    std::cout << 0;
  std::cout << time / 60 << ':';
  if (time % 60 < 10)
    std::cout << 0;
  std::cout << time % 60 << '\n';
}

Club::Club(int nt, const std::string& ot, const std::string& ct, int cph)
  : numTables(nt)
  , openTime(ot)
  , closeTime(ct)
  , costPerHour(cph)
  , tables(nt, -1)
  , tableOccupiedTime(nt, 0)
{
}

void
Club::readEvents(std::ifstream& infile)
{
  std::string line;
  infile.ignore();

  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string time, body;
    int id;
    ss >> time >> id;
    ss.ignore();
    std::getline(ss, body);
    if (!Event::isTimeValid(time) || !Event::isNumber(std::to_string(id))) {
      printErrorAndExit(line);
    }
    events.push_back(Event(time, id, body));
  }
}

void
Club::handleEvents()
{
  for (const auto& event : events) {
    switch (event.id) {
      case 1:
        handleClientArrival(event);
        break;
      case 2:
        handleClientSit(event);
        break;
      case 3:
        handleClientWait(event);
        break;
      case 4:
        handleClientLeave(event);
        break;
      default:
        printErrorAndExit("Invalid event ID");
    }
  }
}

void
Club::handleEndOfDay()
{
  for (const auto& [name, client] : clients) {
    outputEvents.push_back(Event(closeTime, 11, name));
    if (client.table != -1) {
      int occupiedTable = client.table - 1;
      tableOccupiedTime[occupiedTable] +=
        Event::timeToMinutes(closeTime) -
        Event::timeToMinutes(client.arrival_time);
    }
  }
}

void
Club::printResults() const
{
  std::cout << openTime << std::endl;
  for (const auto& event : outputEvents) {
    std::cout << event.time << " " << event.id << " " << event.body
              << std::endl;
  }
  std::cout << closeTime << std::endl;

  for (int i = 0; i < numTables; ++i) {
    std::cout << i + 1 << " "
              << (tableOccupiedTime[i] / 60) * costPerHour +
                   (tableOccupiedTime[i] % 60 != 0) * costPerHour
              << " ";
    printTime(tableOccupiedTime[i]);
  }
}

void
Club::handleClientArrival(const Event& event)
{
  outputEvents.push_back(event);
  std::string name = event.body;
  if (!Client::isNameValid(name))
    printErrorAndExit(std::string(event.time + ' ' + std::to_string(event.id) +
                                  ' ' + event.body));

  if (clients.find(name) != clients.end()) {
    outputEvents.push_back(Event(event.time, 13, "YouShallNotPass"));
  } else if (event.time < openTime || event.time > closeTime) {
    outputEvents.push_back(Event(event.time, 13, "NotOpenYet"));
  } else {
    clients.insert({ name, Client(-1, event.time) });
  }
}

void
Club::handleClientSit(const Event& event)
{
  outputEvents.push_back(event);
  std::stringstream ss(event.body);
  std::string name;
  int table;
  ss >> name >> table;
  if (!Client::isNameValid(name))
    printErrorAndExit(std::string(event.time + ' ' + std::to_string(event.id) +
                                  ' ' + event.body));

  if (clients.find(name) == clients.end()) {
    outputEvents.push_back(Event(event.time, 13, "ClientUnknown"));
  } else if (table < 1 || table > tables.size() ||
             (tables[table - 1] != -1 &&
              tables[table - 1] != clients[name].table)) {
    outputEvents.push_back(Event(event.time, 13, "PlaceIsBusy"));
  } else {
    if (clients[name].table != -1) {
      int oldTable = clients[name].table - 1;
      tableOccupiedTime[oldTable] +=
        Event::timeToMinutes(event.time) -
        Event::timeToMinutes(clients[name].arrival_time);
      tables[oldTable] = -1;
    }
    tables[table - 1] = 1;
    clients[name].table = table;
    clients[name].arrival_time = event.time;
  }
}

void
Club::handleClientWait(const Event& event)
{
  std::string name = event.body;
  outputEvents.push_back(event);
  if (!Client::isNameValid(name))
    printErrorAndExit(std::string(event.time + ' ' + std::to_string(event.id) +
                                  ' ' + event.body));

  if (freeTables()) {
    outputEvents.push_back(Event(event.time, 13, "ICanWaitNoLonger!"));
  } else if (waitingQueue.size() >= clients.size()) {
    outputEvents.push_back(Event(event.time, 11, name));
  } else {
    waitingQueue.push(name);
  }
}

void
Club::handleClientLeave(const Event& event)
{
  outputEvents.push_back(event);
  std::string name = event.body;
  if (!Client::isNameValid(name))
    printErrorAndExit(std::string(event.time + ' ' + std::to_string(event.id) +
                                  ' ' + event.body));

  if (clients.find(name) == clients.end()) {
    outputEvents.push_back(Event(event.time, 13, "ClientUnknown"));
  } else {
    if (clients[name].table != -1) {
      int occupiedTable = clients[name].table - 1;
      tableOccupiedTime[occupiedTable] +=
        Event::timeToMinutes(event.time) -
        Event::timeToMinutes(clients[name].arrival_time);
      tables[occupiedTable] = -1;
      if (!waitingQueue.empty()) {
        std::string waitingClient = waitingQueue.front();
        waitingQueue.pop();
        clients[waitingClient].table = clients[name].table;
        clients[waitingClient].arrival_time = event.time;
        tables[occupiedTable] = 1;
        outputEvents.push_back(
          Event(event.time,
                12,
                waitingClient + " " + std::to_string(occupiedTable + 1)));
      }
    }
    clients.erase(name);
  }
}

void
Club::printErrorAndExit(const std::string& line) const
{
  std::cerr << "Error: " << line << std::endl;
  exit(EXIT_FAILURE);
}

bool
Club::freeTables()
{
  bool flag = 0;
  for (auto table : tables)
    if (table == -1)
      flag = 1;

  return flag;
}
