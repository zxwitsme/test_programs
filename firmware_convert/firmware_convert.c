#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_WORD "16'h"

int main(int argc, char* argv[])
{
	FILE *fp_in, *fp_out;
	unsigned short val;
	unsigned int value;
	unsigned int type;
	char *buf;
	char *buf_out;
	size_t len = 0;
	size_t read;

	if (argc < 4) {
		printf("./firmware_convert file_type firmware_name out_h_file\n");
		printf("-firmware_type 0 - from bin\n");
		printf("-firmware_type 1 - from incv\n");
		return -1;
	}

	if ((fp_in=fopen(argv[2], "r")) == NULL) {
		printf("Filed %s can't be opened\n", argv[2]);
		return -1;
	}

	if ((fp_out=fopen(argv[3], "w+")) == NULL) {
		printf("Filed %s can't be opened\n", argv[3]);
		return -1;
	}

	type = strtol(argv[1], NULL, 16);
	if (type == 0) {
		while (!feof(fp_in)) {
			fread(&val, 1, sizeof(unsigned short), fp_in);
			printf("0x%04x,\n", val);
			fprintf(fp_out, "0x%04x,\n", val);
		}
	} else {
		while ((read = getline(&buf, &len, fp_in)) != -1) {
			if (buf_out = strstr(buf, KEY_WORD)) {
				buf_out += strlen(KEY_WORD);
				value = strtoul(buf_out, NULL, 16);
				printf("0x%04x,\n", value);
				fprintf(fp_out, "0x%04x,\n", value);
			}
		}
	}
	fclose(fp_in);
	fclose(fp_out);

	return 0;
}

