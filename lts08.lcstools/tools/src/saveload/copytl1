#!/usr/bin/ksh
# copy scripts from basescripts directory to scripts directory for 
# provisioning.
#
#########################################################################
#
export CMD=$(basename ${0})
usebackup=no
#
usage="\n
usage: ${CMD} [-?x] [-b] frombranch tobranch tocpuload labid [labid ...]
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-b - use parent branch basescripts as a backup, if BP does not have basescripts. \n
\n
"
#
set -- $(getopt ?xb ${*})
if [[ ${?} -ne 0 ]]
then
	echo $usage
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
	-b)
		usebackup=yes
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
cd ${LCSTOOLSDATA}
#
if [[ $# -lt 4 ]]
then
	echo "ERROR: Not enough parameters were given."
	echo $usage
	exit 2
fi
#
fromrelease=${1}
torelease=${2}
toload=${3}
shift 3
#
for fromlabid in ${*}
do
	# leave this here. we reset the directory below.
	cd ${LCSTOOLSDATA}
	#
	echo
	echo "Copying from ${fromlabid}/${fromrelease} ..."
	echo
	#
	tolabid=${fromlabid}
	#
	echo
	echo "Base Scripts From : ${fromlabid}/${fromrelease}"
	echo "Copying Scripts To: ${torelease}/${toload}/${tolabid}"
	echo
	#
	from="${LCSBASESCRIPTS}/${fromlabid}/${fromrelease}/current"
	to="${LCSSCRIPTS}/${torelease}/${toload}/${tolabid}"
	#
	if [[ ! -d "${from}" ]]
	then
		echo 
		echo "'From' directory ${from} does not exist." 
		if [[ "$usebackup" != "yes" ]]
		then
			# do not use parent branch as a backup.
			echo "Continue to next labid."
			echo 
			continue
		fi
		# try to use backup parent branch for beta, BP.
		echo "Trying parent branch to branch $fromrelease."
		parentfromrelease=$(uprintf -q -f"%s\n" base in basebranch where branch req "^${fromrelease}$" and type req "^tl1$" | sed -n '1,1p')
		if [[ -z "$parentfromrelease" ]]
		then
			echo "Parent branch to branch $fromrelease not found."
			echo "Continue to next labid."
			echo 
			continue
		fi
		parentfrom="${LCSBASESCRIPTS}/${fromlabid}/${parentfromrelease}/current"
		if [[ ! -d "${parentfrom}" ]]
		then
			echo 
			echo "Parent 'From' directory <${parentfrom}> does not exist." 
			echo "Continue to next labid."
			echo 
			continue
		fi
		#
		from=$parentfrom
		fromrelease=$parentfromrelease
		#
		echo "Using parent branch 'From' $from directory."
		echo 
	fi
	if [[ ! -d "${to}" ]]
	then
		echo 
		echo "'To' directory ${to} does not exist." 
		echo "Creating 'To' directory ${to}." 
		echo 
		mkdir -p ${to}
	fi
	#
	# check that scripts exist.
	#
	ls ${from}/* 2>/dev/null 1>/tmp/scripts$$
	if [[ ! -s "/tmp/scripts$$" ]]
	then
		echo 
		echo "'From' directory ${from} is empty." 
		if [[ "$usebackup" != "yes" ]]
		then
			# do not use parent branch as a backup.
			echo "Continue to next labid."
			echo 
			continue
		fi
		# try to use backup parent branch for beta, BP.
		echo "Trying parent branch to branch $fromrelease."
		parentfromrelease=$(uprintf -q -f"%s\n" base in basebranch where branch req "^${fromrelease}$" and type req "^tl1$" | sed -n '1,1p')
		if [[ -z "$parentfromrelease" ]]
		then
			echo "Parent branch to branch $fromrelease not found."
			echo "Continue to next labid."
			echo 
			continue
		fi
		parentfrom="${LCSBASESCRIPTS}/${fromlabid}/${parentfromrelease}/current"
		if [[ ! -d "${parentfrom}" ]]
		then
			echo 
			echo "Parent 'From' directory <${parentfrom}> does not exist." 
			echo "Continue to next labid."
			echo 
			continue
		fi
		#
		from=$parentfrom
		fromrelease=$parentfromrelease
		#
		echo "Using parent branch 'From' $from directory."
		echo 
	fi
	#
	echo 
	echo "Exec'ing cp ${from}/* ${to} ..."
	echo 
	cp ${from}/* ${to} 2>/dev/null
	#
	# copy common files.
	#
	echo "Exec'ing cp ${LCSBASESCRIPTS}/commonfiles/* ${to} ..."
	cp ${LCSBASESCRIPTS}/commonfiles/* ${to} 2>/dev/null
	#
	# update the hardware scripts
	#
	uprintf -q -f"<%s> %s\n" type name in images where branch req "^${torelease}$" and cpuload req "^${toload}$" | 
	sed 's/ \([^_][^_]*\)_.*$/ \1/' | 
	sed 's/\([0-9][0-9]*.P[0-9][0-9]*\)\.[0-9][0-9]*/\1/' |
	sed 's/\([0-9][0-9]*.SP\.[0-9][0-9]*\)\.[0-9][0-9]*/\1/' |
	sed 's?^?s/?; s? ?/?; s?$?/;?;' |
	tee /tmp/sedcmds$$
	#
	cd ${to}
	#
	for script in *
	do
		sed -f /tmp/sedcmds$$ ${script} >/tmp/new${script}
		mv /tmp/new${script} ${script}
		chmod 644 ${script}
	done
	#
	# distributescripts ${to}
done
#
echo 
echo "${CMD} done."
#
rm -f /tmp/*$$ 1>/dev/null 2>&1
exit 0

