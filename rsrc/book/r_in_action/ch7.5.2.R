
# Comparing more than two groups
#
# When there are more than two groups to be compared, you must turn to 
# other methods. Consider the state.x77 dataset from section 7.4. It 
# contains population, income, illiteracy rate, life expectancy, murder 
# rate, and high school graduation rate data for US states. What if you 
# want to compare the illiteracy rates in four regions of the country 
# (Northeast, South, North Central, and West)? This is called a one-way 
# design, and there are both parametric and nonparametric approaches 
# available to address the question.  If you can’t meet the assumptions 
# of ANOVA designs, you can use nonparametric methods to evaluate group 
# differences. If the groups are independent, a Kruskal–Wallis test 
# provides a useful approach. If the groups are dependent (for example, 
# repeated measures or randomized block design), the Friedman test is 
# more appropriate.

states <- data.frame(state.region, state.x77)

kruskal.test(Illiteracy ~ state.region, data=states)

source("http://www.statmethods.net/RiA/wmc.txt")

states <- data.frame(state.region, state.x77)

wmc(Illiteracy ~ state.region, data=states, method="holm")


