#!/opt/exp/bin/expect
#
puts "PID is [pid]";
#
trap SIG_IGN SIGHUP;
#
sleep 30;
#
trap SIG_DFL SIGHUP;
#
exit 2;
