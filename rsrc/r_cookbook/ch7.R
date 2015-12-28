
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

# 7.7 - generating all pairwise combination of strings.
#
# m = outer(strings1, strings2, paste, sep="")
#
locations = c("ny","la","chi","hou")
treatments = c("t1","t2","t3")
o = outer(locations, treatments, paste, sep="-")
o

str(o)
as.vector(o)

outer(treatments, treatments, paste, sep="-")
as.vector(outer(treatments, treatments, paste, sep="-"))
sort(as.vector(outer(treatments, treatments, paste, sep="-")))
unique(sort(as.vector(outer(treatments, treatments, paste, sep="-"))))

o=outer(treatments, treatments, paste, sep="-")
o[!lower.tri(o)]

# 7.8 - get current date
#
# Sys.Date()

today = Sys.Date()
today

class(today)
mode(today)
str(today)

# 7.9 - convert a string to a date
#
# can use as.Date(...)
#

# default format is yyyy-mm-dd
as.Date("2015-09-27")

# other formats see man page for stftime()
as.Date("12/31/2010", format="%m/%d/%Y")
as.Date("12/31/10", format="%m/%d/%y")

# 7.10 - convert Date to string.
#
# for format strings, can use these:
#
# %b - abbreviated month, "Jan", etc.
# %B - full month name, "January", etc.
# %d - day as 2-digit number
# %m - month as 2-digit number
# %y - year without century, (00-99)
# %Y - year as yyyy.

format(Sys.Date())

as.character(Sys.Date())

format(Sys.Date(), format="%m/%d/%Y")
as.character(Sys.Date(), format="%m/%d/%Y")

# 7.11 - convert year, month, date to a Date
#
# ISOdate(year, month, day)
#
# to create a POSIXct object which is
# then converted using as.Date()
#
ISOdate(2015, 12, 27)

# truncate time part
as.Date(ISOdate(2015, 12, 27))

years = c(2010, 2011, 2012, 2013, 2014)
months = c(1,1,1,1,1)
days = c(15, 21, 20, 18, 17)

ISOdate(years, months, days)

as.Date(ISOdate(years, months, days))

# to handle date and times
hours = c(12,17,01,02,23)
minutes = c(1,10,12,40,48)
seconds = c(10,20,30,40,50)

ISOdatetime( years, months, days, hours, minutes, seconds)

# 7.12 - getting the Julian date
#

d = as.Date("2015-12-28")
d
as.integer(d)
julian(d)

as.integer(as.Date("1970-01-01"))
as.integer(as.Date("1970-01-02"))
as.integer(as.Date("1970-01-03"))

# 7.13 - extracting parts of a date
#
d = as.Date("2015-12-28")
p = as.POSIXlt(d)

str(p)
class(p)
mode(p)

p$sec
p$min
p$hour
p$mday
p$mon
1900+p$year
p$wday
p$yday
p$idst

# 7.14 - creating sequence of days

start = as.Date("2015-11-20")
end = as.Date("2015-12-24")

seq(from=start, to=end, by=1)

seq(from=start, by=1, length.out=14)

seq(from=start, by="month", length.out=12)
seq(from=start, by="3 months", length.out=4)
seq(from=start, by="year", length.out=10)





