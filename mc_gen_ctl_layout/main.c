#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gen_result(int offset, int width)
{
	int i, bit = 1;
	int result = 0;

	//printf("offset=%d, width=%d\n", offset, width);
	for (i = offset; i < offset + width; i++)
		result |= bit << i;

	//printf("result=%x\n", result);
	return result;
}

int main()
{
	void *fp;
	char string[1024*1024];
	char f0_str[512][512];
	char f1_str[512][512];
	char f2_str[512][512];
	char f3_str[512][512];
	char *str1, *str2, *token, *token_b;
	char *saveptr1, *saveptr2;
	int i, j[4] = { 0, 0, 0, 0};
	int k, m, n;
	int phy_base = 0x1000;

	fp = fopen("ddr_mc_reg_define.h", "r");
	if (fp == NULL)
		printf("can't open file\n");
	while (!feof(fp)) {
		fread(&string[i],1,1,fp);
		i++;
	}

	printf("\tMC_ASSIGN_CLK_LAYOUT(0, 0x490, 0xffffffff);\n");
	printf("\tMC_ASSIGN_CLK_LAYOUT(1, 0x494, 0xffffffff);\n");
	printf("\tMC_ASSIGN_CLK_LAYOUT(2, 0x498, 0xffffffff);\n");
	printf("\tMC_ASSIGN_CLK_LAYOUT(3, 0x49c, 0xffffffff);\n");

	printf("\n");

	for (str1 = string; ;str1 = NULL) {
		token = strtok_r(str1, "\n", &saveptr1);
		if (token == NULL)
			break;
		//printf("%s\n", token);

		token_b = token;
		if(strstr(token, "_F0_")) {
			strncpy(f0_str[j[0]], token_b, strlen(token_b));
			printf("len=%d\n", strlen(token_b));
			printf("/***\n%s\n", token_b);
			printf("%s\n%d***/\n", f0_str[j[0]]);
			j[0]++;
		}
		if(strstr(token, "_F1_")) {
			strncpy(f1_str[j[1]], token_b, strlen(token_b));
			printf("len=%d\n", strlen(token_b));
			printf("/***\n%s\n", token_b);
			printf("%s\n%d***/\n", f1_str[j[1]]);
			j[1]++;
		}
		if(strstr(token, "_F2_")) {
			strncpy(f2_str[j[2]], token_b, strlen(token_b));
			printf("len=%d\n", strlen(token_b));
			printf("/***\n%s\n", token_b);
			printf("%s\n%d***/\n", f2_str[j[2]]);
			j[2]++;
		}
		if(strstr(token, "_F3_")) {
			strncpy(f3_str[j[3]], token_b, strlen(token_b));
			printf("len=%d\n", strlen(token_b));
			printf("/***\n%s\n", token_b);
			printf("%s\n%d***/\n", f3_str[j[3]]);
			j[3]++;
		}
	}

	printf("j[0] = %d\n", j[0]);
	printf("j[1] = %d\n", j[0]);
	printf("j[2] = %d\n", j[0]);
	printf("j[3] = %d\n", j[0]);

	for (i = 0, k = 0; i < j[0]; i++) {
		str2 = strstr(f0_str[i], "ADDR");
		if(str2) {
			printf("\tMC_ASSIGN_CTL_LAYOUT(0, %3d, ", k++);
			str2 += strlen("ADDR");
			printf("0x%03x, ", atoi(str2)*4);
		}

		str2 = strstr(f0_str[i], "OFFSET");
		if(str2) {
			str2 += strlen("OFFSET");
			m = atoi(str2);
		}

		str2 = strstr(f0_str[i], "WIDTH");
		if(str2) {
			str2 += strlen("WIDTH");
			n = atoi(str2);
			printf("0x%08x);\n", gen_result(m, n));
		}
	}

	printf("\n");

	for (i = 0, k = 0; i < j[1]; i++) {
		str2 = strstr(f1_str[i], "ADDR");
		if(str2) {
			printf("\tMC_ASSIGN_CTL_LAYOUT(1, %3d, ", k++);
			str2 += strlen("ADDR");
			printf("0x%03x, ", atoi(str2)*4);
		}

		str2 = strstr(f1_str[i], "OFFSET");
		if(str2) {
			str2 += strlen("OFFSET");
			m = atoi(str2);
		}

		str2 = strstr(f1_str[i], "WIDTH");
		if(str2) {
			str2 += strlen("WIDTH");
			n = atoi(str2);
			printf("0x%08x);\n", gen_result(m, n));
		}
	}

	printf("\n");

	for (i = 0, k = 0; i < j[2]; i++) {
		str2 = strstr(f2_str[i], "ADDR");
		if(str2) {
			printf("\tMC_ASSIGN_CTL_LAYOUT(2, %3d, ", k++);
			str2 += strlen("ADDR");
			printf("0x%03x, ", atoi(str2)*4);
		}

		str2 = strstr(f2_str[i], "OFFSET");
		if(str2) {
			str2 += strlen("OFFSET");
			m = atoi(str2);
		}

		str2 = strstr(f2_str[i], "WIDTH");
		if(str2) {
			str2 += strlen("WIDTH");
			n = atoi(str2);
			printf("0x%08x);\n", gen_result(m, n));
		}
	}

	printf("\n");

	for (i = 0, k = 0; i < j[3]; i++) {
		str2 = strstr(f3_str[i], "ADDR");
		if(str2) {
			printf("\tMC_ASSIGN_CTL_LAYOUT(3, %3d, ", k++);
			str2 += strlen("ADDR");
			printf("0x%03x, ", atoi(str2)*4);
		}

		str2 = strstr(f3_str[i], "OFFSET");
		if(str2) {
			str2 += strlen("OFFSET");
			m = atoi(str2);
		}

		str2 = strstr(f3_str[i], "WIDTH");
		if(str2) {
			str2 += strlen("WIDTH");
			n = atoi(str2);
			printf("0x%08x);\n", gen_result(m, n));
		}
	}

	printf("\n");

	k = 0;
	printf("\tMC_ASSIGN_PHY_LAYOUT(%3d, ", k++);
	printf("0x1700, 0xffffffff);\n");
	for (m = 0; m < 4; m++)
	for (n = 29; n <=58; n++) {
		printf("\tMC_ASSIGN_PHY_LAYOUT(%3d, ", k++);
		printf("0x%03x, 0xffffffff);\n", phy_base + 4 * (m * 64 + n));
	}
	printf("\tMC_ASSIGN_PHY_LAYOUT(%3d, ", k++);
	printf("0x%03x, 0xffffffff);\n", phy_base + 255 * 4);
	printf("\tMC_ASSIGN_PHY_LAYOUT(%3d, ", k++);
	printf("0x%03x, 0xffffffff);\n", phy_base + 258 * 4);
	printf("\tMC_ASSIGN_PHY_LAYOUT(%3d, ", k++);
	printf("0x%03x, 0xffffffff);\n", phy_base + 259 * 4);

	return 0;
}

