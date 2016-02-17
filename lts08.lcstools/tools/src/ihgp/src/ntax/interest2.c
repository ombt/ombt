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
	Number interest("Interest (in percentage)", 0.0);
	Money interestpaid("Interest Paid This Year", 0.0);
	Number length("Length Of Loan (in years)", 0.0);
	Number current("Current Year", 0.0);

	cout << endl;
	cout << "ENTER DATA ..." << endl;

	cin >> principal;
	cin >> interest;
	cin >> length;
	cin >> current;

	length *= MonthsPerYear;
	interest /= 100.0*MonthsPerYear;
	payment = principal*interest/(1.0-pow((1.0+interest), -1.0*length));

	Number start = (current-1.0)*MonthsPerYear;
	Number end = current*MonthsPerYear;

	if (start < 0.0)
		start = 0.0;

	Money startp("Principal at Start of Year", 0.0);
	startp = principal*pow((1.0+interest), start) -
		 payment*(-1.0+pow((1.0+interest), start))/interest;
	Money endp("Principal at End of Year", 0.0);
	endp = principal*pow((1.0+interest), end) -
		 payment*(-1.0+pow((1.0+interest), end))/interest;

	interestpaid = MonthsPerYear*payment - (startp - endp);

	Money totalInterest("Total Interest Paid", 0.0);
	totalInterest = length*payment - principal;

	Money total("Total Amount Paid", 0.0);
	total = length*payment;

	cout << endl;
	cout << "PRINT RESULTS ..." << endl;

	cout << principal << endl;
	cout << totalInterest << endl;
	cout << total << endl;
	cout << endl;

	cout << payment << endl;
	cout << endl;

	cout << startp << endl;
	cout << endp << endl;
	cout << interestpaid << endl;
	cout << endl;

	return(0);
}
