#include <stdio.h>
#include <stdbool.h>

static int memctl_swap_mr(int val) 
{
	int i;
	int new_val;
        union {
                unsigned char new_val;
                struct {
                        unsigned bit0:1;
                        unsigned bit1:1;
                        unsigned bit2:1;
                        unsigned bit3:1;
                        unsigned bit4:1;
                        unsigned bit5:1;
                        unsigned bit6:1;
                        unsigned bit7:1;
                } b;
        } a;


        a.b.bit0 = (val & 0x40) ? 1 : 0; 
        a.b.bit1 = (val & 0x80) ? 1 : 0; 
        a.b.bit2 = (val & 0x10) ? 1 : 0; 
        a.b.bit3 = (val & 0x20) ? 1 : 0; 
        a.b.bit4 = (val & 0x04) ? 1 : 0; 
        a.b.bit5 = (val & 0x08) ? 1 : 0; 
        a.b.bit6 = (val & 0x01) ? 1 : 0; 
        a.b.bit7 = (val & 0x02) ? 1 : 0; 

	printf("new_val:%x\n", a.new_val);

        return a.new_val;
}

int main()
{
	int val;
	val = memctl_swap_mr(0x1c);
	printf("val=%x\n",val);
}
