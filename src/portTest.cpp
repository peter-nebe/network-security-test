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

#include "portTest.h"
#include "system.h"
#include "log.h"
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

PortTest::PortTest()
: TestCase("port test")
{
}

int PortTest::setup(const string &targtIp)
{
  targetIp = targtIp;
  return 0;
}

namespace
{

struct Port
{
  string number;
  string service;

  bool operator==(const Port &other) const
  {
    return number == other.number && service == other.service;
  }
  bool operator!=(const Port &other) const
  {
    return !operator==(other);
  }
};

typedef vector<Port> Ports;

Ports readOpenPorts(const string &nmapOutput)
{
  Ports ports;
  ports.reserve(50);

  ifstream file(nmapOutput);
  string line;
  const string token = "PORT ";
  while(getline(file, line) && line.compare(0, token.length(), token));

  while(file)
  {
    string port, state, service;
    file >> port >> state >> service;
    if(port == "#")
      break;
    if(state == "open")
      ports.push_back({ port, service });
  }

  return ports;
}

ostream& operator<<(ostream& os, const Port &pt)
{
  return os << pt.number << " (" << pt.service << ')';
}

void logPorts(const Ports &ports)
{
  for(const Port &pt : ports)
    loginfo << pt << endl;
}

} // namespace

int PortTest::execute()
{
  const string output = "nmapOutput";
  int ret = System::exec("nmap -oN " + output + ' ' + targetIp);

  const Ports openPorts = readOpenPorts(output);
#if VERBOSE
  loginfo << openPorts.size() << " open port(s):" << endl;
  logPorts(openPorts);
#endif

  const Port requiredPort{ "22/tcp", "ssh" };
  const int reqPortCount = count(openPorts.begin(), openPorts.end(), requiredPort);
  if(reqPortCount != 1)
  {
    loginfo << "count of port " << requiredPort << " is not equal to 1" << endl;
    ret = 2;
  }

  Ports unnecOpenPorts;
  unnecOpenPorts.reserve(openPorts.size());
  copy_if(openPorts.begin(), openPorts.end(), inserter(unnecOpenPorts, unnecOpenPorts.begin()),
          [&requiredPort](const Port &pt){ return pt != requiredPort; });

  const string text = "unnecessary open ports:";
  if(!unnecOpenPorts.empty())
  {
    loginfo << text << endl;
    logPorts(unnecOpenPorts);
    ret = 3;
  }
  else
    loginfo << text << " none" << endl;

  return ret;
}
