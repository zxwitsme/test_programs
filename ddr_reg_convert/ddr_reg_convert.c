#include <stdio.h>

unsigned short mapping[][2]=
{
	{0x000,0x00},
	{0x001,0x01},
	{0x002,0x02},
	{0x010,0x03},
	{0x011,0x04},
	{0x020,0x05},
	{0x040,0x06},
	{0x090,0x07},

	{0x100,0x08},
	{0x101,0x09},
	{0x102,0x0a},
	{0x110,0x0b},
	{0x111,0x0c},
	{0x120,0x0d},
	{0x140,0x0e},
	{0x190,0x0f},

	{0x200,0x10},
	{0x201,0x11},
	{0x202,0x12},
	{0x210,0x13},
	{0x211,0x14},
	{0x220,0x15},
	{0x240,0x16},
	{0x290,0x17},

	{0x300,0x18},
	{0x301,0x19},
	{0x302,0x1a},
	{0x310,0x1b},
	{0x311,0x1c},
	{0x320,0x1d},
	{0x340,0x1e},
	{0x390,0x1f},

	{0x050,0x20},
	{0x051,0x21},
	{0x052,0x22},
	{0x053,0x23},
	{0x054,0x24},
	{0x055,0x25},
	{0x056,0x26},
	{0x057,0x27},
	{0x070,0x28},
	{0x0c0,0x29},
	{0x0d0,0x2a},
};

union {
	struct {
		unsigned bit_11_0  :12;
		unsigned bit_22_12 :11;
		unsigned bit_31_23 :9;
	} bit_value;
	unsigned int value;
} __attribute__ ((packed)) ddr_addr;

union {
	struct {
		unsigned bit_11_0  :12;
		unsigned bit_17_12 :6;
		unsigned bit_31_18 :14;
	} bit_value;
	unsigned int value;
} __attribute__ ((packed)) axi_addr;

int main(int argc, char* argv[])
{
	int i, type;

	if (argc < 3) {
		printf("./ddr_reg_convert type axi_addr/ddr_addr\n");
		printf("-type 0: ddr address to axi address\n");
		printf("-type 1: axi address to ddr address\n");
		return -1;
	}

	type = strtol(argv[1], NULL, 16);
	switch (type) {
	case 0:
		ddr_addr.value = strtol(argv[2], NULL, 16);
		printf("ddr address to axi address:\n");
		break;
	case 1:
		axi_addr.value = strtol(argv[2], NULL, 16) >> 2;
		printf("axi address to ddr address:\n");
		break;
	default:
		printf("invalid type\n");
		return -1;
	}

	for (i = 0; i < sizeof(mapping)/(2*sizeof(short)); i++) {
		switch (type) {
		case 0:
			if (ddr_addr.bit_value.bit_22_12 == mapping[i][0]) {
				axi_addr.bit_value.bit_17_12 = mapping[i][1];
				axi_addr.bit_value.bit_11_0 = ddr_addr.bit_value.bit_11_0;
			}
			break;
		case 1:
			if (axi_addr.bit_value.bit_17_12 == mapping[i][1]) {
				ddr_addr.bit_value.bit_22_12 = mapping[i][0];
				ddr_addr.bit_value.bit_11_0 = axi_addr.bit_value.bit_11_0;
			}
			break;
		}
	}


	printf("axi_addr:%8x ddr_addr:%8x\n", axi_addr.value << 2, ddr_addr.value);


	return 0;
}
