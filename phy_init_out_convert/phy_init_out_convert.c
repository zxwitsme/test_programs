#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_WORD1 "CUSTOMER_API_COMMENT: "
#define KEY_WORD2 "dwc_ddrphy_apb_wr"
#define KEY_WORD3 "32'h"
#define KEY_WORD4 "16'h"
#define KEY_WORD5 "//"

#define MAX_LINE_LEN 2048

int reg_hit[3278000] = {0};

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

unsigned int reg_convert(unsigned int reg)
{
	int i;

	ddr_addr.value = reg;
	for (i = 0; i < sizeof(mapping)/(2*sizeof(short)); i++) {
		if (ddr_addr.bit_value.bit_22_12 == mapping[i][0]) {
			axi_addr.bit_value.bit_17_12 = mapping[i][1];
			axi_addr.bit_value.bit_11_0 = ddr_addr.bit_value.bit_11_0;
		}
	}

	//printf("axi_addr:%8x ddr_addr:%8x\n", axi_addr.value << 2, ddr_addr.value);

	return axi_addr.value << 2;
}

int main(int argc, char* argv[])
{
	int i,s;
	FILE *fp_in, *fp_out, *fp_h;
	char *buf;
	char *buf_out;
	unsigned int reg, axi_reg, val;
	size_t len = 0;
	size_t read;

	if (argc < 4) {
		printf("./phy_init_out_convert txt_file_name out_c_file out_h_file\n");
		return -1;
	}

	if ((fp_in=fopen(argv[1], "r")) == NULL) {
		printf("Filed %s can't be opened\n", argv[1]);
		return -1;
	}

	if ((fp_out=fopen(argv[2], "w+")) == NULL) {
		printf("Filed %s can't be opened\n", argv[2]);
		return -1;
	}

	if ((fp_h=fopen(argv[3], "w+")) == NULL) {
		printf("Filed %s can't be opened\n", argv[3]);
		return -1;
	}
		
	while ((read = getline(&buf, &len, fp_in)) != -1) {
		if (buf_out = strstr(buf, KEY_WORD1)) {
			buf_out += strlen(KEY_WORD1);
			if (buf_out = strstr(buf, KEY_WORD2)) {
				buf_out += strlen(KEY_WORD2);
				if (buf_out = strstr(buf, KEY_WORD3)) {
					buf_out += strlen(KEY_WORD3);
					reg = strtoul(buf_out, NULL, 16);
					buf_out = strstr(buf, KEY_WORD4);
					buf_out += strlen(KEY_WORD4);
					val = strtoul(buf_out, NULL, 16);
					if ((reg > 0x5403f) && (reg <= 0x55fff))
						continue;
					if ((reg > 0x50000) && (reg <= 0x53fff))
						continue;
					axi_reg = reg_convert(reg);
					if (reg_hit[reg] == 0) {
						fprintf(fp_h, "#define DDRPHY_%x_%x 0x%x\n", reg, axi_reg, axi_reg);
						reg_hit[reg] = 1;
					}
					fprintf(fp_out, "#define DDRPHY_%x_%x_VAL 0x%x\n", reg, axi_reg, val);
				}
			} else if (buf_out = strstr(buf, KEY_WORD5))
				fprintf(fp_out, "%s", buf_out);
		}
		
	}

	fclose(fp_in);
	fclose(fp_out);
	fclose(fp_h);

	return 0;
}

