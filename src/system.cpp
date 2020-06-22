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

#include "system.h"
#include "log.h"
#include "files.h"
#include <fstream>
using namespace std;

int System::exec(const string &command)
{
  const string text = "executing command \"" + command + '"';
#if VERBOSE
  loginfo << text << "..." << endl;
#endif

  const int ret = system(command.c_str());
  if(ret < 0)
    logerror << ret << ' ' << text << endl;
  else if(ret > 0)
    loginfo << "return value " << ret << ' ' << text << endl;

  return ret;
}

int System::exec(const string &command, string &output)
{
  const string text = "executing command \"" + command + '"';
#if VERBOSE
  loginfo << text << "..." << endl;
#endif

  const string outputFile = "/tmp/syscmdoutput";
  const int ret = system((command + " > " + outputFile).c_str());
  if(ret < 0)
    logerror << ret << ' ' << text << endl;
  else if(ret > 0)
    loginfo << "return value " << ret << ' ' << text << endl;

  getline(ifstream(outputFile), output);

  return ret;
}

int System::controlRemoteXinetd(const string &targetIp, Control control)
{
  const string ctrlCmd = "systemctl "s + (control == Control::start ? "start" : "stop") + " xinetd";
  return exec(Command::ssh + Files::sshIdentityFile + " root@" + targetIp + ' ' + ctrlCmd);
}
