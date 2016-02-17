#ifndef __SPECTRUM_H
#define __SPECTRUM_H

// spectrum class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "vector.h"

// forward declararions
class Spectrum;

// local definitions
#define DEFAULTCHANNELS 1024
#define BITSPERBYTE 8

// spectrum class definition
class Spectrum {
public:
        // constructors and destructor
        Spectrum(int = DEFAULTCHANNELS);
        Spectrum(const Number *, int);
        Spectrum(const Vector &);
        Spectrum(const Spectrum &);
        ~Spectrum();

        // arithmetic operations
        Spectrum &operator=(const Spectrum &);
        Spectrum &operator+=(const Spectrum &);
        Spectrum &operator-=(const Spectrum &);
        Spectrum operator+(const Spectrum &) const;
        Spectrum operator-(const Spectrum &) const;

        // arithmetic operations
        Spectrum &operator*=(const Number &);
        Spectrum &operator/=(const Number &);
        Spectrum operator*(const Number &) const;
        Spectrum operator/(const Number &) const;
        friend Spectrum operator*(const Number &, const Spectrum &);
        Spectrum &operator*=(double);
        Spectrum &operator/=(double);
        Spectrum operator*(double) const;
        Spectrum operator/(double) const;
        friend Spectrum operator*(double, const Spectrum &);

        // signal analysis operations
        friend int fft(Spectrum &);
        friend int invfft(Spectrum &);
        friend int correlation(Spectrum &, Spectrum &);
        friend int convolution(Spectrum &, Spectrum &);
        friend int conj(Spectrum &);

        // array access 
        inline Number &operator[](int ich) { 
                return(spectrum[ich]);
        }
        inline Number &operator[](int ich) const { 
                return(spectrum[ich]);
        }

        // other functions
        friend ostream &operator<<(ostream &, const Spectrum &);
        inline int getNumberOfChannels() const {
                return(spectrum.getDimension());
        };


protected:
        // internal data
        Vector spectrum;
};


#endif
