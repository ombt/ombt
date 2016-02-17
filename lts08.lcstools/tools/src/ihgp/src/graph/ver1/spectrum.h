#ifndef __SPECTRUM_H
#define __SPECTRUM_H
/* class for containing a spectrum */

/* headers */
#include "complex.h"

/* define class */
class Spectrum;

/* definitions */
#define PARAMETER "PARAMETER:"
#define PARAMETERSZ (sizeof("PARAMETER:")-1)
#define DATA "DATA:"
#define DATASZ (sizeof("DATA:")-1)
#define BITSPERBYTE 8

/* spectrum class definition */
class Spectrum {
public:
	/* constructor and destructor */
	Spectrum();
	Spectrum(const Spectrum &);
	Spectrum(char *);
	Spectrum(int, ComplexNumber * = 0);
	Spectrum(int, double, double, ComplexNumber * = 0);
	~Spectrum();

	/* initialization */
	int init();
	int init(const Spectrum &);
	int init(char *);
	int init(int, double, double, ComplexNumber *);
	int deleteAll();
	int writeSpectrum(char *);
	int readSpectrum(char *);

	/* arithmetic operators */
	Spectrum &operator=(const Spectrum &);
	Spectrum &operator+=(const Spectrum &);
	Spectrum &operator-=(const Spectrum &);
	Spectrum operator+(const Spectrum &);
	Spectrum operator-(const Spectrum &);

	/* scalar arithmetic operators */
	Spectrum &operator*=(const ComplexNumber &);
	Spectrum &operator/=(const ComplexNumber &);
	Spectrum operator*(const ComplexNumber &);
	Spectrum operator/(const ComplexNumber &);
	friend Spectrum operator*(const ComplexNumber &, const Spectrum &);

	/* signal analysis operations */
	friend Spectrum fft(const Spectrum &);
	friend Spectrum invfft(const Spectrum &);
	friend Spectrum correlation(const Spectrum &, const Spectrum &);
	friend Spectrum convolution(const Spectrum &, const Spectrum &);

	/* N*N fourier operations for comparison only */
	friend Spectrum ft(const Spectrum &);
	friend Spectrum invft(const Spectrum &);

	/* conjugation */
	friend Spectrum conj(const Spectrum &);

	/* array access */
	inline ComplexNumber &operator[](int ichan) { return(counts[ichan]); }

	/* other functions */
	inline int getStatus() { return(status); }
	inline int getNumChan() { return(numberOfChannels); }
	void dump();

protected:
	/* internal data */
	int status;
	double minimumChannel;
	double maximumChannel;
	ComplexNumber *counts;
	int numberOfChannels;
};
#endif
