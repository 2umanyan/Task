#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
  int table;
  std::string arrival_time;

  Client();
  Client(int t, const std::string& a);

  static bool isNameValid(const std::string& name);
};

#endif
