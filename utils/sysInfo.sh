#!/bin/bash

rm info.txt

#Gives OS type. Ex. Linux
echo "Operating System Type:" >> info.txt
uname -s >> info.txt

#Should be distribution, Eg. Ubuntu
#116-Ubuntu SMP Fri Aug 11 21:17:51 UTC 2017
echo "OS Distribution:" >> info.txt
uname -v >> info.txt

#Gives Kernel version
echo "Kernel Version:">>info.txt
uname -r >>info.txt

#Information on Kernel build
echo "Info on Kernel Build" >>info.txt
cat /proc/version >>info.txt


