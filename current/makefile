AS = i686-elf-as
CC = i686-elf-g++

INCLUDE_DIR = src

CFLAGS = -c -I$(INCLUDE_DIR) -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LINKFLAGS = -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -fno-rtti
ISOFLAGS = -no-emul-boot -boot-load-size 4 -boot-info-table

C_OBJS = out/kernel.o out/system.o out/scrn.o
all: out out/boot.o $(C_OBJS) out/myos.bin out/bootable.iso
out:
	mkdir out
#assemble startup segment
out/boot.o:
	$(AS) src/boot.s -o out/boot.o
#compile c sources
out/kernel.o:
	$(CC) $(CFLAGS) -o out/kernel.o src/kernel.cpp
out/system.o:
	$(CC) $(CFLAGS) -o out/system.o src/system.cpp
out/scrn.o:
	$(CC) $(CFLAGS) -o out/scrn.o src/scrn.cpp
#link togheter startup segment and kernel code 
out/myos.bin:
	$(CC) -T src/linker.ld -o out/myos.bin $(LINKFLAGS) out/boot.o $(C_OBJS)
#copy iso directory structure into out directory,
#then move the compiled kernel and finally
#make the iso
out/bootable.iso:
	cp -r ./iso_template_bootable ./out/iso
	cp out/myos.bin out/iso/boot/kernel
	mkisofs -R -b boot/grub/stage2_eltorito $(ISOFLAGS) -o out/bootable.iso out/iso
#delete output directory and all it's contents
clean:
	rm -rf ./out