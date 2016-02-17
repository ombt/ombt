#!/usr/bin/ksh
#
# create a script to update a chassis and execute it.
#
#########################################################################
#
# get data for creating script.
#
CMD=$(basename ${0})
PATH=$PATH:/opt/exp/lib/unity/bin
#
export CURRDIR=$(pwd)
#
trap 'rm -rf /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 3 4 5 6 15
#
usage="
usage: ${CMD} [-?] [-x] [-X] [-t] [-f] [-e] [-E]
	[-l] [-C cfgfile] [-S script] [-b branch] [-c cpuload] [labid] 

where: 
\t-x - debug 
\t-? - usage message 

\t-X - execute script 
\t-t - tail log file
\t-f - list commands that have run
\t-E - edit generated input files
\t-e - edit log file

\t-b branch - new branch 
\t-c cpuload - new cpu load 
\t-C cfgfile - generated hugpp configuration filename 
\t-S script - generated upsh script filename
\t-V - run upsh in verify mode (implies -X) 
\t-l - HUG lite, only run minimal commands in prehug section.
"
#
labid=
branch=
cpuload=
upshscript=
cfgfile=
execscript=no
iomtypes=/tmp/IT$$
iomchoices=/tmp/IC$$
tmpfile=/tmp/TF$$
filepaths=/tmp/FP$$
upshopts=
hugppopts=
taillogfile=no
filterlogfile=no
editinputfiles=no
editlogfile=no
findlogfile=no
#
set -- `getopt ?EetflVxXS:C: ${*}`
if [[ ${?} -ne 0 ]]
then
	echo "\n$usage" >&2
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
	-X)
		execscript=yes
		shift
		;;
	-E)
		editinputfiles=yes
		shift
		;;
	-e)
		findlogfile=yes
		editlogfile=yes
		taillogfile=no
		filterlogfile=no
		shift
		;;
	-t)
		findlogfile=yes
		taillogfile=yes
		filterlogfile=no
		editlogfile=no
		shift
		;;
	-f)
		findlogfile=yes
		filterlogfile=yes
		taillogfile=no
		editlogfile=no
		shift
		;;
	-l)
		hugppopts="${hugppopts} -l"
		shift
		;;
	-V)
		upshopts="${upshopts} -X"
		execscript=yes
		shift
		;;
	-C)
		cfgfile=${2}
		shift 2
		;;
	-S)
		upshscript=${2}
		shift 2
		;;
	-b)
		branch=${2}
		shift 2
		;;
	-c)
		cpuload=${2}
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ $# == 1 ]]
then
	labid=${1}
elif [[ $# > 1 ]]
then
	echo "\n$usage" >&2
	exit 2
fi
#
echo "\nLCS HUG Automation:"
#
if [[ -z "${labid}" ]]
then
	echo
	PS3="Choose LABID to HUG:"
	select labid in QUIT $(cat ${LCSTOOLSDATA}/chassis | cut -d'	' -f1 | sort -u)
	do
		case "${labid}" in
		QUIT)
			echo "\nQuitting ${CMD}."
			exit 0
			;;
		"")
			;;
		*)
			export labid
			[[ -n "${labid}" ]] && break
			;;
		esac
	done
fi
#
switchid=$(echo ${labid} | sed 's/g\([0-9]\)/gw\1/')
#
# check if we processing existing log files.
#
if [[ "${findlogfile}" == yes ]]
then
	lcshuglogfile="$(ls -t ${LCSHUGTMP}/lcshug.log.${labid}.* 2>/dev/null | sed -n '1,1p')"
	if [[ -z "${lcshuglogfile}" ]]
	then
		echo
		echo "Unable to find a log file for ${labid}."
		exit 2
	fi
fi
#
if [[ "${filterlogfile}" == yes ]]
then
	echo
	echo "Filtering log file ${lcshuglogfile}:"
	echo
	exec cgrep -n +1 "^Calling" ${lcshuglogfile}
elif [[ "${taillogfile}" == yes ]]
then
	echo
	echo "Tailing log file ${lcshuglogfile}:"
	echo
	exec tail -f ${lcshuglogfile}
elif [[ "${editlogfile}" == yes ]]
then
	echo
	echo "Editting log file ${lcshuglogfile}:"
	echo
	[[ -z "${EDITOR}" ]] && export EDITOR=vi
	exec ${EDITOR} ${lcshuglogfile}
fi
#
# determine name of output files, if they were not given.
#
if [[ -z "${upshscript}" ]]
then
	upshscript=${LCSHUGTMP}/lcshug.upsh.script.${labid}.$$
fi
if [[ -z "${cfgfile}" ]]
then
	cfgfile=${LCSHUGTMP}/lcshug.hugpp.cfgfile.${labid}.$$
