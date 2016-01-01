
# test for independence

# R provides several methods of testing the independence of 
# categorical variables. The three tests described in this section 
# are the chi-square test of independence, the Fisher exact 
# test, and the Cochran-Mantel–Haenszel test.

library(vcd)

mytable <- xtabs(~Treatment+Improved, data=Arthritis)
chisq.test(mytable)

mytable <- xtabs(~Improved+Sex, data=Arthritis)
chisq.test(mytable)

mytable <- xtabs(~Treatment+Improved, data=Arthritis)
fisher.test(mytable)

mytable <- xtabs(~Treatment+Improved+Sex, data=Arthritis)
mantelhaen.test(mytable)

