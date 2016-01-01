
# for (var in seq) statement
for (i in 1:10) print("Hello")

# while (cond) statement
i <- 10
while (i > 0) { print("Hello"); i <- i - 1 }

# if (cond) statement
# if (cond) statement1 else statement2
for (i in 1:10)
{
	if (i <= 5)
		print(" i <= 5 ...")
	else
		print(" i > 5 ...")
}

# IFELSE
# The ifelse construct is a compact and vectorized version of 
# the if-else construct.
#
# The syntax is
#
# ifelse(cond, statement1, statement2)
#
# The first statement is executed if cond is TRUE. If cond 
# is FALSE, the second statement is executed.

for (i in ceiling(runif(50, 1, 100)))
{
	print(paste("i is ...", i))
	if (i<50)
		print(paste("i < 50", i))
	else
		print(paste("i >= 50", i))
}

ifelse(ceiling(runif(50, 1, 100))<50, 
       print(paste("i < 50")), 
       print(paste("i >= 50")))

print("-------------------");

ivals = ceiling(runif(50, 1, 100))

print("-------------------");

ifelse(ivals<50, 
       print(paste("i < 50", ivals)), 
       print(paste("i >= 50", ivals)))

print("-------------------");

ifelse(ivals<50, ivals, -ivals)

print("-------------------");

lt50 = ifelse(ivals<50, TRUE, FALSE)
ge50 = ! lt50

ivals[lt50]

ivals[ge50]

# switch chooses statements based on the value of an expression. 
# The syntax is
#	switch(expr, ...)
# where ... represents statements tied to the possible outcome 
# values of expr. It’s easiest to understand how switch works by 
# looking at the example in the following listing.

feelings <- c("sad", "afraid")

for (i in feelings)
	print(
		switch(i,
		happy = "I am glad you are happy",
		afraid = "There is nothing to fear",
		sad = "Cheer up",
		angry = "Calm down now"
		)
	)




