#!/usr/bin/ksh
#
echo "Exising crontab:"
crontab -l
#
oldcrontab=/tmp/oldcrontab.$$
newcrontab=/tmp/newcrontab.$$
#
crontab -l >${oldcrontab}
crontab -l | grep -v getloads >${newcrontab}
#
echo "00 1 * * * /autosaveload/bin/getloads 1>/autosaveload/logs/nightly.out 2>&1" >>${newcrontab}
#
crontab < ${newcrontab}
#
echo "New crontab:"
crontab -l
#
exit 0
