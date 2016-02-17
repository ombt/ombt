#!/usr/bin/ksh
#
# edit a HUG log file.
#
#########################################################################
#
if [[ $# == 1 ]]
then
	if [[ "${1}" == "-?" ]]
	then
		echo
		echo "	usage: $(basename ${0}) [-?] [labid [hugtype]]"
		echo
		echo "	where hug type is: pre, cpu, cm, iom"
		exit 0
	fi
	#
	labid="${1}"
elif [[ $# == 2 ]]
then
	labid="${1}"
	hugtype="${2}"
elif [[ $# == 0 ]]
then
	labid=""
	hugtype=""
else
	echo "usage: ${0} [-?] [labid [hugtype]]"
	echo "where hug type is: pre, cpu, cm, iom"
	exit 2
fi
#
if [[ -z "${labid}" ]]
then
	PS3="Choose LABID:"
	select labid in QUIT $(cat ${LCSTOOLSDATA}/chassis | cut -d'	' -f1 | sort -u)
	do
		case "${labid}" in
		QUIT)
			echo "Quitting ${CMD}."
			exit 0
			;;
		*)
			export labid
			[[ -n "${labid}" ]] && break
			;;
		esac
	done
fi
#
if [[ -z "${hugtype}" ]]
then
	PS3="Choose HUG log file type:"
	select hugtype in QUIT pre cpu cm iom
	do
		case "${hugtype}" in
		QUIT)
			echo "Quitting ${CMD}."
			exit 0
			;;
		*)
			export hugtype
			[[ -n "${hugtype}" ]] && break
			;;
		esac
	done
fi
#
huglogfile="$(ls -t /lcsl100/tmp/${hugtype}hug.upsh.logfile.${labid}.* | sed -n '1,1p')"
#
if [[ ! -r "${huglogfile}" ]]
then
	echo "ERROR: no log file found."
	exit 2
else
	echo "Log file: ${huglogfile}"
fi
#
exec ${EDITOR} ${huglogfile}
