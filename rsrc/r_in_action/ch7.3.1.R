
# PEARSON, SPEARMAN, AND KENDALL CORRELATIONS
# The Pearson product-moment correlation assesses the degree of 
# linear relationship between two quantitative variables. 
# Spearman’s rank-order correlation coefficient assesses the 
# degree of relationship between two rank-ordered variables. 
# Kendall’s tau is also a nonparametric measure of rank correlation.

# The cor() function produces all three correlation coefficients, 
# whereas the cov() function provides covariances. There are many 
# options, but a simplified format for producing correlations is
#
# cor(x, use= , method= )
#
# Option - Description
# x - Matrix or data frame.
# use - Specifies the handling of missing data. The options are 
# all.obs (assumes no missing data—missing data will produce an 
# error), everything (any correlation involving a case with missing 
# values will be set to missing), complete.obs (listwise deletion),
# and pairwise.complete.obs (pairwise deletion).
# method - Specifies the type of correlation. The options are 
# pearson, spearman, and kendall.

states<- state.x77[,1:6]
cov(states)

cor(states)

cor(states, method="spearman")

x <- states[,c("Population", "Income", "Illiteracy", "HS Grad")]
y <- states[,c("Life Exp", "Murder")]
cor(x,y)

# partial correlations

# A partial correlation is a correlation between two quantitative 
# variables, controlling for one or more other quantitative 
# variables. You can use the pcor() function in the ggm package to 
# provide partial correlation coefficients. The ggm package isn’t 
# installed by default, so be sure to install it on first use. 
# The format is
#
# pcor(u, S)
# 
# where u is a vector of numbers, with the first two numbers being 
# the indices of the variables to be correlated, and the remaining 
# numbers being the indices of the conditioning variables (that is, 
# the variables being partialed out). S is the covariance matrix
# among the variables. An example will help clarify this:

library(ggm)
colnames(states)
pcor(c(1,5,2,3,6), cov(states))


