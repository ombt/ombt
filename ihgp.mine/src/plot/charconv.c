/*
 * this file contains macros and functions for converting ascii
 * to binary (long, double), and vise-versa.
 */

#include <stdio.h>

/*
 * some basic macros.
 */
#define is_digit(x) ((((x) >= '0') && ((x) <= '9')) ? 1 : 0)
#define is_lowercase(x) ((((x) >= 'a') && ((x) <= 'z')) ? 1 : 0)
#define is_uppercase(x) ((((x) >= 'A') && ((x) <= 'Z')) ? 1 : 0)
#define is_blank(x) (((x) == ' ') ? 1 : 0)

#define to_uppercase(x) ((is_lowercase(x)) ? ((x) - 'a' + 'A') : (x))
#define to_lowercase(x) ((is_uppercase(x)) ? ((x) + 'a' - 'A') : (x))
#define to_decimal(x) ((x) - '0')
#define to_ascii(x) ((((x) >= 0) && ((x) <= 9)) ? ((x) + '0') : -1)

#define ASCIINULL (char) '\0'
#define NDIG 80
#define NULLADDR ((char *) 0L)
#define MAXSIGDIG 14	/* maximum significant digits for E format */
#define MAXDIG 60	/* maximum digits for F format */
#define MAXEXPDIG 2	/* maximum digits in exponent, E format */
#define MAXLONGDIG 10	/* maximum digits in long integer */

#define GOODCONV 1L
#define BADCONV 2L
#define BADFORMAT 4L

/*
 * string buffer for binary to ascii conversions.
 */
static char buffer[NDIG];

/*
 ************************************************************************
 *
 * the following 4 functions are base functions for converting
 * ascii to binary (long, double), and vice-versa. 
 *
 ***********************************************************************
 */

/*
 * function to convert ascii to long.
 */
long a2l(string, status)

char string[];	/* ascii string to convert */
long *status;	/* final status of conversion */
{
    long sign, number;

    number = 0;
    sign = 1;
    while (is_blank(*string))
    {
	string++;
    }
    if (*string == ASCIINULL)
    {
	*status = GOODCONV;	/* blank string, return zero */
	return (0L);
    }
    if ((*string == '-') || (*string == '+'))
    {
	sign = ((*string == '-') ? -1 : 1);
	string++;
    }
    while (*string != ASCIINULL)
    {
 	if (is_digit(*string))
	{
	    number = 10*number + to_decimal(*string);
	    string++;
	}
	else
	{
	    *status = BADCONV;
	    return (0L);
	}
    }
    *status = GOODCONV;
    return (sign * number);
}

/*
 * this function converts ascii to double. the ascii can have
 * either Fortran F or E format.
 *
 * F -> +/-NNNN.DDDD
 * E -> +/-NNNN.DDDDe+/-MMM
 *
 * where NNNN is an integer, DDDD is the decimal fraction, and
 * MMM is an integer which is interpreted as 10**MMM.
 *
 * the number is converted by breaking it down to parts:
 *
 *  nsign * [(N*10**0 + N*10**1 + ...) + 
 *           (D*10**-1 + D*10**-2 + ...)] * 10**(esign*MMM)
 *
 * where nsign = sign of number, esign = sign of exponent.
 */
double a2d(string, status)

