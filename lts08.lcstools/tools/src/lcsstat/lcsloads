#!/usr/bin/ksh
#
# list images in a load.
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
filestoremove=/tmp/filestoremove$$
verify=no
zipfiles=no
copyfiles=no
listloads=no
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
usage() {
	echo "usage: $CMD [-x?] [-X] [-l]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
	echo "	-X - verify file actually exists."
	echo "	-Z - zip all load files together."
	echo "	-C - copy all load files together."
	echo "	-l - list all loads"
}
#
set -- $(getopt ?xCZXsl ${*})
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
	-l)
		listloads=yes
		shift
		;;
	-X)
		verify=yes
		shift
		;;
	-Z)
		zipfiles=yes
		copyfiles=no
		shift
		;;
	-C)
		copyfiles=yes
		zipfiles=no
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
echo "Starting LCS List Loads"
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
uprintf -q -f"%s %s\n" branch cpuload in loads | 
sed 's/[	 ]/__/' |
sort -u >$loads
#
if [[ "${listloads}" == yes ]]
then
	echo
	cat $loads | 
	nawk '
	BEGIN {
		imax = 0;
	}
	{
		imax++;
		load[imax] = $0;
		next;
	}
	END {
		ioffset = int(imax/2);
		imax2 = int(imax/2);
		if (imax%2 != 0) {
			ioffset += 1;
		}
		for (i=1; i<=imax2; i++) {
			printf("%-35s %-35s\n", load[i], load[i+ioffset]);
		}
		if (imax%2 != 0) {
			printf("%-35s\n", load[i]);
		}
	} '
	#
	exit 0
fi
#
PS3="Choose branch/load to list: "
#
select branchload in QUIT $(cat $loads)
do
	case "${branchload}" in
	QUIT)
		echo "Exit $CMD."
		exit 0
		;;
	*)
		branchload=$(echo $branchload | sed 's/__/ /')
		set -- ${branchload}
		branch=${1}
		load=${2}
		break 2
		;;
	esac
done
#
echo
echo "Branch/Load to list: $branch $load"
#
uprint cpuload type name in images where branch leq "$branch" and cpuload leq "$load"
#
if [[ "${verify}" == "yes" ]]
then
	textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
	if [[ -z "${textdir}" ]]
	then
		echo
		echo "No base directory found for ${branch}." >&2
		exit 2
	fi
	#
	echo
	#
	integer ok=0
	integer notok=0
	#
	uprintf -q -f"%s %s %s\n" cpuload type name in images where branch req "^${branch}$" and cpuload req "^${load}$" |
	while read cpuload type name
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
		fi
	done
	#
	echo
	echo "FILES OK    : ${ok}"
	echo "FILES NOT OK: ${notok}"
	echo
fi
#
if [[ "${zipfiles}" == "yes" ]]
then
	textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
	if [[ -z "${textdir}" ]]
	then
		echo
		echo "No base directory found for ${branch}." >&2
		exit 2
	fi
	#
	echo
	#
	zipdir=/tmp/${load}
	[ -r ${zipdir} ] && rm -rf ${zipdir};
	[ -d ${zipdir} ] && rm -rf ${zipdir};
	mkdir -p ${zipdir}
	#
	uprintf -q -f"%s %s %s\n" cpuload type name in images where branch req "^${branch}$" and cpuload req "^${load}$" |
	while read cpuload type name
	do
		fpath=${textdir}/${branch}/${type}/${name}
		#
		echo "Checking ${fpath}:"
		#
		if [[ -r "${fpath}" ]]
		then
			echo "===>>> OK"
			echo "copy ${fpath} to zip directory ${zipdir} ..."
			cp ${fpath} ${zipdir}
			chmod 777 ${zipdir}/${name}
		else
			echo "===>>> NOT OK <<<==="
			echo "File ${fpath} MISSING ... Exiting."
			exit 2
		fi
	done
	#
	cd ${zipdir}
	zip ${load}.zip *tar.gz
	chmod 777 ${load}.zip
	#
	echo
	echo "ZIP file available under ${zipdir}."
	rm -rf *tar.gz
elif [[ "${copyfiles}" == "yes" ]]
then
	textdir=$(uprintf -q -f"%s\n" path in filesystems where branch req "${branch}$" and type req "^text$")
	if [[ -z "${textdir}" ]]
	then
		echo
		echo "No base directory found for ${branch}." >&2
		exit 2
	fi
	#
	echo
	#
	zipdir=/tmp/${load}
	[ -r ${zipdir} ] && rm -rf ${zipdir};
	[ -d ${zipdir} ] && rm -rf ${zipdir};
	mkdir -p ${zipdir}
	#
	uprintf -q -f"%s %s %s\n" cpuload type name in images where branch req "^${branch}$" and cpuload req "^${load}$" |
	while read cpuload type name
	do
		fpath=${textdir}/${branch}/${type}/${name}
		#
		echo "Checking ${fpath}:"
		#
		if [[ -r "${fpath}" ]]
		then
			echo "===>>> OK"
			echo "copy ${fpath} to zip directory ${zipdir} ..."
			cp ${fpath} ${zipdir}
			chmod 777 ${zipdir}/${name}
		else
			echo "===>>> NOT OK <<<==="
			echo "File ${fpath} MISSING ... Exiting."
			exit 2
		fi
	done
	#
	echo
	echo "All files available under ${zipdir}."
fi
#
exit 0
