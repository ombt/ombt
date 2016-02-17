#ifndef __COMPLEX_H
#define __COMPLEX_H
/* class for complex numbers */

/* define class */
class ComplexNumber;

/* complex class definition */
class ComplexNumber {
public:
	/* constructor and destructor */
	ComplexNumber();
	ComplexNumber(double);
	ComplexNumber(double, double);
	ComplexNumber(const ComplexNumber &);
	virtual ~ComplexNumber();

	/* arithmetic operators */
	ComplexNumber &operator=(const ComplexNumber &);
	ComplexNumber &operator+=(const ComplexNumber &);
	ComplexNumber &operator-=(const ComplexNumber &);
	ComplexNumber &operator*=(const ComplexNumber &);
	ComplexNumber &operator/=(const ComplexNumber &);
	ComplexNumber operator+(const ComplexNumber &) const;
	ComplexNumber operator-(const ComplexNumber &) const;
	ComplexNumber operator*(const ComplexNumber &) const;
	ComplexNumber operator/(const ComplexNumber &) const;

	/* logical operators */
	int operator==(const ComplexNumber &) const;
	int operator!=(const ComplexNumber &) const;
	int operator<(const ComplexNumber &) const;
	int operator>(const ComplexNumber &) const;
	int operator<=(const ComplexNumber &) const;
	int operator>=(const ComplexNumber &) const;

	/* mathematical functions */
	friend ComplexNumber exp(const ComplexNumber &);
	friend ComplexNumber log(const ComplexNumber &);
	friend ComplexNumber log10(const ComplexNumber &);
	friend ComplexNumber pow(const ComplexNumber &, const ComplexNumber &);
	friend ComplexNumber sqrt(const ComplexNumber &);
	friend ComplexNumber sin(const ComplexNumber &);
	friend ComplexNumber cos(const ComplexNumber &);
	friend ComplexNumber tan(const ComplexNumber &);
	friend ComplexNumber asin(const ComplexNumber &);
	friend ComplexNumber acos(const ComplexNumber &);
	friend ComplexNumber atan(const ComplexNumber &);
	friend ComplexNumber conj(const ComplexNumber &);
	friend double abs(const ComplexNumber &);
	friend double arg(const ComplexNumber &);
	friend double norm(const ComplexNumber &);
	friend double real(const ComplexNumber &);
	friend double imag(const ComplexNumber &);

	/* miscellaneous */
	void dump();

public:
	/* data */
	double x;
	double y;
};

#endif
