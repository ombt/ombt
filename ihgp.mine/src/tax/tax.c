/* figure out taxes for a year */

/* unix headers */
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <values.h>
#include <errno.h>

/* definitions */
#define NOTOK 0
#define OK 1
#define ONEITEM 1
#define TWOITEMS 2
#define ERROR(errmsg, errval) \
	fprintf(stderr, "%s'%d: %s, errno = %d\n", \
	__FILE__, __LINE__, errmsg, errval)
#define FEDERALEXEMPTION 2450.00
#define STATEEXEMPTION 1000.00
#define SINGLEDEDUCTION 3600.00
#define STATEREALESTATEDEDUCTION 0.05
#define MONTHSPERYEAR 12.0
#define BALANCE(p, i, pv, k) \
	(((p)*(pow(1.0+(i),-1.0*(k))-1.0)/(i)+(pv))/pow(1.0+(i),-1.0*(k)))

/* enumerations */
enum DEDUCTIONTYPE { UNKNOWN, STANDARD, ITEMIZED } ;

/* structure for taxes and income */
struct INCOME {
	double wages;
	double interest;
	double total;
	double adjusted_total;
	double federaltaxable;
	double statetaxable;
	double aftertaxes;
} ;

struct HOUSE {
	double assessed;
	double taxrate;
	double percentdown;
	double mortgage;
	double years;
	double interest;
	int start;
	int end;
	double interestpaid;
	double realestatetax;
	double payment;
} ;

struct TAXES {
	double local;
	double state;
	double federal;
	double fica;
} ;

struct DEDUCTIONS {
	struct {
		int exemptions;
		enum DEDUCTIONTYPE type;
		double homeinterest;
		double points;
		double realestatetax;
		double stateincometax;
		double localincometax;
		double total;
	} federal;
	struct {
		int exemptions;
		double realestatetax;
	} state;
} ;

struct ADJUSTMENTS {
	double percent_401k;
	double total_401k;
} ;

/* tax rates tables */
struct TAXRATES {
	double minimum;
	double maximum;
	double rate;
} ;
	
struct TAXRATES federalrates[] = {
	{ 0.0, 24100.0, 0.15 },
	{ 24100.0, 50325.0, 0.28 },
	{ 50325.0, 116350.0, 0.31 },
	{ 116350.0, 251350.0, 0.36 },
	{ 251350.0, MAXFLOAT, 0.396 }
} ;

struct TAXRATES ssrates[] = {
	{ 0.0, 60600.0, 0.0765 },
	{ 60600.0, MAXFLOAT, 0.0145 }
} ;

#ifdef NEWJERSEY
struct TAXRATES staterates[] = {
	{ 0.0, 20000.0, 0.020 },
	{ 20000.0, 50000.0, 0.025 },
	{ 50000.0, 70000.0, 0.035 },
	{ 70000.0, 80000.0, 0.050 },
	{ 80000.0, 150000.0, 0.065 },
	{ 150000.0, MAXFLOAT, 0.070 }
} ;
#else
struct TAXRATES staterates[] = {
	{ 0.0, MAXFLOAT, 0.03 }
} ;
#endif

/* get income from user */
int
getincome(income)
struct INCOME *income;
{
	/* get wages from user */
	fprintf(stdout, "\n<<< INCOME >>>\n");
	fprintf(stdout, "Enter wages : ");
	if (fscanf(stdin, "%lf", &income->wages) != ONEITEM)
	{
		ERROR("reading of wages failed.", errno);
		return(NOTOK);
	}

	/* get interest from user */
	fprintf(stdout, "Enter interest income : ");
	if (fscanf(stdin, "%lf", &income->interest) != ONEITEM)
	{
		ERROR("reading of interest failed.", errno);
		return(NOTOK);
	}

	/* total income */
	income->total = income->wages + income->interest;

	/* all done */
	return(OK);
}

