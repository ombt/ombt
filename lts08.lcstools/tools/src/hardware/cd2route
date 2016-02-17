#!/usr/bin/ksh
#
# read calldata scripts and generate relations for routing.
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
scriptsdir=$LCSBASESCRIPTS
dflag=0
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
		scriptsdir=$LCSBASESCRIPTS
		shift
		;;
	-s)
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
	tr '[A-Z]' '[a-z]' |
	egrep -i '(ent-trkgrp:|ent-route-list:|ent-digitmod-dn:|ent-route-digits:|ent-ss7-trk:|ent-digitmod-tms:|ent-cas-trk:ent-national-npa:|ent-isdn-if:|ent-isdn-lnk:|ent-route:|ent-isdn-bchan:|ent-trans-plan:)' | 
	sed 's/^[	 ]*"//; s/"[	 ]*$//;'
done |
sed 's/^[ 	]*//g' |
nawk 'BEGIN {
	verbose = '"${verbose}"'
}
$0 ~ /^FILE:/ {
	split($0, data, /\//);
	labid = data[2];
	branch = data[3];
	fname = data[5];
	if (verbose) print "FILE <" labid "," branch "," fname ">";
	scriptlabid = labid;
	if (labid ~ /^ihlcsg[0-9]*$/) {
		# correction for 8 char LTS limit.
		sub(/^ihlcsg/, "ihlcsgw", scriptlabid);
	}
	next;
}
$0 ~ /^ent-trkgrp:/ {
	if (verbose) print "ent-trkgrp: " $0;
	writerec($0, "ent-trkgrp", 7);
	next;
}
$0 ~ /^ent-route-list:/ {
	if (verbose) print "ent-route-list: " $0;
	writerec($0, "ent-route-list", -1);
	next;
}
$0 ~ /^ent-digitmod-dn:/ {
	if (verbose) print "ent-digitmod-dn: " $0;
	writerec($0, "ent-digitmod-dn", 6);
	next;
}
$0 ~ /^ent-route-digits:/ {
	if (verbose) print "ent-route-digits: " $0;
	writerec($0, "ent-route-digits", 6);
	next;
}
$0 ~ /^ent-ss7-trk:/ {
	if (verbose) print "ent-ss7-trk: " $0;
	writerec($0, "ent-ss7-trk", 6);
	next;
}
$0 ~ /^ent-digitmod-tms:/ {
	if (verbose) print "ent-digitmod-tms: " $0;
	writerec($0, "ent-digitmod-tms", 6);
	next;
}
$0 ~ /^ent-cas-trk:/ {
	if (verbose) print "ent-cas-trk: " $0;
	writerec($0, "ent-cas-trk", 6);
	next;
}
$0 ~ /^ent-national-npa:/ {
	if (verbose) print "ent-national-npa: " $0;
	writerec($0, "ent-national-npa", -1);
	next;
}
$0 ~ /^ent-isdn-if:/ {
	if (verbose) print "ent-isdn-if: " $0;
	writerec($0, "ent-isdn-if", 6);
	next;
}
$0 ~ /^ent-isdn-lnk:/ {
	if (verbose) print "ent-isdn-lnk: " $0;
	writerec($0, "ent-isdn-lnk", 6);
	next;
}
$0 ~ /^ent-route:/ {
	if (verbose) print "ent-route: " $0;
	writerec($0, "ent-route", 6);
	next;
}
$0 ~ /^ent-isdn-bchan:/ {
	if (verbose) print "ent-isdn-bchan: " $0;
	writerec($0, "ent-isdn-bchan", 6);
	next;
}
$0 ~ /^ent-trans-plan:/ {
	if (verbose) print "ent-trans-plan: " $0;
	writerec($0, "ent-trans-plan", 6);
	next;
}
{
	if (verbose) print "UNKNOWN: " $0;
	next;
}
END {
}
function writerec(tl1cmd, cmdtype, commafld) {
	colonno = split(tl1cmd, colonfields, ":");
	for (ic=1; verbose && ic<=colonno; ic++)
	{
		printf("%2d: %s\n", ic, colonfields[ic]);
	}
	#
	if (commafld <= 0) {
		return;
	}
	#
	commano = split(colonfields[commafld], commafields, ",");
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
	# printf("%s\t", labid);
	# printf("%s\t", scriptlabid);
	# printf("%s\t", branch);
	# printf("%s\t", "enetid");
	# printf("%s\t", colonfields[3]);
	# printf("%s\t", fname);
	# printf("%s\t", cmdtype);
	# printf("%s\n", "NONE");
	# #
	# printf("%s\t", labid);
	# printf("%s\t", scriptlabid);
	# printf("%s\t", branch);
	# printf("%s\t", "pst");
	# printf("%s\t", colonfields[7]);
	# printf("%s\t", fname);
	# printf("%s\t", cmdtype);
	# printf("%s\n", "NONE");
} '
#
exit 0
