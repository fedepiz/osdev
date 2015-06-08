AS = i686-elf-as
CC = i686-elf-g++

INCLUDE_DIR = include

CFLAGS = -c -I$(INCLUDE_DIR) -ffreestanding -nostdlib -O2 -Wall -Wextra -Wno-write-strings -Wno-unused-parameter \
         -fno-exceptions -fno-rtti
		 
LINKFLAGS = -ffreestanding -nostdlib -O0 -Wall -Wextra -fno-exceptions -fno-rtti
ISOFLAGS = -no-emul-boot -boot-load-size 4 -boot-info-table

C_OBJS = out/kernel.o out/system.o out/scrn.o out/gdt.o  out/idt.o out/isr.o \
         out/irq.o out/timer.o out/keyboard.o out/mmap.o \
		 out/frame_manager.o out/paging.o out/heap.o out/kernel_tests.o
all: out out/boot.o $(C_OBJS) out/myos.bin out/bootable.iso
out:
	mkdir out
#assemble startup segment
out/boot2.o:
	$(AS) --32 src/boot.s -o out/boot.o
out/boot.o:
	nasm -f elf32 src/start.asm -o out/boot.o
#compile c sources
out/kernel.o:
	$(CC) $(CFLAGS) -o out/kernel.o src/kernel.cpp
out/system.o:
	$(CC) $(CFLAGS) -o out/system.o src/system.cpp
out/scrn.o:
	$(CC) $(CFLAGS) -o out/scrn.o src/scrn.cpp
out/gdt.o:
	$(CC) $(CFLAGS) -o out/gdt.o src/gdt.cpp
out/idt.o:
	$(CC) $(CFLAGS) -o out/idt.o src/idt.cpp
out/isr.o:
	$(CC) $(CFLAGS) -o out/isr.o src/isr.cpp
out/irq.o:
	$(CC) $(CFLAGS) -o out/irq.o src/irq.cpp
out/timer.o:
	$(CC) $(CFLAGS) -o out/timer.o src/timer.cpp
out/keyboard.o:
	$(CC) $(CFLAGS) -o out/keyboard.o src/keyboard.cpp
out/mmap.o:
	$(CC) $(CFLAGS) -o out/mmap.o src/mmap.cpp
out/frame_manager.o:
	$(CC) $(CFLAGS) -o out/frame_manager.o src/frame_manager.cpp
out/paging.o:
	$(CC) $(CFLAGS) -o out/paging.o src/paging.cpp
out/heap.o:
	$(CC) $(CFLAGS) -o out/heap.o src/heap.cpp
out/kernel_tests.o:
	$(CC) $(CFLAGS) -o out/kernel_tests.o src/kernel_tests.cpp

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