# network-security-test
Example of a simple network security test (under construction)

When an embedded device is connected to the Internet, a general requirement is that the data exchange cannot be intercepted. To do this, the data exchange must be encrypted. For example, OpenSSH could be used for this. When a solution has been installed, you would like to test whether the data transmission is actually encrypted.

**network-security-test** checks if the download with the *scp* command is really encrypted. The test consists of two steps.

1. Download a file with the *rcp* command. This transmission is not encrypted. Search the TCP dump for a snippet of the downloaded file. The snippet will be found. This is proof that the test procedure works.

2. Download a file with the *scp* command. This transmission IS encrypted. Search the TCP dump for a snippet of the downloaded file. The snippet will NOT be found and the test has been passed.

##### Sample output of a test run:
```
[peter@PC1 network-security-test]$ ./network-security-test 169.254.6.223
network-security-test: executing download test...
network-security-test: executing command "rcp testDummyFile 169.254.6.223:/tmp/testDummyFile"...
network-security-test: executing command "tcpdump -c 24 -w dumpRcp"...
tcpdump: listening on enp7s0, link-type EN10MB (Ethernet), capture size 262144 bytes
network-security-test: executing command "rcp  169.254.6.223:/tmp/testDummyFile /tmp/testDummyFileRcp"...
24 packets captured
24 packets received by filter
0 packets dropped by kernel
network-security-test: last download (dumpRcp) was NOT encrypted
network-security-test: executing command "tcpdump -c 52 -w dumpScp"...
tcpdump: listening on enp7s0, link-type EN10MB (Ethernet), capture size 262144 bytes
network-security-test: executing command "scp -i sshIdentityFile 169.254.6.223:/tmp/testDummyFile /tmp/testDummyFileScp"...
testDummyFile                                                                                                                                               100%   35    81.3KB/s   00:00    
52 packets captured
52 packets received by filter
0 packets dropped by kernel
network-security-test: last download (dumpScp) was encrypted
network-security-test: result of download test: passed

network-security-test: executing signing test...
network-security-test: executing command "openssl dgst -sha256 -sign privkey -out /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: executing command "rcp /tmp/largeDummyFile 169.254.6.223:/tmp/largeDummyFile"...
network-security-test: executing command "rcp /tmp/largeDummyFile.sign 169.254.6.223:/tmp/largeDummyFile.sign"...
network-security-test: executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: return value 256 executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"
network-security-test: signature check result: Verification Failure
network-security-test: executing command "rcp /tmp/largeDummyFile 169.254.6.223:/tmp/largeDummyFile"...
network-security-test: executing command "rcp /tmp/largeDummyFile.sign 169.254.6.223:/tmp/largeDummyFile.sign"...
network-security-test: executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: signature check result: Verified OK
network-security-test: result of signing test: passed

network-security-test: overall test result: passed
[peter@PC1 network-security-test]$
```
