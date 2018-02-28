#include <stdio.h>
//#include <unistd.h>
#include <sys/time.h>
//#include <sched.h>
//#include <asm/system.h>

#define HM_SIZE (1024*1024*8)
#define HM_TIMES 2 
#define HM_US_PER_S 1000000
#define HM_CONTINUE_TIMES (64*8)

#define HM_DUR(tv0, tv1) (((tv1.tv_sec-tv0.tv_sec)*HM_US_PER_S + tv1.tv_usec-tv0.tv_usec))

int k;
int main(void)
{

        {
                struct timeval tv0, tv1, tv2, tv3;
                struct timezone tz0, tz1, tz2, tz3;
                int i,j;


                gettimeofday(&tv0, &tz0);
/*
                for(i=0;i<0x3ff;i++)
                        for (j=0;j<0xfffff;j++)
				k+=(i+j);
*/
                __asm__ __volatile__(
                        ".text\n"
                        ".align 4\n"
                        "1:\n" 
                        "       add %0, %0, #1\n"
                        "       cmp %0, %1\n"
                        "       bne 1b\n"
                        : 
                        :"r"(0), "r"(0x3fffffff)
                );

                gettimeofday(&tv1, &tz1);

                printf("wait %lu(us)\n" , HM_DUR(tv0, tv1));
        }


	int i = 0;
	int times = HM_TIMES;
	unsigned long m_time = 0;
	unsigned long w_time = 0;
	unsigned long r_time = 0;
	unsigned long map_addr = (int *)malloc(HM_SIZE);
	memset((void*)map_addr, 0, HM_SIZE);

	for(i=0; i<times; i++)
	{
        	struct timeval tv0, tv1, tv2, tv3;
        	struct timezone tz0, tz1, tz2, tz3;
	        int getmem;
		int setmem;
		int tmp=0;

// Read SDRAM
	        gettimeofday(&tv1, &tz1);
		__asm__ __volatile__(
			".text\n"
			".align 4\n"
			"1:\n" 
			"	ldp %0, %1, [%2]\n"
			"	add %2, %2, #16\n"
			"	cmp %2, %3\n"
			"	bne 1b\n"
			"	mov %2, %6\n"
			"	add %4, %4, #1\n"
			"	cmp %4, %5\n"
			"	bne 1b\n"
			: 
			:"r"(0), "r"(0), "r"(map_addr),"r"(map_addr+HM_SIZE), "r"(0),"r"(HM_CONTINUE_TIMES),"r"(map_addr)
		);
	        gettimeofday(&tv2, &tz2);
		r_time += HM_DUR(tv1, tv2);

// Write SDRAM

	        gettimeofday(&tv2, &tz2);
                __asm__ __volatile__(
                        ".text\n"
                        ".align 4\n"
			"3:\n" 
                        "       stp %0,%1,[%2]\n"
                        "       add %2, %2, #16\n"
			"	cmp %2, %3\n"
			"	bne 3b\n"
			"	mov %2, %6\n"
                        "       add %4, %4, #1\n"
			"	cmp %4, %5\n"
			"	bne 3b\n"
			:
			:"r"(0), "r"(0),"r"(map_addr), "r"(map_addr+HM_SIZE), "r"(0), "r"(HM_CONTINUE_TIMES),"r"(map_addr)
			:"memory" 
                );

		gettimeofday(&tv3, &tz3);
		w_time += HM_DUR(tv2, tv3);
	}

// Print result

	printf("\nTotal mem %dM:\n", (HM_SIZE/1000000)*HM_CONTINUE_TIMES);

	printf("time: %.4f\n", r_time);

	printf("Average read time: %d(us); Average read speed: %.4f Mbps\n\n", r_time/HM_CONTINUE_TIMES, ((double)(HM_SIZE*8*HM_TIMES)/r_time)*HM_CONTINUE_TIMES);
	printf("Average write time: %d(us); Average write speed: %.4f Mbps\n", w_time/HM_CONTINUE_TIMES, ((double)(HM_SIZE*8*HM_TIMES)/w_time)*HM_CONTINUE_TIMES);
}

