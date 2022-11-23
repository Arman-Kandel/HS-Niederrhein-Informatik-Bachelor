#!/bin/b
# EXECUTE AS ROOT!
# Installs dependecies needed for this project

#
# Libroyale
#
DIR="/opt/libroyale-4.10.0.162-LINUX-x86-64Bit"
if [ -d "$DIR" ]; then # Exists?
   echo "Already installed libroyale in '$DIR', skipped!"
else
   echo "Installing libroyale to '$DIR', please wait ..."
   apt install zip
   unzip libroyale-4.10.0.162-LINUX-x86-64Bit.zip 
   mv libroyale-4.10.0.162-LINUX-x86-64Bit /opt/
fi

#
# OpenCV
#
apt install libopencv-core4.2
apt install libopencv-dev
