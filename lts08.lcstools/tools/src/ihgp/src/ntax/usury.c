#include <stdlib.h>
#include <iostream.h>
#include "number.h"
#include "money.h"

// definitions
#define MonthsPerYear 12.0

main(int, char **)
{
	Money principal("Principal", 0.0);
	Money payment("Monthly Payment", 0.0);
	Money totalPayment("Total Payment", 0.0);
	Number interest("Interest (in percentage)", 0.0);
	Number length("Length Of Loan (in years)", 0.0);

	cin >> principal;
	cin >> interest;
	cin >> length;

	length *= MonthsPerYear;
	interest /= 100.0*MonthsPerYear;

	payment = principal*interest/(1.0-pow((1.0+interest), -1.0*length));
	totalPayment = length*payment;

	cout << payment << endl;
	cout << totalPayment << endl;

	return(0);
}
