
# selecting vector elements

fib <- c(0,1,1,2,3,5,8,13,21,34)

fib
fib[1]
fib[2]
fib[3]

for (i in 1:length(fib))
{
    print(paste("fib[",i,"] = ",fib[i],sep=""))
}

fib[1:3]
fib[c(1,2,4,8)]
fib[-1]
fib[1:3]
fib[-(1:3)]

fib < 10
fib[fib<10]

fib%%2 == 0
fib[fib%%2 == 0]

v[v > median(v)]
v[(v<quantile(v,0.05)) | (v>quantile(v, 0.95))]

v[abs(v-mean(v))>2*sd(v)]
v[ !is.na(v) & !is.null(v)]

years <- c(1960,1964,1976,1994)
names(years) <- c("Kennedy","Johnson","Carter","Clinton")

years

years["Carter"]
years["Clinton"]

years[c("Carter","Clinton")]



