#!/bin/sh
#GUIFILES=""
clear

printf '\033[1;34m'
echo "Compilation de noyau: Edition Personelle"
printf '\033[21;0m'

make

#/usr/libexec/qemu-kvm -no-kvm -net nic -net user,tftp=`pwd`,bootfile=boot.pxe -boot n -cpu pentium -rtc base=localtime -m 64M -S -s
#make run
gnome-terminal -x bash -c "kvm -no-kvm -net nic -net user,tftp=`pwd`,bootfile=boot.pxe -boot n -cpu pentium -rtc base=localtime -m 64M -S -s"
#gnome-terminal -x bash -c "vinagre localhost:5900"
gnome-terminal -x bash -c "gdb kernel.bin"



