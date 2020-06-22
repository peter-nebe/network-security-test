/*
 * network-security-test
 * Copyright (c) 2020 Peter Nebe (mail@peter-nebe.dev)
 *
 * This file is part of network-security-test.
 *
 * network-security-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * network-security-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with network-security-test.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "log.h"
#include "testSuite.h"
using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    loginfo << "usage: " << argv[0] << " IP_ADDR" << endl;
    return 1;
  }

  const string testObjectIp = argv[1];

  TestSuite testSuite;
  const int ret = testSuite.run(testObjectIp);

  return ret;
}
