
# navigating in the R software

# 3.1
oldwd = getwd()
oldwd

setwd("/tmp")
getwd()

setwd(oldwd)
getwd()

# 3.2
save.image()

# 3.3
history()
history(30)
history(Inf)

# 3.4
sin((1:10)*pi/33)
result <- .Last.value
result

# 3.5
search()

# 3.6
library(RSQLite)

library(MASS)

detach(package:MASS)

# 3.7
data()
data(pressure)
data(Cars93, package="MASS")
summary(Cars93)
head(Cars93)

data(package="MASS")

# 3.8
library()
installed.packages()[,c("Package","Version")]

# 3.9
# install.packages("RSQLite")
#
# $ mkdir -P ~/lib/R
# $ export R_LIBS=~/lib/R
# $ R
# > install.packages("some.package", lib="~/lib/R")

# 3.10
chooseCRANmirror()
options("repos")[[1]][1]
# options(repos="above URL")

# 3.12
# source("script.name", echo=TRUE)

# 3.13
# running a batch script
# $ R CMD BATCH script outputfile

# some options and other choices
#
# $ R CMD BATCH --quiet script.R outputfile
#
# or use Rscript
# $ Rscript script.R arg1 arg2 ...
#
# access args from within script as follows:
#
# argv <- commandArgs(TRUE)

# in a script you can use the #! syntax and make the script
# executable as is:

#!/usr/bin/Rscript --slave
# R commands

# if you don't want .Rdata to be read in, then use the 
# option --no-restore. To keep the data from being saved, then
# use the option --no-save.

# 3.14
Sys.getenv("SHELL")
Sys.setenv(SHELL="/bin/ksh");

Sys.getenv("DISPLAY")

# 3.15
Sys.getenv("R_HOME")

# 3.16
help(options)

# start up sequence
# 1) site profile, Rprofile.site found under R_HOME/etc/Rprofile.site
# 2) .Rprofile in the working directory or in user home directory.
# 3) load .Rdata.
# 4) execute .First function if you define one in the .Rprofile.
# 5) .First.sys function.




