#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define TIMES 100000000

int main()
{
	long i, x, y, a=0,b=0;
	struct timeval tpstart;

	gettimeofday(&tpstart,NULL);
	srand(tpstart.tv_usec);
	for (i = 0; i < TIMES; i++) {
		x = rand()%(10000+1);
		y = rand()%(10000+1);
		if ((x*x + y*y) <= 100000000)
			a++;
	}

	printf("%f\n", 4.0 * a / TIMES);
}

