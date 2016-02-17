cdbtl1 ( ) {
#
usage="usage: $0 [-x] [-l labid] [branch] "
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
#
if [[ -z "${branch}" ]]
then
	PS3="Choose a branch for LAB ${labid}: "
	#select branch in QUIT $(uprintf -q -f"%s\n" branch in ${LCSTOOLSDATA}/labloads where labid req "^${labid}$" | sort -u)
	select branch in QUIT $(uprintf -q -f"%s\n" branch in ${LCSTOOLSDATA}/chassis where labid req "^${labid}$" | sort -u)
	do
		[[ "${branch}" == QUIT ]] && return 0;
		[[ -n "${branch}" ]] && break;
	done
fi
#
tl1dir=${LCSBASESCRIPTS}/${labid}/${branch}/current
#
if [[ ! -d "${tl1dir}" ]]
then
	echo "Base scripts TL1 directory $tl1dir does not exist or is not readable." >&2
	return 2
fi
#
cd $tl1dir
pwd
#
return 0
}
