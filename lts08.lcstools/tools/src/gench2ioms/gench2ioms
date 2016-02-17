#!/usr/bin/ksh
#
# generate relation chassis2ioms for a list of switches (labids).
#
#########################################################################
#
CMD=$(basename ${0})
PATH=$PATH:/opt/exp/lib/unity/bin
#
rtrvout=/tmp/rtrvout$$
clei=/tmp/clei$$
labcleis=/tmp/labcleis$$
labiomcleis=/tmp/labiomcleis$$
mergefile=/tmp/mergefile$$
fgreppatterns=/tmp/fgreppatterns$$
newchassis2ioms=/tmp/newchassis2ioms$$
newchassis2ioms2=/tmp/newchassis2ioms$$
#
trap 'rm -rf /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 3 4 5 6 15
#
usage() {
cat <<EOF

usage: ${CMD} [-?] [-x] [-M] [-L] [-B] [-o outfile] [-u username] [-p passwd]
	[-b branch[,branch2[,...]]] [ -R labid RE | [labid [labid2 ...] ] ]

where:
	-x - debug
	-? - usage message
	-L - run for all labs in chassis relation
	-B - run for all branches in chassis relation
	-o filename - output file name
	-u username - TL1 user name (default is telica)
	-p passwd - TL1 passwd (default is telica)
	-b branch[,branch2[,..]]] - comma-separated list of branches
	-R labid regular expression - a regular expression describing the
		labids to handle. all REs are automatically anchored
		using ^ and $.
	-M - merge new results with existing \$LCSTOOLSDATA/chassis2ioms
		relation.

labid of switch to scan and determine iom cleis. all output
is to standard out. variable LABID is used by default. the -C option
takes precedence over any lists of branches and labids that are given.

EOF
}
#
alllabids=no
allbranches=no
branches=
outfile=
username=telica
passwd=telica
labidRE=
merge=no
#
>${mergefile}
#
set -- `getopt ?MLBR:xb:o:u:p: ${*}`
if [[ ${?} -ne 0 ]]
then
	usage
	echo
	echo "Done."
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
	-L)
		alllabids=yes
		shift
		;;
	-M)
		merge=yes
		shift
		;;
	-B)
		allbranches=yes
		shift
		;;
	-b)
		branches="$(echo ${2} | tr ',' ' ')"
		shift 2
		;;
	-o)
		outfile=${2}
		shift 2
		;;
	-u)
		username=${2}
		shift 2
		;;
	-p)
		passwd=${2}
		shift 2
		;;
	-R)
		labidRE=${2}
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
cd ${LCSTOOLSDATA}
#
if [[ "${alllabids}" == yes ]]
then
	set -- $(uprintf -q -f"%s\n" labid in chassis | sort -u)
elif [[ -n "${labidRE}" ]]
then
	set -- $(uprintf -q -f"%s\n" labid in chassis where labid req "^${labidRE}$"| sort -u)
elif [[ $# == 0 ]]
then
	if [[ -n "${LABID}" ]]
	then
		set -- ${LABID}
	else
		echo
		echo "ERROR: LABID is not set and no labid was given."
		usage
		echo
		echo "Done."
		exit 2
	fi
fi
#
if [[ -z "${branches}" && "${allbranches}" == no ]]
then
	PS3="Choose a branch: "
	select branch in QUIT $(cat ${LCSTOOLSDATA}/chassis | cut -d'	' -f2 | sort -u)
	do
		case "${branch}" in
		QUIT)
			echo "Quitting ${CMD}."
			echo
			echo "Done."
			exit 0
			;;
		*)
			break 1
			;;
		esac
	done
	#
	branches=${branch}
fi
#
if [[ -n "${outfile}" ]]
then
	>${outfile}
