#!/usr/bin/ksh
#
# verify all files exist for the all loads
#
###############################################################################
lcssyslog -q syscalls ${0} ${*}
#
CMD=${0}
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
problems=/tmp/probs$$
#
usage() {
	echo "usage: $CMD [-x?]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
}
#
set -- $(getopt ?x ${*})
if [[ ${?} -ne 0 ]]
then
	usage
	exit 0
fi
#
for opt in ${*}
do
	case "${opt}" in
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
echo
echo "Starting LCS Check Loads"
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
# load to delete.
#
>$problems
#
cd ${LCSTOOLSDATA}
#
uprintf -q -f"%s %s\n" branch cpuload in loads | 
while read branch cpuload
do
	echo 
	echo "===>>> Checking $branch/$cpuload <<<=== "
	echo 
	textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
	if [[ -z "${textdir}" ]]
	then
		echo
		echo "No base directory found for ${branch}." >&2
		continue
	fi
	#
	integer ok=0
	integer notok=0
	#
	uprintf -q -f"%s %s\n" type name in images where branch req "^${branch}$" and cpuload req "^${cpuload}$" |
	while read type name
	do
		fpath=${textdir}/${branch}/${type}/${name}
		#
		echo "Checking ${fpath}:"
		#
		if [[ -r "${fpath}" ]]
		then
			echo "===>>> OK"
			ok=$((ok+1))
		else
			echo "===>>> NOT OK <<<==="
			notok=$((notok+1))
			echo "===>>> MISSING FILE ${fpath} <<<=== " >>$problems
		fi
	done
	#
	echo
	echo "FILES OK    : ${ok}"
	echo "FILES NOT OK: ${notok}"
	#
	if [[ ${notok} != 0 ]]
	then
		echo "===>>> $branch/$cpuload/missing=$notok/found=$ok <<<=== " >>$problems
	fi
done
#
echo
echo "Summary of Loads with Missing Files:"
echo
#
if [[ -s "${problems}" ]]
then
	cat $problems
else
	echo "No loads with missing files found."
fi
#
rm -rf $problems 1>/dev/null 2>&1
#
exit 0

