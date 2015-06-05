#include <system.h>
void memcpy(unsigned char* dest, const unsigned char* src,int count) {
	for(int i = 0; i < count;i++) {
		dest[i] = src[i];
	}
}

void memset(unsigned char* dest,unsigned char val, int size) {
	for(int i = 0; i < size;i++) {
		dest[i] = val;
	}
} 

void memcpyw(unsigned short* dest, const unsigned short* src,int count){
	for(int i = 0; i < count;i++) {
		dest[i] = src[i];
	}
}

void memsetw(unsigned short *dest, unsigned short val, int count){
	for(int i = 0; i < count;i++) {
		dest[i] = val;
	}
}

int strlen(const char *str) {
	int c = 0;
	while(*str != '\0') {
		c++;
		str++;
	}
	return c;
}

int atoi(char *str)
{
    int res = 0; // Initialize result
 
    // Iterate through all characters of input string and update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}

void reverse(char* str, int length)
{
   char str2[length];
   for(int i = 0; i < length;i++) {
	   str2[i] = str[length - 1 - i];
   }
   for(int i = 0; i < length;i++) {
	   str[i] = str2[i];
   }
}

char* itoa(int num, char* str)
{
	int base = 10;
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

unsigned char inportb (unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;	
}
void outportb (unsigned short _port, unsigned char _data){
	 __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
