#include <system.h>
#include <screen.h>

void cls() {
	clear_screen();
}

void putch(char c) {
	outputch(c);
}

void puts(char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

void putn(int n) {
	char str[50];
	puts(itoa(n,str));
}

void puth(int n) {
	char str[50];
	puts("0x");
	puts(itoa_hex(n,str));
}

void putb(int n) {
	puts("Bx");
	unsigned char c = (unsigned char)n;
	putn((c >> 7) & 1);
	putn((c >> 6) & 1);
	putn((c >> 5) & 1);
	putn((c >> 4) & 1);
	putn((c >> 3) & 1);
	putn((c >> 2) & 1);
	putn((c >> 1) & 1);
	putn((c >> 0) & 1);
}

void putbool(bool t) {
	t ? puts("true") : puts("false");
}

void putnl() {
	puts("\n");
}

void putvar(char c,void* value) {
	if(c == 'd') {
		int* ptr = (int*)value;
		putn(*ptr);
	} else if (c == 'u'){
		unsigned long *ptr = (unsigned long*)value;
		putn(*ptr);
	} else if (c == 'i'){
		unsigned short *ptr = (unsigned short*)value;
		puth(*ptr);
	}else if (c == 'h') {
		unsigned long *ptr = (unsigned long*)value;
		puth(*ptr);
	} else if (c == 'b'){
		bool *ptr = (bool*)value;
		putbool(*ptr);
	} else if (c == 's'){
		char* ptr = (char*)value;
		puts(ptr);
	}
}

void putf(char* format,void** values) {
	int index = 0;
	int val_index = 0;
	while(format[index] != 0) {
		if(format[index] == '%') {
			format++;
			if (format[index] == 0) {
				return;
			} else if(format[index] == '%') {
				putch('%');
			} else {
				putvar(format[index],values[val_index]);
				val_index++;
			}
		} else {
			putch(format[index]);
		}
		index++;
	}
}

void putf(char* format,void* one) {
	void* arr [] = { one };
	putf(format,arr);
}