/* get income adjustments from user */
int
getadjustments(income, adjustments)
struct INCOME *income;
struct ADJUSTMENTS *adjustments;
{
	/* get adjustments from user */
	fprintf(stdout, "\n<<< ADJUSTMENTS >>>\n");
	fprintf(stdout, "Enter 401K deductions (percent) : ");
	if (fscanf(stdin, "%lf", &adjustments->percent_401k) != ONEITEM)
	{
		ERROR("reading of 401K percent failed.", errno);
		return(NOTOK);
	}

	/* adjusted income */
	adjustments->total_401k = adjustments->percent_401k*income->wages/100.0;
	income->adjusted_total = income->total - adjustments->total_401k;

	/* all done */
	return(OK);
}

/* get data for house */
int
realestate(house)
struct HOUSE *house;
{
	char answer[16];
	int year;
	double payment;
	double numberofpayments;
	double periodinterest;

	/* get house assessed value */
	fprintf(stdout, "\n<<< REAL ESTATE >>>\n");
	fprintf(stdout, "Enter house assessed value : ");
	if (fscanf(stdin, "%lf", &house->assessed) != ONEITEM)
	{
		ERROR("reading of assessed value failed.", errno);
		return(NOTOK);
	}
	if (house->assessed == 0) return(OK);

	/* get real estate tax rate */
	fprintf(stdout, "Enter real estate tax rate (%%) : ");
	if (fscanf(stdin, "%lf", &house->taxrate) != ONEITEM)
	{
		ERROR("reading of tax rate failed.", errno);
		return(NOTOK);
	}
	house->taxrate /= 100.0;

	/* get mortgage amount */
	fprintf(stdout, "Enter [m]ortgage, [p]ercent down or money [d]own : ");
	if (fscanf(stdin, "%s", answer) != ONEITEM)
	{
		ERROR("reading of amount of mortgage failed.", errno);
		return(NOTOK);
	}
	switch (*answer)
	{
	case 'd':
	case 'D':
		fprintf(stdout, "Enter money down : ");
		if (fscanf(stdin, "%lf", &house->percentdown) != ONEITEM)
		{
			ERROR("reading of money down failed.", errno);
			return(NOTOK);
		}
		house->percentdown =
			(house->percentdown/house->assessed)*100.0;
		house->mortgage = 
			house->assessed*(1.0 - house->percentdown/100.0);
		break;
	case 'p':
	case 'P':
		fprintf(stdout, "Enter percent down : ");
		if (fscanf(stdin, "%lf", &house->percentdown) != ONEITEM)
		{
			ERROR("reading of percent down failed.", errno);
			return(NOTOK);
		}
		house->mortgage = 
			house->assessed*(1.0 - house->percentdown/100.0);
		break;
	case 'm':
	case 'M':
		fprintf(stdout, "Enter mortgage amount : ");
		if (fscanf(stdin, "%lf", &house->mortgage) != ONEITEM)
		{
			ERROR("reading of mortgage failed.", errno);
			return(NOTOK);
		}
		break;
	default:
		ERROR("invalid type of mortgage.", errno);
		return(NOTOK);
	}

	/* get term of mortgage */
	fprintf(stdout, "Enter length of mortgage (years) : ");
	if (fscanf(stdin, "%lf", &house->years) != ONEITEM)
	{
		ERROR("reading of mortgage term failed.", errno);
		return(NOTOK);
	}

	/* get interest of mortgage */
	fprintf(stdout, "Enter mortgage interest (%%) : ");
	if (fscanf(stdin, "%lf", &house->interest) != ONEITEM)
	{
		ERROR("reading of mortgage interest failed.", errno);
		return(NOTOK);
	}
	house->interest /= 100.0;

	/* get start and end periods to calculate total interest and taxes */
	fprintf(stdout, "Enter year for calculation : ");
	if (fscanf(stdin, "%d", &year) != ONEITEM)
	{
		ERROR("reading of year failed.", errno);
		return(NOTOK);
	}
	house->start = MONTHSPERYEAR*(year-1)+1;
	house->end = house->start + MONTHSPERYEAR - 1;

	/* calculate interest data */
	numberofpayments = house->years*MONTHSPERYEAR;
	periodinterest = house->interest/MONTHSPERYEAR;
	payment = house->mortgage*periodinterest;
	payment = payment/(1.0-pow((1.0+periodinterest),-1.0*numberofpayments));
	house->payment = payment;

