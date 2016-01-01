myvars <- names(leadership) %in% c("q3", "q4")
newdata <- leadership[!myvars]
newdata

newdata <- leadership[c(-8,-9)]
newdata

newdata <- leadership
newdata$q3 <- newdata$q4 <- NULL
newdata


