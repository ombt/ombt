# remove defautl gateway from /Telica/dbCurrent/Telica_IP
#
echo "Removing 'default_gw=' line from /Telica/dbCurrent/Telica_IP."
#
ipfile=/Telica/dbCurrent/Telica_IP
newipfile=/tmp/Telica_IP.new
#
if [[ ! -f "${ipfile}" ]]
then
	echo "ERROR: File ${ipfile} NOT found." >&2
	exit 2
fi
#
grep -v '^default_gw=' ${ipfile} >${newipfile}
#
mv ${newipfile} ${ipfile}
#
echo "Done."
exit 0

