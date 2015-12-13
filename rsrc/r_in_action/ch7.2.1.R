library(vcd)
head(Arthritis)

# Functions for creating and manipulating contingency tables
#
# Function Description
#
# table(var1, var2, ..., varN) - Creates an N-way contingency table 
# from N categorical variables (factors)
#
# xtabs(formula, data) - Creates an N-way contingency table based 
# on a formula and a matrix or data frame
#
# prop.table(table, margins) - Expresses table entries as fractions 
# of the marginal table defined by the margins
#
# margin.table(table, margins) - Computes the sum of table entries for 
# a marginal table defined by the margins
#
# addmargins(table, margins) - Puts summary margins (sums by 
# default) on a table
#
# ftable(table) - Creates a compact, “flat” contingency table

# one-way tables


# you can generate simple frequency counts using the table() 
# function. Here’s an example:

mytable <- with(Arthritis, table(Improved))
mytable

# You can turn these frequencies into proportions with 
# prop.table()

prop.table(mytable)

# or into percentages using prop.table()*100:
prop.table(mytable)*100

# For two-way tables, the format for the table() function is
#
# mytable <- table(A, B)
#
# where A is the row variable and B is the column variable. 
# Alternatively, the xtabs() function allows you to create a 
# contingency table using formula-style input. The format is
#
# mytable <- xtabs(~ A + B, data=mydata)
#
# where mydata is a matrix or data frame. In general, the variables 
# to be cross-classified appear on the right of the formula (that is, 
# to the right of the ~) separated by + signs. If a variable is 
# included on the left side of the formula, it’s assumed to be 
# a vector of frequencies (useful if the data have already 
# been tabulated).

mytable <- xtabs(~ Treatment+Improved, data=Arthritis)
mytable

margin.table(mytable, 1)

prop.table(mytable, 1)

margin.table(mytable, 2)

prop.table(mytable, 2)

prop.table(mytable)

addmargins(mytable)

addmargins(prop.table(mytable))

addmargins(prop.table(mytable, 1), 2)

addmargins(prop.table(mytable, 2), 1)

library(gmodels)
CrossTable(Arthritis$Treatment, Arthritis$Improved)

# multidimensional tables

mytable <- xtabs(~ Treatment+Sex+Improved, data=Arthritis)
mytable

ftable(mytable)

margin.table(mytable, 1)

margin.table(mytable, 2)

margin.table(mytable, 3)

margin.table(mytable, c(1,3))

ftable(prop.table(mytable, c(1, 2)))

ftable(addmargins(prop.table(mytable, c(1, 2)), 3))

ftable(addmargins(prop.table(mytable, c(1, 2)), 3)) * 100