	/* store the data */
	house->interestpaid = 
		BALANCE(payment, periodinterest, house->mortgage, house->end) -
		BALANCE(payment, periodinterest, house->mortgage, 
			house->start - 1.0) +
		(house->end - house->start - 1.0)*payment;
	house->realestatetax = house->assessed*house->taxrate;

	/* all done */
	return(OK);
}

/* calculate the state taxable income */
int
statetaxableincome(income, deductions)
struct INCOME *income;
struct DEDUCTIONS *deductions;
{
	/* calculate state taxable income */
	income->statetaxable = income->adjusted_total;

	/* subtract exemptions */
	income->statetaxable -= deductions->state.exemptions*STATEEXEMPTION;

	/* subtract state real estate taxes */
	income->statetaxable -= 
		STATEREALESTATEDEDUCTION*deductions->state.realestatetax;

	/* all done */
	return(OK);
}

/* get state deductions from user */
int
statedeductions(deductions, house)
struct DEDUCTIONS *deductions;
struct HOUSE *house;
{
	char answer[64];

	/* get number of exemptions */
	fprintf(stdout, "\n<<< STATE DEDUCTIONS >>>\n");
	fprintf(stdout, "Enter number of exemptions : ");
	if (fscanf(stdin, "%d", &deductions->state.exemptions) != ONEITEM)
	{
		ERROR("reading of exemptions failed.", errno);
		return(NOTOK);
	}

	/* get real estate taxes paid */
	fprintf(stdout, "Real estate taxes : %0.2lf*%0.2lf = %0.2lf\n", 
		STATEREALESTATEDEDUCTION, house->realestatetax, 
		STATEREALESTATEDEDUCTION*house->realestatetax);
	deductions->state.realestatetax = house->realestatetax;

	/* all done */
	return(OK);
}

/* calculate state taxes */
int
statetaxes(income, deductions, taxes)
struct INCOME *income;
struct DEDUCTIONS *deductions;
struct TAXES *taxes;
{
	int irate;
	double diff;

	/* state taxes */
	irate = -1;
	do {
		irate += 1;
		diff = staterates[irate].maximum - income->statetaxable;
		if (diff < 0.0)
		{
			taxes->state += staterates[irate].rate*
			(staterates[irate].maximum - staterates[irate].minimum);
		}
		else
		{
			taxes->state += staterates[irate].rate*
			(income->statetaxable - staterates[irate].minimum);
		}
	} while ( ! (staterates[irate].minimum < income->statetaxable && 
		     income->statetaxable <= staterates[irate].maximum)) ;

	/* all done */
	return(OK);
}

/* get federal deductions from user */
int
federaldeductions(deductions, house, taxes)
struct DEDUCTIONS *deductions;
struct HOUSE *house;
struct TAXES *taxes;
{
	char answer[64];

	/* get number of exemptions from user */
	fprintf(stdout, "\n<<< FEDERAL DEDUCTIONS >>>\n");
	fprintf(stdout, "Enter number of exemptions : ");
	if (fscanf(stdin, "%d", &deductions->federal.exemptions) != ONEITEM)
	{
		ERROR("reading of exemptions failed.", errno);
		return(NOTOK);
	}

	/* standard or itemized deductions */
	fprintf(stdout, "Type of deduction : [s]tandard or [i]temized ");
	if (fscanf(stdin, "%s", answer) != ONEITEM)
	{
		ERROR("reading of deduction type failed.", errno);
		return(NOTOK);
	}
	if (*answer == 's' || *answer == 'S')
	{
		/* standard deduction */
		deductions->federal.type = STANDARD;
		deductions->federal.total = SINGLEDEDUCTION;
		/* all done */
		return(OK);
	}
	else if (*answer == 'i' || *answer == 'I')
	{
		/* itemized deduction */
		deductions->federal.type = ITEMIZED;
	}
	else
	{
		ERROR("wrong deduction type.", EINVAL);
		return(NOTOK);
	}

