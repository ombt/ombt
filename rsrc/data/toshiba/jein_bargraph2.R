# ls()
# jein = jein_load_db_data()
# ls()
# jein
# q()
# ls()
# jein
# names(jein)
# jein$TR01290800_01300800
# tr(jein$TR01290800_01300800)
# help(tr)
# ls()
# tran(jein$TR01290800_01300800)
# tranpose(jein$TR01290800_01300800)
# t(jein$TR01290800_01300800)
# t(jein$TR01290800_01300800[3:,])
# t(jein$TR01290800_01300800[3:])
# t(jein$TR01290800_01300800[3:9,])
# jein$TR01290800_01300800
# t(jein$TR01290800_01300800[,3:9])
# t(jein$TR01290800_01300800[,3:10])
# t(jein$TR01290800_01300800[,3:12])
# t(jein$TR01290800_01300800[,4:12])
# t(jein$TR01290800_01300800[,4:12])
# bd=t(jein$TR01290800_01300800[,4:12])
# bd
# barplot(bd)
# closedevs()
# barplot(bd,col=rainbow(nrow(bd)))
# closedevs()
# barplot(bd,col=rainbow(nrow(bd)),legend=rownames(bd))
# colnames(bd)
# bdm=as.matrix(bd)
# bdm
# colnames(bdm)
# ls()
# names(bd)
# names(jein)
# colnames(bdm)=names(jein)
# bdm
# barplot(bd,col=rainbow(nrow(bd)),legend=rownames(bd))
# closedevs()
# history(50)
# help(history)
# help(history)
# help(history)
# savehistory(file="jein_bargraph.R")

jein = jein_load_db_data()
jein

names(jein)

# bd=t(jein$TR01290800_01300800[,4:12])

# barplot(bd,col=rainbow(nrow(bd)),legend=rownames(bd))

# x11()

# par(mfrow=c(2,1))

data_names = apply(as.matrix(jein$TR01290800_01300800[,1:3]),1,paste,collapse="_")
data_names

data=t(jein$TR01290800_01300800[,4:12])
colnames(data) = data_names
ncol(data)

new_data = data.frame()
new_data_bar_names = ""

for (i in 1:ncol(data))
{
    v = as.vector(data[,i])

    components = v
    components[length(v)] = 0

    total = v
    total[1:(length(v)-1)] = 0

    new_data[1:length(v),2*i-1] = total
    new_data[1:length(v),2*i] = components

    new_data_bar_names[2*i-1] = data_names[i]
    new_data_bar_names[2*i]   = data_names[i]
}

rownames(new_data) = rownames(data)

new_data

barplot(as.matrix(new_data),
        col=rainbow(nrow(new_data)),
        legend=rownames(new_data),
        names.arg=new_data_bar_names)

# colnames(components) = data_names

# total=t(jein$TR01290800_01300800[,12])
# colnames(total) = data_names

# barplot(components,
        # col=rainbow(nrow(components)),
        # legend=rownames(components))

# barplot(total,
        # col=rainbow(nrow(total)),
        # legend=rownames(total))


