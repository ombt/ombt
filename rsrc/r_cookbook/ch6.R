
# 6.1 - splitting a vector into groups

# groups = split(x, f) where x is the vector of data and f is vector 
# of groups.

library(MASS)
names(Cars93)

g = split(Cars93$MPG.city, Cars93$Origin)

names(g)

class(g)

median(g[[1]])

median(g[[2]])

# 6.2 - applying a function to each list element
#
# lst = lapply(lst, func)
# lst = sapply(lst, func)
#
# lapply returns a list and sapply returns a vector if possible.
#

scores = list(s1=round(rnorm(36,mean=70,sd=12),digits=0),
              s2=round(rnorm(39,mean=70,sd=12),digits=0),
              s3=round(rnorm(38,mean=70,sd=12),digits=0),
              s4=round(rnorm(36,mean=70,sd=12),digits=0))
scores

lapply(scores, length)
sapply(scores, length)

lapply(scores, mean)
lapply(scores, sd)
lapply(scores, range)

sapply(scores, mean)
sapply(scores, sd)
sapply(scores, range)

tests = lapply(scores, t.test)
tests

sapply(tests, function(t) { t$conf.int })

# 6.3 applying a function to every row
#
# apply a function to every row in a matrix.
#
# use the apply function.
#
# results = apply(mat, 1, func)
#
# returns the results as a vector.
#

long = matrix(data=c(-1.85, -1.40, -1.01, -3.71, -0.28,
                      0.94,  1.34, -0.01,  1.62,  2.44,
                     -0.54, -1.70, -0.34, -0.27, -1.21),
              byrow=TRUE,
              nrow=3, ncol=5)
colnames(long) = c("trial1", "trial2", "trial3", "trial4", "trial5")
rownames(long) = c("moe", "larry", "curly")

long

apply(long, 1, mean)
apply(long, 1, range)

# 6.4 - applying a function to every column
#
# for a matrix use apply():
#
# results = apply(mat, 2, func)
#
# for a data frame, use lapply (returns a list) or sapply (returns
# a vector).
#
# lst = lapply(dfrm, func)
# vec = sapply(dfrm, func)
#

head(PlantGrowth)
sapply(PlantGrowth, class)

# 6.5 - applying a function to groups of data.
#
# tapply(x, factors, func)
#
# factors is a vector of factors the same length as x.
#

names(PlantGrowth)

head(PlantGrowth)

factor(PlantGrowth$group)
levels(PlantGrowth$group)

tapply(PlantGrowth$weight, PlantGrowth$group, mean)
tapply(PlantGrowth$weight, PlantGrowth$group, sum)

source("create.suburbs.R", echo=TRUE)

attach(suburbs)

sum(pop)
mean(pop)

county

tapply(pop,county,sum)
tapply(pop,county,mean)
tapply(pop,county,length)

detach()

# 6.6 - applying a function to groups of rows
#
# by(dfrm, factor, func)
#

by(suburbs, suburbs$county, summary)

# example

head(mtcars)

by(mtcars, mtcars$cyl, summary)

models = by(mtcars, mtcars$cyl, function(df) lm(mpg~disp+wt,data=df))
models

lapply(models, confint)

# 6.7 - applying a function to parallel vectors or lists
#
# wish to apply a function to a vector of values but the
# function does not support vector arguments.
#
# mapply(func, vec1, ..., vecn)
# or
# mapply(func, list1, ..., listn)
#
# vecs are the same length or the recycling rule is applied.
#

gcd = function(a,b) {
    if (b == 0) 
        return(a)
    else
        return(gcd(b, a %% b))
}

# WRONG
gcd(c(1,2,3), c(9,6,3))

# CORRECT
mapply(gcd, c(1,2,3), c(9,6,3))




