#!/usr/bin/ksh
#
# make the auto.master file for mounting SP file systems on lcsbld1.
# this script will create an auto.spfs file that is referred to 
# from the main auto.master file on lcsbld1.
#
cat >/lcsl100/tmp/auto.master <<EOF
# Sample auto.master file
# This is an automounter map and it has the following format
# key [ -mount-options-separated-by-comma ] location
# For details of the format look at autofs(5).
#/misc  /etc/auto.misc --timeout=60
#/misc  /etc/auto.misc
#/net   /etc/auto.net
#
/labs	/etc/auto.spfs	--timeout=60
EOF
#
getspips -a | 
egrep '^(ihlcs|ihlnc|ihlng|ihlsg|bh)' |
sort -u |
nawk '
NF == 3 {
	# get data
	labid = $1;
	spaip = $2;
	spbip = $3;
	# print out auto.master records
	printf("%s_spa_root\t-ro,soft,intr,noexec,rsize=8192,wsize=8192 %s:/\n", labid, spaip);
	printf("%s_spa_home1\t-ro,soft,intr,noexec,rsize=8192,wsize=8192 %s:/home1\n", labid, spaip);
	printf("%s_spb_root\t-ro,soft,intr,noexec,rsize=8192,wsize=8192 %s:/\n", labid, spbip);
	printf("%s_spb_home1\t-ro,soft,intr,noexec,rsize=8192,wsize=8192 %s:/home1\n", labid, spbip);
} ' | tee /lcsl100/tmp/auto.spfs
#
exit 0
