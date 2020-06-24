/* define spectrum class member functions */

/* headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <values.h>
#include <assert.h>
#include <string.h>
#include "returns.h"
#include "debug.h"
#include "complex.h"
#include "spectrum.h"

/* spectrum constructors and destructor */
Spectrum::Spectrum()
{
	init();
}

Spectrum::Spectrum(const Spectrum &src)
{
	init(src);
}

Spectrum::Spectrum(char *srcfile)
{
	init(srcfile);
}

Spectrum::Spectrum(int numch, ComplexNumber *src)
{
	init(numch, 0.0, double(numch-1), src);
}

Spectrum::Spectrum(int numch, double minch, double maxch, ComplexNumber *src)
{
	init(numch, minch, maxch, src);
}

Spectrum::~Spectrum()
{
	deleteAll();
}

/* initialization */
int
Spectrum::init()
{
	minimumChannel = 0.0;
	maximumChannel = 0.0;
	numberOfChannels = 0;
	counts = (ComplexNumber *)0;
	status = OK;
	return(OK);
}

int
Spectrum::init(const Spectrum &src)
{
	if (src.numberOfChannels <= 0)
	{
		minimumChannel = 0.0;
		maximumChannel = 0.0;
		numberOfChannels = 0;
		counts = (ComplexNumber *)0;
	}
	else
	{
		minimumChannel = src.minimumChannel;
		maximumChannel = src.maximumChannel;
		numberOfChannels = src.numberOfChannels;
		counts = new ComplexNumber [numberOfChannels];
		if (counts == (ComplexNumber *)0)
		{
			status = ENOMEM;
			ERROR("no memory for counts.", status);
			return(NOTOK);
		}
		for (int ichan = 0; ichan < numberOfChannels; ichan++)
		{
			counts[ichan] = src.counts[ichan];
		}
	}
	status = OK;
	return(OK);
}

int
Spectrum::init(char *srcfile)
{
	/* just null out everything */
	init();

	/* check parameters */
	if (srcfile == (char *)0 || *srcfile == 0)
	{
		/* nothing else to do */
		return(OK);
	}

	/* open spectrum file */
	FILE *infd = fopen(srcfile, "r");
	if (infd == (FILE *)0)
	{
		ERRORS("fopen failed for file.", srcfile, errno);
		return(NOTOK);
	}

	/* read in spectrum parameters and data */
	while ( 1 )
	{
		/* read in a record */
		char buf[BUFSIZ+1];
		fgets(buf, BUFSIZ, infd);
		if (ferror(infd))
		{
			ERROR("fgets failed.", errno);
			return(NOTOK);
		}
		else if (feof(infd))
		{
			/* done reading file */
			break;
		}

		/* what type of record */
		if (strncmp(buf, PARAMETER, PARAMETERSZ) == 0)
		{
			/* get parameter name and value */
			char *pname = strtok(buf, " ");
			if (pname == (char *)0) continue;
			pname = strtok((char *)0, " ");
			if (pname == (char *)0) continue;
			char *pvalue = strtok((char *)0, " ");
			if (pvalue == (char *)0) continue;

			/* store parameter value */
			if (strcmp(pname, "NUMCH") == 0)
			{
				/* get number of channels */
				sscanf(pvalue, "%d", &numberOfChannels);
				if (numberOfChannels <= 0)
				{
					status = EINVAL;
					ERROR("channel <= 0.", status);
					return(NOTOK);
				}

				/* allocate spectrum */
				counts = new ComplexNumber [numberOfChannels];
				if (counts == (ComplexNumber *)0)
				{
					status = ENOMEM;
					ERROR("no memory for counts.", status);
					return(NOTOK);
				}
				for (int ichan = 0; ichan < numberOfChannels; 
				     ichan++)
				{
					counts[ichan] = ComplexNumber(0.0, 0.0);
				}
			}
			else if (strcmp(pname, "MIN") == 0)
			{
				/* get minimum value */
				sscanf(pvalue, "%le", &minimumChannel);
			}
			else if (strcmp(pname, "MAX") == 0)
			{
				/* get maximum value */
				sscanf(pvalue, "%le", &maximumChannel);
			}
		}
		else if (strncmp(buf, DATA, DATASZ) == 0)
		{
			/* get data values */
			char *pdata = strtok(buf, " ");
			if (pdata == (char *)0) continue;
			char *pchan = strtok((char *)0, " ");
			if (pchan == (char *)0) continue;
			char *pxval = strtok((char *)0, " ");
			if (pxval == (char *)0) continue;
			char *pyval = strtok((char *)0, " ");
			if (pyval == (char *)0) continue;

			/* store value in spectrum */
			int chan;
			sscanf(pchan, "%d", &chan);
			if (chan < 0 || chan >= numberOfChannels)
			{
				status = EINVAL;
				ERRORI("channel out of range.", chan, status);
				return(NOTOK);
			}
			double xval, yval;
			sscanf(pxval, "%le", &xval);
			sscanf(pyval, "%le", &yval);
			counts[chan] = ComplexNumber(xval, yval);
		}
	}

	/* close file */
	fclose(infd);

	/* all done */
	status = OK;
	return(OK);
}

