#!/usr/bin/ksh
# du of LCS files systems.
#
lcssyslog -q syscalls ${0} ${*}
#
export usagecmd=$(basename ${0})
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1;' 0 1 2 15
#
textonly=
details=
usage="\n
usage: ${usagecmd} [-?xd] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-t - only give text directory details\n
\n
"
#
set -- $(getopt ?xt ${*})
if [[ ${?} -ne 0 ]]
then
	echo $usage >&2
	exit 2
fi
#
for i in ${*}
do
	case "${i}" in
	-x)
		set -x
		shift
		;;
	-t)
		details='/*'
		textonly='where type req text'
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo >&2
	echo "LCSTOOLSDATA is not set." >&2
	exit 2
fi
if [[ ! -d "${LCSTOOLSDATA}" ]]
then
	echo >&2
	echo "$LCSTOOLSDATA does not exist." >&2
	exit 2
fi
#
cd ${LCSTOOLSDATA}
#
uprintf -q -f"%s\n" path in filesystems ${textonly} | 
sort -u |
while read fs
do
	cd ${fs}
	/usr/bin/du -sk ${fs}${details}
done
#
exit 0
