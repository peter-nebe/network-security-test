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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <string>

class System
{
public:
  static int exec(const std::string &command);
  static int exec(const std::string &command, std::string &output);
  static int execSsh(const std::string &targetIp, const std::string &command);

  enum struct Control
  {
    start,
    stop
  };
  static int controlRemoteService(const std::string &targetIp, const std::string &service, Control control);

private:
  static int exec(const std::string &command, const std::string &actualCommand);
};

namespace Command
{
  const std::string rcp = "rcp ";
  const std::string scp = "scp ";
  const std::string ssh = "ssh -i ";
}

namespace Service
{
  const std::string xinetd = "xinetd";
}

#endif /* SYSTEM_H_ */
