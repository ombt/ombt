dose=c(20,20,40,45,60)
drugA=c(16,20,27,40,60)
drugB=c(15,18,25,31,40)

opar=par(no.readonly=TRUE)

par(pin=c(2,3))
par(lwd=2, cex=1.5)
par(cex.axis=0.75,font.axis=3)

plot(dose, drugA, type="b", pch=19, lty=2, col="red")
x11()
plot(dose, drugB, type="b", pch=23, lty=6, col="blue", bg="green")

