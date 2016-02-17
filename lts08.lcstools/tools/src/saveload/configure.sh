#!/usr/bin/ksh
#
# edit parameters for a load.
#
###############################################################################
#
CMD=$(basename ${0})
#
oper=none
loadstatus=oper
loadstatusset=no
newlabloads=/tmp/newlabloads$$
newlabloads2=/tmp/newlabloads2$$
alllabids=/tmp/alllabids$$
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
usage() {
	echo
	echo "usage: $CMD [-x?] [-t status] -a branch cpuload [labid [labid ...] ]"
	echo "	or"
	echo "usage: $CMD [-x?] -d branch cpuload [labid [labid ...] ]"
	echo "	or"
	echo "usage: $CMD [-x?] [-t status] -b branch [branch ...]"
	echo "	or"
	echo "usage: $CMD [-x?] [-t status] -c cpuload [cpuload ...]"
	echo "	or"
	echo "usage: $CMD [-x?] [-t status] -l labid [labid ...]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
	echo "	-a - add branch/cpuload for reload of labid"
	echo "	-d - delete branch/cpuload for reload of labid"
	echo "	-b - list reload data filtered by branch"
	echo "	-c - list reload data filtered by cpuload"
	echo "	-l - list reload data filtered by labid"
	echo "	-t status - load status when adding a load to a labid, oper or test"
	echo
	echo "the last option [-a|-d|-b|-c|-l] given takes precedence."
	echo "the default load status for -a is 'oper' if -t is not used."
	echo "-t option can also be used to query with the -b, -c and -l options."
	echo "if a list of labids is NOT given when using the -a or -d options,"
	echo "then the requested operation is applied to all labids assigned"
	echo "to the given release/load."
	echo
}
#
function add_to_reload {
	echo
	echo "Starting add_to_reload: ${*}"
	#
	if [[ $# -lt 2 ]]
	then
		echo
		echo "Missing parameters: branch, cpuload, [labid [labid ...]]"
		return 2
	fi
	#
	case "${loadstatus}" in
	oper|test)
		;;
	*)
		echo
		echo "Invalid load status value ${loadstatus}."
		echo "Allowed values are: 'oper' and 'test'."
		return 2
		;;
	esac
	#
	branch=${1}
	cpuload=${2}
	shift 2
	#
	# back up files before updating
	#
	# backupdir="BACKUP/$(date '+%y%m%d%H%M%S')"
	# [ ! -d ${backupdir} ] && mkdir -p ${backupdir};
	# cp loads images labloads ${backupdir}
	#
	# remove any old tuples for this release, load, and labid.
	#
	cp labloads $newlabloads
	#
	# check if a list of labids was given. if not, try to
	# generate one.
	#
	if [[ $# == 0 ]]
	then
		echo
		echo "Generating the list of labids for ${branch}/${cpuload}"
		#
		uprintf -q -f"%s\n" labid in labloads where cpuload req "^${cpuload}\$" and branch req "^${branch}\$" >${alllabids}
		if [[ ! -s "${alllabids}" ]]
		then
			echo
			echo "No labids found for ${branch}/${cpuload}"
			return 2
		else
			cat ${alllabids}
			set -- $(cat ${alllabids})
		fi
	fi
	#
	for labid in ${*}
	do
		echo
		echo "Deleting $branch/$cpuload from reload of $labid."
		grep -v "^${labid}	${branch}	${cpuload}	" $newlabloads >$newlabloads2
		mv $newlabloads2 $newlabloads
	done
	#
	echo
	echo "DIFF of OLD and NEW labloads:"
	diff labloads $newlabloads
	#
	for labid in ${*}
	do
		echo
		echo "Adding $branch/$cpuload for reload of $labid."
		#
		echo "${labid}	${branch}	${cpuload}	${loadstatus}" >>$newlabloads
	done
	#
	# backup of old labloads
	[ ! -d BACKUP ] && mkdir -p BACKUP ;
	echo $(date '+%y%m%d%H%M%S') >>BACKUP/configure.logfile
	echo 'diff labloads newlabloads' >>BACKUP/configure.logfile
	diff labloads $newlabloads >>BACKUP/configure.logfile
	#
	cat $newlabloads | sort -u >labloads
	#
	return 0
}
#
function delete_from_reload {
	#
	echo
	echo "Starting delete_from_reload: ${*}"
	#
	if [[ $# -lt 2 ]]
	then
		echo
		echo "Missing parameters: branch, cpuload, [labid [labid ...]]"
		return 2
	fi
	#
	branch=${1}
	cpuload=${2}
	shift 2
	#
	# back up files before updating
	#
	# backupdir="BACKUP/$(date '+%y%m%d%H%M%S')"
	# [ ! -d ${backupdir} ] && mkdir -p ${backupdir};
	# cp loads images labloads ${backupdir}
	#
	cp labloads $newlabloads
	#
	# check if a list of labids was given. if not, try to
	# generate one.
	#
	if [[ $# == 0 ]]
	then
		echo
		echo "Generating the list of labids for ${branch}/${cpuload}"
		#
		uprintf -q -f"%s\n" labid in labloads where cpuload req "^${cpuload}\$" and branch req "^${branch}\$" >${alllabids}
		if [[ ! -s "${alllabids}" ]]
		then
			echo
			echo "No labids found for ${branch}/${cpuload}"
			return 2
		else
			cat ${alllabids}
			set -- $(cat ${alllabids})
		fi
	fi
	#
	#
	for labid in ${*}
	do
		echo
		echo "Deleting $branch/$cpuload from reload of $labid."
		grep -v "^${labid}	${branch}	${cpuload}	" $newlabloads >$newlabloads2
		mv $newlabloads2 $newlabloads
	done
	#
	# backup of old labloads
	[ ! -d BACKUP ] && mkdir -p BACKUP ;
	echo $(date '+%y%m%d%H%M%S') >>BACKUP/configure.logfile
	echo 'diff labloads newlabloads' >>BACKUP/configure.logfile
	diff labloads $newlabloads >>BACKUP/configure.logfile
	#
	cat $newlabloads | sort -u >labloads
	#
	return 0
}
#
function list_by_branch {
	echo
	echo "Starting list_by_branch: ${*}"
	#
	if [[ $# -lt 1 ]]
	then
		echo
		echo "Load data for all branches: "
		uprint loads 
		#
		echo
		echo "Lab Load data for all branches:"
		uprint labloads 
		return 0
	fi
	#
	if [[ "${loadstatusset}" == yes ]]
	then
		statusclause=" and status req ^${loadstatus}$"
	else
		statusclause=
	fi
	#
	for branch in ${*}
	do
		echo
		echo "Load data for branch $branch:"
		uprint loads where branch req "^$branch$"
		#
		echo
		echo "Lab Load data for branch $branch:"
		uprint labloads where branch req "^$branch$" $statusclause
	done
	#
	return 0
}
#
function list_by_cpuload {
	echo
	echo "Starting list_by_cpuload: ${*}"
	#
	if [[ $# -lt 1 ]]
	then
		echo
		echo "Load data for all cpuloads: "
		uprint loads 
		#
		echo
		echo "Lab Load data for all cpuloads:"
		uprint labloads 
		return 0
	fi
	#
	if [[ "${loadstatusset}" == yes ]]
	then
		statusclause=" and status req ^${loadstatus}$"
	else
		statusclause=
	fi
	#
	for cpuload in ${*}
	do
		echo
		echo "Load data for cpuload $cpuload:"
		uprint loads where cpuload req "^$cpuload$"
		#
		echo
		echo "Lab Load data for cpuload $cpuload:"
		uprint labloads where cpuload req "^$cpuload$" $statusclause
	done
	#
	return 0
}
#
function list_by_labid {
	echo
	echo "Starting list_by_labid: ${*}"
	#
	if [[ $# -lt 1 ]]
	then
		echo
		echo "Lab Load data for all labids:"
		uprint labloads 
		return 0
	fi
	#
	if [[ "${loadstatusset}" == yes ]]
	then
		statusclause=" and status req ^${loadstatus}$"
	else
		statusclause=
	fi
	#
	for labid in ${*}
	do
		echo
		echo "Lab Load data for labid $labid:"
		uprint labloads where labid req "^$labid$" $statusclause
	done
	#
	return 0
}
#
function none {
	echo
	echo "Nothing to do."
	#
	usage
	#
	return 0
}
#
set -- $(getopt ?xadbclt: ${*})
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
	-a)
		oper=add_to_reload
		shift
		;;
	-d)
		oper=delete_from_reload
		shift
		;;
	-b)
		oper=list_by_branch
		shift
		;;
	-c)
		oper=list_by_cpuload
		shift
		;;
	-l)
		oper=list_by_labid
		shift
		;;
	-t)
		loadstatus=${2}
		loadstatusset=yes
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
echo
echo "Starting LCS Configure Load"
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
cd ${LCSTOOLSDATA}
#
${oper} ${*}
#
exit 0
