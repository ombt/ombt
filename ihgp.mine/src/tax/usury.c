/*
 * program to figure percentages.
 */

#include <math.h>

main()
{
	double interest;
	double payment;
	double principal;
	long number_of_payments;

	printf("enter amount to be borrowed : ");
	scanf("%lf",&principal);

	printf("enter interest per year (percent) : ");
	scanf("%lf",&interest);
	interest=interest/100.0;

	printf("enter number of years (12 payments per year) : ");
	scanf("%ld",&number_of_payments);
	number_of_payments *= 12;

	/* monthly payment */
	interest = interest/12.0;
	payment = principal*interest/
		  (1.0-pow((1.0+interest),-1.0*(double)number_of_payments));

	printf("\nMonthly payment = %7.2lf\n",payment);
	printf("\nTotal payment   = %7.2lf\n",
		(double)number_of_payments*payment);

	/* biweekly payment */
	interest /= 2.0;
	number_of_payments *= 2;
	payment = principal*interest/
		  (1.0-pow((1.0+interest),-1.0*(double)number_of_payments));

	printf("\nBiweekly payment = %7.2lf\n",payment);
	printf("\nTotal payment   = %7.2lf\n",
		(double)number_of_payments*payment);
}
