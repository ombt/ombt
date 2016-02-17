#!/opt/exp/bin/perl
#
use Socket;
#
print "Content-type: text/html\n\n";
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
print "<h3> Environment Variables</h3>\n";
#
print "<pre>";
#
foreach my ${key} (sort(keys(%ENV))) {
	print "${key} = $ENV{${key}}\n";
}
#
my $hostname = gethostbyaddr(inet_aton($ENV{REMOTE_ADDR}), AF_INET);
print "REMOTE_ADDR: ${hostname}\n";
#
print "</pre>\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0


