
newdata <- leadership[order(leadership$age),]
newdata

attach(leadership)
newdata <- leadership[order(gender, age),]
newdata
detach(leadership)

attach(leadership)
newdata <- leadership[order(gender, -age),]
newdata
detach(leadership)



