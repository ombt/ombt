#!/usr/bin/ksh
#
if [[ "${1}" == "-?" ]]
then
	echo
	echo "usage: ${0} [-?]"
	exit 0
fi
#
if [[ "$(hostname)" != lcstools ]]
then
	echo
	echo "ERROR: this tool is only for lcstools.ih.lucent.com."
	exit 2
fi
if [[ "$(logname)" != lcstools ]]
then
	echo
	echo "ERROR: this tool is only for user lcstools."
	exit 2
fi
#
echo
echo "Existing crontab:"
crontab -l
#
oldcrontab=/tmp/oldcrontab.$$
newcrontab=/tmp/newcrontab.$$
#
crontab -l >${oldcrontab}
crontab -l | grep -v syncfs >${newcrontab}
#
echo "00 1 * * * /home/lcstools/tools/bin/weblcssyncfs /lcsl100/basescripts /home2/tester/bin/lcstools/lcsl100/basescripts 1>/lcsl100/tmp/nightlysyncfs.out 2>&1" >>${newcrontab}
#
crontab < ${newcrontab}
#
echo
echo "New crontab:"
crontab -l
#
exit 0
