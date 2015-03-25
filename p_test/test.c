#include <stdio.h>

struct a {
	int value;
};

struct b {
	struct a test_a[4];
};

int main()
{
	struct b test_b;

	printf("%x %x\n", (unsigned int)(test_b.test_a), 
			(unsigned int)(&test_b.test_a[0]));
}
