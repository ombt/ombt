
boxplot(mtcars$mpg, main="Box plot", ylab="Miles per Gallon")

# Box plots can be created for individual variables or for variables 
# by group. The format is
#
# boxplot(formula, data=dataframe)
#
# where formula is a formula and dataframe denotes the data frame 
# (or list) providing the data. An example of a formula is y ~ A, 
# where a separate box plot for numeric variable y is generated 
# for each value of categorical variable A. The formula y ~ A*B
# would produce a box plot of numeric variable y, for each 
# combination of levels in categorical variables A and B.

boxplot(mpg ~ cyl, 
        data=mtcars,
        main="Car Mileage Data",
        xlab="Number of Cylinders",
        ylab="Miles Per Gallon")

x11()
boxplot(mpg ~ cyl, 
        data=mtcars,
        notch=TRUE,
        varwidth=TRUE,
        col="red",
        main="Car Mileage Data",
        xlab="Number of Cylinders",
        ylab="Miles Per Gallon")

x11()
mtcars$cyl.f <- factor(mtcars$cyl,
                       levels=c(4,6,8),
                       labels=c("4","6","8"))

mtcars$am.f <- factor(mtcars$am,
                      levels=c(0,1),
                      labels=c("auto", "standard"))

boxplot(mpg ~ am.f *cyl.f,
        data=mtcars,
        varwidth=TRUE,
        col=c("gold","darkgreen"),
        main="MPG Distribution by Auto Type",
        xlab="Auto Type", 
        ylab="Miles Per Gallon")

x11()
library(vioplot)

x1 <- mtcars$mpg[mtcars$cyl==4]
x2 <- mtcars$mpg[mtcars$cyl==6]
x3 <- mtcars$mpg[mtcars$cyl==8]

vioplot(x1, 
        x2, 
        x3,
        names=c("4 cyl", 
                "6 cyl", 
                "8 cyl"),
        col="gold")
title("Violin Plots of Miles Per Gallon", 
      ylab="Miles Per Gallon",
      xlab="Number of Cylinders")



