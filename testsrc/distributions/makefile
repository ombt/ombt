# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	uniform1 \
	exponential1 \
	gaussian1 \
	lognormal1 \
	weibull1 \
	merlang1 \
	discreteuniform1 \
	naivearbitrarydiscrete1 \
	qplacearbitrarydiscrete1 \
	bernoulli1 \
	binomial1 \
	geometric1 \
	negativebinomial1 \
	poisson1 \
	gamma1 \
	beta1 \
	pearsontypev1 \
	pearsontypevi1 \

CXXEXTRAFLAGS = 
CXXLIBFLAGS = -lombtlogging -lombtrandom -lombtdistributions

include $(ROOT)/build/makefile.test
