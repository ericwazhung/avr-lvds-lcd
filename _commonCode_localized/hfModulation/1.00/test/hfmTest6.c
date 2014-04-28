#include <stdio.h>
#define TRUE 1
#define FALSE 0
#include "../hfModulation.c"

int main(void)
{
	hfm_t hfmThing;

	hfm_setup(&hfmThing, 30, 50);

	int i;

	printf("Go!\n");
	printf("\n");

	int useSetPower;
	for(useSetPower=0; useSetPower<=1; useSetPower++)
	{
		if(useSetPower)
			printf("Using hfm_setPower on each change:\n");
		else
			printf("Using hfm_setup on each change:\n");


	int j;
	for(j=0; j<10; j++)
	{
		int k = (j > 5) ? 9-j : j;

		printf("%d/%d:", k, 5);

		if(useSetPower)
			hfm_setPower(&hfmThing, k);
		else
			hfm_setup(&hfmThing, k, 5);

	for(i=0; i<=73; i++)
	{
		//int output = hfm_nextOutput(&hfmThing);

		//if(output)
		//	printf("#\n");
		//else
		//	printf("\n");

		//hfm_setPower&hfmThing, i);
		printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
	}
		printf("\n");
	}
	}

	printf("\n");
}
