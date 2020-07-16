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

#include "dosTest.h"
#include "files.h"
#include "log.h"
#include "system.h"
#include <thread>
using namespace std;

DosTest::DosTest()
: TestCase("DoS test")
{
}

namespace
{

const string kindIp = "169.254.92.46";
const string attackIp = "169.254.92.47";
const string ipMask = "/16";
const string interfaceName = "enp7s0";

int setOwnIp(const string &ip)
{
  int ret = System::exec("ip addr flush dev " + interfaceName);
  int err = System::exec("ip addr add " + ip + ipMask + " dev " + interfaceName);
  if(err)
    ret = err;

  return ret;
}

} // namespace

int DosTest::setup(const string &targtIp)
{
  targetIp = targtIp;
  return setOwnIp(attackIp);
}

int DosTest::execute()
{
  string output;
  int ret = execDummySsh(Files::sshIdentityFile, output);
  if(!output.empty())
  {
    logerror << "output of valid ssh command: " << output << endl;
    ret = -2;
  }

  // simulate DoS attack
  for(int i = 0; i < 5; i++)
  {
    if(execDummySsh(Files::sshIdentityFileInvalid, "Permission denied", output))
      ret = 1;
  }
  this_thread::sleep_for(1s);

  const string expectedText = "Connection refused";
  if(execDummySsh(Files::sshIdentityFileInvalid, expectedText, output))
    ret = 2;

  if(execDummySsh(Files::sshIdentityFile, expectedText, output))
    ret = 3;

#if VERBOSE
  loginfo << "output of ssh command after simulated DoS attack: " << output << endl;
#endif

  return ret;
}

void DosTest::teardown()
{
  setOwnIp(kindIp);
  System::execSsh(targetIp, "fail2ban-client unban " + attackIp);
}

int DosTest::execDummySsh(const string &identityFile, string &output)
{
  return System::exec(Command::ssh + identityFile + ' ' + targetIp + " exit", output);
}

int DosTest::execDummySsh(const string &identityFile, const string &expectedText, string &output)
{
  int ret = 0;

  output.clear();
  execDummySsh(identityFile, output);
  if(output.find(expectedText) == string::npos)
  {
    loginfo << "output of ssh command ('" << output << "') does not contain the expected text ('" << expectedText << "')" << endl;
    ret = 1;
  }

  return ret;
}