	/* get home interest */
	fprintf(stdout, "Interest paid : %8.2lf\n", house->interestpaid);
	deductions->federal.homeinterest = house->interestpaid;
	deductions->federal.total += deductions->federal.homeinterest;

	/* read in points */
	fprintf(stdout, "Enter amount of points : ");
	if (fscanf(stdin, "%lf", &deductions->federal.points) != ONEITEM)
	{
		ERROR("reading of points failed.", errno);
		return(NOTOK);
	}
	deductions->federal.total += deductions->federal.points;

	/* read in real estate tax */
	fprintf(stdout, "Real estate tax paid : %8.2lf\n", house->realestatetax);
	deductions->federal.realestatetax = house->realestatetax;
	deductions->federal.total += deductions->federal.realestatetax;

#ifdef DEDUCTINCOMETAX
	/* read in state income tax */
	fprintf(stdout, "State income tax paid : %8.2lf\n", taxes->state);
	deductions->federal.stateincometax = taxes->state;
	deductions->federal.total += deductions->federal.stateincometax;

	/* read in local income tax */
	fprintf(stdout, "Enter amount of local income tax : ");
	if (fscanf(stdin, "%lf", &deductions->federal.localincometax) != ONEITEM)
	{
		ERROR("reading of local income failed.", errno);
		return(NOTOK);
	}
	deductions->federal.total += deductions->federal.localincometax;
#else
	deductions->federal.stateincometax = 0;
	deductions->federal.localincometax = 0;
#endif

	/* all done */
	return(OK);
}

/* calculate the federal taxable income */
int
federaltaxableincome(income, deductions)
struct INCOME *income;
struct DEDUCTIONS *deductions;
{
	/* calculate federal taxable income */
	income->federaltaxable = income->adjusted_total;

	/* subtract exemptions */
	income->federaltaxable -= 
		deductions->federal.exemptions*FEDERALEXEMPTION;

	/* subtract deductions */
	switch (deductions->federal.type)
	{
	case STANDARD:
	case ITEMIZED:
		/* standard or itemized deduction type */
		income->federaltaxable -= deductions->federal.total;
		break;
	default:
		/* wrong deduction type */
		ERROR("wrong deduction type.", EINVAL);
		return(NOTOK);
	}

	/* all done */
	return(OK);
}

/* calculate federal taxes */
int
federaltaxes(income, deductions, taxes)
struct INCOME *income;
struct DEDUCTIONS *deductions;
struct TAXES *taxes;
{
	int irate;
	double diff;

	/* FICA */
	irate = -1;
	do {
		irate += 1;
		diff = ssrates[irate].maximum - income->adjusted_total;
		if (diff < 0.0)
		{
			taxes->fica += ssrates[irate].rate*
			(ssrates[irate].maximum - ssrates[irate].minimum);
		}
		else
		{
			taxes->fica += ssrates[irate].rate*
			(income->adjusted_total - ssrates[irate].minimum);
		}
	} while ( ! (ssrates[irate].minimum < income->adjusted_total && 
		     income->adjusted_total <= ssrates[irate].maximum)) ;

	/* federal taxes */
	irate = -1;
	do {
		irate += 1;
		diff = federalrates[irate].maximum - income->federaltaxable;
		if (diff < 0.0)
		{
			taxes->federal += federalrates[irate].rate*
			(federalrates[irate].maximum - federalrates[irate].minimum);
		}
		else
		{
			taxes->federal += federalrates[irate].rate*
			(income->federaltaxable - federalrates[irate].minimum);
		}
	} while ( ! (federalrates[irate].minimum < income->federaltaxable && 
		     income->federaltaxable <= federalrates[irate].maximum)) ;

