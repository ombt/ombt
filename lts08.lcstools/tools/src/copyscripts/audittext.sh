#!/usr/bin/ksh
# audit file in text directory against images relation.
#
#########################################################################
#
export usagecmd=$(basename ${0})
#
unknownfs=/tmp/UFS$$
textfiles=/tmp/TF$$
textfiles2=/tmp/TF2$$
files2remove=/tmp/F2RM$$
tmp=/tmp/TMP$$
rflag=no
yflag=no
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1;' 0 1 2 15
#
usage="\n
usage: ${usagecmd} [-?x] [-r] [-y] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-r - remove files \n
\t-y - assume yes for any question (used with -r) \n
\n
"
#
set -- $(getopt ?yrx ${*})
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
		rflag=yes
		shift
		;;
	-y)
		yflag=yes
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
cd ${LCSTOOLSDATA}
#
# generate a list of all files under text partitions. then compare
# with a list of all known images files. the difference will be
# files that should not be in the text partitions and should
# be deleted.
#
>$unknownfs
>$textfiles
#
uprintf -q -f"%s/%s\n" path branch in filesystems where type req "^text$" |
while read fs
do
	if [[ ! -d "$fs" ]]
	then
		echo $fs >>$unknownfs
	else
		find $fs -type f -print >>$textfiles
	fi
done
#
cat $textfiles | 
sort -u >$tmp
mv $tmp $textfiles
#
cat $unknownfs | 
sort -u >$tmp
mv $tmp $unknownfs
#
uprintf -q -f"%s %s %s\n" basedir branch cpuload in loads |
while read basedir branch cpuload
do
	uprintf -q -f"%s %s\n" type name in images where branch req "^${branch}$" and cpuload req "^${cpuload}$" |
	while read type name
	do
		echo "$basedir/$branch/$type/$name"
	done
done >$textfiles2
#
cat $textfiles2 | 
sort -u >$tmp
mv $tmp $textfiles2
#
echo
echo "Number of files in text partitions and images relation: "
echo
echo "Number of /text files: $(cat $textfiles | wc -l )"
echo "Number of images     : $(cat $textfiles2 | wc -l )"
echo
echo "Files in text partition but not in images relation:"
echo
#
comm -23 $textfiles $textfiles2
#
if [[ "$rflag" == yes ]]
then
	comm -23 $textfiles $textfiles2 >$files2remove
	if [[ -s "${files2remove}" ]]
	then
		if [[ "${yflag}" == yes ]]
		then
			echo
			echo "Removing files ..."
			cat ${files2remove} | xargs rm -f
		else
			echo "Remove files? [y/n/<cr=n] "
			read ans
			case "${ans}" in
			y)
				echo
				echo "Removing files ..."
				cat ${files2remove} | xargs rm -f
				;;
			*)
				echo
				echo "No files removed."
				;;
			esac
		fi
	fi
fi
#
echo
echo "Done."
#
exit 0
