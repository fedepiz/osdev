all: out out/boot.o out/kernel.o out/myos.bin out/bootable.iso
#create the output directory
out:
	mkdir out
#assemble startup segment
out/boot.o:
	i686-elf-as src/boot.s -o out/boot.o
#compile the kernel
out/kernel.o:
	i686-elf-g++ -c src/kernel.cpp -o out/kernel.o -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -fno-rtti
#link togheter startup segment and kernel code 
out/myos.bin:
	i686-elf-g++ -T src/linker.ld -o out\myos.bin -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -fno-rtti out/boot.o out/kernel.o
#copy iso directory structure into out directory,
#then move the compiled kernel and finally
#make the iso
out/bootable.iso:
	cp -r ./iso_template_bootable ./out/iso
	cp out/myos.bin out/iso/boot/kernel
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o out/bootable.iso out/iso
#delete output directory and all it's contents
clean:
	rm -rf ./out