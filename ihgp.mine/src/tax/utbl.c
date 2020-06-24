// program to figure percentages.

#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include <math.h>

static int lnno = 0;

void
printit(double principal, double interest, long years, double payment, 
	double total_payment)
{
	if (++lnno%50 == 1) 
	{
		if (lnno != 1) cout << "\014" << endl;
		cout << "principal interest years  payment total_payment" << endl;
	}
	cout.width(9);
	cout << principal << " ";
	cout.width(8);
	cout << interest << " ";
	cout.width(5);
	cout << years << " ";
	cout.width(8);
	cout << payment << " ";
	cout.width(13);
	cout << total_payment << endl;
	return;
}

main()
{
	double min_principal, max_principal, delta_principal;
	if (isatty(0)) cerr << "enter minimum amount to be borrowed: ";
	cin >> min_principal;
	if (isatty(0)) cerr << "enter maximum amount to be borrowed: ";
	cin >> max_principal;
	if (isatty(0)) cerr << "enter delta for principal: ";
	cin >> delta_principal;

	double min_interest, max_interest, delta_interest;
	if (isatty(0)) cerr << "enter minimum interest: ";
	cin >> min_interest;
	if (isatty(0)) cerr << "enter maximum interest: ";
	cin >> max_interest;
	if (isatty(0)) cerr << "enter delta for interest: ";
	cin >> delta_interest;

	long min_years, max_years, delta_years;
	if (isatty(0)) cerr << "enter minimum years: ";
	cin >> min_years;
	if (isatty(0)) cerr << "enter maximum years: ";
	cin >> max_years;
	if (isatty(0)) cerr << "enter delta for years: ";
	cin >> delta_years;

	for (double principal = min_principal; 
	     principal <= max_principal; 
	     principal += delta_principal)
	{
		for (double interest = min_interest; 
		     interest <= max_interest; 
		     interest += delta_interest)
		{
			for (long years = min_years; 
			     years <= max_years; 
			     years += delta_years)
			{
				double p;
				double i = interest/(12.0*100.0);
				double np = years*12.0;
				p = principal*i/(1.0-pow((1.0+i),-1.0*np));
				printit(principal, interest, years, p, p*np);
			}
		}
	}
	return(0);
}
