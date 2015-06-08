.code32
# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard. We put this into a special
# section so we can force the header to be in the start of the final program.
# You don't need to understand all these details as it is just magic values that
# is documented in the multiboot standard. The bootloader will search for this
# magic sequence and recognize us as a multiboot kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Currently the stack pointer register (esp) points at anything and using it may
# cause massive harm. Instead, we'll provide our own stack. We will allocate
# room for a small temporary stack by creating a symbol at the bottom of it,
# then allocating 16384 bytes for it, and finally creating a symbol at the top.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text 
.global _start
.type _start, @function
_start:
	# To set up a stack, we simply set the esp register to point to the top of
	# our stack (as it grows downwards).
	movl $stack_top, %esp

	# We are now ready to actually execute C code. We cannot embed that in an
	# assembly file, so we'll create a kernel.c file in a moment. In that file,
	# we'll create a C entry point called kernel_main and call it here.
	call kernel_main

	# In case the function returns, we'll want to put the computer into an
	# infinite loop. To do that, we use the clear interrupt ('cli') instruction
	# to disable interrupts, the halt instruction ('hlt') to stop the CPU until
	# the next interrupt arrives, and jumping to the halt instruction if it ever
	# continues execution, just to be safe. We will create a local label rather
	# than real symbol and jump to there endlessly.
	cli
	hlt
.Lhang:
	jmp .Lhang
	
.global _gdt_flush
_gdt_flush:
	lgdt _gp
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08, $_flush2
_flush2:
	ret

.global _idt_load
_idt_load:
	lidt idtp
	ret
.global _isr0
.global _isr1
.global _isr2
.global _isr3
.global _isr4
.global _isr5
.global _isr6
.global _isr7
.global _isr8
.global _isr9
.global _isr10
.global _isr11
.global _isr12
.global _isr13
.global _isr14
.global _isr15
.global _isr16
.global _isr17
.global _isr18
.global _isr19
.global _isr20
.global _isr21
.global _isr22
.global _isr23
.global _isr24
.global _isr25
.global _isr26
.global _isr27
.global _isr28
.global _isr29
.global _isr30
.global _isr31

_isr0:
	cli
	pushw $0
	pushw $0
	jmp isr_common_stub
_isr1:
	cli
	pushw $0
	pushw $1
	jmp isr_common_stub
_isr2:
	cli
	pushw $0
	pushw $2
	jmp isr_common_stub
_isr3:
	cli
	pushw $0
	pushw $3
	jmp isr_common_stub
_isr4:
	cli
	pushw $0
	pushw $4
	jmp isr_common_stub
_isr5:
	cli
	pushw $0
	pushw $5
	jmp isr_common_stub
_isr6:
	cli
	pushw $0
	pushw $6
	jmp isr_common_stub
_isr7:
	cli
	pushw $0
	pushw $7
	jmp isr_common_stub
_isr8:
	cli
	pushw $8
	jmp isr_common_stub
_isr9:
	cli
	pushw $0
	pushw $9
	jmp isr_common_stub
_isr10:
	cli
	pushw $10
	jmp isr_common_stub
_isr11:
	cli
	pushw $11
	jmp isr_common_stub
_isr12:
	cli
	pushw $12
	jmp isr_common_stub
	cli
_isr13:
	cli
	pushw $13
	jmp isr_common_stub
_isr14:
	cli
	pushw $14
	jmp isr_common_stub
_isr15:
	cli
	pushw $0
	pushw $15
	jmp isr_common_stub
_isr16:
	cli
	pushw $0
	pushw $16
	jmp isr_common_stub
_isr17:
	cli
	pushw $0
	pushw $17
	jmp isr_common_stub
_isr18:
	cli
	pushw $0
	pushw $18
	jmp isr_common_stub
_isr19:
	cli
	pushw $0
	pushw $19
	jmp isr_common_stub
_isr20:
	cli
	pushw $0
	pushw $20
	jmp isr_common_stub
_isr21:
	cli
	pushw $0
	pushw $21
	jmp isr_common_stub
_isr22:
	cli
	pushw $0
	pushw $22
	jmp isr_common_stub
_isr23:
	cli
	pushw $0
	pushw $23
	jmp isr_common_stub
_isr24:
	cli
	pushw $0
	pushw $24
	jmp isr_common_stub
_isr25:
	cli
	pushw $0
	pushw $25
	jmp isr_common_stub
_isr26:
	cli
	pushw $0
	pushw $26
	jmp isr_common_stub
_isr27:
	cli
	pushw $0
	pushw $27
	jmp isr_common_stub
_isr28:
	cli
	pushw $0
	pushw $28
	jmp isr_common_stub
_isr29:
	cli
	pushw $0
	pushw $29
	jmp isr_common_stub
_isr30:
	cli
	pushw $0
	pushw $30
	jmp isr_common_stub
_isr31:
	cli
	pushw $0
	pushw $31
	jmp isr_common_stub	
	
isr_common_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %gs
	mov %ax, %gs
	mov %ax, %gs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	mov $_fault_handler, %eax
	call *%eax
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add %esp, 8
	iret	
# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start
