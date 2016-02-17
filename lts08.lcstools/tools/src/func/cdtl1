cdtl1 ( ) {
#
usage="usage: $0 [-x] [-l labid] [branch [cpuload]] "
labid=${LABID}
#
set -- $(getopt ?xl: ${*})
if [[ $? != 0 ]]
then
	echo $usage >&2
	return
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
		labid=${2}
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
branch=${1}
cpuload=${2}
#
if [[ -z "${branch}" ]]
then
	PS3="Choose a branch for LAB ${labid}: "
	select branch in QUIT $(uprintf -q -f"%s\n" branch in ${LCSTOOLSDATA}/labloads where labid req "^${labid}$" | sort -u)
	do
		[[ "${branch}" == QUIT ]] && return 0;
		[[ -n "${branch}" ]] && break;
	done
fi
#
if [[ -z "${cpuload}" ]]
then
	PS3="Choose a CPU load for LAB ${labid}: "
	select cpuload in QUIT $(uprintf -q -f"%s\n" cpuload in ${LCSTOOLSDATA}/labloads where branch req "^${branch}$" and labid req "^${labid}$" | sort -u)
	do
		[[ "${cpuload}" == QUIT ]] && return 0;
		[[ -n "${cpuload}" ]] && break;
	done
fi
#
tl1dir=${LCSSCRIPTS}/${branch}/${cpuload}/${labid}
#
if [[ ! -d "${tl1dir}" ]]
then
	echo "TL1 directory $tl1dir does not exist or is not readable." >&2
	return 2
fi
#
cd $tl1dir
pwd
#
return 0
}
