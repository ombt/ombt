#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main(int argc, char **argv)
{
	double delta = atof(argv[1]);
	int nchan = atoi(argv[2]);

	printf("PARAMETER: NUMCH %d\n", nchan);
	printf("PARAMETER: MIN 0.0\n", nchan);
	printf("PARAMETER: MAX %f\n", nchan*delta);
	for (int ichan = 0; ichan < nchan; ichan++)
	{
		printf("DATA: %d %f %f\n", ichan, exp(-1.0*delta*ichan), 0.0);
	}

	exit(0);
}
