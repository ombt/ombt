#!/usr/bin/ksh
# set -x
#
# compare local files in a CPU load with what is on the SP.
#
#####################################################################
#
# local 
#
tfile="/tmp/t$$"
ttfile="/tmp/tt$$"
rfiles="/tmp/r$$"
lfiles="/tmp/l$$"
llfiles="/tmp/ll$$"
dfiles="/tmp/d$$"
ch2ioms="/tmp/ch2ioms$$"
#
# remote data
#
labid="${1}"
sp="${2}"
cpuload="${3}"
remotefile="${4}"
audtype="${5}"
verbose="${6}"
summary="${7}"
results="${8}"
ucond="${9}"
allfiles="${10}"
lastreload="${11}"
#
if [[ ! -w "${summary}" ]]
then
	# create the file
	echo "\n\n" >$summary
	echo "SUMMARY OF RESULTS\n" >>  $summary
	chmod 777 $summary
fi
#
if [[ ! -w "${results}" ]]
then
	# create the file
	echo "\n\n" >$results
	echo "AUDIT RESULTS\n" >> $results
	chmod 777 $results
fi
#
echo 
echo "Lab ID is ${labid}." 
echo "SP is ${sp}." 
echo "CPU Load is ${cpuload}." 
echo "Last Reload is ${lastreload}." 
echo "Remote File is ${remotefile}." 
echo "Audit Type is ${audtype}." 
#
# filter remote data into a useable form.
#
cat $remotefile |
cut -c20-33,47- | 
sed 's/^  *//g; s/  */ /g;' | 
nawk '{ print $2 " " $1; }' |
sort +0 -1 | 
uniq >$rfiles
#
# get list of local files associated with the CPU load.
#
if [[ -z "$LCSTOOLSDATA" ]]
then
	echo
	echo "LCSTOOLSDATA not set."
	exit 2
fi
if [[ ! -d "$LCSTOOLSDATA" ]]
then
	echo
	echo "LCSTOOLSDATA $LCSTOOLSDATA is not a directory."
	exit 2
fi
#
cd $LCSTOOLSDATA
#
if [[ "${lastreload}" != "UNKNOWN" ]]
then
	realcpuload="${lastreload}"
else
	realcpuload="${cpuload}"
fi
#
branch=$(uprintf -q -f"%s\n" branch in labloads where cpuload leq "$realcpuload" and labid leq "$labid")
if [[ -z "$branch" ]]
then
	echo
	echo "Unable to determine BRANCH for CPU load $realcpuload."
	exit 2
fi
echo
echo "Branch is $branch."
#
fsys=$(uprintf -q -f"%s\n" path in filesystems where branch leq "$branch" and type leq "text")
if [[ -z "$fsys" ]]
then
	echo
	echo "Unable to determine file system for CPU load $realcpuload."
	exit 2
fi
echo
echo "File System is $fsys."
#
uprintf -q -f"%s %s\n" type name in images where cpuload leq "$realcpuload" and branch leq "$branch" >$tfile
#
case "${audtype}" in
cpu)
	grep cpu $tfile | grep -v debug_cpu >$ttfile
	mv $ttfile $tfile
	;;
iom)
	grep -v cpu $tfile >$ttfile
	mv $ttfile $tfile
	# check if all files are audited, or just the required
	# ones in chassis2ioms.
	if [[ "${allfiles}" == 0 ]]
	then
		uprintf -q -f"%s\n" type in chassis2ioms where labid leq "$labid" and branch leq "$branch" | sort -u >$ch2ioms
		if [[ -s "$ch2ioms" ]]
		then
			echo
			echo "These IOM types will be audited:"
			cat $ch2ioms
			#
			cat $ch2ioms |
			while read iomtype
			do
				grep "${iomtype}.tar.gz" $tfile
			done >$ttfile
			mv $ttfile $tfile
		else
			echo
			echo "No data found in chassis2ioms. All IOMs will be audited."
		fi
	fi
	;;
*)
	# check if all files are audited, or just the required
	# ones in chassis2ioms.
	if [[ "${allfiles}" == 0 ]]
	then
		uprintf -q -f"%s\n" type in chassis2ioms where labid leq "$labid" and branch leq "$branch" | sort -u >$ch2ioms
		if [[ -s "$ch2ioms" ]]
		then
			echo
			echo "These IOM types will be audited:"
			cat $ch2ioms
			#
			cat $ch2ioms |
			while read iomtype
			do
				grep "${iomtype}.tar.gz" $tfile
			done >$ttfile
			# don't forget to add cpu
			grep cpu $tfile | grep -v debug_cpu >>$ttfile
			mv $ttfile $tfile
		else
			echo
			echo "No data found in chassis2ioms. All IOMs will be audited."
		fi
	fi
	;;
esac
#
>$lfiles
#
cat $tfile |
while read stype fname
do
	fpath="$fsys/$branch/$stype/$fname"
	tocfdir="$fsys/../toc/$branch/$stype"
	tocfpath="$fsys/../toc/$branch/$stype/$fname.toc"
	#
	echo
	echo "Checking $fpath ..."
	#
	if [[ -r "$fpath" ]]
	then
		if [[ "${verbose}" == 1 ]]
		then
			echo
			echo "File $fpath is readable ..."
			echo "Checking if TOC exists for tar file:"
			echo "==>> $fpath"
		fi
		#
		[[ ! -d "$tocfdir" ]] && mkdir -p $tocfdir;
		#
		if [[ "$tocfpath" -nt "$fpath" && $ucond == 0 ]]
		then
			echo "Using existing TOC ..."
			cat $tocfpath |
			sed 's/  */ /g;' |
			cut -d ' ' -f3,6 |
			nawk '{ print $2 " " $1; }' |
			sed 's/^/\//g;' >>$lfiles
		else
			echo "Creating new TOC ..."
			/opt/exp/gnu/bin/tar tzvf $fpath | 
			grep -v -- "->" |
			grep -v "^d" >$tocfpath
			chmod 777 $tocfpath
			#
			cat $tocfpath |
			sed 's/  */ /g;' |
			cut -d ' ' -f3,6 |
			nawk '{ print $2 " " $1; }' |
			sed 's/^/\//g;' >>$lfiles
		fi
	else
		echo
		echo "$fpath is NOT readable ..."
	fi
done
#
cat $lfiles |
sort +0 -1 | 
uniq >$llfiles
#
echo >>$results
echo "========================================================================" >>$results
echo "Comparing Load $realcpuload Files To $labid $sp $realcpuload files." >>$results
echo >>$results
comm -23 $llfiles $rfiles >$dfiles
#
if [[ -s "$dfiles" ]]
then
	echo "The following files where found to be different." >>$results
	echo >>$results
	cat $dfiles >>$results
	#
	echo "Files different between load $realcpuload and $labid $sp $realcpuload: $(cat $dfiles | wc -l  | sed 's/  *//g;' )." >> $summary
else
	echo "No differences found." >>$results
	echo >>$results
	#
	echo "No files different between load $realcpuload and $labid $sp $realcpuload." >> $summary
fi
echo "========================================================================" >>$results
#
#
rm -f /tmp/*$$ 2>/dev/null 1>/dev/null
#
exit 0

