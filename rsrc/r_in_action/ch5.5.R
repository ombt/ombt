mystats <- function(x, parametric=TRUE, print=FALSE)
{
    if (parametric)
    {
        center <- mean(x); 
        spread <- sd(x)
    }
    else
    {
        center <- median(x); 
        spread <- mad(x)
    }

    if (print & parametric)
    {
        cat("Mean=", center, "\n", "SD=", spread, "\n")
    }
    else if (print & !parametric)
    {
        cat("Median=", center, "\n", "MAD=", spread, "\n")
    }

    result <- list(center=center, spread=spread)

    return(result)
}

mystats2 <- function(x, parametric=TRUE, print=FALSE)
{
    if (parametric)
    {
        center <- mean(x); 
        spread <- sd(x)
        if (print)
        {
            cat("Mean=", center, "\n", "SD=", spread, "\n")
        }
    }
    else
    {
        center <- median(x); 
        spread <- mad(x)
        if (print)
        {
            cat("Median=", center, "\n", "MAD=", spread, "\n")
        }
    }


    result <- list(center=center, spread=spread)

    return(result)
}

set.seed(1234)
x <- rnorm(500)

y = mystats(x)
y

y <- mystats(x, parametric=FALSE, print=TRUE)
y

y = mystats2(x)
y

y <- mystats2(x, parametric=FALSE, print=TRUE)
y

mydate <- function(type="long")
{
    switch(type,
    long = format(Sys.time(), "%A %B %d %Y"),
    short = format(Sys.time(), "%m-%d-%y"),
    cat(type, "is not a recognized type\n")
    )
}

mydate("long")
mydate("short")
mydate()
mydate("medium")

