#!/bin/bash


cd ~/AdvESE/Kernel/linux-4.12.10

#Commands to install and build
make defconfig
make -j2
sudo make modules_install 
sudo make install
