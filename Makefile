CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = ~/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

build/boot.o: src/boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c src/boot.S -o build/boot.o

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: build/boot.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib build/boot.o $(OFILES) -T src/link.ld -o build/kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary build/kernel8.elf build/kernel8.img

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true