char string[];		/* ascii string to convert to double */
long *status;		/* final conversion status */
{
    char *pstring, *pn, *pend;
    double pow10;
    long nsign;
    double number;
    double fraction;
    long exponent;
    long a2l();

    pstring = string;	/* beginning of string */
    /*
     * find 1st non-blank character
     */
    while (is_blank(*pstring))
    {
	pstring++;
    }
    /*
     * get sign for number.
     */
    nsign = 1;
    if ((*pstring == '-') || (*pstring == '+'))
    {
	nsign = ((*pstring == '-') ? -1 : 1);
	pstring++;
    }
    /*
     * get non-fractional part of number.
     */
    number = 0.0;
    if (is_digit(*pstring))
    {
	pn = pstring;
	while (is_digit(*pn))
 	{
	    pn++;
 	}
	pend = pn;
	pow10 = 1.0;
	pn--;
	while (pn >= pstring)
	{
	    number = number + pow10 * ((double) to_decimal(*pn));
	    pow10 *= 10.0;
	    pn--;
 	}
	pstring = pend;		/* next character to look at */
    }
    /*
     * get decimal fraction.
     */
    fraction = 0.0;
    if (*pstring == '.')
    {
	pstring++;
	pend = pstring;
	while (is_digit(*pend))
 	{
	    pend++;
 	}
	pow10 = 0.1;
	while (pstring < pend)
	{
	    fraction = fraction + pow10 * ((double) to_decimal(*pstring));
	    pow10 *= 0.1;
	    pstring++;		/* next character to look at */
 	}
    }
    /*
     * get exponent, if any.
     */
    exponent = 0L;
    if ((*pstring == 'e') || (*pstring == 'E'))
    {
	pstring++;
	exponent = a2l(pstring, status);
	if (*status != GOODCONV)
	{
	    return (0.0);
	}
    }
    /*
     * put together the entire number, but check for bad character,
     * first.
     */
    if (!(is_digit(*pstring) || (*pstring == ASCIINULL) ||
	  (*pstring == '-') || (*pstring == '+')))
    {
	*status = BADCONV;
	return (0.0);
    }
    number = nsign * (number + fraction);
    if (exponent != 0)
    {
	pow10 = ((exponent < 0) ? 0.1 : 10.0);
	exponent = ((exponent < 0) ? (0L - exponent) : (exponent));
	for ( ; exponent > 0; exponent--)
	{
	    number *= pow10;
	}
    }
    *status = GOODCONV;
    return (number);
}

/*
 * function to convert long to ascii. the format returned is:
 *
 * format:	+/-IIIIII
 */
char *l2a(number, ndigit, nchar, status)

long number;	/* number to convert to ascii */
long ndigit;	/* maximum number of significant digits */
long *nchar;	/* number of characters in string */
long *status;	/* final status of conversion */
{
    char temp, *pstring, *pstart, *pend;

    pstring = buffer;		/* buffer for ascii string */
    *nchar = 0;			/* no characters, yet */
    ndigit = ((ndigit < 1) ? MAXLONGDIG : ndigit);
    ndigit = ((ndigit > MAXLONGDIG) ? MAXLONGDIG : ndigit);
    /*
     * get sign of integer.
     */
    *pstring = ((number < 0) ? '-' : '+');
    pstring++;		/* next byte */
    (*nchar)++;
    number = ((number < 0) ? (0 - number) : number);
    /*
     * start converting to ascii.
     */
    do
    {
	*pstring = to_ascii(number % 10);
	number = number/10;
	pstring++;
 	(*nchar)++;
	if (*nchar > (ndigit + 1))
	{
	    pstring = buffer;
	    for ( ; pstring <= (buffer + ndigit); pstring++)
	    {
		*pstring = '*';
	    }
	    *nchar = ndigit + 1;
	    *pstring=ASCIINULL;
	    *status = BADFORMAT;
	    return (buffer);
	}
    }
    while (number > 0);
    *pstring = ASCIINULL;
    /*
     * reverse order of characters.
     */
    pstart = buffer + 1;
    pend = pstring - 1;
    for ( ; pend > pstart; pstart++, pend--)
    {
	temp = *pstart;
	*pstart = *pend;
	*pend = temp;
    }
    *status = GOODCONV;
    return (buffer);
}

/*
 * function to convert double to ascii, ascii is returned
 * in E format:
 *
 * +/-0.DDDDDDDe+/-EE
 */
char *d2a(number, ndigit, edigit, nchar, status)