int
Spectrum::init(int numch, double minch, double maxch, ComplexNumber *src)
{
	if (numch <= 0)
	{
		minimumChannel = 0.0;
		maximumChannel = 0.0;
		numberOfChannels = 0;
		counts = (ComplexNumber *)0;
	}
	else
	{
		if (minch >= maxch)
		{
			minch = 0.0;
			maxch = double(numch - 1);
		}
		minimumChannel = minch;
		maximumChannel = maxch;
		numberOfChannels = numch;
		counts = new ComplexNumber [numberOfChannels];
		if (counts == (ComplexNumber *)0)
		{
			status = ENOMEM;
			ERROR("no memory for counts.", status);
			return(NOTOK);
		}
		if (src != (ComplexNumber *)0)
		{
			for (int ichan = 0; ichan < numberOfChannels; ichan++)
			{
				counts[ichan] = src[ichan];
			}
		}
		else
		{
			for (int ichan = 0; ichan < numberOfChannels; ichan++)
			{
				counts[ichan] = ComplexNumber(0.0, 0.0);
			}
		}
	}
	status = OK;
	return(OK);
}

int
Spectrum::deleteAll()
{
	if (numberOfChannels > 0 && counts != (ComplexNumber *)0)
	{
		delete [] counts;
	}
	minimumChannel = 0.0;
	maximumChannel = 0.0;
	numberOfChannels = 0;
	counts = (ComplexNumber *)0;
	status = OK;
	return(OK);
}

int
Spectrum::writeSpectrum(char *outfile)
{
	/* check parameter */
	if (outfile == (char *)0 || *outfile == 0)
	{
		status = EINVAL;
		ERROR("file name is null.", status);
		return(NOTOK);
	}

	/* check if anything to write */
	if (numberOfChannels <= 0 || counts == (ComplexNumber *)0)
	{
		ERROR("nothing to save, spectrum is empty.", EINVAL);
		return(NOTOK);
	}

	/* open output file for write */
	FILE *outfd = fopen(outfile, "w+");
	if (outfd == (FILE *)0)
	{
		status = errno;
		ERRORS("unable to open file.", outfile, status);
		return(NOTOK);
	}

	/* write out channels, min, max */
	fprintf(outfd, "PARAMETER: NUMCH %d\n", numberOfChannels);
	fprintf(outfd, "PARAMETER: MIN %.12le\n", minimumChannel);
	fprintf(outfd, "PARAMETER: MAX %.12le\n", maximumChannel);
	for (int ich = 0; ich < numberOfChannels; ich++)
	{
		fprintf(outfd, "DATA: %d %.12le %.12le\n", 
			ich, real(counts[ich]), imag(counts[ich]));
	}

	/* close file */
	fclose(outfd);

	/* all done */
	status = OK;
	return(OK);
}

