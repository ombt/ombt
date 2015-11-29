# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

# CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = distributions

FILES = Uniform.cpp \
	Gaussian.cpp \
	Poisson.cpp \
	Exponential.cpp \
	Binomial.cpp \
	LogNormal.cpp \
	Weibull.cpp \
	mErlang.cpp \
	DiscreteUniform.cpp \
	NaiveArbitraryDiscrete.cpp \
	QPlaceArbitraryDiscrete.cpp \
	Bernoulli.cpp \
	Geometric.cpp \
	NegativeBinomial.cpp \
	Gamma.cpp \
	Beta.cpp \
	PearsonTypeV.cpp \
	PearsonTypeVI.cpp \

include $(ROOT)/build/makefile.lib2

Uniform.o: \
		Uniform.cpp \
		Uniform.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Gaussian.o: \
		Gaussian.cpp \
		Gaussian.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Poisson.o: \
		Poisson.cpp \
		Poisson.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Exponential.o: \
		Exponential.cpp \
		Exponential.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Binomial.o: \
		Binomial.cpp \
		Binomial.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

LogNormal.o: \
		LogNormal.cpp \
		LogNormal.h \
		Gaussian.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/ExtUseCntPtr.h

Weibull.o: \
		Weibull.cpp \
		Weibull.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

mErlang.o: \
		mErlang.cpp \
		mErlang.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

DiscreteUniform.o: \
		DiscreteUniform.cpp \
		DiscreteUniform.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

NaiveArbitraryDiscrete.o: \
		NaiveArbitraryDiscrete.cpp \
		NaiveArbitraryDiscrete.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

QPlaceArbitraryDiscrete.o: \
		QPlaceArbitraryDiscrete.cpp \
		QPlaceArbitraryDiscrete.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Bernoulli.o: \
		Bernoulli.cpp \
		Bernoulli.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Geometric.o: \
		Geometric.cpp \
		Geometric.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

NegativeBinomial.o: \
		NegativeBinomial.cpp \
		NegativeBinomial.h \
		Geometric.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Gamma.o: \
		Gamma.cpp \
		Gamma.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h

Beta.o: \
		Beta.cpp \
		Beta.h \
		Gamma.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/ExtUseCntPtr.h

PeasonTypeV.o: \
		PeasonTypeV.cpp \
		PeasonTypeV.h \
		Gamma.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/ExtUseCntPtr.h

PeasonTypeVI.o: \
		PeasonTypeVI.cpp \
		PeasonTypeVI.h \
		Gamma.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/random/Random.h \
		$(INCLUDEDIR)/atomic/BaseObject.h \
		$(INCLUDEDIR)/atomic/ExtUseCntPtr.h

