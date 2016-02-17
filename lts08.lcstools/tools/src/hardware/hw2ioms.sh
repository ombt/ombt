#!/usr/bin/ksh
#
# read tl1 hardware scripts and generates iom relation.
#
###########################################################################
#
usage="
\n
usage: ${0} [-?] [-xva] [-D | -b | -s | -S directory] \n
\n
where \n
\t-x - debug \n
\t-v - verbose \n
\t-? - help text \n
\t-a - filter all scripts, otherwise only hardware scripts are filtered. \n
\t-b - basescripts are searched (default). \n
\t-s - scripts are searched. \n
\t-S DIR - search scripts in private directory, DIR. \n
\t-D - print Descriptor file to stdout \n
\n
"
#
verbose=0
hardwareonly="-name '*hardware*'"
scriptstype=basescripts
scriptsdir=$LCSBASESCRIPTS
privscriptsdir=
dflag=0
#
set -- $(getopt ?xS:Dvabs ${*})
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
	-S)
		scriptstype=private
		privscriptsdir=${2}
		if [[ ! -d "${privscriptsdir}" ]]
		then
			echo "Private scripts directory ${privscriptsdir} is not readable." >&2
			exit 2
		fi
		scriptsdir=${privscriptsdir}
		shift 2
		;;
	-v)
		verbose=1
		shift
		;;
	-D)
		dflag=1
		echo 'labid\tt\\t\tl9'
		echo 'scriptlabid\tt\\t\tl9'
		echo 'branch\tt\\t\tl15'
		echo 'iom\tt\\t\tl3'
		echo 'type\tt\\t\tl8'
		echo 'clei\tt\\t\tl12'
		echo 'rearmodclei\tt\\t\tl16'
		echo 'redundancy\tt\\t\tl5'
		echo 'initialstate\tt\\t\tl5'
		echo 'threshold1\tt\\t\tl5'
		echo 'threshold2\tt\\t\tl5'
		echo 'threshold3\tt\\t\tl5'
		echo 'filename\tt\\t\tl20'
		echo 'tl1cmd\tt\\t\tl20'
		echo 'remarks\tt\\n\t40'
		exit 0
		;;
	-a)
		hardwareonly=
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ -z "$LCSTOOLSDATA" ]]
then
	echo "LCSTOOLSDATA is not set." >&2
	exit 2
fi
if [[ ! -r "$LCSTOOLSDATA/ncleis" ]]
then
	echo "relation $LCSTOOLSDATA/ncleis is not readable." >&2
	exit 2
fi
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
eval find . $hardwareonly -type f -print |
sort -u |
while read file
do
	echo "FILE: $file"
	cat $file | 
	egrep -i '(ent-eqpt|ed-eqpt)' | 
	sed 's/^[	 ]*"//; s/"[	 ]*$//;' |
	tr '[A-Z]' '[a-z]'
done |
sed 's/^[ 	]*//g' |
nawk 'BEGIN {
	verbose = '"${verbose}"';
	scriptstype = "'"${scriptstype}"'";
	cleifile = "'"${LCSTOOLSDATA}/ncleis"'";
	#
	while (getline tuple < cleifile > 0)
	{
		split(tuple, fields, /\t/);
		branch = fields[1];
		clei = fields[2];
		rearclei = fields[3];
		isrearreq = fields[4];
		type = fields[5];
		cleis[branch,clei,rearclei] = type;
	}
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
	next;
}
$0 ~ /^ed-chassis-eqpt:/ {
	if (verbose) print "ED-CHASSIS-EQPT: " $0;
	next;
}
$0 ~ /^ent-eqpt:/ {
	split($0, data, /:/);
	iom = data[3];
	iomno = substr(iom, index(iom,"-")+1);
	moredata = data[6];
	status = data[7];
	if (verbose) {
		print "TL1-CMD: " $0;
		print "ENT-EQPT <" iom "," moredata "," status ">";
	}
	split(moredata, datafields, /,/);
	clei = datafields[1];
	# no rear clei exists for ent-eqpt command
	if ((branch,clei,"none") in cleis) {
		type = cleis[branch,clei,"none"];
	} else if (("default",clei,"none") in cleis) {
		type = cleis["default",clei,"none"];
	} else {
		type = "UNKNOWNCLEI";
	}
	redundancy = datafields[2];
	threshold1 = datafields[3];
	threshold2 = datafields[4];
	threshold3 = datafields[5];
	software = datafields[6];
	if (verbose) printf("ENT-EQPT: ");
	scriptlabid = labid;
	if (labid ~ /^ihlcsg[0-9]*$/) {
		# correction for 8 char LTS limit.
		sub(/^ihlcsg/, "ihlcsgw", scriptlabid);
	}
	printf("%s\t", labid);
	printf("%s\t", scriptlabid);
	printf("%s\t", branch);
	printf("%s\t", iomno);
	printf("%s\t", type);
	printf("%s\t", clei);
	printf("%s\t", "NONE");
	printf("%s\t", redundancy);
	printf("%s\t", status);
	printf("%s\t", threshold1);
	printf("%s\t", threshold2);
	printf("%s\t", threshold3);
	printf("%s\t", fname);
	printf("%s\t", "ent-eqpt");
	printf("%s\n", "NONE");
	next;
}
$0 ~ /^ent-eqpt-all:/ {
	split($0, data, /:/);
	iom = data[3];
	iomno = substr(iom, index(iom,"-")+1);
	moredata = data[6];
	status = data[7];
	if (verbose) {
		print "TL1-CMD: " $0;
		print "ENT-EQPT-ALL <" iom "," moredata "," status ">";
	}
	split(moredata, datafields, /,/);
	# use clei and rear clei to determine type
	clei = substr(datafields[1], index(datafields[1],"=")+1);
	rearmodcli = substr(datafields[7], index(datafields[7],"=")+1);
	#
	if ((branch,clei,rearmodcli) in cleis) {
		type = cleis[branch,clei,rearmodcli];
	} else if (("default",clei,rearmodcli) in cleis) {
		type = cleis["default",clei,rearmodcli];
	} else if ((branch,clei,"none") in cleis) {
		type = cleis[branch,clei,"none"];
	} else if (("default",clei,"none") in cleis) {
		type = cleis["default",clei,"none"];
	} else {
		type = "UNKNOWNCLEI";
	}
	redundancy = substr(datafields[2], index(datafields[2],"=")+1);
	threshold1 = substr(datafields[3], index(datafields[3],"=")+1);
	threshold2 = substr(datafields[4], index(datafields[4],"=")+1);
	threshold3 = substr(datafields[5], index(datafields[5],"=")+1);
	software = substr(datafields[6], index(datafields[6],"=")+1);
	if (verbose) printf("ENT-EQPT-ALL: ");
	scriptlabid = labid;
	if (labid ~ /^ihlcsg[0-9]*$/) {
		# correction for 8 char LTS limit.
		sub(/^ihlcsg/, "ihlcsgw", scriptlabid);
	}
	printf("%s\t", labid);
	printf("%s\t", scriptlabid);
	printf("%s\t", branch);
	printf("%s\t", iomno);
	printf("%s\t", type);
	printf("%s\t", clei);
	printf("%s\t", rearmodcli);
	printf("%s\t", redundancy);
	printf("%s\t", status);
	printf("%s\t", threshold1);
	printf("%s\t", threshold2);
	printf("%s\t", threshold3);
	printf("%s\t", fname);
	printf("%s\t", "ent-eqpt-all");
	printf("%s\n", "NONE");
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

