#include <stdio.h>
#include "/home/amir/AMHM_Approximation_Benchmarks/Test/Test/m5op.h" 
int main()
{
	FILE * fptr;
   	fptr = fopen("/data/users/monazzah/AMHM_approx_bench/Test/output/Output.txt","w");	
	printf("This is the start of test program.\n");
	long int vector[10000];
	uint32_t Reliability_Level = 3;
	printf("Program: Start Address %lx\n",(uint32_t)&(vector[0]));
	printf("Program: End Address %lx\n",(uint32_t) (&vector[9999] + sizeof(long int) - 1));
	printf("Program: Reliability Level Value %lu\n",Reliability_Level);
	m5_add_approx((uint32_t)&(vector[0]), (uint32_t) (&vector[9999] + sizeof(long int) - 1), Reliability_Level);
	if(fptr == NULL)
	   {
	      printf("Error!");   
	      return 1;             
	   }
	vector[0] = 0xCCCCCCCCCCCCCCCC;
	for (int i = 0; i < 9999; i++)
			vector[i+1] = vector [i] + 1;
	for (int i = 0; i < 9999; i++)
		fprintf(fptr,"0x%llx\n",vector[i]);
	fclose(fptr);
	printf("This is the end of test program.\n");
	return 0;
}
