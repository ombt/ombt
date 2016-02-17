#
export VPATH=".:..:../..:../hdr:../../hdr"
#
for oty in ${*}
do
	otypp -V ${oty} | otyscan -V | grep -c syntax
done
#
exit 0
