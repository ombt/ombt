#!/usr/bin/ksh
# audit scripts directory, compare existing scripts to loads that
# are available for reload. if the scripts directory is not in
# the reload list, then it is old and it can be removed.
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
\t-r - remove old scripts directories\n
\n
"
#
remove=no
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
	-r)
		remove=yes
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ -z "${LCSSCRIPTS}" ]]
then
	echo >&2
	echo "LCSSCRIPTS is not set." >&2
	exit 2
fi
if [[ ! -d "${LCSSCRIPTS}" ]]
then
	echo >&2
	echo "$LCSSCRIPTS does not exist." >&2
	exit 2
fi
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
cd ${LCSSCRIPTS}
#
find . -type d -print |
sed 's?/? ?g;' | 
nawk '
NF == 3 {
	print $2 " " $3;
	next;
}
{
	# skip all other records
	next;
} ' |
sort -u >/tmp/scriptsdirs$$
#
cd ${LCSTOOLSDATA}
uprintf -q -f"%s %s\n" branch cpuload in labloads | sort -u >/tmp/labloads$$
#
echo 
echo "Scripts in $LCSSCRIPTS, but not in labloads relation:"
echo 
#
comm -23 /tmp/scriptsdirs$$ /tmp/labloads$$ >/tmp/unused$$
if [[ ! -s "/tmp/unused$$" ]]
then
	echo
	echo "No unused scripts found."
	echo 
	echo "Done."
	exit 0;
fi
#
cat /tmp/unused$$
#
if [[ "${remove}" == "yes" ]]
then
	echo 
	echo "Delete scripts? [y/n/cr=n] \c"
	read ans
	case "${ans}" in
	y)
		echo
		cat /tmp/unused$$ | 
		while read branch cpuload
		do
			echo "rm -rf $LCSSCRIPTS/${branch}/${cpuload}"
			rm -rf $LCSSCRIPTS/${branch}/${cpuload}
		done
		;;
	*)
		echo 
		echo "Nothing removed."
		;;
	esac
fi
#
echo 
echo "Done."
#
exit 0
