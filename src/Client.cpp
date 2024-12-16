#include "Client.h"

Client::Client()
  : table(-1)
{
}

Client::Client(int t, const std::string& a)
  : table(t)
  , arrival_time(a)
{
}

bool
Client::isNameValid(const std::string& name)
{
  for (char ch : name) {
    if (!isalnum(ch) && ch != '_' && ch != '-')
      return false;
  }

  return true;
}
