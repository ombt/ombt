#
if [[ $# == 0 ]]
then
	echo "usage: iomsfui iomno [iomno2 ...]"
	exit 0
elif [[ "${1}" == "-?" ]]
then
	echo "usage: iomsfui iomno [iomno2 ...]"
	exit 0
fi
#
for iom in ${*}
do
	cd /Telica/swCPU/CurrRel/system
	./sfui <<EOF
port ${iom} set
mlw ${iom} 0x50000030 0x2
quit
EOF
done
#
exit 0