fi
#
lcshuglogfile=${LCSHUGTMP}/lcshug.log.${labid}.$$
#
echo "\nGenerated HUGPP Configuration File:\n\t====>>>> ${cfgfile}"
echo "\nGenerated UPSH Script File:\n\t====>>>> ${upshscript}"
echo "\nLog File:\n\t====>>>> ${lcshuglogfile}"
#
if [[ -z "${branch}" ]]
then
	echo
	PS3="Choose BRANCH for ${switchid} HUG:"
	select branch in QUIT $(uprintf -q -f"%s\n" branch in ${LCSTOOLSDATA}/labloads where labid req ^${labid}$ | sort -u)
	do
		case "${branch}" in
		QUIT)
			echo "\nQuitting ${CMD}."
			exit 0
			;;
		"")
			;;
		*)
			export branch
			[[ -n "${branch}" ]] && break
			;;
		esac
	done
fi
#
if [[ -z "${cpuload}" ]]
then
	echo
	PS3="Choose CPULOAD for ${switchid} HUG:"
	select cpuload in QUIT $(uprintf -q -f"%s\n" cpuload in ${LCSTOOLSDATA}/labloads where labid req ^${labid}$ and branch req ^${branch}$ | sort -u)
	do
		case "${cpuload}" in
		QUIT)
			echo "\nQuitting ${CMD}."
			exit 0
			;;
		"")
			;;
		*)
			export cpuload
			[[ -n "${cpuload}" ]] && break
			;;
		esac
	done
fi
#
echo "\nEnter FTP machine: [cr=lts08.ih.lucent.com] \c"
read ftpmachine
: ${ftpmachine:="lts08.ih.lucent.com"}
#
echo "Enter FTP login: [cr=lcsinteg] \c"
read ftplogin
: ${ftplogin:="lcsinteg"}
#
echo "Enter FTP passwd: [cr=plexus9000] \c"
read ftppasswd
: ${ftppasswd:="plexus9000"}
#
echo "\nEnter BACKUP machine: [cr=lts08.ih.lucent.com] \c"
read backupmachine
: ${backupmachine:="lts08.ih.lucent.com"}
#
echo "Enter BACKUP login: [cr=lcsinteg] \c"
read backuplogin
: ${backuplogin:="lcsinteg"}
#
echo "Enter BACKUP passwd: [cr=plexus9000] \c"
read backuppasswd
: ${backuppasswd:="plexus9000"}
#
echo "\nEnter EMAIL address: [cr=lcstools@lts08.ih.lucent.com] \c"
read email
: ${email:="lcstools@lts08.ih.lucent.com"}
#
echo "\nCURRENT HUG DATA:"
echo "\nLABID   : ${labid}"
echo "BRANCH  : ${branch}"
echo "CPU LOAD: ${cpuload}"
echo "\nFTP MACHINE: ${ftpmachine}"
echo "FTP LOGIN  : ${ftplogin}"
echo "FTP PASSWD : ${ftppasswd}"
echo "\nBACKUP MACHINE: ${backupmachine}"
echo "BACKUP LOGIN  : ${backuplogin}"
echo "BACKUP PASSWD : ${backuppasswd}"
echo "\nEMAIL: ${email}"
#
uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where labid req ^${labid}$ and branch req ^${branch}$ | sort -u >${iomchoices}
#
if [[ ! -s "${iomchoices}" ]]
then
	echo
	echo "No IOMs found in chassis2ioms for ${branch} and ${labid}."
	echo "Will try parent branch."
	# no ioms found. try parent branch.
	parentbranch=$(uprintf -q -f"%s\n" base in ${LCSTOOLSDATA}/basebranch where branch req "^${branch}$" and type req "^tl1$" | sed -n '1,1p')
	if [[ -n "${parentbranch}" ]]
	then
		uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where labid req ^${labid}$ and branch req ^${parentbranch}$ | sort -u >${iomchoices}
		if [[ ! -s "${iomchoices}" ]]
		then
			echo
			echo "No IOMs found in chassis2ioms for ${parentbranch} and ${labid}."
		fi
	else
		echo
		echo "No parent branch found for ${branch}."
	fi
fi
#
>${iomtypes}
#
echo
PS3="Choose CPU/IOM types for ${switchid} HUG:"
# select iomtype in QUIT DONE_CHOOSING cpu $(uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where labid req ^${labid}$ and branch req ^${branch}$ | sort -u)
select iomtype in QUIT DONE_CHOOSING cpu $(cat ${iomchoices})
do
	case "${iomtype}" in
	QUIT)
		echo "\nQuitting ${CMD}."
		exit 0
		;;
	DONE_CHOOSING)
		break 2
		;;
	"")
		;;
	*)
		echo ${iomtype} >>${iomtypes}
		echo "\nIOM types to HUG:\n"
		cat ${iomtypes}
		;;
	esac