int 
Spectrum::readSpectrum(char *infile)
{ 
	deleteAll();
	return(init(infile));
}

/* arithmetic operators */
Spectrum &
Spectrum::operator=(const Spectrum &rhs)
{
	status = OK;
	if (this != &rhs)
	{
		deleteAll();
		init(rhs);
	}
	return(*this);
}

Spectrum &
Spectrum::operator+=(const Spectrum &rhs)
{
	assert(numberOfChannels == rhs.numberOfChannels);
	assert(numberOfChannels > 0);
	for (int ichan = 0; ichan < numberOfChannels; ichan++)
	{
		counts[ichan] += rhs.counts[ichan];
	}
	status = OK;
	return(*this);
}

Spectrum &
Spectrum::operator-=(const Spectrum &rhs)
{
	assert(numberOfChannels == rhs.numberOfChannels);
	assert(numberOfChannels > 0);
	for (int ichan = 0; ichan < numberOfChannels; ichan++)
	{
		counts[ichan] -= rhs.counts[ichan];
	}
	status = OK;
	return(*this);
}

Spectrum
Spectrum::operator+(const Spectrum &rhs)
{
	assert(numberOfChannels == rhs.numberOfChannels);
	assert(numberOfChannels > 0);
	Spectrum tmp(*this);
	tmp += rhs;
	status = OK;
	return(tmp);
}

Spectrum
Spectrum::operator-(const Spectrum &rhs)
{
	assert(numberOfChannels == rhs.numberOfChannels);
	assert(numberOfChannels > 0);
	Spectrum tmp(*this);
	tmp -= rhs;
	status = OK;
	return(tmp);
}

/* scalar arithmetic operators */
Spectrum &
Spectrum::operator*=(const ComplexNumber &rhs)
{
	assert(numberOfChannels > 0);
	for (int ichan = 0; ichan < numberOfChannels; ichan++)
	{
		counts[ichan] *= rhs;
	}
	status = OK;
	return(*this);
}

Spectrum &
Spectrum::operator/=(const ComplexNumber &rhs)
{
	assert(numberOfChannels > 0);
	for (int ichan = 0; ichan < numberOfChannels; ichan++)
	{
		counts[ichan] /= rhs;
	}
	status = OK;
	return(*this);
}

Spectrum
Spectrum::operator*(const ComplexNumber &rhs)
{
	assert(numberOfChannels > 0);
	Spectrum tmp(*this);
	tmp *= rhs;
	status = OK;
	return(tmp);
}

Spectrum
Spectrum::operator/(const ComplexNumber &rhs)
{
	assert(numberOfChannels > 0);
	Spectrum tmp(*this);
	tmp /= rhs;
	status = OK;
	return(tmp);
}

Spectrum
operator*(const ComplexNumber &lhs, const Spectrum &rhs)
{
	Spectrum tmp(rhs);
	tmp *= lhs;
	return(tmp);
}

/* signal analysis operations */
static long 
ibitr(int nbits, long input)
{
	long output = 0;
	for (int ibit = 1; ibit <= nbits; ibit++)
	{
		output = (output << 1) + (input & 1);
		input >>= 1;
	}
	return(output);
}

