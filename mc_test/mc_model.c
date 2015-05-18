#include <stdio.h>
#include <stdlib.h>

int copy[4];
int cur_copy = 0;
int cur_index = 0;
int mc_index[16];
int lookup_index[16];

int ap[4]={100, 200, 300, 500};
int cp[4]={100, 200, 300, 500};
//int ap[4]={200, 300, 400, 600};
//int cp[4]={200, 300, 400, 600};

int max_freq = 900;

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int sum(int a, int b)
{
	return (a+b)> max_freq ? max_freq: (a+b);
}

void gen_lookup_sum()
{
	int i;

	printf("Get the sum from AP & CP\n");

	for (i = 0; i < 16; i++) {
		lookup_index[i] = sum(ap[i>>2], cp[i&0x3]);
		printf("%d: %3d\n", i, lookup_index[i]);
	}
}

void gen_lookup_higher()
{
	int i;

	printf("Get the highest from AP & CP\n");

	for (i = 0; i < 16; i++) {
		lookup_index[i] = max(ap[i>>2], cp[i&0x3]);
		printf("%d: %3d\n", i, lookup_index[i]);
	}
}

void scan_copy()
{
	int hot_copy;
	int i;

	for (i = 0; i < 16; i++) {
		hot_copy = find_copy_for_cp(lookup_index[i]);
		mc_index[i] = hot_copy;
	#ifdef VERBOSE_DEBUG
		printf("index:%d lookup freq:%d hot_copy:%d copy freq:%d\n",
				i, lookup_index[i], hot_copy, copy[hot_copy]);
	#endif
	}
}

void change_copy(int copy_index, int freq)
{
	printf("change %d to %d\n", copy_index, freq);
	copy[copy_index] = freq;
}

/*first find closest and less one*/
/*then find closest and bigger one*/
int find_and_replace_s1(int new_freq)
{
	int i, k = -1, copy_min = 1000;

	for (i = 0; i < 4; i++) {
		if (copy[i] < new_freq &&
			(new_freq - copy[i]) < copy_min) {
				k = i;
				copy_min = new_freq - copy[i];
		}
	}

	if (k >= 0)
		return k;

	for (i = 0; i < 4; i++) {
		if (copy[i] > new_freq &&
				(copy[i] - new_freq) < copy_min) {
			k = i;
			copy_min = copy[i] - new_freq;
		}
	}

	return k;
}

/*first find closest and bigger one*/
/*then find closest and less one*/
int find_and_replace_s2(int new_freq)
{
	int i, k = -1, copy_min = 1000;

	for (i = 0; i < 4; i++) {
		if (copy[i] > new_freq &&
				(copy[i] - new_freq) < copy_min) {
			k = i;
			copy_min = copy[i] - new_freq;
		}
	}

	if (k >= 0)
		return k;

	for (i = 0; i < 4; i++) {
		if (copy[i] < new_freq &&
			(new_freq - copy[i]) < copy_min) {
				k = i;
				copy_min = new_freq - copy[i];
		}
	}

	return k;
}

/*first find the closest and bigger*/
/*then find the biggest*/
int find_copy_for_cp(int new_freq)
{
	int i, k = -1, max, freq_max=0, copy_min = 1000;

	for (i = 0; i < 4; i++) {
		if (copy[i] >= new_freq && (copy[i] - new_freq) < copy_min) {
			k = i;
			copy_min = copy[i] - new_freq;
		}

		if (copy[i] > freq_max) {
			freq_max = copy[i];
			max = i;
		}
	}

	if (k >= 0)
		return k;
	else {
		printf("select the maxium copy for cp!!!!\n");
		return max;
	}

}

#if 0
void fire(int copy_index, int new_index)
{
	/*check if the index has been modified correctly*/
	printf("mc_index[%d]=%d\n", new_index, mc_index[new_index]);
	if(mc_index[new_index] != copy_index)
		printf("FATAL ERROR: copy doesn't hit!\n");


	mc_index[new_index] = copy_index;

	/*write the fire bit and wait for complete*/

	/*********************************************/

	cur_copy = copy_index;
	cur_index = new_index;
	printf("cur_index=%d\n", cur_index);

}
#else
void fire(int new_index)
{
	/*write the fire bit and wait for complete*/

	/*********************************************/

	cur_copy = mc_index[new_index];
	cur_index = new_index;
	printf("cur_index=%d\n", cur_index);

}
#endif