done
#
cat ${iomtypes} | sort -u >${tmpfile}
#
cat ${iomtypes} | grep -v '^[ 	]$' | sort -u >${tmpfile}
mv ${tmpfile} ${iomtypes}
#
echo "\nIOM types to ${switchid} HUG:\n"
cat ${iomtypes}
#
if [[ ! -s "${iomtypes}" ]]
then
	echo "\nNo CPU/IOM types were chosen." >&2
	exit 2
fi
#
# get list of tar files for this load and iom/cpu types.
#
textdir=$(uprintf -q -f"%s\n" path in ${LCSTOOLSDATA}/filesystems where branch req ^${branch}$ and type req ^text$ | sort -u)
#
if [[ -z "${textdir}" ]]
then
	echo "\nNo base directory found for branch ${branch}." >&2
	exit 2
fi
#
>${filepaths}
#
echo "\nCPU/IOM tar files for ${switchid} HUG are: "
#
cat ${iomtypes} |
while read iomtype
do
	set -- $(uprintf -q -f"%s %s\n" type name in ${LCSTOOLSDATA}/images where branch req "^${branch}$" and cpuload req "^${cpuload}$" and type req ^${iomtype}$)
	#
	type=${1}
	if [[ -z "${type}" ]]
	then
		echo "\nNo type found for IOM/CPU type ${iomtype}." >&2
		exit 2
	fi
	#
	name=${2}
	if [[ -z "${name}" ]]
	then
		echo "\nNo name found for IOM/CPU type ${iomtype}." >&2
		exit 2
	fi
	#
	fpath=${textdir}/${branch}/${type}/${name}
	if [[ ! -r "${fpath}" ]]
	then
		echo "\n${iomtype} tar file is NOT readable: ${fpath}" >&2
		exit 2
	else
		echo "\n${iomtype} tar file: ${fpath}"
		#
		echo "${iomtype} ${fpath}" >>${filepaths}
	fi
done
#
# create hugpp input data file ...
#
spaip=$(getspips ${labid} | sed -n '1,1p' | cut -d' ' -f2)
spbip=$(getspips ${labid} | sed -n '1,1p' | cut -d' ' -f3)
#
cat >${cfgfile} <<EOF
globals data {
	scriptsdir .
	#
	switchid ${switchid}
	#
	${switchid} {
		login root
		passwd plexus9000
		tl1login telica
		tl1passwd telica
		spa {
			ip ${spaip}
		}
		spb {
			ip ${spbip}
		}
	}
	#
	backup {
		machine ${backupmachine}
		login ${backuplogin}
		passwd ${backuppasswd}
		dir /tmp
	}
	load {
		ftpmachine ${ftpmachine}
		ftplogin ${ftplogin}
		ftppasswd ${ftppasswd}
		branch ${branch}
		cpuload ${cpuload}
		images {
EOF
#
echo "\t\t\ttypes $(cat ${iomtypes} | tr '\n' ' ')" >>${cfgfile}
#
cat ${filepaths} |
while read iomtype path
do
	tarfile=$(basename ${path})
	tarload=$(expr ${tarfile} : '\(.*\)_'${iomtype}'.tar.gz')
	#
	echo "\t\t\t${iomtype} ${path}" >>${cfgfile}
	echo "\t\t\t${iomtype}load ${tarload}" >>${cfgfile}
done
#
echo "\t\t}" >>${cfgfile}
echo "\t}" >>${cfgfile}
#
if [[ -n "${email}" ]]
then
	echo "\temail ${email}" >>${cfgfile}
fi
#
echo "}" >>${cfgfile}
#
# run HUG preprocessor
#
if [[ "${editinputfiles}" == yes ]]
then
	[[ -z "${EDITOR}" ]] && export EDITOR=vi
	${EDITOR} ${cfgfile}
fi
#
export hugppstatus=2
#
{
	nhugpp ${hugppopts} -o ${upshscript} ${cfgfile}
	hugppstatus=$?
	echo "\nhugpp exit status: ${hugppstatus}"
	if [[ ${hugppstatus} != 0 ]]
	then
		echo "\nhugpp failed."
		exit 2
	fi
} | tee ${lcshuglogfile}
#
# execute the script?
#
if [[ "${editinputfiles}" == yes ]]
then
	[[ -z "${EDITOR}" ]] && export EDITOR=vi
	${EDITOR} ${upshscript}
fi
#
if [[ "${execscript}" == yes ]]
then
	# yup, exec the script.
	export upshstatus=2
	#
	{
		upsh ${upshopts} -I ${upshscript}
		upshstatus=$?
		echo "\nupsh exit status: ${hugppstatus}"
		#
		if [[ ${upshstatus} != 0 ]]
		then
			echo "\nupsh failed."
			exit 2
		fi
	} | tee -a ${lcshuglogfile}
fi
#
echo "\nGenerated HUGPP Configuration File:\n\t====>>>> ${cfgfile}"
echo "\nGenerated UPSH Script File:\n\t====>>>> ${upshscript}"
echo "\nLog File:\n\t====>>>> ${lcshuglogfile}"
#
echo "\nSuccess."
exit 0
