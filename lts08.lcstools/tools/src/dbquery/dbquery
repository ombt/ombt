#!/usr/bin/ksh
#
# list relations.
#
###########################################################################
#
usage="
\n
usage: ${0} [-?] [-x] relation unity_where_clause \n
\n
or: \n
\n
usage ${0} [-?] [-x] [-s] [-l|-D] relation \n
\n
where \n
\t-? - print usage message \n
\t-x - enable trace mode \n
\t-l - list available relations \n
\t-D - list fields for a relation \n
\t-s - use uselect instead of default uprint\n
\n
and clause does NOT include the keyword 'where'.
\n
"
#
queryrelation() {
	cd ${LCSTOOLSDATA}
	#
	if [[ $# == 0 ]]
	then
		echo >&2
		echo "no relation given." >&2
		echo "use 'dbquery -l' to list relations." >&2
		echo $usage >&2
		exit 2
	elif [[ $# == 1 ]]
	then
		/opt/exp/lib/unity/bin/${unitycmd} ${1}
	else
		relation=${1}
		shift
		/opt/exp/lib/unity/bin/${unitycmd} ${relation} where ${*}
	fi
	return 0
}
#
listrelations() {
	cd ${LCSTOOLSDATA}
	/usr/bin/ls -C D* | /usr/bin/sed 's/D//g;'
	return 0
}
#
listfields() {
	if [[ -z "${1}" ]]
	then
		echo >&2
		echo "relation was not given." >&2
		echo $usage >&2
		exit 2
	fi
	#
	cd ${LCSTOOLSDATA}
	if [[ ! -r "D${1}" ]]
	then
		echo >&2
		echo "relation ${1} is not readable." >&2
		echo "use 'dbquery -l' to list relations." >&2
		echo $usage >&2
		exit 2
	fi
	#
	/usr/bin/cat D${1}
	return 0
}
#
operation=queryrelation
unitycmd=uprint
#
set -- $(getopt ?sxlD ${*})
if [[ ${?} -ne 0 ]]
then
	echo >&2
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
	-l)
		operation=listrelations
		shift
		;;
	-D)
		operation=listfields
		shift
		;;
	-s)
		unitycmd="uselect -q from "
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
	echo "LCSTOOLSDATA is not set. run '. setup_lcs';" >&2
	exit 2
fi
if [[ ! -d "${LCSTOOLSDATA}" ]]
then
	echo >&2
	echo "$LCSTOOLSDATA is not a directory or does not exist." >&2
	exit 2
fi
#
${operation} "${@}"
#
exit 0
