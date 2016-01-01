
# T-tests
#
# The most common activity in research is the comparison of two groups. Do 
# patients receiving a new drug show greater improvement than patients using 
# an existing medication? Does one manufacturing process produce fewer 
# defects than another? Which of two teaching methods is most cost-effective? 
# If your outcome variable is categorical, you can use the methods described 
# in section 7.3. Here, we’ll focus on group comparisons, where the outcome 
# variable is continuous and assumed to be distributed normally.

library(MASS)
t.test(Prob ~ So, data=UScrime)

