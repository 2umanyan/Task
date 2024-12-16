#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event
{
public:
  std::string time;
  int id;
  std::string body;

  Event(const std::string& t, int i, const std::string& b);

  static bool isTimeValid(const std::string& time);
  static bool isNumber(const std::string& s);
  static int timeToMinutes(const std::string& time);
};

#endif
