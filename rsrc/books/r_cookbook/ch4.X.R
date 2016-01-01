pi
print(pi, digits=3)
100*pi
print(100*pi, digits=3)

cat(pi, "\n")
cat(format(pi, digits=3), "\n")

pnorm(-3:3)
print(pnorm(-3:3), digits=3)

q = seq(from=0, to=3, by=0.5)
tbl = data.frame(Quant=q, Lower=pnorm(-q), Upper=pnorm(q))
tbl
print(tbl, digits=2)

# for all output change the option
options("digits")
pi
options(digits=15)
pi
options(digits=7)
options("digits")

# output to a file
# cat("this output goes to a file", "\n", file="/tmp/R.out")
# system("cat /tmp/R.out")

sink("/tmp/R.out2")
system("cat /tmp/R.out")
sink()

system("cat /tmp/R.out2")

# sink()

# redirecting out from a script
# > sink("/tmp/out.file")
# > source("/some/R/script")
# > sink()

# writing to a file
filename = "/tmp/q.out"
con = file(filename, "w")

anything = "hi! this is anything!"

cat(anything, file=con)

close(con)

# listin files

list.files()
list.files(all.files=TRUE)

#
# write a tab-separated file out, then read it in.
#
filename = "/tmp/funky.joes.out"
con = file(filename, "w")
cat("first","\t","last","\t","age","\n", file=con)
cat("mike","\t","rumore","\t","58","\n", file=con)
cat("inez","\t","rumore","\t","53","\n", file=con)
cat("mark","\t","rumore","\t","19","\n", file=con)
cat("jacob","\t","rumore","\t","17","\n", file=con)
close(con)

funky.joes = read.table(filename, 
                        sep="\t",
                        header=TRUE,
                        stringsAsFactor=FALSE)
funky.joes

funky.joes = read.csv(filename, 
                      sep="\t",
                      header=TRUE,
                      as.is=TRUE)
funky.joes
class(funky.joes$age)

# writing csv files. the columns are weird.
names(funky.joes)

write.csv(funky.joes, file="/tmp/xxx")
system("cat /tmp/xxx")

write.csv(funky.joes, file="/tmp/xxx", quote=FALSE, row.names=FALSE)
system("cat /tmp/xxx")

# reading csv data from the web
#
# using http ...
# tbl = read.csv("http://www.example.com/download/data.csv")
#
# or using ftp ...
# tbl = read.csv("ftp://www.example.com/download/data.csv")

# reading file with non-standard formats, then use readLines or scan.

u03 = readLines("test.u03", n=-1)
u03

# saving data to a file and reading it in.

q = seq(from=0, to=3, by=0.5)

test.data = data.frame(Quant=q, Lower=pnorm(-q), Upper=pnorm(q))

save(test.data, file="test.data.out")

rm(test.data)
ls()

load(file="test.data.out")

ls()

dput(test.data, file="ascii_test.data")
dump("test.data2", file="ascii_test.data")