double number;	/* double to convert to ascii */
long ndigit;	/* number of significant digits */
long edigit;	/* number of digits in exponent */
long *nchar;	/* number of characters in string */
long *status;	/* final status of conversion */
{
    char *pstring, *pstart, *pend;
    long n, pow10, digit, first, maxsigdig;

    pstring = buffer;
    /*
     * check input format parameters.
     */
    edigit = ((edigit < 2) ? 2 : edigit);
    edigit = ((edigit > MAXEXPDIG) ? MAXEXPDIG : edigit);
    ndigit = ((ndigit < 2) ? 2 : ndigit);
    ndigit = ((ndigit > MAXSIGDIG) ? MAXSIGDIG : ndigit);
    /*
     * get sign of number.
     */
    *pstring = ((number < 0.0) ? '-' : '+');
    number = ((number < 0.0) ? (-1.0*number) : number);
    pstring++;
    /*
     * scale value to within: 1.0 <= number < 10.0
     */
    pow10 = 0;
    if (number == 0.0)
    {
	/* do nothing */
    }
    else if (number < 1.0)
    {
	pow10--;
	number *= 10.0;
	while (number < 1.0)
	{
	    number *= 10.0;
	    pow10--;
 	}
    }
    else if (number >= 10.0)
    {
	pow10++;
	number *= 0.1;
	while (number >= 10.0)
	{
	    number *= 0.1;
	    pow10++;
 	}
    }
    /*
     * convert the first (ndigit + 1) digits to ascii.
     * the extra digit is for rounding.
     */
    first = 1;
    for (n = 1; n <= (ndigit + 1); n++)
    {
	digit = (long) number;
	*pstring = to_ascii(digit);
	number -= ((double) digit);
	number *= 10.0;
	pstring++;
	if (first)
	{
	    *pstring = '.';	/* add decimal point */
	    pstring++;
	    first = 0;
	}
    }
    pstring--;
    pend = pstring;		/* don't include round off digit */
    if (*pstring >= '5')
    {
	do
	{
	    if (pstring == (buffer + 1))
	    {
	  	*(buffer + 1) = '1';
		pow10++;
		break;
	    }
	    *pstring = '0';
	    pstring--;
	    if (*pstring == '.') pstring--;
	    (*pstring)++;
	}
	while (*pstring > '9');
    }
    pstring = pend;
    /*
     * get exponent.
     */
    *pstring = 'e';
    pstring++;
    if (pow10 < 0)
    {
	*pstring = '-';
	pow10 = 0 - pow10;
    }
    else
    {
	*pstring = '+';
    }
    pstring++;
    pstart = pstring;
    pend = (pstring + edigit - 1);
    pstring = pend;
    while (pstring >= pstart)
    {
	*pstring = to_ascii(pow10 % 10);
	pow10 = pow10/10;
	pstring--;
    }
    /*
     * check if exponent is to large.
     */
    if (pow10 != 0)
    {
	for (n = 0; n < (ndigit + edigit + 4); n++)
	{
	     *(buffer + n) = '*';
	}
	*(buffer + n) = ASCIINULL;
	*nchar = 0;
	*status = BADFORMAT;
	return (buffer);
    }
    *(pend + 1) = ASCIINULL;
    *nchar = 0;
    pstring = buffer;
    while (*pstring != ASCIINULL)
    {
	(*nchar)++;
	pstring++;
    }
    *status = GOODCONV;
    return (buffer);
}

/*
 ********************************************************************
 *
 * functions a user should call are given below, the previous
 * base functions should never be called directly.
 *
 ********************************************************************
 */

/*
 * function to convert ASCII to long.
 */
long char_a2l(string, number)

char string[];		/* ascii string to convert */
long *number;		/* resulting number */
{
    long status;
    long a2l();

    *number = a2l(string, &status);
    return (status);
}

/*
 * function to convert ASCII to double.
 */
long char_a2d(string, number)

