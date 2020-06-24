#include <stdio.h>
#include <math.h>

main()
{
	double p, m;
	while (scanf("%d%d", &p, &m) != EOF)
	{
		printf("grade is ... %d\n", 100.0*(0.4*p/60.0+0.6*m/100.0));
	}
	exit(0);
}
