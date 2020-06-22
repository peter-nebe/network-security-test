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

#ifndef SIGNINGTEST_H_
#define SIGNINGTEST_H_

#include "testCase.h"

class SigningTest : public TestCase
{
public:
  SigningTest();

private:
  int setup(const std::string &targetIp) override;
  int execute() override;
  void teardown() override;
  int upload();
  int turnBit();
  int checkSignature(std::string &result);

  std::string targetIp;
  std::string signature;
};

#endif /* SIGNINGTEST_H_ */