char string[];		/* ascii string to convert */
double *number;		/* resulting number */
{
    long status;
    double a2d();

    *number = a2d(string, &status);
    return (status);
}

/*
 * function to convert long to ascii.
 */
long char_l2a(number, nplace, string, nchar)

long number;	/* long to convert to ascii */
long nplace;	/* max size of ascii string */
char string[];	/* returned ascii string */
long *nchar;	/* number of characters in string */
{
    long status;
    char *pbuff, *pstr, *l2a();

    pstr = string;
    /*
     * convert long to ascii.
     */
    pbuff = l2a(number, (nplace - 1), nchar, &status);
    if ((status != GOODCONV) || (*nchar > nplace))
    {
	*string = ASCIINULL;
	*nchar = 0;
	return (BADFORMAT);
    }
    /*
     * pack into string.
     */
    for ( ; *pbuff != ASCIINULL; pstr++, pbuff++)
    {
	*pstr = *pbuff;
    }
    *pstr = ASCIINULL;
    return (GOODCONV);
}

/*
 * function to convert double to ascii (E format).
 */
long char_d2a_e(number, nplace, sigdig, string, nchar)

double number;	/* double to convert to ascii */
long nplace;	/* max number of characters in string */
long sigdig;	/* number of significant digits required */
char string[];	/* returned ascii string */
long *nchar;	/* number of characters in string */
{
    long status;
    char *pstr, *pbuff, *d2a();

    pstr = string;
    /*
     * convert double to ascii.
     */
    pbuff = d2a(number, sigdig, MAXEXPDIG, nchar, &status);
    if ((status != GOODCONV) || (*nchar > nplace))
    {
	*string = ASCIINULL;
	*nchar = 0;
	return (BADFORMAT);
    }
    /*
     * transfer to returned string.
     */
    for ( ; *pbuff != ASCIINULL; pstr++, pbuff++)
    {
	*pstr = *pbuff;
    }
    *pstr = ASCIINULL;
    return (GOODCONV);
}

/*
 * function to convert double to ascii (F format).
 */
long char_d2a_f(number, nplace, decimal, string, nchar)

