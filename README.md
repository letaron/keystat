# KEYSTAT

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
