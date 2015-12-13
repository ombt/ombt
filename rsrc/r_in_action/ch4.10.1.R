newdata <- leadership[, c(6:10)]
newdata

myvars <- c("q1", "q2", "q3", "q4", "q5")
newdata <-leadership[myvars]
newdata

myvars <- paste("q", 1:5, sep="")
newdata <- leadership[myvars]
newdata


