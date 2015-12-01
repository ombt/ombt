set -x
gcc -c md5.c 2>&1|tee elfcompare.out
gcc -D_ELOG -I. -c elfcompare.c 2>&1 |tee elfcompare.out
gcc -W -Wall -g -O2 -o elfcompare elfcompare.o  ./md5.o -lintl  2>&1|tee -a elfcompare.out
#./../intl/libintl.a 

gcc -I.  -c elfcompare.c 2>&1 |tee elfcompare.out
gcc -W -Wall -g -O2 -o elfcompare2 elfcompare.o  ./md5.o  -lintl 2>&1|tee -a elfcompare.out
#gcc -W -Wall -g -O2 -o elfcompare elfcompare.o  ./../intl/libintl.a ../libiberty/libiberty.a 2>&1|tee -a elfcompare.out
set +x
