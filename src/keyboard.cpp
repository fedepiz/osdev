#include <system.h>
#include <keyboard.h>
unsigned char kbstatus[128];
bool _keyboard_echo = false;
bool _keyboard_inited = false;

//getch_called is used as a synchronisation
//variable between getch and the keyboard 
//interrupt.
volatile bool _getch_called = false;
char _getch_char = '\0';

void set_keyboard_echo(bool status) {
	_keyboard_echo = status;
}
/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
		
		//Updates interal keyboard status
		scancode = scancode & (~0x80);
		kbstatus[scancode] = 0;
    }
    else
    {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
		if(_keyboard_echo) {
			putch(kbdus[scancode]);
		}
		//Updates interal keyboard status
		kbstatus[scancode] = 1;
		
		if(_getch_called) {
			_getch_char = kbdus[scancode];
			_getch_called = false;
		}
	}
}

bool get_key_status(int scancode) {
	return kbstatus[(unsigned char)scancode] == 1;
}

keyboard_status get_keyboard_status() {
	keyboard_status k;
	memcpy(&k,&kbstatus,128);
	return k;
}

char getch() {
	if(!_keyboard_inited)
		return '\0';
	_getch_called = true;
	while(true) {
		if(_getch_called == false)
			break;
	}
	return _getch_char;
}

char* gets(char* dest) {
	int index = 0;
	while(true) {
		char c = getch();
		putch(c);
		if (c == 0 || c == '\n') {
			dest[index] = '\0';
			return dest;
		} else {
			dest[index] = c;
			index++;
		}
	}
}



void keyboard_install() {
	_keyboard_echo = false;
	_keyboard_inited = true;
	_getch_called = false;
	_getch_char = '\0';
	memset((unsigned char*)&kbstatus,0,128);
	irq_install_handler(1, &keyboard_handler);
}
	