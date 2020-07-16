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

#include "downloadTest.h"
#include "files.h"
#include "log.h"
#include "system.h"
#include <filesystem>
#include <fstream>
#include <cstring>
namespace fs = std::filesystem;
using namespace std;

namespace
{
  const string rcpSuffix = "Rcp";
  const string scpSuffix = "Scp";
}

DownloadTest::DownloadTest()
: TestCase("download test")
{
}

int DownloadTest::setup(const string &targtIp)
{
  targetIp = targtIp;
  originalPath = Files::testDummyFile;
  remotePath = targetIp + ":/tmp/" + Files::testDummyFile;

  if(!fs::exists(originalPath))
  {
    logerror << quoted(originalPath) << " does not exist" << endl;
    return 1;
  }

  int ret = System::controlRemoteService(targetIp, Service::xinetd, System::Control::start);
  int err = System::exec(Command::rcp + originalPath + ' ' + remotePath);
  if(err)
    ret = err;

  return ret;
}

int DownloadTest::execute()
{
  int ret = download(Command::rcp, string(), "-c 24", rcpSuffix);
  int err = checkEncryption();
  if(err < 0)
    ret = -3;
  else if(err != 1)
    ret = 1;

  if(download(Command::scp, "-i " + Files::sshIdentityFile, "-c 52", scpSuffix))
    ret = -4;
  err = checkEncryption();
  if(err < 0)
    ret = -5;
  else if(err > 0)
    ret = 2;

  return ret;
}

void DownloadTest::teardown()
{
  System::controlRemoteService(targetIp, Service::xinetd, System::Control::stop);
}

namespace
{

int filecmp(const string &lhsPath, const string &rhsPath)
{
  error_code ec;
  const uintmax_t lhsSize = fs::file_size(lhsPath, ec);
  const uintmax_t rhsSize = fs::file_size(rhsPath, ec);
  if(lhsSize == 0 || lhsSize == static_cast<uintmax_t>(-1) || lhsSize != rhsSize)
    return -1;

  unique_ptr<char> lhs(new char[lhsSize]);
  if(!ifstream(lhsPath).read(lhs.get(), lhsSize))
    return -2;

  unique_ptr<char> rhs(new char[rhsSize]);
  if(!ifstream(rhsPath).read(rhs.get(), rhsSize))
    return -3;

  if(memcmp(lhs.get(), rhs.get(), rhsSize))
    return -4;

  return 0;
}

size_t filestr(const string &path, const string &str)
{
  const size_t error = -1;

  if(str.empty())
    return error;

  error_code ec;
  const uintmax_t fsize = fs::file_size(path, ec);
  if(fsize < str.size() || fsize == static_cast<uintmax_t>(-1))
    return error;

  unique_ptr<char> buf(new char[fsize]);
  if(!ifstream(path).read(buf.get(), fsize))
    return error;

  char *ptr = buf.get();
  size_t size = fsize - str.size() + 1;
  char *loc;
  while((loc = static_cast<char*>(memchr(ptr, str.front(), size))))
  {
    if(memcmp(loc, str.c_str(), str.size()) == 0)
      return loc - buf.get();

    size -= loc + 1 - ptr;
    ptr = loc + 1;
  }

  return error;
}

} // namespace

int DownloadTest::download(const string &downloadCmd, const string &cmdOption, const string &dumpOption, const string &suffix)
{
  const string downloadPath = "/tmp/" + originalPath + suffix;
  fs::remove(downloadPath);
  if(fs::exists(downloadPath))
    return -1;

  dumpPath = "dump" + suffix;
  thr = thread([&]{ System::exec("tcpdump " + dumpOption + " -w " + dumpPath); });
  this_thread::sleep_for(1s);

  int ret = System::exec(downloadCmd + cmdOption + ' ' + remotePath + ' ' + downloadPath);
  if(filecmp(originalPath, downloadPath))
    ret = -2;

  return ret;
}

int DownloadTest::checkEncryption()
{
  thr.join();

  const size_t minSnippetSize = 10;
  const size_t div = 3;
  error_code ec;
  const uintmax_t fsize = fs::file_size(originalPath, ec);
  if(fsize < minSnippetSize * div || fsize == static_cast<uintmax_t>(-1))
    return -1;

  const size_t snippetSize = fsize / div;
  unique_ptr<char> buf(new char[snippetSize]);
  ifstream file(originalPath);
  if(!file.seekg(snippetSize) || !file.read(buf.get(), snippetSize))
    return -2;

  const string snippet(buf.get(), snippetSize);
  const size_t pos = filestr(dumpPath, snippet);
  const int ret = (pos != static_cast<size_t>(-1)) ? 1 : 0;
#if VERBOSE
  loginfo << "last download (" << dumpPath << ") was " << (ret ? "NOT " : "") << "encrypted" << endl;
#endif

  return ret;
}
