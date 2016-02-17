#!/usr/bin/ksh
#
# read tl1 hardware scripts and generates TDM relation.
#
###########################################################################
#
usage="
\n
usage: ${0} [-?] [-xva] [-D | -b | -s | -d dir] \n
\n
where \n
\t-x - debug \n
\t-v - verbose \n
\t-? - help text \n
\t-b - basescripts are searched (default). \n
\t-s - scripts are searched. \n
\t-d - scripts are searched in directory dir. \n
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
set -- $(getopt ?d:xDvbs ${*})
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
	-d)
		scriptsdir=${2}
		shift 2
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
		echo 'tl1cmd\tt\\t\tl20'
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
	egrep -i '(ent-t1:|ent-e1:|ent-t3:|ent-sts1:|ent-oc3:|ent-oc12:)' | 
	sed 's/^[	 ]*"//; s/"[	 ]*$//;' |
	tr '[A-Z]' '[a-z]'
done |
sed 's/^[ 	]*//g' |
nawk 'BEGIN {
	verbose = '"${verbose}"'
	scriptstype = '"${scriptstype}"'
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
$0 ~ /^ent-t1:/ {
	if (verbose) print "ENT-T1: " $0;
	writerec($0, "ent-t1");
	next;
}
$0 ~ /^ent-e1:/ {
	if (verbose) print "ENT-E1: " $0;
	writerec($0, "ent-e1");
	next;
}
$0 ~ /^ent-t3:/ {
	if (verbose) print "ENT-T3: " $0;
	writerec($0, "ent-t3");
	next;
}
$0 ~ /^ent-sts1:/ {
	if (verbose) print "ENT-STS1: " $0;
	writerec($0, "ent-sts1");
	next;
}
$0 ~ /^ent-oc3:/ {
	if (verbose) print "ENT-OC3: " $0;
	writerec($0, "ent-oc3");
	next;
}
$0 ~ /^ent-oc12:/ {
	if (verbose) print "ENT-OC12: " $0;
	writerec($0, "ent-oc12");
	next;
}
{
	if (verbose) print "UNKNOWN: " $0;
	next;
}
END {
}
function writerec(tl1cmd, cmdtype) {
	colonno = split(tl1cmd, colonfields, ":");
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
		printf("%s\t", cmdtype);
		printf("%s\n", "NONE");
	}
	#
	printf("%s\t", labid);
	printf("%s\t", scriptlabid);
	printf("%s\t", branch);
	printf("%s\t", "enetid");
	printf("%s\t", colonfields[3]);
	printf("%s\t", fname);
	printf("%s\t", cmdtype);
	printf("%s\n", "NONE");
	#
	printf("%s\t", labid);
	printf("%s\t", scriptlabid);
	printf("%s\t", branch);
	printf("%s\t", "pst");
	printf("%s\t", colonfields[7]);
	printf("%s\t", fname);
	printf("%s\t", cmdtype);
	printf("%s\n", "NONE");
} '
#
exit 0