double number;	/* double to convert to ascii */
long nplace;	/* max number of characters in string */
long decimal;	/* number of decimal places required */
char string[];	/* returned ascii string */
long *nchar;	/* number of characters in string */
{
    long status, pow10, a2l();
    char *pstr, *pexp, *pbuff, *pstart, *d2a();

    pstr = string;
    /*
     * check minimum size and require decimal places.
     */
    if ((nplace < (decimal + 3)) || (decimal < 1))
    {
	*string = ASCIINULL;
	*nchar = 0;
	return (BADFORMAT);
    }
    /*
     * convert double to ascii.
     */
    pbuff = d2a(number, MAXSIGDIG, MAXEXPDIG, nchar, &status);
    if (status != GOODCONV)
    {
	*string = ASCIINULL;
	*nchar = 0;
	return (BADFORMAT);
    }
    /*
     * get exponential power of number.
     */
    pexp = pbuff;
    for ( ; ((*pexp != 'E') && (*pexp != 'e')); pexp++)
    {
	if (*pexp == ASCIINULL)
	{
	    *string = ASCIINULL;
	    *nchar = 0;
	    return (BADFORMAT);
	}
    }
    /*
     * convert exponent to long.
     */
    pexp++;
    pow10 = a2l(pexp, status);
    if (status != GOODCONV)
    {
	*string = ASCIINULL;
	*nchar = 0;
	return (BADFORMAT);
    }
    /*
     * branch according to power, and stuff number into string.
     */
    if (pow10 < 0)
    {
	pow10 = 0 - pow10;
	*pstr++ = *pbuff++;	/* store sign */
	*pstr++ = '0';
	*pstr++ = '.';
	for ( ; pstr < (string + pow10 - 1 + 3); pstr++)
	{
	    if (pstr >= (string + nplace))
	    {
		*string = ASCIINULL;
		*nchar = 0;
		return (BADFORMAT);
	    }
	    if (pstr >= (string + decimal + 3))
	    {
		*pstr = ASCIINULL;
		*nchar = pstr - string;
		return (GOODCONV);
	    }
	    *pstr = '0';
	}
	while (pstr < (string + decimal + 3))
	{
	    if (pstr >= (string + nplace))
	    {
		*string = ASCIINULL;
		*nchar = 0;
		return (BADFORMAT);
	    }
	    switch (*pbuff)
	    {
	    case '.':
		pbuff++;		/* skip decimal point */
		break;
	    case 'e':
	    case 'E':
	    case ASCIINULL:
		*pstr++ = '0';		/* fill with zeroes */
		break;
	    default:
		if (is_digit(*pbuff))
		{
		    *pstr++ = *pbuff++;		/* store digit */
		}
		else
		{
		    *string = ASCIINULL;
		    *nchar = 0;
		    return (BADFORMAT);
		}
		break;
	    }
	}
    }
    else if (pow10 > 0)
    {
	while (pstr < (string + pow10 + 2))
	{
	    if (pstr >= (string + nplace))
	    {
		*string = ASCIINULL;
		*nchar = 0;
		return (BADFORMAT);
	    }
	    switch (*pbuff)
	    {
	    case '.':
		pbuff++;
		break;
	    case '-':
	    case '+':
		*pstr++ = *pbuff++;
		break;
	    case 'e':
	    case 'E':
	    case ASCIINULL:
		*pstr++ = '0';
		break;
	    default:
		if (is_digit(*pbuff))
		{
		    *pstr++ = *pbuff++;		/* store digit */
		}
		else
		{
		    *string = ASCIINULL;
		    *nchar = 0;
		    return (BADFORMAT);
		}
		break;
	    }
	}
	if (pstr >= (string + nplace))
	{
	    *string = ASCIINULL;
	    *nchar = 0;
	    return (BADFORMAT);
	}
	*pstr++ = '.';
	if (pstr >= (string + nplace))
	{
	    *string = ASCIINULL;
	    *nchar = 0;
	    return (BADFORMAT);
	}
	pstart = pstr;
	while (pstr < (pstart + decimal))
	{
	    if (pstr >= (string + nplace))
	    {
		*string = ASCIINULL;
		*nchar = 0;
		return (BADFORMAT);
	    }
	    switch (*pbuff)
	    {
	    case '.':
		pbuff++;		/* skip decimal point */
		break;
	    case 'e':
	    case 'E':
	    case ASCIINULL:
		*pstr++ = '0';		/* fill with zeroes */
		break;
	    default:
		if (is_digit(*pbuff))
		{
		    *pstr++ = *pbuff++;		/* store digit */
		}
		else
		{
		    *string = ASCIINULL;
		    *nchar = 0;
		    return (BADFORMAT);
		}
		break;
	    }
	}
	*pstr = ASCIINULL;
    }
    else
    {
	while (pstr < (string + decimal + 3))
	{
	    if (pstr >= (string + nplace))
	    {
		*string = ASCIINULL;
		*nchar = 0;
		return (BADFORMAT);
	    }
	    switch (*pbuff)
	    {
	    case '.':
	    case '-':
	    case '+':
		*pstr++ = *pbuff++;
		break;
	    case 'e':
	    case 'E':
	    case ASCIINULL:
		*pstr++ = '0';
		break;
	    default:
		if (is_digit(*pbuff))
		{
		    *pstr++ = *pbuff++;		/* store digit */
		}
		else
		{
		    *string = ASCIINULL;
		    *nchar = 0;
		    return (BADFORMAT);
		}
		break;
	    }
	}
    }
    *pstr = ASCIINULL;
    *nchar = pstr - string;
    return (GOODCONV);
}
