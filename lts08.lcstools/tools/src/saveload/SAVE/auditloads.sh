#!/usr/bin/ksh
#
# audit the loads relation and all other loads. check if the files
# exist under /lcsl100.
#
###############################################################################
#
CMD=${0}
#
integer OKNUM
integer NOTOKNUM
integer TOTALOKNUM
integer TOTALNOTOKNUM
#
verbose=no
exitonerror=no
#
tmp=/tmp/tmp$$
okcpuloads=/tmp/okcpuloads$$
notokcpuloads=/tmp/notokcpuloads$$
#
vecho() {
	if [[ "${verbose}" == "yes" ]]
	then
		echo "${@}"
	fi
}
#
usage() {
	echo "usage: $CMD [-x?] [-V]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
	echo "	-V - enable verbose mode"
	echo "	-X - exit on error"
}
#
set -- $(getopt ?xXV ${*})
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
	-V)
		verbose=yes
		shift
		;;
	-X)
		exitonerror=yes
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
echo "Auditing LCS relation 'loads' and dependent relations ..." 
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
cd "${LCSTOOLSDATA}"
#
RELATIONS="loads images "
EXIT=no
#
OKNUM=0
NOTOKNUM=0;
#
echo
echo "Verify relations exist ..."
#
for relation in ${RELATIONS}
do
	OK=OK
	#
	vecho "Checking relation ${relation} ..."
	#
	relfile="${LCSTOOLSDATA}/${relation}"
	drelfile="${LCSTOOLSDATA}/D${relation}"
	if [[ ! -r "${relfile}" ]]
	then
		echo "${relfile} not readable." >&2
		OK="NOT OK"
		EXIT=yes
	fi
	if [[ ! -r "${drelfile}" ]]
	then
		echo "${drelfile} not readable." >&2
		OK="NOT OK"
		EXIT=yes
	fi
	#
	case "${OK}" in
	"OK")
		OKNUM=$OKNUM+1
		;;
	"NOT OK")
		NOTOKNUM=$NOTOKNUM+1
		;;
	esac
	#
	vecho "${OK}."
done
#
TOTALOKNUM=${TOTALOKNUM}+${OKNUM}
TOTALNOTOKNUM=${TOTALNOTOKNUM}+${NOTOKNUM}
#
if [[ "${EXIT}" == "yes" ]]
then
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
	echo "Exit ${CMD}."
	exit 2
else
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
fi
#
OKNUM=0
NOTOKNUM=0;
#
>${okcpuloads}
>${notokcpuloads}
#
echo
echo "Verify CPU loads exist ..."
#
cat loads | 
while read branch cpuload basedir
do
	OK=OK
	#
	cpuloadpath="${basedir}/${branch}/swCPU/${cpuload}_cpu.tar.gz"
	vecho "Checking ${cpuloadpath} ..."
	#
	if [[ ! -r "${cpuloadpath}" ]]
	then
		echo "CPU load file ${cpuloadpath} does not exist." >&2
		OK="NOT OK"
		EXIT=${exitonerror}
	fi
	#
	case "${OK}" in
	"OK")
		OKNUM=$OKNUM+1
		echo "$branch $cpuload $basedir" >>${okcpuloads}
		;;
	"NOT OK")
		NOTOKNUM=$NOTOKNUM+1
		echo "$branch $cpuload $basedir" >>${notokcpuloads}
		;;
	esac
	#
	vecho "${OK}."
done
#
TOTALOKNUM=${TOTALOKNUM}+${OKNUM}
TOTALNOTOKNUM=${TOTALNOTOKNUM}+${NOTOKNUM}
#
if [[ "${EXIT}" == "yes" ]]
then
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
	echo "Exit ${CMD}."
	exit 2
else
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
fi
#
if [[ -s "${notokcpuloads}" ]]
then
	echo
	echo "Non-existant load(s) are:"
	cat ${notokcpuloads}
fi
#
OKNUM=0
NOTOKNUM=0;
#
>${okcpuloads}
>${notokcpuloads}
#
echo
echo "Verify images exist ..."
#
cat images  | 
while read branch cpuload imagetype imagename
do
	OK=OK
	# get base directory for this branch and cpuload
	grep "$branch $cpuload " ${okcpuloads} >$tmp
	if [[ ! -s "$tmp" ]]
	the
		echo "No BASEDIR found for $branch $cpuload $imagename"
		NOTOKNUM=$NOTOKNUM+1
		continue
	fi

	cpuloadpath="${basedir}/${branch}/swCPU/${cpuload}_cpu.tar.gz"
	vecho "Checking ${cpuloadpath} ..."
	#
	if [[ ! -r "${cpuloadpath}" ]]
	then
		echo "CPU load file ${cpuloadpath} does not exist." >&2
		OK="NOT OK"
		EXIT=${exitonerror}
	fi
	#
	case "${OK}" in
	"OK")
		OKNUM=$OKNUM+1
		echo "$branch $cpuload $basedir" >>${okcpuloads}
		;;
	"NOT OK")
		NOTOKNUM=$NOTOKNUM+1
		echo "$branch $cpuload $basedir" >>${notokcpuloads}
		;;
	esac
	#
	vecho "${OK}."
done
#
TOTALOKNUM=${TOTALOKNUM}+${OKNUM}
TOTALNOTOKNUM=${TOTALNOTOKNUM}+${NOTOKNUM}
#
if [[ "${EXIT}" == "yes" ]]
then
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
	echo "Exit ${CMD}."
	exit 2
else
	echo "OK=$OKNUM, NOTOK=$NOTOKNUM"
fi
#
if [[ -s "${notokcpuloads}" ]]
then
	echo
	echo "Non-existant load(s) are:"
	cat ${notokcpuloads}
fi
#
# all done
#
echo
echo "TOTAL OK=$TOTALOKNUM, TOTAL NOTOK=$TOTALNOTOKNUM"
#
if [[ "${TOTALNOTOKNUM}" == 0 ]]
then
	echo "Everything OK."
else
	echo "Errors detected."
fi
#

#
exit 0

