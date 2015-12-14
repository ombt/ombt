
# list all libraries 
library()

colnames(installed.packages())

rownames(installed.packages())

head(installed.packages())

# this WILL NOT work when accessing rows via a row name.
installed.packages()["DBI"]

# this WILL work when accessing rows via a row name.
installed.packages()["DBI",]
installed.packages()[c("DBI","digest"),]

installed.packages()[,c("Package","Version")]



