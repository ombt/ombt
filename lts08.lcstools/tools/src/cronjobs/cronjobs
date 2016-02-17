#!/usr/bin/ksh
# 
. setup_lcs
#
# 10 04 * * 1-5 /home/lcstools/tools/bin/savemgc5.4 -G -R 2>&1 | mailx -s "savemgc5.4 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 00 04 * * 1-5 /home/lcstools/tools/bin/savedev6.2.1 -G -R 2>&1 | mailx -s "savedev6.2.1 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 20 04 * * 1-5 /home/lcstools/tools/bin/save5.4 -G -R 2>&1 | mailx -s "save5.4 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 40 03 * * 1-5 /home/lcstools/tools/bin/save6.2.0 -G -R 2>&1 | mailx -s "save6.2.0 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 30 01 * * 1-5 /home/lcstools/tools/bin/save6.2.1.1 -t oper -G -R 2>&1 | mailx -s "save6.2.1.1 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 0 07 * * 1-5 /home/lcstools/tools/bin/save6.2.1 -t oper -G -R 2>&1 | mailx -s "BRANCH-6-2-1 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 30 04 * * 1-5 /home/lcstools/tools/bin/save6.3.0 -t oper -G -R 2>&1 | mailx -s "save6.3.0 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
# 30 05 * * 1-5 /home/lcstools/tools/bin/savemgmain -t oper -G -R 2>&1 | mailx -s "savemgmain output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
#
crontab <<EOF
00 03 * * * /home/lcstools/tools/bin/makedata 2>&1 | mailx -s "make data output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
00 05 * * 1-5 /home/lcstools/tools/bin/save6.3.2 -t oper -G -R 2>&1 | mailx -s "save6.3.2 output" lcstools@lts08.ih.lucent.com mrumore@lucent.com
EOF
