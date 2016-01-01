d = as.Date(c("2015-09-27", "2015-10-27"))
d
as.Date(c("2015-09-27", "2015-10-27"))
myformat <- "%m/%d/%y"
# leadership$date <- as.Date(leadership$date, myformat)
dd = as.Date(leadership$date, myformat)
dd

today <- Sys.Date()
format(today, format="%B %d %Y")
format(today, format="%A")

start = as.Date("1957-09-27")
end = as.Date("2037-09-27")
today = Sys.Date()
today-start
end-start
difftime(today, start, units="weeks")
difftime(end, start, units="weeks")
difftime(today, start, units="secs")
difftime(end, start, units="secs")

