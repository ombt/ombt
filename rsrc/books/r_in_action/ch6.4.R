
par(mfrow=c(2,1))

d <- density(mtcars$mpg)
plot(d)

d <- density(mtcars$mpg)

plot(d, 
     main="Kernel Density of Miles Per Gallon")
polygon(d, col="red", border="blue")
rug(mtcars$mpg, col="brown")

# The sm.density.compare() function in the sm package allows you 
# to superimpose the kernel density plots of two or more groups. 
# The format is
#
# sm.density.compare(x, factor)
#
# where x is a numeric vector and factor is a grouping variable. Be 
# sure to install the sm package before first use.

library(sm)

attach(mtcars)

cyl.f <- factor(cyl, 
                levels= c(4,6,8),
                labels = c("4 cylinder", 
                           "6 cylinder",
                           "8 cylinder"))

sm.density.compare(mpg, 
                   cyl, 
                   xlab="Miles Per Gallon")

title(main="MPG Distribution by Car Cylinders")

colfill<-c(2:(1+length(levels(cyl.f))))
legend(locator(1), levels(cyl.f), fill=colfill)


detach(mtcars)
