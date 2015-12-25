
v = c(10,20,30)

names(v) = c("moe","larry","culy")

print(v)

v["moe"]

mode(v)

mode(3.14159265)

mode(c(2.718281828, 3.141592647))

mode("moe")

mode(list("moe","larry","curly"))

class(v)

class(3.14159265)

class(c(2.718281828, 3.141592647))

class("moe")

class(list("moe","larry","curly"))

pp=3.141592647
pp
pp[1]
pp[2]

A=1:6
A
dim(A)

dim(A) = c(2,3)
A
dim(A)


D = 1:32
D

dim(D) = c(2,4,4)
D

C = list(1,2,3,"A","B","C")
C
dim(C) = c(2,3)
C

# factors - enumerated values
# factors are categorical variables and used in
# contingency tables, linear regression, anova, logistic 
# regression, ...

# data frames are list:
# the elements of the list are vectors or factors.
# vectors and factors are the columns in the data frame.
# vectors and factors all have the same length.
# rectangular in shape.
# columns must have names.

# since data frames are both lists and rectangular, you can 
# access the data in two ways:
#
# use list syntax to access columns:
# dfrm[1], dfrm[[1]], or dfrm$name.
#
# you can use matrix-like notation:
# dfrm[i,j], dfrm[i,] and dfrm[,j]
#

# appending data to a vector.
v = c(1,2,3,4)
v

newv = c(5,6,7,8)
newv

v = c(v,newv)
v

v[length(v)+1] = 9
v

v = c(1,2,3)
v

v[10] = 10
v

v = 1:10
v

newv = append(v, 10:20, after=5)
newv
v

# recycle rule
1:6
1:3

1:6 + 1:3

cbind(1:6)

cbind(1:3)

cbind(1:6, 1:3)

# (1:6) + (1:5)
# In (1:6) + (1:5) :
#   longer object length is not a multiple of shorter object length

# 5.4 - creating a factor

f = c("a","b","c","a","b","c","c","c")
f
f = factor(f)
f

f.levels = c("a","b","c","d","e","f","g")
f.levels

f = c("a","b","c","a","b","c","c","c")
f
f = factor(f,f.levels)
f

# 5.5 - combining vectors into one vector and a factor.

v1 = c("v1-1", "v1-2", "v1-3", "v1-4")
v2 = c("v2-1", "v2-2", "v2-3", "v2-4")
v3 = c("v3-1", "v3-2", "v3-3", "v3-4")
v4 = c("v4-1", "v4-2", "v4-3", "v4-4")

# the factors are v1,v2,v3,v4 and the values
# the vector of values as grouped by the
# factors v1,v2,v3,v3, in this case.
#
comb = stack(list(v1=v1, v2=v2, v3=v3, v4=v4))
comb

# 5.6 - creating a list

# list positions are identified by the notation [[1]], [[2]], etc.
# lists are linear data structures. lists are heterigensous, that is, 
# they can contain data items of all types.
#

demolist = list(3.14159, "Moe", c(1,2,3,4), mean)
demolist

demolist = list()
demolist

demolist[[1]] = 3.14159
demolist[[2]] = "Moe"
demolist[[3]] = c(1,2,3,4)
demolist[[4]] = mean

demolist

demolist = list(mid=0.5, right=0.841, far.right=0.977)
demolist

demolist$far.far.right =  1.5555
demolist

# 5.7 accessing list elements

# list[[n]] to access the n-th element.
# list[c(n1,n2,...,n3)] to access the n1, n2, ... elements
#
# very important, list[[n]] returns a one atomic element, but
# list[n] returns a list with one element.
#

years = list(1957, 1962, 1996, 1998, 2008, 2014)
years

years[[1]]

years[c(1,2)]

class(years[[1]])
class(years[1])

# also remember, the cat() function can handle cat(list[[1]]) but
# it fails for cat(list[1]) because the item returned has internal
# structure, it is not atomic.
#

# 5.8 accessing list elements by name.

# list[["name"]]
# or
# list$name
# or
# list[c(name,name2,...,namen)]

years = list(kennedy=1960, johnson=1964, carter=1976, clinton=1992)

years["kennedy"]

years[["kennedy"]]

years$kennedy

(years["kennedy"])[1]

class(years["kennedy"])

class(years[["kennedy"]])

class(years$kennedy)

class((years["kennedy"])[1])

# 5.9 - name-value table

htbl = list(mid=0.5, right=0.841, left=0.977)
htbl

nms = c("mike", "inez", "mark", "jacob")
vals = c("rumore", "rumore", "chicken", "duck")

htbl2 = list()

htbl2

htbl2[nms] = vals
htbl2

# 5.10 deleting item in a list

htbl2
htbl2$mike = NULL
htbl2

# 5.11 = flattening a list to a vector.
# use unlist function.

iq.scores = list(a=103, b=110, c=90, d=145)
iq.scores

# the following will complain. must unlist as shown right below.
# mean(iq.scores)

mean(unlist(iq.scores))

# to print list using cat, ...

cat("IQ scores:", unlist(iq.scores), "\n")

# 5.12 - removing NULLs from a list.

tlist = list("moe", NULL, "larry", NULL, "curly", NULL, "shemp")

tlist

# remove nulls from list

tlist[sapply(tlist, is.null)] = NULL

tlist


# 5.13 - removing elements from a list using a conditional

iq.scores = list(a=103, b=110, c=90, d=145, e=75, f=180)
iq.scores

# remove scores below the average

mean(unlist(iq.scores))

iq.scores[iq.scores < mean(unlist(iq.scores))] = NULL
iq.scores

scores = as.list(runif(20, min=-50, max=50))
scores

abs(unlist(scores)) < 25

scores[abs(unlist(scores)) < 25] <- NULL
scores

scores = as.list(runif(20, min=-50, max=50))
scores

scores[lapply(scores, abs) < 25] <- NULL
scores

# creating a matrix

v = c(1,2,3,4,5,6)

matrix(v, 2, 3)

# uses repeat also

z = 1:12
z

# by column is the default
matrix(z,3,4)

# try by row instead
matrix(z,3,4, byrow=TRUE)

matrix(0,3,2)

matrix(NA,2,4)

theData = c(1.1, 1.2, 1.3,
            2.1, 2.2, 2.3)
mat = matrix(theData, 2, 3, byrow=TRUE)
mat

v = c(1.1, 1.2, 1.3,
      2.1, 2.2, 2.3)

dim(v) = c(2,3)

# notice it did it by column and not by row. not to good.
v

# 5.15 - matrix ops

# t(A) - transpose

m = matrix(c(1,2,3,
             4,5,6,
             7,8,9),
           3,3,byrow=TRUE)
m
t(m)

m2 = matrix(c(1,5,3,
              4,3,6,
              7,9,9),
            3,3,byrow=TRUE)

solve(m2)

solve(t(m2))

m %*% m2

diag(4)

diag(5)

# 5.16 - row and col names for a matrix

tech.corr = matrix(c(1.000, 0.566, 0.390,
                     0.556, 1.000, 0.444,
                     0.390, 0.444, 1.000),
                   3,3,byrow=TRUE)
tech.corr

# add names
rownames(tech.corr) = c("ibm", "msft", "goog")
colnames(tech.corr) = c("ibm", "msft", "goog")
tech.corr

tech.corr["ibm","goog"]

# 5.17 - dropping row or col from a matrix

m = matrix(1:12, 3, 4, byrow=TRUE)
m

# converted to a vector
m[2,]
m[,2]

# 1-row matrix or 1-col matrix
m[2,,drop=FALSE]
m[,2,drop=FALSE]



