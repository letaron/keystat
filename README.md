# MINITEL

Port knocking Minitel Kernel Module

[![Travis CI](https://api.travis-ci.org/letaron/keystat.svg?branch=master)](https://travis-ci.org/letaron/keystat)

## TODO

  - Display payload of the UDP packet.

## BUILD

```sh
apt-get install build-essential linux-headers-$(uname -r)
make
```
## USAGE

```sh
insmod keystat.ko
dmesg | tail -1
rmmod hello.ko
dmesg | tail -1
```
