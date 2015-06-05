#ifndef _IRQ_H
#define _IRQ_H
#include<system.h>
typedef void (*irq_handler)(struct regs*);

void irq_install();
void irq_enable();

void irq_install_handler(int irq, irq_handler handler);
void irq_unistall_handler(int irq);
#endif