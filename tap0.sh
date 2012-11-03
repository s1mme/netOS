#!/bin/sh 
sudo /etc/qemu-ifup tap0
sudo qemu -m 2000 -kernel KERNEL -net nic,model=rtl8139 -net tap,ifname=tap0,script=no -monitor stdio
sudo /etc/qemu-ifdown tap0 
