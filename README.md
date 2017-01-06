# MINITEL

Port knocking Minitel Kernel Module

[![Travis CI](https://api.travis-ci.org/letaron/krnlmdl_minitel.svg?branch=master)](https://travis-ci.org/letaron/krnlmdl_minitel)

## TODO

  - Display payload of the UDP packet.

## BUILD

```sh
apt-get install build-essential linux-headers-$(uname -r)
make
```
## USAGE

```sh
insmod minitel.ko
dmesg | tail -1
rmmod minitel.ko
dmesg | tail -1
```
