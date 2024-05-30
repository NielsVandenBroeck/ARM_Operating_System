CFILES = $(shell find $(SOURCEDIR) -name '*.c')
SFILES = $(shell find $(SOURCEDIR) -name '*.S')
OFILES = $(CFILES:.c=.o) $(SFILES:.S=.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdlib -mcpu=cortex-a53+nosimd
GCCPATH = ~/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

%.o: %.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib $(OFILES) -T src/link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf build/kernel8.img
	find . -type f -name '*.o' -delete
	/bin/rm *.elf
	
clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true