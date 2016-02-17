#
# get a list of images for a given: branch, branch type and type of
# image. the allowed branch types are: dev and beta.  the supported
# list of image types is: all, cpu, ... whatever.
#
############################################################################
#
# tmp files
#
tmp=/tmp/tmp$$
tmp2=/tmp/tmp2$$
supported=/tmp/supported$$
required=/tmp/required$$
requiredfiles=/tmp/requiredfiles$$
files=/tmp/files$$
sortedfiles=/tmp/sortedfiles$$
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1;' 0 1 2
#
lflag=no
dflag=no	# debug flag, get debug files also.
usage="
usage: ${0} [-?xld] branch branchtype imagetype
where:
\t-? - usage message
\t-x - debug mode
\t-l - list mode (???)
\t-d - get CPU debug files
"
#
# supported image file types.
#
# NOTE: tca,bts are treated differently than the other file types.
#
cat >$supported <<EOF
ana
atmds3
cm
cpu
ds1_2
ds1_c
ds1
ds3
e1
ena2
ena
octds3_2
octds3_3
octds3
pna
tdmoc
trids3_3
trids3_d
trids3
voip6
voip
vs2
vs3
vsm
bts
tca
EOF
#
# cmd line options
#
set -- $(getopt ?dxl ${*})
if [[ $? != 0 ]]
then
	echo "ERROR: invalid option, ${usage}."
	exit 2
fi
#
for opt in ${*}
do
	case "${opt}" in
	-x)
		set -x 
		shift
		;;
	-l)
		lflag=yes
		shift
		;;
	-d)
		dflag=yes
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
branch="${1}"
branchtype="${2}"
imagetype="${3}"
btsbranch="${4}"
#
# echo
# echo "Starting ${0}: branch=<$branch>, type=<$branchtype>, image=<$imagetype>"
# echo "${0} Args: <${*}>"
# echo
#
# sanity check
#
if [[ -z "${branch}" ]]
then
	echo "ERROR: branch is not given."
	exit 2
fi
if [[ -z "${branchtype}" ]]
then
	echo "ERROR: branch type is not given."
	exit 2
fi
if [[ -z "${imagetype}" ]]
then
	echo "ERROR: image type is not given."
	exit 2
fi
#
case "${branchtype}" in
dev)
	# ok
	;;
beta)
	# ok
	;;
*)
	# not supported
	echo "ERROR: branch type <${branchtype}> is not supported."
	exit 2
	;;
esac
#
if [[ "$imagetype" == "all" ]]
then
	cp $supported $required
else
	grep "^${imagetype}$" $supported >$required
	#
	if [[ $? != 0 ]]
	then
		echo "ERROR: image type ${imagetype} is not supported."
		exit 2
	fi
fi
#
# generate list of files and filter by type.
#
case "${branchtype}" in
dev)
	roots="/telica/toaster/home/release/${branch} /telica/toaster/home/release/PLXStats"
	;;
beta)
	# search under release/ECO and newpatches
	ecobranch="${branch#BP-}"
	#
	roots="/home2/builder/newpatches/${branch}"
	# roots="${roots} /telica/toaster/home/release/ECO/${ecobranch} /telica/toaster/home/release/ECO/${ecobranch}-* /telica/toaster/home/release/${branch} /telica/toaster/home/release/PLXStats"
	roots="${roots} /telica/toaster/home/release/ECO/CI/${ecobranch} /telica/toaster/home/release/ECO/${ecobranch} /telica/toaster/home/release/ECO/CI/${ecobranch}-* /telica/toaster/home/release/ECO/${ecobranch}-* /telica/toaster/home/release/${branch} /telica/toaster/home/release/PLXStats"
	;;
*)
	# not supported
	echo "ERROR: branch type ${branchtype} is not supported."
	exit 2
	;;
esac
#
cat $required | grep -v '^bts$' | sed 's/^/_/; s/$/.tar.gz/' >$requiredfiles
#
case "${imagetype}" in
all)
	echo "BTS_.*tar.gz" >>$requiredfiles
	echo "TCA_.*tar.gz" >>$requiredfiles
	;;
bts)
	echo "BTS_.*tar.gz" >>$requiredfiles
	;;
tca)
	echo "TCA_.*tar.gz" >>$requiredfiles
	;;
esac
#
if [[ "${dflag}" == yes ]]
then
	# get debug products also
	echo "/DEBUG/.*tar.gz" >>$requiredfiles
fi
#
for root in ${roots}
do
	find $root -type f -name '*.tar.gz' -print 2>/dev/null |
	grep -f $requiredfiles
done >$files
#
if [[ "${btsbranch}" != "${branch}" && "${btsbranch}" != "UNKNOWN" ]]
then
	case "${imagetype}" in
	all|bts)
		extrabtsroot="/telica/toaster/home/release/${btsbranch}"
		find $extrabtsroot -type f -name '*.tar.gz' -print 2>/dev/null |
		grep 'BTS_.*tar.gz' >>$files
		;;
	esac
fi
#
if [[ ! -s "${files}" ]]
then
	echo "ERROR: No files found for type: ${imagetype}."
	exit 2
fi
#
# sort the files by time stamp
#
cat $supported |
while read imagetype
do
	#
	# get regular tar files
	#
	case "${imagetype}" in
	bts)
		echo "BTS.*tar.gz" >$tmp
		grep -f $tmp $files >$tmp2
		;;
	tca)
		echo "TCA.*tar.gz" >$tmp
		grep -f $tmp $files >$tmp2
		;;
	*)
		echo "_${imagetype}.tar.gz" >$tmp
		grep -f $tmp $files >$tmp2
		;;
	esac
	#
	if [[ -s "${tmp2}" ]]
	then
		if [[ "${lflag}" == yes  ]]
		then
			ls -tl $(cat ${tmp2}) |
			sed 's/  */ /g' |
			cut -d' ' -f5,9 |
			awk '{ print $2 " " $1; }' |
			sed -n "s/^/${imagetype} /p" |
			sed -n "1,1p"
		else 
			ls -tl $(cat ${tmp2}) |
			sed 's/  */ /g' |
			cut -d' ' -f5,9 |
			awk '{ print $2 " " $1; }' |
			sed -n "s/^/${imagetype} /p"
		fi
	else
		echo "${imagetype} NONE 0"
	fi
	#
	# do we need debug files?
	#
	if [[ "${dflag}" == no ]]
	then
		# skip getting debug files.
		continue
	fi
	#
	# get debug files
	#
	case "${imagetype}" in
	cpu|cm)
		echo "_${imagetype}.debug.tar.gz" >$tmp
		grep -f $tmp $files >$tmp2
		;;
	*)
		continue;
		;;
	esac
	#
	if [[ -s "${tmp2}" ]]
	then
		if [[ "${lflag}" == yes  ]]
		then
			ls -tl $(cat ${tmp2}) |
			sed 's/  */ /g' |
			cut -d' ' -f5,9 |
			awk '{ print $2 " " $1; }' |
			sed -n "s/^/debug_${imagetype} /p" |
			sed -n "1,1p"
		else 
			ls -tl $(cat ${tmp2}) |
			sed 's/  */ /g' |
			cut -d' ' -f5,9 |
			awk '{ print $2 " " $1; }' |
			sed -n "s/^/debug_${imagetype} /p"
		fi
	else
		echo "debug_${imagetype} NONE 0"
	fi
done > $sortedfiles
#
cat $sortedfiles
#
exit 0
