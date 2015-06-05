#ifndef __SYSTEM_H
#define __SYSTEM_H
void memcpy(unsigned char *dest, const unsigned char *src, int count);
void memset(unsigned char *dest, unsigned char val, int count);
void memcpyw(unsigned short* dest, const unsigned short* src,int count);
void memsetw(unsigned short *dest, unsigned short val, int count);
int strlen(const char *str);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);
int atoi(char *str);
void reverse(char* str, int length);
char* itoa(int num, char* str);

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp ,ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};
//VGA
void cls();
void putch(char c);
void puts(char *str);
void putn(int n);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video();

//GDT
void gdt_install();
//IDT
void idt_set_gate(unsigned char num, unsigned long base, 
				  unsigned short sel, unsigned char flags);
void idt_install();
//ISR
#define EXCEPTION_HANDLER_COUNT 32
//The type of exception handler calls
typedef void (*exception_handler)(regs*);
//Sets a specific function to handle exception number n
void isr_install_handler(int n,exception_handler h);
//De selects handler for exception n, restoring fallback handler.
void isr_unistall_handler(int n);
//Install the isrs. Must be called before all other isr_related
//functionality is enabled.
void isr_install();
//IRQ
typedef void (*irq_handler)(struct regs*);
void irq_install();
void irq_enable();

void irq_install_handler(int irq, irq_handler handler);
void irq_unistall_handler(int irq);
#endif