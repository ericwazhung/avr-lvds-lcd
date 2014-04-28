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

	//int k;
	//for(k=1; k>=-2; k-=2)
	//{
	int j;
	for(j=0; j<=11; j++)
	{
		int k = (j > 5) ? 11-j : j;

		printf("%d/%d\n", k, 5);

		hfm_setPower(&hfmThing, k*10);

	for(i=0; i<=73; i++)
	{
		int output = hfm_nextOutput(&hfmThing);

		//if(output)
		//	printf("#\n");
		//else
		//	printf("\n");

		//hfm_setPower&hfmThing, i);
		printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
	}
		printf("\n");
	}
	//}

	printf("\n");
}
