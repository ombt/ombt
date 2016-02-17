#!/usr/bin/ksh
#
# ping one or both SPs.
#
#########################################################################
#
CMD=${0}
PATH=$PATH:/opt/exp/lib/unity/bin
#
trap 'rm -rf /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 3 4 5 6 15
#
usage="\n
usage: ${CMD} [-?] [-x] [-a] [-A|-B] [labid] \n
\n
where: \n
\t-x - debug \n
\t-? - usage message \n
\t-a - ping all labids in chassis relation \n
\t-A - ping SP-A \n
\t-B - ping SP-B \n
\n
"
#
alllabids=no
sps_to_ping=both
#
set -- `getopt ?axAB ${*}`
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
	-a)
		alllabids=yes
		shift
		;;
	-A)
		sps_to_ping=spa
		shift
		;;
	-B)
		sps_to_ping=spb
		shift
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
elif [[ $# == 0 ]]
then
	set -- ${LABID}
fi
#
for labid in ${*}
do
	uprintf -q -f"%s %s\n" cpu_a_ip cpu_b_ip in chassis where labid req "^${labid}$" |
	sort -u |
	while read spaip spbip
	do
		case "${sps_to_ping}" in
		spa)
			echo
			echo "Ping ${labid} SP-A ..."
			/usr/sbin/ping ${spaip} 2
			;;
		spb)
			echo
			echo "Ping ${labid} SP-B ..."
			/usr/sbin/ping ${spbip} 2
			;;
		both)
			echo
			echo "Ping ${labid} SP-A ..."
			/usr/sbin/ping ${spaip} 2
			echo "Ping ${labid} SP-B ..."
			/usr/sbin/ping ${spbip} 2
			;;
		*)
			;;
		esac
	done
done
#
exit 0
