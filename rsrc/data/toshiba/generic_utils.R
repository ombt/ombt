#
# general purpose utils
#
closedevs <- function()
{
    while (dev.cur() > 1) { dev.off() }
}

