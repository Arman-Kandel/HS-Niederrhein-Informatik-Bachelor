#!/bin/bash
make &&
# Test at HS Niederrhein:
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile1
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile2
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile3
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile4
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile5
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile6
#&& ./hpdb -d dbs141 -u dbs141 -h 194.94.121.224 -p 15434 -c 1 -f ./hpdb-tests/commandfile7
# Test locally, note that postgres must have no password:
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile1 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile2 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile3 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile4 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile5 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile6 &&
./hpdb -d postgres -u postgres -h /var/run/postgresql -p 5432 -c "" -f ./hpdb-tests/commandfile7 