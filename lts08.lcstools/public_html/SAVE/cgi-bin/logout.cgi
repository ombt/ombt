#!/opt/exp/bin/perl
user CGI qw(/:standard);
print header(-status=>'401 Unauthorized');
