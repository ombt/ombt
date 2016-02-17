#!/usr/bin/ksh
# audit images relation for duplicate entries per cpuload.
#
#########################################################################
#
export usagecmd=$(basename ${0})
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1;' 0 1 2 15
#
usage="\n
usage: ${usagecmd} [-?x] [-r] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\n
"
#
set -- $(getopt ?xr ${*})
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
	echo "LCSTOOLSDATA is not set." >&2
	exit 2
fi
if [[ ! -d "${LCSTOOLSDATA}" ]]
then
	echo >&2
	echo "$LCSTOOLSDATA does not exist." >&2
	exit 2
fi
#
echo
echo "Multiple entries in Images relation are: "
echo
cd ${LCSTOOLSDATA}
uprintf -q -f"%s %s %s\n" branch cpuload type in images | 
sort | 
uniq -c |
grep -v '^ *1 ' |
tee /tmp/images$$
#
# if [[ -s "/tmp/images$$" ]]
# then
	# cat /tmp/images$$ |
	# while read junk branch cpuload type 
	# do
		# uprint images where branch req "^${branch}$" and cpuload req "^${cpuload}$" and type req "^${type}$"
	# done
# fi
#
echo 
echo "Done."
#
exit 0
