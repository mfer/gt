d=read.csv("data", sep="\t")
jpeg("data-log.jpg")
plot(d,main="Dealer Histogram for 'stands on 17' strategy",log="y")
dev.off()
