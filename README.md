# network-security-test
Example of a simple network security test (under construction)

When an embedded device is connected to the Internet, a general requirement is adequate protection against the associated dangers. Integrity and availability of the device and confidentiality of information must not be compromised. When the corresponding security concept has been implemented, you would like to test the effectiveness of the measures.

**network-security-test** is an example of how important security features can be tested. It contains the following tests:

### Port test
Open ports can be entry gates for attackers, especially if you cannot rely on a firewall. Only one port should be open, and the service listening on it must be secure. The port test checks the following:
- port 22/tcp is open for SSH service
- no other port is open

##### Sample output of a test run
```
network-security-test: executing port test...
network-security-test: executing command "nmap -oN nmapOutput 169.254.6.223"...
Starting Nmap 7.80 ( https://nmap.org ) at 2020-06-23 01:39 CEST
Nmap scan report for 169.254.6.223
Host is up (0.00020s latency).
Not shown: 999 closed ports
PORT   STATE SERVICE
22/tcp open  ssh

Nmap done: 1 IP address (1 host up) scanned in 0.07 seconds
network-security-test: 1 open port(s):
network-security-test: 22/tcp (ssh)
network-security-test: unnecessary open ports: none
network-security-test: result of port test: passed
```
### Download test
Confidentiality requires that the data exchange cannot be intercepted. To do this, the data exchange must be encrypted. For example, OpenSSH could be used for this. The download test checks if the download with the *scp* command is actually encrypted. The test consists of two steps.

1. Download a file with the *rcp* command. This transmission is not encrypted. Search the TCP dump for a snippet of the downloaded file. The snippet will be found. This is proof that the test procedure works.

2. Download a file with the *scp* command. This transmission IS encrypted. Search the TCP dump for a snippet of the downloaded file. The snippet will NOT be found and the test will be passed.

##### Sample output of a test run
```
network-security-test: executing download test...
network-security-test: executing command "ssh -i sshIdentityFile root@169.254.6.223 systemctl start xinetd"...
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
testDummyFile                                                                    100%   35    75.4KB/s   00:00    
52 packets captured
53 packets received by filter
0 packets dropped by kernel
network-security-test: last download (dumpScp) was encrypted
network-security-test: executing command "ssh -i sshIdentityFile root@169.254.6.223 systemctl stop xinetd"...
network-security-test: result of download test: passed
```
### Signature test
Digital signatures are commonly used for software distribution through non-secure channels. Successful signature verification guarantees the authenticity and integrity of the data received. The signature test checks if the device is able to verify a digital signature. This test also runs in two steps:

1. Generate the signature of a large file. Then turn a bit of this file. The signature verification on the device will fail. This is proof that the test procedure works.

2. Turn the same bit again. The file is now back in its original state. The signature verification will be successful and the test will be passed.

##### Sample output of a test run
```
network-security-test: executing signature test...
network-security-test: executing command "openssl dgst -sha256 -sign privkey -out /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: executing command "ssh -i sshIdentityFile root@169.254.6.223 systemctl start xinetd"...
network-security-test: executing command "rcp /tmp/largeDummyFile 169.254.6.223:/tmp/largeDummyFile"...
network-security-test: executing command "rcp /tmp/largeDummyFile.sign 169.254.6.223:/tmp/largeDummyFile.sign"...
network-security-test: executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: return value 256 executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"
network-security-test: signature check result: Verification Failure
network-security-test: executing command "rcp /tmp/largeDummyFile 169.254.6.223:/tmp/largeDummyFile"...
network-security-test: executing command "rcp /tmp/largeDummyFile.sign 169.254.6.223:/tmp/largeDummyFile.sign"...
network-security-test: executing command "ssh -i sshIdentityFile 169.254.6.223 openssl dgst -sha256 -verify /tmp/pubkey -signature /tmp/largeDummyFile.sign /tmp/largeDummyFile"...
network-security-test: signature check result: Verified OK
network-security-test: executing command "ssh -i sshIdentityFile root@169.254.6.223 systemctl stop xinetd"...
network-security-test: result of signature test: passed
```
