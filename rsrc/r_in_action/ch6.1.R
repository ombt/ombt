library(vcd)

counts <- table(Arthritis$Improved)
counts

x11()
barplot(counts,
        main="Simple Bar Plot",
        xlab="Improvement", 
        ylab="Frequency")
x11()
barplot(counts,
        main="Horizontal Bar Plot",
        xlab="Frequency", 
        ylab="Improvement",
        horiz=TRUE)

x11()
plot(Arthritis$Improved, 
     main="Simple Bar Plot",
     xlab="Improved", 
     ylab="Frequency")
x11()
plot(Arthritis$Improved, 
     horiz=TRUE, 
     main="Horizontal Bar Plot",
     xlab="Frequency", 
     ylab="Improved")

