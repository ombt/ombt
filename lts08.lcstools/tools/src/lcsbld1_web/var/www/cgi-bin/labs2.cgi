#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$labid = $FORM_DATA{labid};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> LABS LOG FILES</h3>\n";
print "<h4>($labid)</h4>\n";
#
logmsg("Listing Lab Logs - (${labid})");
#
print "<ul>\n";
#
print "<li>\n";
print "<a href=\"http://lcsbld1.ih.lucent.com/labs/${labid}_spa_root/\">${labid} SP-A Root </a>\n";
#
print "<li>\n";
print "<a href=\"http://lcsbld1.ih.lucent.com/labs/${labid}_spa_home1/\">${labid} SP-A Home1 </a>\n";
#
print "<li>\n";
print "<a href=\"http://lcsbld1.ih.lucent.com/labs/${labid}_spb_root/\">${labid} SP-B Root </a>\n";
#
print "<li>\n";
print "<a href=\"http://lcsbld1.ih.lucent.com/labs/${labid}_spb_home1/\">${labid} SP-B Home1 </a>\n";
#
print "</ul>\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


