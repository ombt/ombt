#!/usr/bin/ksh
#
# audit labloads relation.
#
#########################################################################
#
export usagecmd=$(basename ${0})
#
labloads="/tmp/labloads$$"
newlabloads="/tmp/newlabloads$$"
labloadsonly="/tmp/labloadsonly$$"
loads="/tmp/loads$$"
removetuples=no
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1;' 0 1 2 15
#
usage="\n
usage: ${usagecmd} [-?x] [-r] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-r - remove extra tuples from labloads. \n
\n
"
#
set -- $(getopt ?xr ${*})
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
	-r)
		removetuples=yes
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
echo
echo "Checking for loads in LABLOADS, but NOT in LOADS:"
echo
#
cd ${LCSTOOLSDATA}
#
cut -d'	' -f1,2 loads | sed -n 's/$/	/p' | sort -u >${loads}
cut -d'	' -f2,3 labloads | sed -n 's/$/	/p' | sort -u >${labloads}
#
comm -13 ${loads} ${labloads} >${labloadsonly}
#
if [[ -s "${labloadsonly}" ]]
then
	echo
	echo "Loads in LABLOADS and NOT in LOADS:"
	echo
	#
	cat ${labloadsonly}
	#
	echo
	echo "Labs assigned loads NOT in LOADS:"
	echo
	fgrep -f ${labloadsonly} labloads
	#
	if [[ "${removetuples}" == yes ]]
	then
		echo
		echo "Removing all LABLOADS tuples containing loads NOT in LOADS:"
		echo
		# generate new labloads without the extra tuples.
		fgrep -v -f ${labloadsonly} labloads >${newlabloads}
		# backup of old labloads
		[ ! -d BACKUP ] && mkdir -p BACKUP ;
		echo $(date '+%y%m%d%H%M%S') >>BACKUP/configure.logfile
		echo 'diff labloads newlabloads' >>BACKUP/configure.logfile
		diff labloads $newlabloads >>BACKUP/configure.logfile
		#
		cat $newlabloads | sort -u >labloads
	fi
else
	echo
	echo "All loads in LABLOADS are also in LOADS."
	echo
fi
#
echo 
echo "Done."
#
exit 0