	/* all done */
	return(OK);
}
/* print taxes */
int
printtaxes(income, deductions, adjustments, taxes, house)
struct INCOME *income;
struct DEDUCTIONS *deductions;
struct ADJUSTMENTS *adjustments;
struct TAXES *taxes;
struct HOUSE *house;
{
	fprintf(stdout, "\n<<< INCOME >>>\n");
	fprintf(stdout, "Total             : %8.2lf\n", 
		income->total);
	fprintf(stdout, "Adjusted Total    : %8.2lf\n", 
		income->adjusted_total);
	fprintf(stdout, "Wages             : %8.2lf\n", 
		income->wages);
	fprintf(stdout, "Interest          : %8.2lf\n", 
		income->interest);
	fprintf(stdout, "401K Adjustment   : %8.2lf\n", 
		adjustments->total_401k);
	fprintf(stdout, "Taxable (state)   : %8.2lf\n", 
		income->statetaxable);
	fprintf(stdout, "Taxable (federal) : %8.2lf\n", 
		income->federaltaxable);

	fprintf(stdout, "\n<<< REAL ESTATE >>>\n");
	fprintf(stdout, "Assessed     : %9.2lf\n", 
		house->assessed);
	fprintf(stdout, "Tax rate (%%) : %9.2lf%%\n", 
		house->taxrate*100.0);
	fprintf(stdout, "Mortgage     : %9.2lf\n", 
		house->mortgage);
	fprintf(stdout, "Years        : %9.2lf\n", 
		house->years);
	fprintf(stdout, "Interest (%%) : %9.2lf%%\n", 
		house->interest*100.0);
	fprintf(stdout, "Start, End   : %d, %d\n", 
		house->start, house->end);

	fprintf(stdout, "\n<<< FEDERAL DEDUCTIONS >>>\n");
	if (deductions->federal.type == STANDARD)
	{
		fprintf(stdout, "Exemptions : %d\n", 
			deductions->federal.exemptions);
		fprintf(stdout, "Type       : STANDARD\n");
		fprintf(stdout, "Total      : %8.2lf\n", 
			deductions->federal.total);
	}
	else
	{
		fprintf(stdout, "Exemptions       : %d\n", 
			deductions->federal.exemptions);
		fprintf(stdout, "Type             : ITEMIZED\n");
		fprintf(stdout, "Home interest    : %8.2lf\n", 
			deductions->federal.homeinterest);
		fprintf(stdout, "Points           : %8.2lf\n", 
			deductions->federal.points);
		fprintf(stdout, "Real estate tax  : %8.2lf\n", 
			deductions->federal.realestatetax);
#ifdef DEDUCTINCOMETAX
		fprintf(stdout, "State income tax : %8.2lf\n", 
			deductions->federal.stateincometax);
		fprintf(stdout, "Local income tax : %8.2lf\n", 
			deductions->federal.localincometax);
#endif
		fprintf(stdout, "Total            : %8.2lf\n", 
			deductions->federal.total);
	}

	fprintf(stdout, "\n<<< STATE DEDUCTIONS >>>\n");
	fprintf(stdout, "Exemptions      : %d\n", 
		deductions->state.exemptions);
	fprintf(stdout, "Real estate tax : %0.2lf*%0.2lf = %0.2lf\n", 
		STATEREALESTATEDEDUCTION, deductions->state.realestatetax, 
		STATEREALESTATEDEDUCTION*deductions->state.realestatetax);

	fprintf(stdout, "\n<<< TAXES >>>\n");
	fprintf(stdout, "Federal : %8.2lf\n", 
		taxes->federal);
	fprintf(stdout, "FICA    : %8.2lf\n", 
		taxes->fica);
	fprintf(stdout, "State   : %8.2lf\n", 
		taxes->state);
	fprintf(stdout, "Local   : %8.2lf\n", 
		taxes->local);

	fprintf(stdout, "\n<<< AFTER TAXES INCOME >>>\n");
	income->aftertaxes = income->adjusted_total - 
		taxes->federal - taxes->fica - taxes->state - taxes->local;
	fprintf(stdout, "Total income   : %8.2lf\n", 
		income->aftertaxes);
	fprintf(stdout, "Monthly income : %8.2lf\n", 
		income->aftertaxes/MONTHSPERYEAR);
	fprintf(stdout, "Percent Tax    : %8.2lf %%\n", 
		100.0*(1.0 - income->aftertaxes/income->adjusted_total));

