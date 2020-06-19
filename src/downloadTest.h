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

#ifndef DOWNLOADTEST_H_
#define DOWNLOADTEST_H_

#include <string>
#include <thread>

class DownloadTest
{
public:
  int execute(const std::string &targetIp);

private:
  int prepare(const std::string &dummyFile, const std::string &targetIp);
  int download(const std::string &downloadCmd, const std::string &cmdOption, const std::string &dumpOption, const std::string &suffix);
  int checkEncryption();

  std::string originalPath;
  std::string remotePath;
  std::string dumpPath;
  std::thread thr;
};

#endif /* DOWNLOADTEST_H_ */
