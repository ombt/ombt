
# Testing correlations for significance
# Once you’ve generated correlation coefficients, how do you test them 
# for statistical significance? The typical null hypothesis is no 
# relationship (that is, the correlation in the population is 0). You can 
# use the cor.test() function to test an individual Pear-son, Spearman, and 
# Kendall correlation coefficient. A simplified format is
#
# cor.test(x, y, alternative = , method = )
#
# where x and y are the variables to be correlated, alternative specifies a 
# two-tailed or one-tailed test ( "two.side" , "less" , or "greater" ), and 
# method specifies the type of correlation ( "pearson" , "kendall" , or 
# "spearman" ) to compute. Use alternative ="less" when the research 
# hypothesis is that the population correlation is less than 0.  Use 
# alternative="greater" when the research hypothesis is that the 
# population correlation is greater than 0. By default, 
# alternative="two.side" (population correlation isn’t equal to 0) is assumed.

library(psych)
corr.test(states, use="complete")

# Before leaving this topic, it should be mentioned that the r.test() 
# function in the psych package also provides a number of useful significance 
# tests. The function can be used to test the following:
#
# The significance of a correlation coefficient
# The difference between two independent correlations
# The difference between two dependent correlations sharing a single variable
# The difference between two dependent correlations based on completely 
# different variablescor.test(states[,3], states[,5])

help(r.test)
