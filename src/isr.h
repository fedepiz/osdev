#ifndef _ISR_H
#define _ISR_H

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp ,ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};
#define EXCEPTION_HANDLER_COUNT 32
//The type of exception handler calls
typedef void (*exception_handler)(regs*);
//Sets a specific function to handle exception number n
void set_handler(int n,exception_handler h);
//De selects handler for exception n, restoring fallback handler.
void clear_handler(int n);
//Install the isrs. Must be called before all other isr_related
//functionality is enabled.
void isr_install();
#endif