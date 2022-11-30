#!/bin/bash
# Execute as SU
apt install -y tar
# PostgreSQL
apt-get install wget ca-certificates
wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'
apt-get update
apt install -y postgresql postgresql-contrib
apt-get install -y libpqxx-dev # c++/c libs
