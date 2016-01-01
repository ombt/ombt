
# Nonparametric tests of group differences
#
# If you’re unable to meet the parametric assumptions of a t-test or ANOVA, 
# you can turn to nonparametric approaches. For example, if the outcome 
# variables are severely skewed or ordinal in nature, you may wish to use 
# the techniques in this section.

with(UScrime, by(Prob, So, median))

wilcox.test(Prob ~ So, data=UScrime)
