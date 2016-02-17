#!/opt/exp/bin/expect
#
foreach name [array name env] {
	puts "env($name)=$env($name)";
}
#
exit 0

