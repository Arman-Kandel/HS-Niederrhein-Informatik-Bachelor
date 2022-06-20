# Exam BSY-Praktika
Program to solve exercise 20 of the final BSY-Praktika exam.

## Steps
- Build with `Cmake` first
- Create a file named `ssh-config` in this directory and populate it
with your credentials
```
Host bsy-pi
HostName 172.31.0.1 <pi>
User <user>
ProxyJump bsy - gateway
IdentityFile ./private-key
Host bsy-gateway
HostName 194.94.121.114
Port 15306
User <user>
IdentityFile ./private-key
```
- Also insert your `private-key` file in this directory
- Run the start script (`.bat` for Windows and `.sh` for Unix like systems).

Notes for Windows users:
- You need to install OpenSSH from [here](https://docs.microsoft.com/de-de/windows-server/administration/openssh/openssh_install_firstuse).