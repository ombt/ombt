#!/bin/bash -x
#
TZ="Asia/Jakarta"
export TZ
#
(
cat <<EOF
01/29/2015 08:00:00
01/30/2015 08:00:00
01/30/2015 08:00:00
01/30/2015 10:00:00
EOF
) |
while read dia 
do
	date -d "${dia}" '+%s'
done
#
(
cat <<EOF
1422540000
1422626400
EOF
) |
while read dia 
do
	date -d@${dia}
done
#
exit 0
