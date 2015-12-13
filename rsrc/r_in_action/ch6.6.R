#
# Dot plots provide a method of plotting a large number of labeled 
# values on a simple horizontal scale. You create them with the 
# dotchart() function, using the format
#
# dotchart(x, labels=)
#
# where x is a numeric vector and labels specifies a vector that 
# labels each point. You can add a groups option to designate a 
# factor specifying how the elements of x are grouped. If so, the 
# option gcolor controls the color of the groups label, and cex 
# controls the size of the labels

dotchart(mtcars$mpg, 
         labels=row.names(mtcars), 
         cex=.7,
         main="Gas Mileage for Car Models",
         xlab="Miles Per Gallon")

x11()
x <- mtcars[order(mtcars$mpg),]

x$cyl <- factor(x$cyl)

x$color[x$cyl==4] <- "red"
x$color[x$cyl==6] <- "blue"
x$color[x$cyl==8] <- "darkgreen"

dotchart(x$mpg,
         labels = row.names(x),
         cex=.7,
         groups = x$cyl,
         gcolor = "black",
         color = x$color,
         pch=19,
         main = "Gas Mileage for Car Models\ngrouped by cylinder",
         xlab = "Miles Per Gallon")

