
ASMSRC = kernel32.o
SOURCES = buffer.o
LDFLAGS = -T linkthefiles.ld
CFLAGS = -c -std=c99 -march=i386 -Wshadow -m32   -s -O0 -ffreestanding -nostdinc -nostdlib -fno-pic \
       -fno-strict-aliasing -fno-builtin -fno-stack-protector -fno-common 


NASMFLAGS = -Ox -g -f elf 
AsmDir = asm/
SourceDir=asm/
ASMsources = $(addprefix $(AsmDir),$(ASMSRC))
CSources=$(addprefix $(SourceDir),$(SOURCES))
.s.o:	
	nasm $(NASMFLAGS) $<
all: $(CSources) $(ASMsources) link
link:	
	ld $(LDFLAGS) -o KERNEL $(ASMsources)  $(CSources) -Map kernel.map
	sudo mount /dev/sdc1 /mnt
	sudo cp KERNEL /mnt/boot
	sudo umount /mnt
	qemu -monitor stdio -s -net nic,model=rtl8139 -kernel KERNEL 
clean:	
	-rm $(AsmDir)*.o $(SourceDir)*.o
	-rm KERNEL
