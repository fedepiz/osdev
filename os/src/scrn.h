#ifndef __SCRN_H
#define __SCRN_H
void cls();
void putch(char c);
void puts(char *str);
void putn(int n);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video();
#endif