Spectrum
fft(const Spectrum &sp)
{
	/* check parameters */
	assert(sp.numberOfChannels > 0);

	/* copy spectrum before doing fft calculation */
	Spectrum fftsp(sp);

	/* get power of 2 */
	int gamma = int(log(double(fftsp.numberOfChannels))/log(2.0)+0.5);

	/* calculate the exponential factors for fft */
	ComplexNumber *W = new ComplexNumber [fftsp.numberOfChannels];
	assert(W != (ComplexNumber *)0);
	double dtheta = 2.0*M_PI/double(fftsp.numberOfChannels);
	for (int iw = 0; iw < fftsp.numberOfChannels; iw++)
	{
		W[iw] = ComplexNumber(cos(dtheta*iw), -1.0*sin(dtheta*iw));
	}

	/* do fft calculation */
	for (int iter = 1; iter <= gamma; iter++)
	{
		for (long idx = 0; idx < fftsp.numberOfChannels; idx++)
		{
			/* get indexes for values to add */
			long idxmin = idx;
			long idxmax = idx | (1L << (gamma - iter));

			/* check if we already did this pair */
			if (idxmax <= idxmin) continue;

			/* get powers for exponential */
			long pmin = ibitr(iter, (idxmin >> (gamma - iter)));
			pmin <<= (gamma - iter);
			long pmax = ibitr(iter, (idxmax >> (gamma - iter)));
			pmax <<= (gamma - iter);

			/* calculate new values */
			ComplexNumber xmin = fftsp.counts[idxmin] + 
					W[pmin]*fftsp.counts[idxmax];
			ComplexNumber xmax = fftsp.counts[idxmin] + 
					W[pmax]*fftsp.counts[idxmax];
			fftsp.counts[idxmin] = xmin;
			fftsp.counts[idxmax] = xmax;
		}
	}

	/* unscramble FFT by doing bit reversal */
	for (long ifft1 = 0; ifft1 < fftsp.numberOfChannels; ifft1++)
	{
		long ifft2 = ibitr(gamma, ifft1);
		if (ifft2 > ifft1)
		{
			ComplexNumber tmpc = fftsp.counts[ifft1];
			fftsp.counts[ifft1] = fftsp.counts[ifft2];
			fftsp.counts[ifft2] = tmpc;
		}
	}

	/* release exponential array */
	delete [] W;

	/* return fft spectrum */
	return(fftsp);
}

Spectrum
invfft(const Spectrum &sp)
{
	Spectrum fftsp(sp);
	for (int idx = 0; idx < sp.numberOfChannels; idx++)
	{
		fftsp.counts[idx].y *= -1.0;
	}
	Spectrum invfftsp = fft(fftsp);
	for (idx = 0; idx < sp.numberOfChannels; idx++)
	{
		invfftsp.counts[idx].x /= double(sp.numberOfChannels);
		invfftsp.counts[idx].y /= -1.0*double(sp.numberOfChannels);
	}
	return(invfftsp);
}

Spectrum
correlation(const Spectrum &sp1, const Spectrum &sp2)
{
	/* verify that both spectrums have non-zero length */
	assert(sp1.numberOfChannels > 0);
	assert(sp2.numberOfChannels > 0);

	/* find length of new spectra */
	int nchan = sp1.numberOfChannels + sp2.numberOfChannels - 1;
	int ngamma = 2;
	int gamma = 1;
	for ( ; ngamma < nchan && gamma < BITSPERBYTE*sizeof(int); 
		ngamma *= 2, gamma++) ;
	assert(ngamma >= nchan);

	/* create new spectra with all zeroes */
	Spectrum xsp1(ngamma);
	Spectrum xsp2(ngamma);

	/* copy spectra */
	int ichan = sp1.numberOfChannels - 1;
	int ichan2 = xsp1.numberOfChannels - 1;
	for ( ; ichan >= 0; ichan--, ichan2--)
	{
		xsp1.counts[ichan2] = sp1.counts[ichan];
	}
	for (ichan = 0; ichan < sp2.numberOfChannels; ichan++)
	{
		xsp2.counts[ichan] = sp2.counts[ichan];
	}

	/* compute fft for each spectrum */
	Spectrum fftxsp1 = fft(xsp1);
	Spectrum fftxsp2 = fft(xsp2);

	/* multiply spectra together */
	for (ichan = 0; ichan < ngamma; ichan++)
	{
		fftxsp1.counts[ichan] *= conj(fftxsp2.counts[ichan]);
	}

	/* do inverse fft */
	Spectrum invfftsp = invfft(fftxsp1);
	
	/* return correlation */
	return(invfftsp);
}

