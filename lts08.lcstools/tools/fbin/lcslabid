lcslabid ( ) {
#
usage="usage: $0 [-x] [-u | -c | labid]"
unsetlabid=no
chooselabid=no
#
set -- $(getopt ?cxu ${*})
if [[ $? != 0 ]]
then
	echo $usage >&2
	return 0
fi
#
for i in ${*}
do
	case "${i}" in
	-x)
		set -x
		shift
		;;
	-u)
		unsetlabid=yes
		chooselabid=no
		shift
		;;
	-c)
		chooselabid=yes
		unsetlabid=no
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ "${unsetlabid}" == yes ]]
then
	unset LCSLABID
	echo "Unset LCSLABID.";
elif [[ "${chooselabid}" == yes ]]
then
	PS3="Choose LCSLABID:"
	select labid in QUIT $(cat ${LCSTOOLSDATA}/chassis | cut -d'	' -f1 | sort -u)
	do
		case "${labid}" in
		QUIT)
			echo "Quitting ${CMD}."
			return 0
			;;
		*)
			[[ -n "${labid}" ]] && break
			;;
		esac
	done
	export LCSLABID="${labid}"
	echo "LCSLABID=${LCSLABID}";
	unset labid
elif [[ -n "${1}" ]]
then
	export LCSLABID="${1}"
	echo "LCSLABID=${LCSLABID}";
elif [[ -n "${LABID}" ]]
then
	export LCSLABID="${LABID}"
	echo "LCSLABID=${LCSLABID}";
else
	echo "No LABID found. Unset LCSLABID.";
	unset LCSLABID
fi
#
return 0
}
