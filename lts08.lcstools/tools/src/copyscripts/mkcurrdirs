#
# make directories for current scripts.
#
cd ${LCSTOOLSDATA}
#
uprintf -q -f"%s %s\n" labid branch in chassis |
while read labid branch
do
	CURRDIR="${LCSBASESCRIPTS}/${labid}/${branch}/current"
	if [[ ! -d "${CURRDIR}" ]]
	then
		echo "Creating ${CURRDIR} ..."
		mkdir -p ${CURRDIR}
	else
		echo "${CURRDIR} exists. Skipping it ..."
	fi
done
#
exit 0
