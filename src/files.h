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

#ifndef FILES_H_
#define FILES_H_

#include <string>

namespace Files
{
  const std::string testDummyFile = "testDummyFile";
  const std::string largeDummyFile = "/tmp/largeDummyFile";
  const std::string sshIdentityFile = "sshIdentityFile";
  const std::string privKeyFile = "privkey";
  const std::string pubKeyFile = "/tmp/pubkey";
}

#endif /* FILES_H_ */
