#!/usr/bin/ksh
# du of LCS files systems.
#
lcssyslog -q syscalls ${0} ${*}
#
export usagecmd=$(basename ${0})
#
usage="\n
usage: ${usagecmd} [-?x] [branch] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\n
"
#
set -- $(getopt ?x ${*})
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
if [[ $# == 0 ]]
then
	uprintf -q -f"%s\n" labid in chassis | sort -u
elif [[ $# == 1 ]]
then
	# uprintf -q -f"%s\n" labid in chassis where branch req ^${1}$ | sort -u
	uprintf -q -f"%s\n" labid in labloads where branch req ^${1}$ | sort -u
else
	echo >&2
	echo "Invalid number of arguments."
	echo $usage >&2
	exit 2
fi
#
exit 0

