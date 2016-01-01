
newdata <- subset(leadership, 
                  age >= 35 | age < 24,
                  select=c(q1, q2, q3))
newdata

newdata <- subset(leadership, 
                  gender=="M" & age > 25,
                  select=Sex:q3)
newdata

