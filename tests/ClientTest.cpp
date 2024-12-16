#include "Client.h"
#include <gtest/gtest.h>

TEST(ClientTest, ValidClientName)
{
  ASSERT_TRUE(Client::isNameValid("client_1"));
  ASSERT_TRUE(Client::isNameValid("client-2"));
  ASSERT_FALSE(Client::isNameValid("client@1"));
  ASSERT_FALSE(Client::isNameValid("client 1"));
}
