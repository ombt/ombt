#!/usr/bin/ksh
#
# read tl1 hardware scripts and generates iom relation.
#
###########################################################################
#
usage="
\n
usage: ${0} [-?] [-xva] [-D | -b | -s] \n
\n
where \n
\t-x - debug \n
\t-v - verbose \n
\t-? - help text \n
\t-b - basescripts are searched (default). \n
\t-s - scripts are searched. \n
\t-D - print Descriptor file to stdout \n
\n
"
#
verbose=0
scriptstype=basescripts
scriptsdir=$LCSBASESCRIPTS
dflag=0
# finalsort="sort -t'	' +0 -3"
#
set -- $(getopt ?xDvbs ${*})
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
	-b)
		scriptstype=basescripts
		scriptsdir=$LCSBASESCRIPTS
		shift
		;;
	-s)
		scriptstype=scripts
		scriptsdir=$LCSSCRIPTS
		shift
		;;
	-v)
		verbose=1
		# finalsort="cat"
		shift
		;;
	-D)
		dflag=1
		echo 'labid\tt\\t\tl9'
		echo 'scriptlabid\tt\\t\tl9'
		echo 'branch\tt\\t\tl15'
		echo 'fieldname\tt\\t\tl20'
		echo 'fieldvalue\tt\\t\tl20'
		echo 'filename\tt\\t\tl20'
		echo 'remarks\tt\\n\t40'
		exit 0
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ -z "${scriptsdir}" ]]
then
	echo "scripts directory is not set." >&2
	exit 2
fi
#
if [[ ! -d "${scriptsdir}" ]]
then
	echo "directory ${scriptsdir} does not exist." >&2
	exit 2
fi
#
cd ${scriptsdir}
#
find . -type f -print |
sort -u |
while read file
do
	echo "FILE: $file"
	cat $file | 
	egrep -i 'ed-chassis-eqpt' | 
	sed 's/^[	 ]*"//; s/"[	 ]*$//;' |
	tr '[A-Z]' '[a-z]'
done |
sed 's/^[ 	]*//g' |
nawk 'BEGIN {
	verbose = '"${verbose}"';
	scriptstype = "'"${scriptstype}"'";
}
$0 ~ /^FILE:/ {
	split($0, data, /\//);
	if (scriptstype == "basescripts") {
		labid = data[2];
		branch = data[3];
		fname = data[5];
	} else if (scriptstype == "scripts") {
		labid = data[4];
		branch = data[2];
		fname = data[5];
	} else {
		# default to basescripts for now.
		labid = data[2];
		branch = data[3];
		fname = data[5];
	}
	if (verbose) print "FILE <" labid "," branch "," fname ">";
	scriptlabid = labid;
	if (labid ~ /^ihlcsg[0-9]*$/) {
		# correction for 8 char LTS limit.
		sub(/^ihlcsg/, "ihlcsgw", scriptlabid);
	}
	next;
}
$0 ~ /^ed-chassis-eqpt:/ {
	if (verbose) print "ED-CHASSIS-EQPT: " $0;
	#
	colonno = split($0, colonfields, ":");
	for (ic=1; verbose && ic<=colonno; ic++)
	{
		printf("%2d: %s\n", ic, colonfields[ic]);
	}
	#
	commano = split(colonfields[6], commafields, ",");
	for (ic=1; ic<=commano; ic++)
	{
		if (verbose) printf("%2d: %s\n", ic, commafields[ic]);
		if (length(commafields[ic]) <= 0) {
			continue;
		}
		equalno = split(commafields[ic], equalfields, "=");
		if (equalno != 2) {
			continue;
		}
		printf("%s\t", labid);
		printf("%s\t", scriptlabid);
		printf("%s\t", branch);
		printf("%s\t", equalfields[1]);
		printf("%s\t", equalfields[2]);
		printf("%s\t", fname);
		printf("%s\n", "NONE");
	}
	#
	next;
}
{
	if (verbose) print "UNKNOWN: " $0;
}
END {
} '
# } ' |
# sort -t'	' +0 -3
#
exit 0
