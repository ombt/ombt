#!/usr/bin/ksh
#
# list files in a load.
#
###############################################################################
lcssyslog -q syscalls ${0} ${*}
#
CMD=${0}
#
tmp=/tmp/tmp$$
loads=/tmp/loads$$
newloads=/tmp/newloads$$
newimages=/tmp/newimages$$
verify=no
listloads=no
basename=no
isbranch=no
iomtypes=all
llflag=no
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
usage() {
	echo "usage: $CMD [-x?] [-X] [-l] [-b] [-a]"
	echo "	[-t iomtype[,iomtype2,...]] "
	echo "	[cpuload [cpuload2 ...] | -B branch [branch2 ...]]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-a - do 'ls -l' on the files"
	echo "	-? - print usage message"
	echo "	-X - verify file actually exists."
	echo "	-l - list all loads"
	echo "	-t iomtype_list - print data for given list of iom types"
	echo "	-b - list basename only, not full path."
	echo "	-B - list of branches, instead of list of cpu loads."
	echo
	echo "-l option overwrites any cpu loads given in the command line."
	echo "if -B is used, the argument list is interpreted as branches."
	echo
	echo "valid iom types for -t option are:"
	echo
	echo "ana atmds3 cm cpu ds1_2 ds1 ds3 e1 ena2 ena octds3_2 octds3_3"
	echo "octds3 pna tdmoc trids3_3 trids3 voip6 voip vs3 bts tca"
	echo
}
#
set -- $(getopt ?at:BbxXl ${*})
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
		llflag=yes
		shift
		;;
	-l)
		listloads=yes
		shift
		;;
	-b)
		basename=yes
		shift
		;;
	-X)
		verify=yes
		shift
		;;
	-B)
		isbranch=yes
		shift
		;;
	-t)
		iomtypes="$(echo ${2} | sed 's/,/|/g; s/^\(.*\)$/\^(\1)$/g;')"
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
# load to delete.
#
cd ${LCSTOOLSDATA}
#
if [[ "${listloads}" == yes ]]
then
	if [[ "${isbranch}" == yes ]]
	then
		set -- $(uprintf -q -f"%s\n" branch in loads | sort -u)
	else
		set -- $(uprintf -q -f"%s\n" cpuload in loads | sort -u)
	fi
fi
#
if [[ $# == 0 ]]
then
	echo "No CPU loads or branches were given."
	usage
	exit 2
fi
#
if [[ "${isbranch}" == yes ]]
then
	for branch in ${*}
	do
		uprintf -q -f"%s\n" cpuload in loads where branch req "^${branch}$" | 
		while read cpuload
		do
			echo
			echo "CPU Branch: $branch"
			echo "CPU Load: $cpuload"
			#
			integer ok=0
			integer notok=0
			#
			textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
			if [[ -z "${textdir}" ]]
			then
				echo
				echo "No base directory found for ${branch}." >&2
				exit 2
			fi
			#
			uprintf -q -f"%s %s %s\n" cpuload type name in images where branch leq "$branch" and cpuload leq "$cpuload" |
			while read cpuload2 type name
			do
				if [[ "${iomtypes}" != all ]]
				then
					echo ${type} | egrep -s "${iomtypes}"
					if [[ $? != 0 ]]
					then
						continue
					fi
				fi
				#
				fpath=${textdir}/${branch}/${type}/${name}
				#
				if [[ "${verify}" == yes ]]
				then
					echo "Checking ${fpath}:"
					#
					if [[ -r "${fpath}" ]]
					then
						echo "===>>> OK"
						ok=$((ok+1))
					else
						echo "===>>> NOT OK <<<==="
						notok=$((notok+1))
					fi
				elif [[ "${llflag}" == yes ]]
				then
					/usr/bin/ls -l ${fpath}
				else
					if [[ "${basename}" == yes ]]
					then
						echo "${name}"
					else
						echo "${fpath}"
					fi
				fi
			done
			#
			if [[ "${verify}" == yes ]]
			then
				echo
				echo "FILES OK    : ${ok}"
				echo "FILES NOT OK: ${notok}"
				echo
			fi
		done
	done
else
	for cpuload in ${*}
	do
		uprintf -q -f"%s\n" branch in loads where cpuload req "^${cpuload}$" | 
		while read branch
		do
			echo
			echo "CPU Load: $cpuload"
			echo "CPU Branch: $branch"
			#
			integer ok=0
			integer notok=0
			#
			textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
			if [[ -z "${textdir}" ]]
			then
				echo
				echo "No base directory found for ${branch}." >&2
				exit 2
			fi
			#
			uprintf -q -f"%s %s %s\n" cpuload type name in images where branch leq "$branch" and cpuload leq "$cpuload" |
			while read cpuload2 type name
			do
				if [[ "${iomtypes}" != all ]]
				then
					echo ${type} | egrep -s "${iomtypes}"
					if [[ $? != 0 ]]
					then
						continue
					fi
				fi
				#
				fpath=${textdir}/${branch}/${type}/${name}
				#
				if [[ "${verify}" == yes ]]
				then
					echo "Checking ${fpath}:"
					#
					if [[ -r "${fpath}" ]]
					then
						echo "===>>> OK"
						ok=$((ok+1))
					else
						echo "===>>> NOT OK <<<==="
						notok=$((notok+1))
					fi
				elif [[ "${llflag}" == yes ]]
				then
					/usr/bin/ls -l ${fpath}
				else
					if [[ "${basename}" == yes ]]
					then
						echo "${name}"
					else
						echo "${fpath}"
					fi
				fi
			done
			#
			if [[ "${verify}" == yes ]]
			then
				echo
				echo "FILES OK    : ${ok}"
				echo "FILES NOT OK: ${notok}"
				echo
			fi
		done
	done
fi
#
exit 0
