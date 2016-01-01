# cookbook recipes for dataframes

# 5.18 - initializing data frame from column frame

pred1 = c(1,2,3,4)
pred2 = c(5,6,7,8)
pred3 = c("AM","PM","AM","PM")
resp  = c(31,43,54,65)

dfrm = data.frame(pred1, pred2, pred3, resp)
dfrm

dfrm = data.frame(p1=pred1, p2=pred2, p3=pred3, r=resp)
dfrm

lst = list(p1=pred1, p2=pred2, p3=pred3, r=resp)
dfrm = as.data.frame(lst)
dfrm

# 5.19 - initializing data frame from row data

# dfrm = do.call(rbind, obs)
# where obs is a list of one-row data frames.

obs = list(r1=data.frame(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           r2=data.frame(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           r3=data.frame(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           r4=data.frame(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

dfrm = do.call(rbind, obs)
dfrm

obs = list(data.frame(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           data.frame(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           data.frame(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           data.frame(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

rbind(obs[[1]], obs[[2]])

dfrm = do.call(rbind, obs)
dfrm

dfrm[1]
dfrm[[1]]
dfrm$o1

obs = list(list(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           list(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           list(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           list(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

rbind(obs[[1]], obs[[2]])

dfrm = do.call(rbind, Map(as.data.frame,obs))
dfrm

dfrm[1]
dfrm[[1]]
dfrm$o1

# 5.20 - appending rows to a data frame.

city = c("chicago", "kenosha", "aurora", "elgin", 
         "gary", "joliet", "naperville", "arlington heights", 
         "bolingbrook", "cicero", "evanston", "hammond", 
         "palatine", "schaumburg", "skokie", "waukegan")

county = c("cook", "kenosha", "kane", "elgin", 
           "lake(in)", "kendall", "dupage", "cook", 
           "will", "cook", "cook", "lake(in)", 
           "cook", "cook", "cook", "lake(il)")

state = c("il", "wi", "il", "il", 
          "in", "il", "il", "il", 
          "il", "il", "il", "in", 
          "il", "il", "il", "il")

pop = c(2853114, 90352, 171782, 94487, 
        102746, 106221, 147779, 76031, 
        70834, 72616, 74239, 83048, 
        67232, 75386, 63348, 91452)

suburbs = data.frame(city=city,
                     county=county,
                     state=state,
                     pop=pop)
suburbs

newrow = data.frame(city="west dundee", 
                    county="kane",
                    state="il", 
                    pop=5248)
newrow

suburbs = rbind(suburbs, newrow)
suburbs

suburbs = rbind(suburbs, 
                data.frame(city="west dundee", 
                           county="kane",
                           state="il", 
                           pop=5248),
                data.frame(city="east dundee", 
                           county="kane",
                           state="il", 
                           pop=2955))
suburbs

# 5.21 - preallocating data frames

n=30
dfrm = data.frame(c1=numeric(n), 
                  c2=character(n), 
                  c3=logical(n), 
                  c4=factor(n, levels=c("L1", "L2", "L3")))
dfrm

# 5.22 selecting data frame columns by position
#
# dfrm[[n]] - selects one column
#
# to select one or more than one column:
#
# dfrm[n]
# dfrm(c(n1,...,nk))
# dfrm[,n]
# dfrm[c(n1m,...,nk)]

suburbs = data.frame(city=city,
                     county=county,
                     state=state,
                     pop=pop)
suburbs

# returns a vector
class(suburbs[[1]])
suburbs[[1]]

# returns a data frame
class(suburbs[1])
suburbs[1]
class(suburbs[c(1,3)])
suburbs[c(1,3)]

# returns a vector
class(suburbs[,1])
suburbs[,1]

# however this returns a data frame
class(suburbs[,c(1,4)])
suburbs[,c(1,4)]

# force a data frame
class(suburbs[,1,drop=FALSE])
suburbs[,1,drop=FALSE]

# 5.23 - selecting a data frame column by name

# dfrm[["name"]] - selects one column
# dfrm$name - selects one column
#
# to select one or more than one column:
#
# dfrm["name"]
# dfrm(c("name",...,"name"))
# dfrm[,"name"]
# dfrm[c("name",...,'name")]

# returns a vector
class(suburbs[["city"]])
suburbs[["city"]]
class(suburbs$city)
suburbs$city

# returns a data frame
class(suburbs["city"])
suburbs["city"]
class(suburbs[c("city","state")])
suburbs[c("city","state")]

# returns a vector
class(suburbs[,"city"])
suburbs[,"city"]

# however this returns a data frame
class(suburbs[,c("city","pop")])
suburbs[,c("city","pop")]

# force a data frame
class(suburbs[,"city",drop=FALSE])
suburbs[,"city",drop=FALSE]

# 5.24 - selecting rows and cols more easily

# use subset:
# subset(dfrm, select=colname)
# subset(dfrm, select=c(colname,...,colname))
#
# no quotes needed.
#
# can also give a logical expression to select 
# specific values.
#
# subset(dfrm, subst=(logical-expr))
# subset(dfrm, select=c(colname,...,colname), subst=(logical-expr))
#

library(MASS)
data(Cars93)
names(Cars93)

subset(Cars93, 
       select=Model, 
       subset=(MPG.city>30))

subset(Cars93, 
       select=c(Model,Min.Price,Max.Price),
       subset=(Cylinders==4 & Origin=="USA"))

subset(Cars93, 
       select=c(Manufacturer, Model),
       subset=(MPG.highway > median(MPG.highway)))

# 5.25 changing the name of a data frame column

m = matrix(c(1,2,3,
             4,5,6,
             7,8,9,
             10,11,12,
             13,14,15),
             ncol=3,
             nrow=5,
             byrow=TRUE)
m

as.data.frame(m)

l = list(c(1,2,3), 
         c(4,5,6), 
         c(7,8,9), 
         c(10,11,12), 
         c(13,14,15))
l
as.data.frame(l)

dfrm = as.data.frame(l)
colnames(dfrm) = c("Moe", "Larry", "Curly", "Shemp", "Curly-Joe")
dfrm

# 5.26 - editing a data frame

temp = edit(dfrm)
dfrm = temp
dfrm

# 5.27 - removing NAs from a data frame

dfrm = data.frame(x=c(1,2,NA,4,5), y=c(NA,2,3,NA,NA))



