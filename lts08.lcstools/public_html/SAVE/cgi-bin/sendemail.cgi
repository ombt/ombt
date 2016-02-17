#!/bin/ksh
echo "Content-type: text/html"
echo
cat ../index.html
exit 0
#
# remove "&" signs from query
#
#set -- $(echo ${QUERY_STRING} | sed 's/&/ /g; s/=/ /g;')
set -- ${QUERY_STRING}
#
echo "Content-type: text/html"
echo
#
echo "<html>"
echo "<head>"
echo "<title> send email</title>"
echo "<body bgcolor=yellow>"
#
echo "<h1>"
echo "${*}"
echo "</h1>"
#
echo "</body>"
echo "</html>"
#
exit 0

