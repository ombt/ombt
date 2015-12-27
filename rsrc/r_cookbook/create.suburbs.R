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
head(suburbs)
