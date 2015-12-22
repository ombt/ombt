
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

 
