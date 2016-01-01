
myvars <- c("mpg", "hp", "wt")

head(mtcars[myvars])

summary(mtcars[myvars])

mystats <- function(x, na.omit=FALSE)
{
    if (na.omit)
    {
        x <- x[!is.na(x)]
    }

    m <- mean(x)
    n <- length(x)
    s <- sd(x)

    skew <- sum((x-m)^3/s^3)/n
    kurt <- sum((x-m)^4/s^4)/n - 3

    return(c(n=n, mean=m, stdev=s, skew=skew, kurtosis=kurt))
}

myvars <- c("mpg", "hp", "wt")
sapply(mtcars[myvars], mystats)
sapply(mtcars[myvars], mystats, na.omit=TRUE)

library(Hmisc)
myvars <- c("mpg", "hp", "wt")
describe(mtcars[myvars])

library(pastecs)
myvars <- c("mpg", "hp", "wt")
stat.desc(mtcars[myvars])

library(psych)
myvars <- c("mpg", "hp", "wt")
describe(mtcars[myvars])

myvars <- c("mpg", "hp", "wt")
aggregate(mtcars[myvars], by=list(am=mtcars$am), mean)
aggregate(mtcars[myvars], by=list(am=mtcars$am), sd)

# Unfortunately, aggregate() only allows you to use single-value 
# mfunctions such as mean, standard deviation, and the like in each 
# call. It won’t return several statistics at once. For that task, you 
# can use the by() function. The format is
#
# by(data, INDICES, FUN)
#
# where data is a data frame or matrix, INDICES is a factor or list of 
# factors that defines the groups, and FUN is an arbitrary function that 
# operates on all the columns of a data frame.

dstats <- function(x)sapply(x, mystats)
myvars <- c("mpg", "hp", "wt")
by(mtcars[myvars], mtcars$am, dstats)

library(doBy)
summaryBy(mpg+hp+wt~am, data=mtcars, FUN=mystats)

library(psych)
myvars <- c("mpg", "hp", "wt")
describeBy(mtcars[myvars], list(am=mtcars$am))
