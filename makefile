AS = i686-elf-as
CC = i686-elf-g++

INCLUDE_DIR = include

CFLAGS = -c -I$(INCLUDE_DIR) -ffreestanding -nostdlib -O2 -Wall -Wextra -Wno-write-strings -Wno-unused-parameter \
         -fno-exceptions -fno-rtti
		 
LINKFLAGS = -ffreestanding -nostdlib -O0 -Wall -Wextra -fno-exceptions -fno-rtti
ISOFLAGS = -no-emul-boot -boot-load-size 4 -boot-info-table

#This gathers the names of all the .cpp files in src,
#replaces their directory with out and their extension with out.
# src/X.cpp -> out/X.out
C_OBJS = $(patsubst src/%.cpp,out/%.o,$(wildcard src/*.cpp))

all: out out/boot.o $(C_OBJS) out/myos.bin out/bootable.iso
#make output directory
out:
	mkdir out
#assemble startup segment
out/boot.o:
	nasm -f elf32 src/start.asm -o out/boot.o
#compile c++ sources, individually
out/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ $<
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