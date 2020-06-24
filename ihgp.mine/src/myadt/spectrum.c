// spectrum class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// local headers
#include "spectrum.h"

// constructors and destructor
Spectrum::Spectrum(int nch): spectrum(nch)
{
        // do nothing
}

Spectrum::Spectrum(const Number *ch, int nch): spectrum(ch, nch)
{
        // do nothing
}

Spectrum::Spectrum(const Vector &v): spectrum(v)
{
        // do nothing
}

Spectrum::Spectrum(const Spectrum &sp): spectrum(sp.spectrum)
{
        // do nothing
}

Spectrum::~Spectrum()
{
        // do nothing
}

// arithmetic operators 
Spectrum &
Spectrum::operator=(const Spectrum &sp)
{
        if (this != &sp)
                spectrum = sp.spectrum;
        return(*this);
}

Spectrum &
Spectrum::operator+=(const Spectrum &sp)
{
        spectrum += sp.spectrum;
        return(*this);
}

Spectrum &
Spectrum::operator-=(const Spectrum &sp)
{
        spectrum -= sp.spectrum;
        return(*this);
}

Spectrum
Spectrum::operator+(const Spectrum &sp) const
{
        return(Spectrum(*this) += sp);
}

Spectrum
Spectrum::operator-(const Spectrum &sp) const
{
        return(Spectrum(*this) -= sp);
}

// scalar arithmetic operations
Spectrum &
Spectrum::operator*=(const Number &n)
{
        spectrum *= n;
        return(*this);
}

Spectrum &
Spectrum::operator/=(const Number &n)
{
        spectrum /= n;
        return(*this);
}

Spectrum
Spectrum::operator*(const Number &n) const
{
        return(Spectrum(*this) *= n);
}

Spectrum
Spectrum::operator/(const Number &n) const
{
        return(Spectrum(*this) /= n);
}

Spectrum
operator*(const Number &n, const Spectrum &sp)
{
        return(Spectrum(sp) *= n);
}

Spectrum &
Spectrum::operator*=(double d)
{
        return(*this *= Number(d));
}

Spectrum &
Spectrum::operator/=(double d)
{
        return(*this /= Number(d));
}

Spectrum
Spectrum::operator*(double d) const
{
        return(Spectrum(*this) *= Number(d));
}

Spectrum
Spectrum::operator/(double d) const
{
        return(Spectrum(*this) /= Number(d));
}

Spectrum
operator*(double d, const Spectrum &sp)
{
        return(Spectrum(sp) *= Number(d));
}

// signal analysis operations
static int
ibitr(int nbits, int input)
{
        int output = 0;
        for (int ibit = 1; ibit <= nbits; ibit++)
        {
                output = (output << 1) + (input & 1);
                input >>= 1;
        }
        return(output);
}

int
fft(Spectrum &sp)
{
        // get power of 2
        int gamma = 
                int(log(double(sp.getNumberOfChannels()))/log(2.0)+0.5);

        // calculate exponential factors for fft
        Number *W = new Number[sp.getNumberOfChannels()];
        assert(W != (Number *)0);
        Number dtheta = 2.0*M_PI/sp.getNumberOfChannels();
        for (int iw = 0; iw < sp.getNumberOfChannels(); iw++)
        {
                W[iw] = exp(-1.0*iw*dtheta);
        }

        // do fft calculation
        for (int iter = 1; iter <= gamma; iter++)
        {
                for (int idx = 0; idx < sp.getNumberOfChannels(); idx++)
                {
                        // indexes for values to add
                        int idxmin = idx;
                        int idxmax = idx | (1 << (gamma - iter));

                        // check if this pair is already done
                        if (idxmax < idxmin) continue;

                        // get powers for exponential
                        int pmin = ibitr(iter, (idxmin >> (gamma - iter)));
                        pmin <<= (gamma - iter);
                        int pmax = ibitr(iter, (idxmax >> (gamma - iter)));
                        pmax <<= (gamma - iter);

                        // calculate new values
                        Number xmin = sp[idxmin] + W[pmin]*sp[idxmax];
                        Number xmax = sp[idxmin] + W[pmax]*sp[idxmax];
                        sp[idxmin] = xmin;
                        sp[idxmax] = xmax;

                }
        }

        // unscramble FFT by doing bit reversal
        for (int ifft1 = 0; ifft1 < sp.getNumberOfChannels(); ifft1++)
        {
                int ifft2 = ibitr(gamma, ifft1);
                if (ifft2 > ifft1)
                {
                        Number tmp = sp[ifft1];
                        sp[ifft1] = sp[ifft2];
                        sp[ifft2] = tmp;
                }
        }

        // delete exponential array
        delete [] W;

        // all done
        return(OK);
}

