# cookbook recipes for dataframes

# 5.18 - initializing data frame from column frame

pred1 = c(1,2,3,4)
pred2 = c(5,6,7,8)
pred3 = c("AM","PM","AM","PM")
resp  = c(31,43,54,65)

dfrm = data.frame(pred1, pred2, pred3, resp)
dfrm

dfrm = data.frame(p1=pred1, p2=pred2, p3=pred3, r=resp)
dfrm

lst = list(p1=pred1, p2=pred2, p3=pred3, r=resp)
dfrm = as.data.frame(lst)
dfrm

# 5.19 - initializing data frame from row data

# dfrm = do.call(rbind, obs)
# where obs is a list of one-row data frames.

obs = list(r1=data.frame(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           r2=data.frame(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           r3=data.frame(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           r4=data.frame(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

dfrm = do.call(rbind, obs)
dfrm

obs = list(data.frame(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           data.frame(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           data.frame(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           data.frame(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

rbind(obs[[1]], obs[[2]])

dfrm = do.call(rbind, obs)
dfrm

dfrm[1]
dfrm[[1]]
dfrm$o1