Spectrum
convolution(const Spectrum &sp1, const Spectrum &sp2)
{
	/* verify that both spectrums have non-zero length */
	assert(sp1.numberOfChannels > 0);
	assert(sp2.numberOfChannels > 0);

	/* find length of new spectra */
	int nchan = sp1.numberOfChannels + sp2.numberOfChannels - 1;
	int ngamma = 2;
	int gamma = 1;
	for ( ; ngamma < nchan && gamma < BITSPERBYTE*sizeof(int); 
		ngamma *= 2, gamma++) ;
	assert(ngamma >= nchan);

	/* create new spectra with all zeroes */
	Spectrum xsp1(ngamma);
	Spectrum xsp2(ngamma);

	/* copy spectra */
	for (int ichan = 0; ichan < sp1.numberOfChannels; ichan++)
	{
		xsp1.counts[ichan] = sp1.counts[ichan];
	}
	for (ichan = 0; ichan < sp2.numberOfChannels; ichan++)
	{
		xsp2.counts[ichan] = sp2.counts[ichan];
	}

	/* compute fft for each spectrum */
	Spectrum fftxsp1 = fft(xsp1);
	Spectrum fftxsp2 = fft(xsp2);

	/* multiply spectra together */
	for (ichan = 0; ichan < ngamma; ichan++)
	{
		fftxsp1.counts[ichan] *= fftxsp2.counts[ichan];
	}

	/* do inverse fft */
	Spectrum invfftsp = invfft(fftxsp1);
	
	/* return convolution */
	return(invfftsp);
}

/* conjugation */
Spectrum
conj(const Spectrum &sp)
{
	Spectrum tmp(sp);
	for (int ichan = 0; ichan < sp.numberOfChannels; ichan++)
	{
		tmp[ichan] = conj(tmp[ichan]);
	}
	return(tmp);
}

/* dump spectrum */
void 
Spectrum::dump()
{
	status = OK;
	if (numberOfChannels <= 0) return;
	double delta = (maximumChannel - minimumChannel)/(numberOfChannels - 1);
	for (int ichan = 0; ichan < numberOfChannels; ichan++)
	{
		double chval = minimumChannel + ichan*delta;
		fprintf(stderr, "spectrum[%d, %f] = (%f, %f)\n",
			ichan, chval, counts[ichan].x, counts[ichan].y);
	}
	return;
}

/* N*N fourier functions, just for comparison */
Spectrum
ft(const Spectrum &sp)
{
	/* check parameters */
	assert(sp.numberOfChannels > 0);

	/* copy spectrum before doing ft calculation */
	Spectrum ftsp(sp);

	/* do the N*N fourier transform */
	double dtheta = 2.0*M_PI/double(sp.numberOfChannels);
	for (int n = 0; n < sp.numberOfChannels; n++)
	{
		ftsp.counts[n] = ComplexNumber(0.0, 0.0);
		for (int k = 0; k < sp.numberOfChannels; k++)
		{
			ftsp.counts[n] += sp.counts[k]*
				ComplexNumber(cos(dtheta*n*k), 
					     -1.0*sin(dtheta*n*k));
		}
	}
	
	/* return fft spectrum */
	return(ftsp);
}

Spectrum
invft(const Spectrum &sp)
{
	Spectrum ftsp(sp);
	for (int idx = 0; idx < sp.numberOfChannels; idx++)
	{
		ftsp.counts[idx].y *= -1.0;
	}
	Spectrum invftsp = ft(ftsp);
	for (idx = 0; idx < sp.numberOfChannels; idx++)
	{
		invftsp.counts[idx].x /= double(sp.numberOfChannels);
		invftsp.counts[idx].y /= -1.0*double(sp.numberOfChannels);
	}
	return(invftsp);
}

