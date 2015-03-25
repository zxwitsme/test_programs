struct a_data {
	int a;
};

struct b_data {
	int b;
};
struct my_data {
	struct a_data a;
	struct b_data b;
};

int main()
{
	struct my_data data;
	unsigned int *p=&data;
	data.a.a=0x12345678;
	data.b.b=0x55aa55aa;
	printf("%x %x\n", &data.a, &data.b);
	printf("%x %x %x\n",p, *p, *(p+1));
}

