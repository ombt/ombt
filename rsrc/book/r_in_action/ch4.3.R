#
leadership$age[leadership$age == 99] <- NA
#
leadership <- within(leadership,
{
    agecat <- NA
    agecat[age > 75] <- "Elder"
    agecat[age >= 55 & age <= 75] <- "Middle Aged"
    agecat[age < 55] <- "Young"
})
