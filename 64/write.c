#include <stdio.h>
//#include <unistd.h>
#include <sys/time.h>
//#include <sched.h>
//#include <asm/system.h>

#define HM_SIZE (1024*1024*8)
#define HM_FOR_TIMES 10
#define HM_US_PER_S 1000000
#define HM_CONTINUE_TIMES (64*8)

#define HM_DUR(tv0, tv1) (((tv1.tv_sec-tv0.tv_sec)*HM_US_PER_S + tv1.tv_usec-tv0.tv_usec))

int k;
int main(int argc, char *argv[])
{

	int i, times;
	unsigned long m_time = 0;
	unsigned long w_time = 0;
	unsigned long r_time = 0;
	char* map_addr;//address
	unsigned hm_size, loops, jump;
       	struct timeval tv0, tv1, tv2, tv3;
       	struct timezone tz0, tz1, tz2, tz3;

	if (argc < 4) {
		printf("Usage:  <KB> <loop> <jump>, e.g., 2048 2 16\n");
		return -1;
	}

	hm_size = atoi(argv[1])*1024;
	map_addr = (char*)malloc(hm_size);
	for (i=0; i<hm_size; i+=4096) 
		*(map_addr+i) = 0;
	times = HM_FOR_TIMES;	
	loops = atoi(argv[2]);
	jump = atoi(argv[3]);

	for(i=0; i<times; i++)
	{
                gettimeofday(&tv2, &tz2);
                __asm__ __volatile__(
                        ".text\n"
                        ".align 4\n"
                        "3:\n"
                        "       stp %0,%1,[%2]\n"
                        "       add %2, %2, %7\n"
                        "       cmp %2, %3\n"
                        "       bne 3b\n"
                        "       mov %2, %6\n"
                        "       add %4, %4, #1\n"
                        "       cmp %4, %5\n"
                        "       bne 3b\n"
                        :
                        :"r"(0), "r"(0),"r"(map_addr), "r"(map_addr+hm_size), "r"(0), "r"(loops),"r"(map_addr),"r"(jump)
                        :"memory"
                );
                gettimeofday(&tv3, &tz3);
                w_time += HM_DUR(tv2, tv3);
	}

// Print result

	printf("\nTotal mem %dM:\n", (hm_size/1024000)*loops);

	printf("time: %d usec\n", w_time);

	printf("Average write time: %d(us); Average write speed: %.4f Mbps\n", w_time/loops, ((double)(hm_size*8*HM_FOR_TIMES)/w_time)*loops);
}

