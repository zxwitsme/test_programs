#include <stdio.h>

int main()
{
	unsigned long r;
	unsigned int a,b,c;
	char *str = "ap";

	if (!strcmp(str, "ap"))
		printf("ap hit\n");
	else
		printf("ap not hit\n");

	a = 4096;
	b = 1024;
	c = 1024;

	//r = (unsigned long)(a) * (unsigned long)b * (unsigned long)c;
	r = a * b * (unsigned long)(1024);

	printf("r=%ld\n", r);
}
