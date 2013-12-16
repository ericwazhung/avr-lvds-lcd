#include <stdio.h>
#define TRUE 1
#define FALSE 0
#include "../hfModulation.c"

int main(void)
{
	hfm_t hfmThing;

	hfm_setup(&hfmThing, 60, 79);

	int i;

	printf("Go!\n");
	printf("\n");

	for(i=0; i<=79; i++)
	{
		int output = hfm_nextOutput(&hfmThing);

		if(output)
			printf("#\n");
		else
			printf("\n");

		//hfm_setPower&hfmThing, i);
		//printf("%c", (hfm_nextOutput(&hfmThing) ? '#' : ' '));
	}

	printf("\n");
}
