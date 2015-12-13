newdata <- leadership[leadership$gender=="M" &
                      leadership$age > 30,]
newdata

attach(leadership)
newdata <- leadership[gender=="M" & age > 30,]
newdata
detach(leadership)

startdate <- as.Date("2008-10-01")
enddate <- as.Date("2008-10-31")

newdata <- 
    leadership[which(as.Date(leadership$date,"%m/%d/%Y") >= startdate &
                     as.Date(leadership$date,"%m/%d/%Y") <= enddate),]

newdata

