both=read.csv("both.data", sep="\t")
jpeg("both.jpg")
plot(both,main="Dealer Histogram for 'stands on 17' strategy")
dev.off()

#hist(both$cases)
#jpeg("both-log.jpg")
#plot(both,main="Dealer Histogram for 'stands on 17' strategy",log="y")
#dev.off()
