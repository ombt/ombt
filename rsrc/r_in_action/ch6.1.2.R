library(vcd)

counts <- table(Arthritis$Improved, Arthritis$Treatment)
counts

x11()
barplot(counts,
        main="Stacked Bar Plot",
        xlab="Treatment", 
        ylab="Frequency",
        col=c("red", "yellow","green"),
        legend=rownames(counts))

x11()
barplot(counts,
        main="Grouped Bar Plot",
        xlab="Treatment", 
        ylab="Frequency",
        col=c("red", "yellow", "green"),
        legend=rownames(counts), 
        beside=TRUE)
