# turn AMA on for a lab.
#
case "${0}" in
*amagen-on)
	AMAOPER=on
	;;
*amagen-off)
	AMAOPER=off
	;;
*)
	echo "unknown AMA operation." >&2
	echo "usage: ${0} [tl1exec options ...]" >&2
	exit 2
	;;
esac
#
PATH=~lcstools/tools/bin:~exptools/lib/unity/bin:$PATH
#
if [[ -z "${LABID}" ]] 
then
	echo "LABID is not set." >&2
	echo "usage: ${0} [tl1exec options ...]" >&2
	exit 2
fi
#
if [[ -z "${LCSTOOLSDATA}" ]] 
then
	echo "LCSTOOLSDATA is not set." >&2
	echo "usage: ${0} [tl1exec options ...]" >&2
	exit 2
fi
#
if [[ "${1}" == "-?" ]]
then
	echo "usage: ${0} [tl1exec options ...]" >&2
	exit 0
fi
#
cd $LCSTOOLSDATA
#
activesp=$(lcsstat $LABID | grep active)
#
case "${activesp}" in
*SP_A_IP*)
	activespip=$(uprintf -q cpu_a_ip in chassis where labid req $LABID| sed -n '1,1p')
	echo "SP-A-IP=$activespip"
	;;
*SP_B_IP*)
	activespip=$(uprintf -q cpu_b_ip in chassis where labid req $LABID| sed -n '1,1p')
	echo "SP-B-IP=$activespip"
	;;
*)
	echo "unknown active SP found."
	activespip=$(uprintf -q cpu_a_ip in chassis where labid req $LABID| sed -n '1,1p')
	echo "SP-A-IP=$activespip"
	cat >/tmp/amaon.$$ <<EOF
rtrv-billsys
ed-billsys:${LABID}::::amagen=${AMAOPER}
rtrv-billsys
EOF
	tl1exec -i "${activespip}" "${@}" /tmp/amaon.$$
	#
	activespip=$(uprintf -q cpu_b_ip in chassis where labid req $LABID| sed -n '1,1p')
	echo "SP-B-IP=$activespip"
	cat >/tmp/amaon.$$ <<EOF
rtrv-billsys
ed-billsys:${LABID}::::amagen=${AMAOPER}
rtrv-billsys
EOF
	tl1exec -i "${activespip}" "${@}" /tmp/amaon.$$
	exit 2
	;;
esac
#
cat >/tmp/amaon.$$ <<EOF
rtrv-billsys
ed-billsys:${LABID}::::amagen=${AMAOPER}
rtrv-billsys
EOF
#
tl1exec -i "${activespip}" "${@}" /tmp/amaon.$$
#
rm -f /tmp/amaon.$$ 1>/dev/null 2>&1
#
exit 0