	fprintf(stdout, "\n<<< REAL ESTATE PAYMENTS >>>\n");
	fprintf(stdout, "Interest paid   : %8.2lf\n", 
		house->interestpaid);
	fprintf(stdout, "Real estate tax : %8.2lf\n", 
		house->realestatetax);
	fprintf(stdout, "Payment         : %8.2lf\n", 
		house->payment);
	fprintf(stdout, "Percent of after tax income      : %8.2lf %%\n", 
		100.0*(MONTHSPERYEAR*house->payment)/income->aftertaxes);
	fprintf(stdout, "Percent of gross income          : %8.2lf %%\n", 
		100.0*(MONTHSPERYEAR*house->payment)/income->adjusted_total);
	fprintf(stdout, "Payment + Tax                    : %8.2lf\n", 
		house->payment + house->realestatetax/MONTHSPERYEAR);
	fprintf(stdout, "After tax income - payment - tax : %8.2lf\n", 
		income->aftertaxes/MONTHSPERYEAR - house->payment -
		house->realestatetax/MONTHSPERYEAR);
	fprintf(stdout, "Percent of after tax income      : %8.2lf %%\n", 
		100.0*(house->payment*MONTHSPERYEAR + house->realestatetax)/
		income->aftertaxes);
	fprintf(stdout, "Percent of gross income          : %8.2lf %%\n", 
		100.0*(house->payment*MONTHSPERYEAR + house->realestatetax)/
		income->adjusted_total);
	fprintf(stdout, "Years                            : %8.2lf\n", 
		house->years);

	/* all done */
	return(OK);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	struct TAXES taxes;
	struct HOUSE house;
	struct INCOME income;
	struct DEDUCTIONS deductions;
	struct ADJUSTMENTS adjustments;

	/* initialize to zero */
	(void) memset((char *) &income, NULL, sizeof(struct INCOME));
	(void) memset((char *) &house, NULL, sizeof(struct HOUSE));
	(void) memset((char *) &taxes, NULL, sizeof(struct TAXES));
	(void) memset((char *) &deductions, NULL, sizeof(struct DEDUCTIONS));
	(void) memset((char *) &adjustments, NULL, sizeof(struct ADJUSTMENTS));

	/* get user's income */
	if (getincome(&income) != OK)
	{
		fprintf(stderr, "getincome failed.\n");
		exit(2);
	}

	/* get adjustments to income */
	if (getadjustments(&income, &adjustments) != OK)
	{
		fprintf(stderr, "getincome failed.\n");
		exit(2);
	}

	/* get real estate data */
	if (realestate(&house) != OK)
	{
		fprintf(stderr, "realestate failed.\n");
		exit(2);
	}

	/* get state deductions */
	if (statedeductions(&deductions, &house) != OK)
	{
		fprintf(stderr, "statedeductions failed.\n");
		exit(2);
	}

	/* state taxable income */
	if (statetaxableincome(&income, &deductions) != OK)
	{
		fprintf(stderr, "statetaxableincome failed.\n");
		exit(2);
	}

	/* state taxes */
	if (statetaxes(&income, &deductions, &taxes) != OK)
	{
		fprintf(stderr, "statetaxes failed.\n");
		exit(2);
	}

	/* get federal deductions */
	if (federaldeductions(&deductions, &house, &taxes) != OK)
	{
		fprintf(stderr, "federaldeductions failed.\n");
		exit(2);
	}

	/* federal taxable income */
	if (federaltaxableincome(&income, &deductions) != OK)
	{
		fprintf(stderr, "federaltaxableincome failed.\n");
		exit(2);
	}

	/* federal taxes */
	if (federaltaxes(&income, &deductions, &taxes) != OK)
	{
		fprintf(stderr, "federaltaxes failed.\n");
		exit(2);
	}

	/* print user's taxes */
	if (printtaxes(&income, &deductions, &adjustments, &taxes, &house) != OK)
	{
		fprintf(stderr, "printtaxes failed.\n");
		exit(2);
	}

	/* all done */
	exit(0);
}
