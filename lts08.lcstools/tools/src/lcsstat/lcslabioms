#!/usr/bin/ksh
#
# list ioms in a lab
#
###############################################################################
lcssyslog -q syscalls ${0} ${*}
#
CMD=${0}
#
branchclause=
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
usage() {
	echo
	echo "usage: $CMD [-x?] [-b branch] [labid]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
	echo "	-b branch - search using this branch"
	echo
	echo "if labid is not given, then LABID is used."
}
#
set -- $(getopt ?b:x ${*})
#
if [[ ${?} -ne 0 ]]
then
	usage
	exit 2
fi
#
for opt in ${*}
do
	case "${opt}" in
	-x)
		set -x
		shift
		;;
	-b)
		branch=${2}
		branchclause="and branch req ^${branch}$"
		shift 2
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
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
if [[ $# == 1 ]]
then
	LABID=${1}
elif [[ $# != 0 ]]
then
	echo "Wrong number of arguments."
	usage
	exit 2
fi
#
uprint ${LCSTOOLSDATA}/chassis2ioms where labid req "^${LABID}$" ${branchclause}
#
exit 0
