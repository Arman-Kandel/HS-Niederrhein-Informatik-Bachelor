# Exam BSY-Praktika
Program to solve exercise 20 of the final BSY-Praktika exam.

## Steps
- Create a file named `ssh-config` in this directory and populate it
with your credentials, like shown below:
```
Host bsy-pi
    HostName <ip>
    User <user>
    ProxyJump bsy-gateway
    IdentityFile ./private-key

Host bsy-gateway
    HostName 194.94.121.114
    Port 15306
    User <user>
    IdentityFile ./private-key
```
- Also insert your `private-key` file in this directory with chmod 600.
If on Windows, insert it in `C:\Users\<user>\bsy-ssh-private-key`,
and make sure to update the `IdentityFile` path in your config.
- Run the start script (`.bat` for Windows and `.sh` for Unix like systems).
It will upload `main.c` to the pi, compile it with `gcc` on that machine and execute the
compiled program.

Notes for Windows users:
- You need to install OpenSSH from [here](https://docs.microsoft.com/de-de/windows-server/administration/openssh/openssh_install_firstuse).