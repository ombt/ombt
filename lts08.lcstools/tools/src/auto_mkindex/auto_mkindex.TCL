#!/opt/exp/bin/tcl
#
# create TCL lib index file.
#
##############################################################################
#
if {$argc != 2} {
	puts "\nusage: [file tail $argv0] tcl_lib_path tcl_filename_pattern";
	puts "\nexample: [file tail $argv0] /usr/lib/tcl '*.tcl'\n";
	exit 2;
}
#
set tcl_lib_path [lindex $argv 0 ];
set tcl_pattern [lindex $argv 1 ];
#
# puts "";
# puts "Creating TCL Lib Index File:"
# puts "TCL Lib Path        : $tcl_lib_path";
# puts "TCL Filename Pattern: $tcl_pattern";
# puts "";
#
cd $tcl_lib_path;
#
# example: auto_mkindex [pwd] "*.tcl";
#
auto_mkindex [pwd] "$tcl_pattern";
#
#
exit 0;
