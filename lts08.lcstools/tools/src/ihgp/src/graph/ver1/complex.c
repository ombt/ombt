/* functions for complex class */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "complex.h"

/* local constants */
static const ComplexNumber plusi(0.0,1.0);
static const ComplexNumber minusi(0.0,-1.0);
static const ComplexNumber plusone(1.0,0.0);
static const ComplexNumber minusone(-1.0,0.0);

/* local definitions */
#define ABS(x_num) (((x_num) < 0.0) ? (-1.0*x_num) : (x_num))

/* constructors and destructor */
ComplexNumber::ComplexNumber()
{
	x = 0.0;
	y = 0.0;
}

ComplexNumber::ComplexNumber(double nx)
{
	x = nx;
	y = 0.0;
}

ComplexNumber::ComplexNumber(double nx, double ny)
{
	x = nx;
	y = ny;
}

ComplexNumber::ComplexNumber(const ComplexNumber &src)
{
	x = src.x;
	y = src.y;
}

ComplexNumber::~ComplexNumber()
{
}

/* arithmetic operators */
ComplexNumber &
ComplexNumber::operator=(const ComplexNumber &rhs)
{
	x = rhs.x;
	y = rhs.y;
	return(*this);
}

ComplexNumber &
ComplexNumber::operator+=(const ComplexNumber &rhs)
{
	x += rhs.x;
	y += rhs.y;
	return(*this);
}

ComplexNumber &
ComplexNumber::operator-=(const ComplexNumber &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return(*this);
}

ComplexNumber &
ComplexNumber::operator*=(const ComplexNumber &rhs)
{
	double tmpx = x*rhs.x - y*rhs.y;
	double tmpy = x*rhs.y + y*rhs.x;
	x = tmpx;
	y = tmpy;
	return(*this);
}

ComplexNumber &
ComplexNumber::operator/=(const ComplexNumber &rhs)
{
	assert((rhs.x != 0.0) || (rhs.y != 0.0));
	double tmpm = rhs.x*rhs.x + rhs.y*rhs.y;
	double tmpx = (x*rhs.x + y*rhs.y)/tmpm;
	double tmpy = (y*rhs.x - x*rhs.y)/tmpm;
	x = tmpx;
	y = tmpy;
	return(*this);
}

ComplexNumber 
ComplexNumber::operator+(const ComplexNumber &rhs) const
{
	ComplexNumber tmp(*this);
	tmp += rhs;
	return(tmp);
}

ComplexNumber 
ComplexNumber::operator-(const ComplexNumber &rhs) const
{
	ComplexNumber tmp(*this);
	tmp -= rhs;
	return(tmp);
}

ComplexNumber 
ComplexNumber::operator*(const ComplexNumber &rhs) const
{
	ComplexNumber tmp(*this);
	tmp *= rhs;
	return(tmp);
}

ComplexNumber 
ComplexNumber::operator/(const ComplexNumber &rhs) const
{
	ComplexNumber tmp(*this);
	tmp /= rhs;
	return(tmp);
}

/* logical operators */
int 
ComplexNumber::operator==(const ComplexNumber &rhs) const
{
	return((x == rhs.x) && (y == rhs.y));
}

int 
ComplexNumber::operator!=(const ComplexNumber &rhs) const
{
	return((x != rhs.x) || (y != rhs.y));
}

int 
ComplexNumber::operator<(const ComplexNumber &rhs) const
{
	return(ABS(x) < abs(rhs));
}

int 
ComplexNumber::operator>(const ComplexNumber &rhs) const
{
	return(ABS(x) > abs(rhs));
}

int 
ComplexNumber::operator<=(const ComplexNumber &rhs) const
{
	return(ABS(x) <= abs(rhs));
}

int 
ComplexNumber::operator>=(const ComplexNumber &rhs) const
{
	return(ABS(x) >= abs(rhs));
}

/* mathematical functions */
ComplexNumber
exp(const ComplexNumber &p)
{
	return(ComplexNumber(exp(p.x)*cos(p.y), exp(p.x)*sin(p.y)));
}

ComplexNumber
log(const ComplexNumber &p)
{
	assert((p.x != 0.0) || (p.y != 0.0));
	return(ComplexNumber(log(abs(p)), arg(p)));
}

ComplexNumber
log10(const ComplexNumber &p)
{
	return(log(p)/ComplexNumber(log(10.0)));
}

ComplexNumber
pow(const ComplexNumber &b, const ComplexNumber &e)
{
	if (b == 0.0)
		return(ComplexNumber(0.0));
	else
		return(exp(e*log(b)));
}

ComplexNumber
sqrt(const ComplexNumber &p)
{
	if (p == 0.0)
		return(ComplexNumber(0.0));
	else
		return(exp(ComplexNumber(0.5)*log(p)));
}

ComplexNumber
sin(const ComplexNumber &p)
{
	return(ComplexNumber(sin(p.x)*cosh(p.y), cos(p.x)*sinh(p.y)));
}

ComplexNumber
cos(const ComplexNumber &p)
{
	return(ComplexNumber(cos(p.x)*cosh(p.y), -1.0*sin(p.x)*sinh(p.y)));
}

ComplexNumber
tan(const ComplexNumber &p)
{
	return(sin(p)/cos(p));
}

ComplexNumber
asin(const ComplexNumber &p)
{
	return(minusi*log(plusi*p+sqrt(plusone-p*p)));
}

ComplexNumber
acos(const ComplexNumber &p)
{
	return(minusi*log(p+plusi*sqrt(plusone-p*p)));
}

ComplexNumber
atan(const ComplexNumber &p)
{
	return(ComplexNumber(0.5)*plusi*log((plusi+p)/(plusi-p)));
}

ComplexNumber
conj(const ComplexNumber &p)
{
	return(ComplexNumber(p.x, -1.0*p.y));
}

double
abs(const ComplexNumber &p)
{
	return(sqrt(p.x*p.x+p.y*p.y));
}

double
norm(const ComplexNumber &p)
{
	return(sqrt(p.x*p.x+p.y*p.y));
}

double
arg(const ComplexNumber &p)
{
	/*
	 *                     Y (arg=pi/2)
	 *                     | 
	 *             x<0,y>0 | x>0,y>0
	 *                     | 
	 * -X (arg=pi) --------+-------- X (arg=0)
	 *                     |
	 *             x<0,y<0 | x>0,y<0
	 *                     |
	 *                    -Y (arg=3*pi/2)
	 */
	if (p.x > 0.0)
	{
		if (p.y > 0.0)
		{
			return(atan(p.y/p.x));
		}
		else if (p.y < 0.0)
		{
			return(2.0*M_PI-atan(abs(p.y/p.x)));
		}
		else
		{
			return(0.0);
		}
	}
	else if (p.x < 0.0)
	{
		if (p.y > 0.0)
		{
			return(M_PI-atan(abs(p.y/p.x)));
		}
		else if (p.y < 0.0)
		{
			return(M_PI+atan(p.y/p.x));
		}
		else
		{
			return(M_PI);
		}
	}
	else
	{
		if (p.y > 0.0)
		{
			return(M_PI/2.0);
		}
		else if (p.y < 0.0)
		{
			return(-3.0*M_PI/2.0);
		}
		else
		{
			return(0.0);
		}
	}
}

double
real(const ComplexNumber &p)
{
	return(p.x);
}

double
imag(const ComplexNumber &p)
{
	return(p.y);
}

/* print complex number */
void
ComplexNumber::dump()
{
	fprintf(stderr, "x = %f, y = %f\n", x, y);
	return;
}
