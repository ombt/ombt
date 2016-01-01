#
# when sourcing to get output on the screen, do this:
#
# > source("ch4.2.R", echo=TRUE)
#

mydata<-data.frame(x1 = c(2, 2, 6, 4),
                   x2 = c(3, 4, 2, 8))
mydata$sumx <- mydata$x1 + mydata$x2
mydata$meanx <- (mydata$x1 + mydata$x2)/2
mydata

mydata$sumx <- NULL
mydata$meanx <- NULL
mydata

attach(mydata)

mydata$sumx <- x1 + x2
mydata$meanx <- (x1 + x2)/2
mydata

mydata$sumx <- NULL
mydata$meanx <- NULL
mydata

detach(mydata)

mydata <- transform(mydata,
                    sumx = x1 + x2,
                    meanx = (x1 + x2)/2)
mydata

mydata$sumx <- NULL
mydata$meanx <- NULL
mydata
