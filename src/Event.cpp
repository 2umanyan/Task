#include "Event.h"
#include <algorithm>

Event::Event(const std::string& t, int i, const std::string& b)
  : time(t)
  , id(i)
  , body(b)
{
}

bool
Event::isTimeValid(const std::string& time)
{
  if (time.size() != 5 || time[2] != ':' || !isNumber(time.substr(0, 2)) ||
      !isNumber(time.substr(3, 2)))
    return false;

  int hours = std::stoi(time.substr(0, 2));
  int minutes = std::stoi(time.substr(3, 2));

  return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}

bool
Event::isNumber(const std::string& s)
{
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

int
Event::timeToMinutes(const std::string& time)
{
  return std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2));
}
