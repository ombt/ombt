#!/usr/bin/ksh
if [[ ! -z "${REMOTE_USER}" ]]
then
	echo "<b>${REMOTE_USER}</b>"
else
	echo "<b>UNKNOWN USER</b>"
fi
#
exit 0
