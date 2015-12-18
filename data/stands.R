png("stands.png")
s=read.csv("stands.data", sep=" ")
plot(s,type="b",main="Expected Winnings for ever 1 unit spent",xlim=c(min(s$playerHand),max(s$playerHand)),ylim=c(min(s$winnings),0.476722),lty=3); grid();
abline(h = 0.440602, lty=2, xlim=c(4,20)); abline(h = 0.476722, lty=1); legend(c("optimal","wizard","standsOn"), lty=c(1,2,3), x=10, y=0.3);
dev.off()
