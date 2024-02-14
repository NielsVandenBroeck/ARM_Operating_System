CFILES = $(shell find $(SOURCEDIR) -name '*.c')
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -Wunknown-pragmas
GCCPATH = ~/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

interrupt_vector.o: src/basic/interrupt_vector.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c src/basic/interrupt_vector.S -o interrupt_vector.o

boot.o: src/boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c src/boot.S -o boot.o

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o interrupt_vector.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib boot.o interrupt_vector.o $(OFILES) -T src/link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf build/kernel8.img
	/bin/rm -r *.o
	find . -type f -name '*.o' -delete
	/bin/rm *.elf
	
clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true