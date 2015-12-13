# 
# Table 5.6 Character functions
# Function Description
# nchar(x) Counts the number of characters of x.
# x <- c("ab", "cde", "fghij")
# length(x) returns 3 (see table 5.7).
# nchar(x[3]) returns 5.
# substr(x, start, stop) Extracts or replaces substrings in a character vector.
# x <- "abcdef"
# substr(x, 2, 4) returns bcd.
# substr(x, 2, 4) <- "22222" (x is now "a222ef").
# grep(pattern, x,
# ignore.case=FALSE,
# fixed=FALSE)
# Searches for pattern in x. If fixed=FALSE, then pattern is a
# regular expression. If fixed=TRUE, then pattern is a text string.
# Returns the matching indices.
# grep("A", c("b","A","c"), fixed=TRUE) returns 2.
# sub(pattern, replacement,
# x, ignore.case=FALSE,
# fixed=FALSE)
# Finds pattern in x and substitutes the replacement text. If
# fixed=FALSE, then pattern is a regular expression. If
# fixed=TRUE, then pattern is a text string.
# sub("\\s",".","Hello There") returns Hello.There. Note
# that "\s" is a regular expression for finding whitespace; use
# "\\s" instead, because "\" is R’s escape character (see section
# 1.3.3).
# strsplit(x, split,
# fixed=FALSE)
# Splits the elements of character vector x at split. If
# fixed=FALSE, then pattern is a regular expression. If
# fixed=TRUE, then pattern is a text string.
# y <- strsplit("abc", "") returns a one-component,
# three-element list containing
# "a" "b" "c"
# unlist(y)[2] and sapply(y, "[", 2) both return “b”.
# paste(..., sep="") Concatenates strings after using the sep string to separate them.
# paste("x", 1:3, sep="") returns c("x1", "x2", "x3").
# paste("x",1:3,sep="M") returns c("xM1","xM2" "xM3").
# paste("Today is", date()) returns
# Today is Mon Dec 28 14:17:32 2015
# (I changed the date to appear more current.)
# toupper(x) Uppercase.
# toupper("abc") returns “ABC”.
# tolower(x) Lowercase.
# tolower("ABC") returns “abc”.
# 


# Table 5.7 Other useful functions
# Function Description
# length(x) Returns the length of object x.
# x <- c(2, 5, 6, 9)
# length(x) returns 4.
# seq(from, to, by) Generates a sequence.
# indices <- seq(1,10,2)
# indices is c(1, 3, 5, 7, 9).
# rep(x, n) Repeats x n times.
# y <- rep(1:3, 2)
# y is c(1, 2, 3, 1, 2, 3).
# cut(x, n) Divides the continuous variable x into a factor with n levels. To create
# an ordered factor, include the option ordered_result =
# TRUE.
# pretty(x, n) Creates pretty breakpoints. Divides a continuous variable x into n
# intervals by selecting n + 1 equally spaced rounded values. Often
# used in plotting.
# cat(... , file = "myfile",
# append = FALSE)
# Concatenates the objects in … and outputs them to the screen or
# to a file (if one is declared).
# name <- c("Jane")
# cat("Hello" , name, "\n")


name <- "Bob"
cat( "Hello", name, "\b.\n", "Isn\'t R", "\t", "GREAT?\n")

a <- 5
sqrt(a)

b <- c(1.243, 5.654, 2.99)
round(b)

c <- matrix(runif(12), nrow=3)
c

log(c)

mean(c)

# R provides a function, apply(), that allows you to apply an 
# arbitrary function to any dimension of a matrix, array, or 
# data frame. The format for the apply() function is
#
# apply(x, MARGIN, FUN, ...)
#
# where x is the data object, 
# MARGIN is the dimension index, FUN is a function you specify,
# and ... are any parameters you want to pass to FUN. 
#
# In a matrix or data frame, MARGIN=1 indicates rows and MARGIN=2 
# indicates columns. 
#
# Look at the following examples.
#

mydata <- matrix(rnorm(30), nrow=6)
mydata

apply(mydata, 1, mean)

apply(mydata, 2, mean)

apply(mydata, 2, mean, trim=0.2)