fi
#
for labid in ${*}
do
	echo
	echo "Using RTRV-EQPT-ALL to get ${labid} IOM CLEIS:"
	echo
	#
	>${labcleis}
	#
	tl1 -u ${username} -p ${passwd} -l ${labid} RTRV-EQPT-ALL 2>/dev/null |
	tr '[A-Z]' '[a-z]' |
	sed -n 's/^.*iom-\([0-9][0-9]*\):.*iomoduletype=\([^,:]*\).*rearmoduletype=\([^,:]*\).*$/<\1> <\2> <\3>/p' >${labiomcleis}
	#
	if [[ "${allbranches}" == yes ]]
	then
		branches="$(uprintf -q -f"%s\n" branch in chassis where labid req "^${labid}$" | sort -u)"
	fi
	#
	echo
	echo "Branch(es) for ${labid}:"
	echo "${branches}"
	echo
	#
	for branch in ${branches}
	do
		cat ${labiomcleis} |
		nawk 'BEGIN {
			labid = "'"${labid}"'";
			labcleis = "'"${labcleis}"'";
			branch = "'"${branch}"'";
			cleifile = "'"${LCSTOOLSDATA}/ncleis"'";
			#
			while (getline tuple < cleifile > 0)
			{
				split(tuple, fields, /\t/);
				filebranch = fields[1];
				fileclei = fields[2];
				filerearclei = fields[3];
				fileisrearreq = fields[4];
				filetype = fields[5];
				cleis[filebranch,fileclei,filerearclei] = filetype;
			}
		}
		$1 ~ /<[0-9]+>/ && $2 != /<>/ {
			# get iom and front clei
			iom = substr($1,2,length($1)-2);
			# check if we have a clei
			if (length($2) > 2) {
				clei = substr($2,2,length($2)-2);
			} else {
				# iom is not provisioned
				next;
			}
			# check if we have a rear clei
			if (length($3) > 2) {
				rearclei = substr($3,2,length($3)-2);
				if (rearclei == "unprov" || rearclei == "null") {
					rearclei = "none";
				}
			} else {
				rearclei = "none";
			}
			# determine iom type
			if ((branch,clei,rearclei) in cleis) {
				type = cleis[branch,clei,rearclei];
			} else if (("default",clei,rearclei) in cleis) {
				type = cleis["default",clei,rearclei];
			} else if ((branch,clei,"none") in cleis) {
				type = cleis[branch,clei,"none"];
			} else if (("default",clei,"none") in cleis) {
				type = cleis["default",clei,"none"];
			} else {
				type = "UNKNOWN_" clei "_" rearclei;
			}
			# print results
			printf("%s\t", labid);
			printf("%s\t", branch);
			printf("%s\t", iom);
			printf("%s\n", type);
			#
			printf("%s\t", labid) >> labcleis;
			printf("%s\t", branch) >> labcleis;
			printf("%s\t", iom) >> labcleis;
			printf("%s\n", type) >> labcleis;
			next;
		}
		END {
		} ' 
	done
	#
	# check if we found anything
	#
	if [[ ! -s "${labcleis}" ]]
	then
		echo
		echo "WARNING: No IOM/CLEIS found for labid ${labid}"
	elif [[ -n "${outfile}" ]]
	then
		cat ${labcleis} >>${outfile}
	fi
	#
	cat ${labcleis} >>${mergefile}
done
#
# do we need to merge the results?
#
if [[ "${merge}" == yes ]]
then
	if [[ -s "${mergefile}" ]]
	then
		echo 
		echo "Merging results with $LCSTOOLSDATA/chassis2ioms.";
		#
		cat ${mergefile} | 
		cut -d'	' -f1,2 | 
		sed -n 's/^/\^/p' |
		sort -u >${fgreppatterns}
		#
		fgrep -v -f ${fgreppatterns} ${LCSTOOLSDATA}/chassis2ioms >${newchassis2ioms}
		cat ${mergefile} >>${newchassis2ioms}
		sort -u -o ${newchassis2ioms} ${newchassis2ioms}
		#
		cp ${newchassis2ioms} ${LCSTOOLSDATA}/chassis2ioms
	else
		echo
		echo "No new records generated. No merge will be done."
	fi
	
fi
#
echo
echo "Done."
exit 0


