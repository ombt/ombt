#
echo "Disabling Disk Cleanup ..."
#
for xml in /Telica/swCPU/*[0-9]*/schema/debug.xml /home1/Telica/swCPU/*[0-9]*/schema/debug.xml 
do
	echo "Checking XML file ${xml}"
	if [[ -r "${xml}" ]]
	then
		echo "File is readable. It will be modified ...";
		cat ${xml} |
		sed "/COL.*DisableDiskCleanup/ {
			s/> *[0-9][0-9]* *</>1</;
		}" > ${xml}.new
		mv ${xml}.new ${xml}
	else
		echo "File is NOT readable.";
	fi
done
#
exit 0
