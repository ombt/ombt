#!/usr/bin/ksh
#
MAXTIME=${1}
if [[ -z "${MAXTIME}" ]]
then
	MAXTIME=3600
fi
#
trap 'echo "SECONDS=${I}"; exit 0;' 1 2 15
#
integer I
export I=0
#
while [[ $I -le $MAXTIME ]]
do
	sleep 1
	I=$I+1
	echo "I=$I"
done
#
exit 0
