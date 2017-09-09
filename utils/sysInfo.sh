#!/bin/bash

rm info.txt

#Gives OS type. Ex. Linux
echo "Operating System Type:" >> info.txt
uname -s >> info.txt

#Should be distribution, Eg. Ubuntu
#116-Ubuntu SMP Fri Aug 11 21:17:51 UTC 2017
echo "OS Distribution:" >> info.txt
cat /etc/os-release | grep "PRETTY_NAME" | cut -d'"' -f2 >> info.txt

#Gives Kernel version
echo "Kernel Version:">>info.txt
uname -r >>info.txt

#Information on Kernel build
echo "Info on Kernel Build:" >>info.txt
cat /proc/version >>info.txt

#Kernel gcc version build
echo "Kernel gcc version build:" >> info.txt
cat /proc/version | cut -d' ' -f7,8 >> info.txt

#Kernel build time
echo "Kernel build time:" >> info.txt
cat /proc/version | cut -d'#' -f2 | cut -d' ' -f3- >> info.txt

#System Architecture information
echo "System Architecture information:" >> info.txt
uname -p >> info.txt
