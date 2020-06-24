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

#include "testSuite.h"
#include "portTest.h"
#include "downloadTest.h"
#include "signatureTest.h"
#include "log.h"
using namespace std;

TestSuite::TestSuite()
{
  tests.push_back(make_unique<PortTest>());
  tests.push_back(make_unique<DownloadTest>());
  tests.push_back(make_unique<SignatureTest>());
}

int TestSuite::run(const string &arg)
{
  int ret = 0;

  for(auto &test : tests)
  {
    loginfo << "executing " << test->name << "..." << endl;

    int err = test->setup(arg);
    if(err == 0)
      err = test->execute();
    if(err)
      ret = 1;
    test->teardown();

    loginfo << "result of " << test->name << ": " << (err ? "FAILED" : "passed") << endl << endl;
  }

  loginfo << "overall test result: " << (ret ? "FAILED" : "passed") << endl;

  return ret;
}
