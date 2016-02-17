#!/opt/exp/bin/perl
use Time::Local;
$tm = localtime;
print "tm is " . ($tm) . "\n";
#
#printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", $tm->sec,$tm->min,$tm->hour,$tm->mday,$tm->month,$tm->year,$tm->wday,$tm->yday,$tm->idat);
#
exit 0;

