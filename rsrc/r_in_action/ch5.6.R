
data(mtcars)
mtcars

cars <- mtcars[1:5,1:4]
cars

t(cars)

# It’s relatively easy to collapse data in R using one or more 
# by variables and a defined function. The format is
#
# aggregate(x, by, FUN)
#
# where x is the data object to be collapsed, by is a list of 
# variables that will be crossed to form the new observations, and 
# FUN is the scalar function used to calculate summary
# statistics that will make up the new observation values.
#
# aggregate works like "group by" in SQL. it gathers all the
# data for given unique set of values.
#
mtcars

options(digits=3)
attach(mtcars)
aggdata <-aggregate(mtcars, 
                    by=list(Group.cyls=cyl,
                            Group.gears=gear), 
                    FUN=mean, 
                    na.rm=TRUE)
aggdata


#
# can use sqldf to get similar results as using aggregate. compare
# the wt and hp columns from above the results below.
#
# sql_join_roads <- sqldf(join_string,stringsAsFactors = FALSE)
#
library(sqldf)
grp_by_sql <- "
select 
    cyl, 
    gear, 
    avg(wt) as avg_wt, 
    avg(hp) as avg_hp 
from 
    mtcars
group by
    gear,
    cyl
order by
    gear,
    cyl"

sqldf(grp_by_sql, stringsAsFactors = FALSE)

# reshape2 function

library(reshape2)

ID   = c(1,1,2,2)
TIME = c(1,2,1,2)
X1   = c(5,3,6,2)
X2   = c(6,5,1,4)

rsd = data.frame(ID,TIME,X1,X2)
rsd

md = melt(rsd, id=c("ID","TIME"))
md

# The dcast() function starts with a melted data frame and 
# reshapes it into a new data frame using a formula that you provide 
# and an (optional) function used to aggregate the data. The format 
# is
#
# newdata <- dcast(md, formula, fun.aggregate)
#
# where md is the melted data, formula describes the desired end 
# result, and fun.aggregate is the (optional) aggregating function. 
# The formula takes the form 
#
# rowvar1 + rowvar2 + ... ~ colvar1 + colvar2 + ...
#
# In this formula, rowvar1 + rowvar2 + ... defines the set of 
# crossed variables that defines the rows, and 
# colvar1 + colvar2 + ... defines the set of crossed 
# variables that defines the columns.

dcast(md, ID~variable, mean)
dcast(md, TIME~variable, mean)
dcast(md, ID~TIME, mean)
dcast(md, ID+TIME~variable)
dcast(md, ID+variable~TIME)
dcast(md, ID~variable+TIME)

