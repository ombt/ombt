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

obs = list(list(o1=1.1, o2=1.2, o3=1.3, o4=1.4),
           list(o1=2.1, o2=2.2, o3=2.3, o4=2.4),
           list(o1=3.1, o2=3.2, o3=3.3, o4=3.4),
           list(o1=4.1, o2=4.2, o3=4.3, o4=4.4))
obs

rbind(obs[[1]], obs[[2]])

dfrm = do.call(rbind, Map(as.data.frame,obs))
dfrm

dfrm[1]
dfrm[[1]]
dfrm$o1

# 5.20 - appending rows to a data frame.

city = c("chicago", "kenosha", "aurora", "elgin", 
         "gary", "joliet", "naperville", "arlington heights", 
         "bolingbrook", "cicero", "evanston", "hammond", 
         "palatine", "schaumburg", "skokie", "waukegan")

county = c("cook", "kenosha", "kane", "elgin", 
           "lake(in)", "kendall", "dupage", "cook", 
           "will", "cook", "cook", "lake(in)", 
           "cook", "cook", "cook", "lake(il)")

state = c("il", "wi", "il", "il", 
          "in", "il", "il", "il", 
          "il", "il", "il", "in", 
          "il", "il", "il", "il")

pop = c(2853114, 90352, 171782, 94487, 
        102746, 106221, 147779, 76031, 
        70834, 72616, 74239, 83048, 
        67232, 75386, 63348, 91452)

suburbs = data.frame(city=city,
                     county=county,
                     state=state,
                     pop=pop)
suburbs

newrow = data.frame(city="west dundee", 
                    county="kane",
                    state="il", 
                    pop=5248)
newrow

suburbs = rbind(suburbs, newrow)
suburbs

suburbs = rbind(suburbs, 
                data.frame(city="west dundee", 
                           county="kane",
                           state="il", 
                           pop=5248),
                data.frame(city="east dundee", 
                           county="kane",
                           state="il", 
                           pop=2955))
suburbs

# 5.21 - preallocating data frames


