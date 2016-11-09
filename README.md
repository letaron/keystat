# KEYSTAT

[![Travis CI](https://api.travis-ci.org/letaron/keystat.svg?branch=master)](https://travis-ci.org/letaron/keystat)

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
