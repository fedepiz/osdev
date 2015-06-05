#ifndef _ISR_H
#define _ISR_H

#include <system.h>

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
#endif