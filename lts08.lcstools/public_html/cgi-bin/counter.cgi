#!/usr/bin/ksh
#
# update counters
#
##########################################################################
#
integer i
#
key=${1}
hits=${2}
#
if [[ ! -r "${hits}" ]]
then
	i=1
	echo "${key}	${i}" >>${hits}
	echo "<p><b>Hits for ${key}: ${i}</b>"
	exit 0
fi
#
grep "^${key}	" $hits | cut -d'	' -f2 >/tmp/$$.1
#
if [[ -s "/tmp/$$.1" ]]
then
	i=$(cat /tmp/$$.1)
	i=i+1
	grep -v "^${key}	" $hits >/tmp/$$.1
	echo "${key}	${i}" >>/tmp/$$.1
	cp /tmp/$$.1 ${hits}
else
	i=1
	echo "${key}	${i}" >>${hits}
fi
#
cat ${hits} | sort -u >/tmp/$$.1
cp /tmp/$$.1 ${hits}
#
rm -f /tmp/$$.1 2>/dev/null
#
echo "<p><b>Hits for ${key}: ${i}</b>"
#
exit 0