int
invfft(Spectrum &sp)
{
        for (int idx = 0; idx < sp.getNumberOfChannels(); idx++)
        {
                sp[idx] = conj(sp[idx]);
        }
        if (fft(sp) != OK) return(NOTOK);
        for (idx = 0; idx < sp.getNumberOfChannels(); idx++)
        {
                sp[idx] = conj(sp[idx])/double(sp.getNumberOfChannels());
        }
        return(OK);
}

int
correlation(Spectrum &sp1, Spectrum &sp2)
{
        // find length of new spectrum
        int nchan = 
                sp1.getNumberOfChannels() + sp2.getNumberOfChannels() - 1;
        int ngamma = 2;
        int gamma = 1;
        for ( ; ngamma < nchan && gamma < BITSPERBYTE*sizeof(int);
                ngamma *= 2, gamma++) ;
        assert(ngamma >= nchan);

        // create new spectra with all zeroes
        Spectrum xsp1(ngamma);
        Spectrum xsp2(ngamma);

        // copy spectra
        int ich = sp1.getNumberOfChannels() - 1;
        int ich2 = xsp1.getNumberOfChannels() - 1;
        for ( ; ich >= 0; ich--, ich2--)
        {
                xsp1[ich2] = sp1[ich];
        }
        for (ich = 0; ich < sp2.getNumberOfChannels(); ich++)
        {
                xsp2[ich] = sp2[ich];
        }

        // compute fft for each spectrum
        if (fft(xsp1) != OK) return(NOTOK);
        if (fft(xsp2) != OK) return(NOTOK);

        // multiply spectra together
        for (ich = 0; ich < ngamma; ich++)
        {
                xsp1[ich] *= conj(xsp2[ich]);
        }

        // do inverse fft and copy
        if (invfft(xsp1) != OK) return(NOTOK);
        sp1 = xsp1;

        // all done
        return(OK);

}

int
convolution(Spectrum &sp1, Spectrum &sp2)
{
        // find length of new spectrum
        int nchan = 
                sp1.getNumberOfChannels() + sp2.getNumberOfChannels() - 1;
        int ngamma = 2;
        int gamma = 1;
        for ( ; ngamma < nchan && gamma < BITSPERBYTE*sizeof(int);
                ngamma *= 2, gamma++) ;
        assert(ngamma >= nchan);

        // create new spectra with all zeroes
        Spectrum xsp1(ngamma);
        Spectrum xsp2(ngamma);

        // copy spectra
        for (int ich = 0; ich < sp1.getNumberOfChannels(); ich++)
        {
                xsp1[ich] = sp1[ich];
        }
        for (ich = 0; ich < sp2.getNumberOfChannels(); ich++)
        {
                xsp2[ich] = sp2[ich];
        }

        // compute fft for each spectrum
        if (fft(xsp1) != OK) return(NOTOK);
        if (fft(xsp2) != OK) return(NOTOK);

        // multiply spectra together
        for (ich = 0; ich < ngamma; ich++)
        {
                xsp1[ich] *= xsp2[ich];
        }

        // do inverse fft and copy
        if (invfft(xsp1) != OK) return(NOTOK);
        sp1 = xsp1;

        // all done
        return(OK);
}

// conjugation of spectrum
int
conj(Spectrum &sp)
{
        sp.spectrum = conj(sp.spectrum);
        return(OK);
}

// print spectrum
ostream &
operator<<(ostream &os, const Spectrum &sp)
{
        os << "spectrum = {" << endl;
        for (int ich = 0; ich < sp.getNumberOfChannels()-1; ich++)
        {
                os << sp[ich] << "," << endl;
        }
        os << sp[ich] << endl << "}" << endl;
        return(os);
}

