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

struct bw_master {
	const char *name;
	unsigned char addr;
};

struct bw_master master[]={
	{"aaa", 0x0001},
	{"bbb", 0x0002},
	{"ccc", 0x0003},
};

int main()
{
	struct my_data data;
	unsigned int *p=&data;
	int i;

	data.a.a=0x12345678;
	data.b.b=0x55aa55aa;
	printf("%x %x\n", &data.a, &data.b);
	printf("%x %x %x\n",p, *p, *(p+1));

	for(i=0;i<3;i++) {
		printf("name:%s\n", master[i].name);
		printf("addr:%d\n", master[i].addr);
	}

	return 0;
}