int main()
{
	int i,j;
	int r,hit;
	int new_index, new_freq;
	int hot_copy;
	int request_source; /*1: from AP  0: from CP*/
	unsigned long average=0, average_count=0;

	printf("ap:"); 
	for (i = 0; i < 4; i++)
		printf("%d ", ap[i]); 
	printf("\n"); 
	printf("cp:"); 
	for (i = 0; i < 4; i++)
		printf("%d ", cp[i]); 
	printf("\n\n"); 

	gen_lookup_sum();

	copy[0] = 200;
	copy[1] = 400;
	copy[2] = 600;
	copy[3] = 800;
	cur_index = 0;
	cur_copy = 0;

	scan_copy();

	srand((unsigned)time(NULL));
	while(1)
	{
		printf("\nfreq in copy: %d %d %d %d: ###%d### used\n",
				copy[0], copy[1], copy[2], copy[3], copy[cur_copy]);

		/*Calculate average frequency*/
		average+=copy[cur_copy]/100;
		average_count++;
		printf("%ld:average_freq=%ld\n", average_count, average*100/average_count);

		//sleep(1);

		/*Generate a random number: bit 2 indicates request source*/
		/*bit1-bit0 indicates the request index*/
		r = 8 * (rand() / (RAND_MAX + 1.0));

		request_source = r >> 2;

		if(request_source) {
			new_index = (r&0x3) << 2  | (cur_index&0x3);
			printf("###AP VOTE###: %d request new index: %d\n", r&0x3, new_index);
			printf("###AP VOTE###: ");
		} else {
			new_index = (r&0x3) | (cur_index&0xc);
			printf("###CP VOTE###: %d request new index: %d\n", r&0x3, new_index);
			printf("###CP VOTE###: ");
		}
		
		hit = 0;
		new_freq = lookup_index[new_index];
		printf("request new freq = %d\n", new_freq);

		for (i = 0; i < 4; i++) {
			if (copy[i] == new_freq)
			{
				if (i == cur_copy) {
					printf("already use the copy. ignore the request\n");
					hit = 1;
					break;
				}
				printf("hit, fire\n");
#if 0
				fire(i, new_index);
#else
				fire(new_index);
#endif
				hit = 1;
				break;
			}
		}

		if (hit)
			continue;

		if (!request_source) {
#if 0
			hot_copy = find_copy_for_cp(new_freq);
			printf("hot_copy is %d\n", hot_copy);
			if(hot_copy!=mc_index[new_index]) {
				printf("error happen\n");
				return -1;
			}
			//printf("mc_index[%d] = %d\n", new_index, mc_index[new_index]);
			if (hot_copy >= 0)
				fire(hot_copy, new_index);
			else {
				printf("can't find hot_copy for cp\n");
				return -1;
			}
#else
			fire(new_index);
#endif
			continue;
		}

		if (i == 4) {
			printf("look for copy for replacement\n");
			/*****************************/
			/*start copy replace operation*/
			/*****************************/
			hot_copy = find_and_replace_s2(new_freq);
			if (hot_copy >= 0) {
				if (hot_copy == cur_copy) {
					printf("hot_copy in use\n");
					if (hot_copy != 3)
						hot_copy += 1;
					else
						hot_copy = 2;
				}
				change_copy(hot_copy, new_freq);
				scan_copy();
				/*todo:change the ddr parameters to fit the new freq*/

				printf("hot_copy is %d, fire\n", hot_copy);
#if 0
				mc_index[new_index] = hot_copy;
				fire(hot_copy, new_index);
#else
				fire(new_index);
#endif

			} else {
				printf("can't find hot_copy for ap\n");
				return -1;
			}
		}
	}

	return 0;
}

