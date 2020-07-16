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

#include "signatureTest.h"
#include "files.h"
#include "log.h"
#include "system.h"
#include <fstream>
using namespace std;

namespace
{
  const string signatureCmd = "openssl dgst -sha256 ";
}

SignatureTest::SignatureTest()
: TestCase("signature test")
{
}

int SignatureTest::setup(const string &targtIp)
{
  targetIp = targtIp;
  signature = Files::largeDummyFile + ".sign";

  size_t fileSize = 0xA00000; // 10 MiB
  ofstream file(Files::largeDummyFile);
  uint8_t counter = 0;

  while(fileSize--)
    file << counter++;

  if(!file)
    return -1;
  file.close();

  int ret = System::exec(signatureCmd + "-sign " + Files::privKeyFile + " -out " + signature + ' ' + Files::largeDummyFile);
  int err = System::controlRemoteService(targetIp, Service::xinetd, System::Control::start);
  if(err)
    ret = err;

  return ret;
}

int SignatureTest::execute()
{
  int ret = turnBit();
  if(upload())
    ret = -3;

  string result;
  int err = checkSignature(result);
  if(err < 0)
    ret = -4;
  else if(result != "Verification Failure")
    ret = 1;

  if(turnBit())
    ret = -5;
  if(upload())
    ret = -6;

  err = checkSignature(result);
  if(err < 0)
    ret = -7;
  else if(result != "Verified OK")
    ret = 2;

  return ret;
}

void SignatureTest::teardown()
{
  System::controlRemoteService(targetIp, Service::xinetd, System::Control::stop);
}

int SignatureTest::upload()
{
  int ret = System::exec(Command::rcp + Files::largeDummyFile + ' ' + targetIp + ':' + Files::largeDummyFile);
  int err = System::exec(Command::rcp + signature + ' ' + targetIp + ':' + signature);
  if(err)
    ret = err;

  return ret;
}

int SignatureTest::turnBit()
{
  fstream file(Files::largeDummyFile);
  char byte;
  if(!file.seekg(-1, ios_base::end) || !file.read(&byte, 1))
    return -1;

  byte ^= 1;
  if(!file.seekp(-1, ios_base::end) || !file.write(&byte, 1))
    return -2;

  return 0;
}

int SignatureTest::checkSignature(string &result)
{
  const string verifyCmd = signatureCmd + "-verify " + Files::pubKeyFile + " -signature " + signature + ' ' + Files::largeDummyFile;
  const int ret = System::exec(Command::ssh + Files::sshIdentityFile + ' ' + targetIp + ' ' + verifyCmd, result);
#if VERBOSE
  loginfo << "signature check result: " << result << endl;
#endif

  return ret;
}
