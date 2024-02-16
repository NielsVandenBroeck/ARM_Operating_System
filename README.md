# ARM_Operating_System

## usefull links
 ```
https://jsandler18.github.io/
https://www.rpi4os.com/
https://developer.arm.com/documentation/den0024/a/
https://github.com/jserv/mini-arm-os/tree/master
https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 ```

## how to get started
first install this:
 ```
sudo apt install gcc make gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
 ```

download this:
1. go to your home dir: 
```
cd ~
```
2. download compiler
 ```
 wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf.tar.xz
 ```
3. untar file
 ```
tar -xf gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf.tar.xz
```
4. make file
 ```
make
 ```

start emultor
```
qemu-system-aarch64 -M raspi3b -serial stdio -kernel build/kernel8.img
qemu-system-aarch64 -M raspi3 -smp cores=4 -serial stdio -kernel build/kernel8.img
```