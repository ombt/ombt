#
# data for this chapter
#
# Student		Math	Science	English
# John Davis		502	95	25
# Angela Williams	600	99	22
# Bullwinkle Moose	412	80	18
# David Jones		358	82	15
# Janice Markhammer	495	75	20
# Cheryl Cushing	512	85	28
# Reuven Ytzrhak	410	80	15
# Greg Knox		625	95	30
# Joel England		573	89	27
# Mary Rayburn		522	86	18

#
# mathematical functions
#
# abs(x) Absolute value
# abs(-4) returns 4.
# sqrt(x) Square root
# sqrt(25) returns 5. This is the same as 25^(0.5).
# ceiling(x) Smallest integer not less than x
# ceiling(3.475) returns 4.
# floor(x) Largest integer not greater than x
# floor(3.475) returns 3.
# trunc(x) Integer formed by truncating values in x toward 0
# trunc(5.99) returns 5.
# round(x, digits=n) Rounds x to the specified number of decimal places
# round(3.475, digits=2) returns 3.48.
# signif(x, digits=n) Rounds x to the specified number of significant digits
# signif(3.475, digits=2) returns 3.5.
# cos(x), sin(x), tan(x) Cosine, sine, and tangent
# cos(2) returns –0.416.
# acos(x), asin(x), atan(x) Arc-cosine, arc-sine, and arc-tangent
# acos(-0.416) returns 2.
# cosh(x), sinh(x), tanh(x) Hyperbolic cosine, sine, and tangent
# sinh(2) returns 3.627.
# acosh(x), asinh(x), atanh(x) Hyperbolic arc-cosine, arc-sine, and arc-tangent
# asinh(3.627) returns 2.
# log(x,base=n)
# log(x)
# log10(x)
# Logarithm of x to the base n
# For convenience:
# log(x) is the natural logarithm.
# log10(x) is the common logarithm.
# log(10) returns 2.3026.
# log10(10) returns 1.
# exp(x) Exponential function
# exp(2.3026) returns 10.

#
# statistical functions
#
# mean(x) Mean
# mean(c(1,2,3,4)) returns 2.5.
# median(x) Median
# median(c(1,2,3,4)) returns 2.5.
# sd(x) Standard deviation
# sd(c(1,2,3,4)) returns 1.29.
# var(x) Variance
# var(c(1,2,3,4)) returns 1.67.
# mad(x) Median absolute deviation
# mad(c(1,2,3,4)) returns 1.48.
# quantile(x,
# probs)
# Quantiles where x is the numeric vector, where quantiles are desired and
# probs is a numeric vector with probabilities in [0,1]
# # 30th and 84th percentiles of x
# y <- quantile(x, c(.3,.84))
# range(x) Range
# x <- c(1,2,3,4)
# range(x) returns c(1,4).
# diff(range(x)) returns 3.
# sum(x) Sum
# sum(c(1,2,3,4)) returns 10.
# diff(x, lag=n) Lagged differences, with lag indicating which lag to use. The default lag is 1.
# x<- c(1, 5, 23, 29)
# diff(x) returns c(4, 18, 6).
# min(x) Minimum
# min(c(1,2,3,4)) returns 1.
# max(x) Maximum
# max(c(1,2,3,4)) returns 4.
# scale(x,
# center=TRUE,
# scale=TRUE)
# Column center (center=TRUE) or standardize (center=TRUE,
# scale=TRUE) data object x. An example is given in listing 5.6.
# 

x <- c(1,2,3,4,5,6,7,8)
x
mean(x)
sd(x)

n <- length(x)
meanx <- sum(x)/n
css <- sum((x - meanx)^2)
sdx <- sqrt(css / (n-1))

meanx
sdx


# In R, probability functions take the form
# [dpqr]distribution_abbreviation()
# where the first letter refers to the aspect of the distribution returned:
# d = density
# p = distribution function
# q = quantile function
# r = random generation (random deviates)

# Table 5.4 Probability distributions
# Distribution Abbreviation Distribution Abbreviation
# Beta beta Logistic logis
# Binomial binom Multinomial multinom
# Cauchy cauchy Negative binomial nbinom
# Chi-squared (noncentral) chisq Normal norm
# Exponential exp Poisson pois
# F f Wilcoxon signed rank signrank
# Gamma gamma T t
# Geometric geom Uniform unif
# Hypergeometric hyper Weibull weibull
# Lognormal lnorm Wilcoxon rank sum wilcox

# To see how these work, let’s look at functions related to the normal distribution. If you
# don’t specify a mean and a standard deviation, the standard normal distribution is
# assumed (mean=0, sd=1). Examples of the density (dnorm), distribution (pnorm), quantile
# (qnorm), and random deviate generation (rnorm) functions are given in table


x <- pretty(c(-3,3), 30)
y <- dnorm(x)
plot(x, y,
     type = "l",
     xlab = "Normal Deviate",
     ylab = "Density",
     yaxs = "i")

pnorm(1.96)

qnorm(.9, mean=500, sd=100)

rnorm(50, mean=50, sd=10)

runif(5)
runif(5)

set.seed(1234)
runif(5)
set.seed(1234)
runif(5)

library(MASS)

options(digits=3)

set.seed(1234)
mean <- c(230.7, 146.7, 3.6)
sigma <- matrix(c(15360.8, 6721.2, -47.1,
                  6721.2, 4700.9, -16.5,
                 -47.1, -16.5, 0.3), nrow=3, ncol=3)

mydata <- mvrnorm(500, mean, sigma)
mydata <- as.data.frame(mydata)
names(mydata) <- c("y","x1","x2")
dim(mydata)
head(mydata, n=10)


