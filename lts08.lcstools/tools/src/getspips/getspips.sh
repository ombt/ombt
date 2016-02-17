# get SP IPs for the given labid.
verbose=no
all=no
pattern=
#
usage=" \n
usage: ${0} [-?] [-x] [-V] [-g grep_pattern] [-a | [labid [...]]] \n
where: \n
	-? - print usage message \n
	-x - enable debugging \n
	-V - verbose mode \n
	-g pattern - use pattern for a grep \n
	-a - list all labids \n
 \n
If no labid is given, then LABID is used. If -a is used, then all \n
labids are listed, regardless of the labid list. If the -g option  \n
is used, then the relation hwchassis is searched, not chassis. \n
"
#
set -- $(getopt ?g:axV ${*})
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
	-V)
		verbose=yes
		shift
		;;
	-a)
		all=yes
		shift
		;;
	-g)
		pattern="${2}"
		shift 2
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
if [[ "${all}" == yes ]]
then
	case "${verbose}" in
	yes)
		if [[ -z "${pattern}" ]]
		then
			uprint labid cpu_a_ip cpu_b_ip in chassis 
		else
			dbquery hwchassis "fieldvalue req ${pattern}"
		fi
		;;
	*)
		if [[ -z "${pattern}" ]]
		then
			uprintf -q -f'%s %s %s\n' labid cpu_a_ip cpu_b_ip in chassis
		else
			dbquery -s hwchassis "fieldvalue req ${pattern}"
		fi
		;;
	esac
	#
	exit 0
fi
#
if [[ $# == 0 ]]
then
	set -- ${LABID}
fi
#
case "${verbose}" in
yes)
	for labid in ${*}
	do
		if [[ -z "${pattern}" ]]
		then
			uprint labid cpu_a_ip cpu_b_ip in chassis where labid req "^$labid$"
		else
			dbquery hwchassis "labid req ${labid} and fieldvalue req ${pattern}"
		fi
	done
	;;
*)
	for labid in ${*}
	do
		if [[ -z "${pattern}" ]]
		then
			uprintf -q -f'%s %s %s\n' labid cpu_a_ip cpu_b_ip in chassis where labid req "^$labid$"
		else
			dbquery -s hwchassis "labid req ${labid} and fieldvalue req ${pattern}"
		fi
	done
	;;
esac
#
exit 0
