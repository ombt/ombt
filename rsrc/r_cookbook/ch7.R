
# chapter 7 - strings and dates

# general purpose classes
#
# Date - supports year, month, day, but not time.
# POSIXct - datetime in seconds from 1/1/1970.
# POSIXlt - datetime as year, month, day, hour, minute, seconds.
#
# conversion functions:
#
# as.Date()
# as.POSIXct()
# as.POSIXlt()
#
# additional packages for download.
#
# chron - datetime class
# lubridate - datetime class
# mondate - months, days and years.
# timeData - high-power datetime class.
#
# help(DateTimeClasses)
#

# 7.1 - length of a string. use nchar() not length()

nchar("Moe")

s = c("moe", "larry", "curly")
nchar(s)

length("Moe")

s = c("moe", "larry", "curly")
length(s)

# 7.2 - concatenate 2 or more strings together
#
# paste(str1, str2, ...)
#

paste("everybody", "loves", "stats")
paste("everybody", "loves", "stats", sep="")

# converts using as.character()
paste("square root of 2*pi is ...", sqrt(2*pi))

stooges = c("moe","larry","curly")
paste(stooges, "loves", "stats")
paste(stooges, "loves", "stats", collapse=", and ")

# 7.3 - extracting substrings
#
# substr(string, start, end)
#

substr("statistics", 1, 4)
substr("statistics", 7, 10)

stooges = c("moe","larry","curly")
substr(stooges, 1, 3)

cities = c("new york, ny", "los angeles, ca", "peoria, il")
substr(cities, nchar(cities)-1, nchar(cities))

# 7.4 - splitting a string using delimiters
#
# strsplit(string, delimiter)
#
# returns a list of vectors
#

getwd()
strsplit(getwd(), "/")

chickens = c("i'm/not/a/chicken",
             "my/wife/is/not/a/chicken",
             "my/sons/are/not/chickens")

strsplit(chickens, "/")

# 7.5 - replacing substrings
#
# sub(old, new, string) - replace first instance of old with new
# gsub(old, new, string) - replace all instances of old with new

s = "curly is the smart one; curly is funny, too"
s
sub("curly", "moe", s)
gsub("curly", "moe", s)

# old pattern is an RE by default. use fixed=TRUE to interprete old
# as a regular string.
#
sub("r.*y", "moe", s)
gsub("r.*y", "moe", s)

# help(regexp)

# 7.6 - seeing special characters in strings.
#
# use print to show them. cat will not show them.

s = "first\rsecond\n"
nchar(s)
cat(s)
print(s)









