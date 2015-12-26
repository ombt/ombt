
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

# 5.27 - removing NAs from a data frame

dfrm = data.frame(x=c(1,2,NA,4,5), y=c(NA,2,3,NA,NA))

dfrm
cumsum(dfrm)

# na.omit removes ANY row containing an NA
na.omit(dfrm)
cumsum(na.omit(dfrm))

# 5.28 excluding columns by name
#
# subset(dfrm, select=-colname)
#
dfrm = data.frame(id=c(1,2,3,4,5), 
                  pre=c(6,7,8,9,10),
                  dosage=c(7,8,9,10,11),
                  post=c(8,9,10,11,12))
dfrm
cor(dfrm)

dfrm = data.frame(id=c(1,2,3,4,5), 
                  pre=runif(5,min=1,max=2),
                  dosage=runif(5,min=0,max=5),
                  post=runif(5,min=2,max=4))
dfrm

cor(dfrm)
cor(subset(dfrm, select=-id))
cor(subset(dfrm, select=c(-id,-dosage)))

# 5.29 combining two data frames

# all.cols = cbind(dfrm1, dfrm2)
# all.rows = rbind(dfrm1, dfrm2)
#
# rbind, cbind also work with vectors, matrices, and lists

stooges = data.frame(name=c("moe","larry","curly"),
                     n.marry=c(1,1,4),
                     n.child=c(2,2,2))
birth = data.frame(birth.year=c(1887,1902,1903),
                   birth.place=c("bensonhurst","philadelphia","brooklyn"))

stooges
birth

cbind(stooges, birth)

new_stooges = data.frame(name=c("shemp","curly-joe"),
                         n.marry=c(1,1),
                         n.child=c(2,2))
new_birth = data.frame(birth.year=c(1902,1903),
                   birth.place=c("philadelphia","brooklyn"))

rbind(stooges, 
      new_stooges)

rbind(stooges, 
      new_stooges)
rbind(birth, 
      new_birth)

cbind(rbind(stooges, new_stooges),
      rbind(birth, new_birth))

# 5.30 - merge data frames by common column

born = data.frame(name=c("moe","larry","curly"),
                  year.born=c(1887,1902,1903),
                  place.born=c("bensonhurst","philadelphia","brooklyn"))
born

died = data.frame(name=c("curly","larry","moe"),
                  year.died=c(1952,1975,1975))
died

merge(born, died, by="name")


# 5.31 accessing data frame contents more easily

# with(dfrm, expr)
# or
# attach(dfrm)
# ...
# detach(dfrm)

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

z = (suburbs$pop - mean(suburbs$pop))/sd(suburbs$pop)
z

z = with(suburbs, (pop - mean(pop))/sd(pop))
z

attach(suburbs)
search()
z = (pop - mean(pop))/sd(pop)
z

# new local pop. the suburbs pop is NOT affected.
pop = pop/1000
ls()
suburbs$pop
detach()

# 5.32 - converting one atomic value to another atomic value.

# as.character(x)
# as.complex(x)
# as.numeric(x) or as.double(x)
# as.integer(x)
# as.logical(x)

as.numeric(" 3.14 ")
as.integer(3.14)
as.numeric("foo")
as.character(101)

as.numeric(c("1", "2.718", "3.14", "etc"))

as.character(103:105)

as.logical(1.4)

as.numeric(c(TRUE, FALSE))

# 5.33 - converting one data type to another data type

# as.data.frame(x)
# as.list(x)
# as.matrix(x)
# as.vector(x)

# vector to list - as.list(vec)
# vector to matrix - cbind(vec) or as.matrix(vec) 
#                    rbind(vec)
#                    matrix(vec, n, m)
# vector to data frame - as.data.frame(vec) or
#                        as.data.frame(rbind(vec))
# list to vector - unlist(list)
# list to matrix - as.matrix(list) or
#                  as.matrix(rbind(list)) or
#                  matrix(list, b, m,)
# list to data frame - as.data.frame(list) 
#                      recipe 5.19
# matrix to list - as.list(matrix)
# matrix to vector - as.vector(matrix)
# matrix to data frame - as.data.frame(matrix)
# data frame to vector - dfrm[1,] for 1-row frame
#                        dfrm[,1] or dfrm[[1]] for 1-col frame
# data frame to list - as.list(dfrm)
# data frame to matrix - as.matrix(dfrm)


# 

