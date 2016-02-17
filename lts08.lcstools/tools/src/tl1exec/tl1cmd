# exec a tl1 cmd
sendcmd() {
	cat >/tmp/tl1cmd.$$ <<EOF
${TL1CMD}
EOF
	#
	tl1exec -i "${activespip}" "${@}" /tmp/tl1cmd.$$
	#
	return 0
}
#
PATH=~lcstools/tools/bin:~exptools/lib/unity/bin:$PATH
#
if [[ -z "${LABID}" ]] 
then
	echo "LABID is not set." >&2
	echo "usage: ${0} tl1-cmd [tl1exec options ...]" >&2
	rm -f /tmp/tl1cmd.$$ 1>/dev/null 2>&1
	exit 2
fi
#
if [[ -z "${LCSTOOLSDATA}" ]] 
then
	echo "LCSTOOLSDATA is not set." >&2
	echo "usage: ${0} [tl1exec options ...]" >&2
	rm -f /tmp/tl1cmd.$$ 1>/dev/null 2>&1
	exit 2
fi
#
if [[ "${1}" == "-?" || ${#} == 0 ]]
then
	echo "usage: ${0} tl1-cmd [tl1exec options ...]" >&2
	rm -f /tmp/tl1cmd.$$ 1>/dev/null 2>&1
	exit 0
fi
#
TL1CMD="${1}"
shift
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
	cat >/tmp/tl1cmd.$$ <<EOF
${TL1CMD}
EOF
	tl1exec -i "${activespip}" "${@}" /tmp/tl1cmd.$$
	#
	echo "SP-A-IP=$activespip"
	activespip=$(uprintf -q cpu_b_ip in chassis where labid req $LABID| sed -n '1,1p')
	echo "SP-B-IP=$activespip"
	cat >/tmp/tl1cmd.$$ <<EOF
${TL1CMD}
EOF
	tl1exec -i "${activespip}" "${@}" /tmp/tl1cmd.$$
	rm -f /tmp/tl1cmd.$$ 1>/dev/null 2>&1
	exit 0
	;;
esac
#
cat >/tmp/tl1cmd.$$ <<EOF
${TL1CMD}
EOF
#
tl1exec -i "${activespip}" "${@}" /tmp/tl1cmd.$$
#
rm -f /tmp/tl1cmd.$$ 1>/dev/null 2>&1
#
exit 0
