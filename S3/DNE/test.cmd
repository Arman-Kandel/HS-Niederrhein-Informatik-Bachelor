@echo off

echo Connect to R1 and press any key to continue.
pause > nul
echo g000v4='172.16.1.1 255.255.255.0' ;/24
ping 172.16.1.1
echo g001v4='192.168.1.1 255.255.255.0' ;/24
ping 192.168.1.1 
echo g000v6='2001:db8:acad:2::1/64'
ping 2001:db8:acad:2::1
echo g001v6='2001:db8:acad:1::1/64'
ping 2001:db8:acad:1::1
echo v6link='fe80::1' ; same for g000 and g001
echo loop1='10.1.0.1 255.255.255.0' ;/24
ping 10.1.0.1
echo loop2='209.165.200.225 255.255.255.224' ;/27
ping 209.165.200.225

echo Connect to R2 and press any key to continue.
pause > nul
echo g000v4='172.16.1.2 255.255.255.0' ;/24
ping 172.16.1.2
echo g001v4='192.168.1.2 255.255.255.0' ;/24
ping 192.168.1.2
echo g000v6='2001:db8:acad:2::2/64'
ping 2001:db8:acad:2::2
echo g001v6='2001:db8:acad:1::2/64'
ping 2001:db8:acad:1::2
echo v6link='fe80::2' ; same for g000 and g001
echo loop1='10.2.0.1 255.255.255.0' ;/24
ping 10.2.0.1
echo loop2='209.165.200.193 255.255.255.224' ;/27
ping 209.165.200.193

echo Connect to S1 and press any key to continue.
pause > nul
echo vlan1v4='192.168.1.4 255.255.255.0'
ping 192.168.1.4
echo vlan1v4def='192.168.1.1' ; (R1 IP)
echo vlan1v6='2001:db8:acad:1::4/64'
ping 2001:db8:acad:1::4
echo v6link='fe80::4'

echo Connect to S2 and press any key to continue.
pause > nul
echo vlan1v4='192.168.1.5 255.255.255.0'
ping 192.168.1.5
echo vlan1v4def='192.168.1.1' ; (R1 IP)
echo vlan1v6='2001:db8:acad:1::5/64'
ping 2001:db8:acad:1::5
echo v6link='fe80::5'
