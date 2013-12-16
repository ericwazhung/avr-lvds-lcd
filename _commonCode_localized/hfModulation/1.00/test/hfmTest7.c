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
	for(j=0; j<11; j++)
	{
		int k = (j > 5) ? 10-j : j;

		printf("%d/%d:|", k, 5);

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
		printf("|\n");
	}
	}

	printf("\n");

	printf("Attempting a gradient with realtime hfm_setPower() calls\n");
	hfm_setup(&hfmThing, 0, 79);

	for(i=0; i<=79; i++)
	{
		
		hfm_setPower(&hfmThing, i);
		printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
		//hfm_setPower(&hfmThing, i);
	}
	printf("\n");


	printf("\n");

#define NUMSTEPS	16
	for(useSetPower = 0; useSetPower <= 1; useSetPower++)
	{

		printf("Attempting three short gradients with realtime hfm_setPower() calls\n");
		printf(" This sorta simulates a color-gradient on an LCD that has \n"
			 " a limited number of colors (e.g. 4 shades of red: 0,1,2,3)\n");
		if(useSetPower)
			printf(" Not calling hfmSetup between each gradient\n");
		else
			printf(" Calling hfmSetup between each gradient\n");

		//if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);

		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? '.' : ' '));
			//hfm_setPower(&hfmThing, i);
		}

		//printf("|");
	
		if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);

		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? 'o' : '.'));
			//hfm_setPower(&hfmThing, i);
		}

		//printf("|");
		if(!useSetPower)
			hfm_setup(&hfmThing, 0, NUMSTEPS);
		for(i=0; i<=NUMSTEPS; i++)
		{
		
			hfm_setPower(&hfmThing, i);
			printf("%c", (hfm_nextOutput(&hfmThing) ? 'O' : 'o'));
			//hfm_setPower(&hfmThing, i);
		}



		//printf("|<end\n");
		printf("\n");
	}
}
