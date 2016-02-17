# functions related to the local machine
package provide local 1.0
#
package require checkretval
#
# clean up routine ...
#
proc local_close { spawn_id } {
	catch { send "exit\r"; sleep 0.5; } ignore;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	return;
}
#
# interface to local machine OS functions
#
proc local_df { data_name { filesystems "" }  { timeout 30 } { maxtries 5 } } {
	upvar $data_name data;
	#
	for {set i 0} {$i<$maxtries} {incr i} {
		if {${filesystems} != ""} {
			spawn -noecho "/usr/sbin/df" "-k" ${filesystems};
		} else {
			spawn -noecho "/usr/sbin/df" "-k";
		}
		#
		expect {
		-re {Filesystem.*kbytes.*used.*avail.*capacity.*Mounted.*on} {
			# ok
			break;
		}
		-re {df:.* not *a * block *device} {
			local_close $spawn_id;
			puts "\nUnknown file system: ${filesystems}";
			return "-1 - local_df: unknown file system: ${filesystems}";
		}
		timeout {
			local_close $spawn_id;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
		}
		eof {
			local_close $spawn_id;
			puts "\nEOF. Sleep and retry.";
			sleep 5;
		}
		}
	}
	if {$i>=$maxtries} {
		puts "\nUnable to run /usr/sbin/df.";
		return "-1 - local_df: unable to run /usr//sbin/df.";
	}
	#
	expect {
	-re {[\n\r]([^\r\n ][^\r\n ]*)[\r\n ][\r\n ]*([0-9][0-9]*)  *([0-9][0-9]*)  *([0-9][0-9]*)  *([0-9][0-9]*%)  *([^\r\n ][^\r\n ]*)} {
		set remotefs $expect_out(1,string);
		set fstotal $expect_out(2,string);
		set fsused $expect_out(3,string);
		set fsavail $expect_out(4,string);
		set fspercent $expect_out(5,string);
		set localfs $expect_out(6,string);
		#
		puts "\nLOCAL FS      : ${localfs}";
		puts "TOTAL (kbytes): ${fstotal}";
		puts "USED (kbytes) : ${fsused}";
		puts "AVAIL (kbytes): ${fsavail}";
		puts "PERCENT USED  : ${fspercent}";
		puts "REMOTE FS     : ${remotefs}";
		#
		set data(localfs) ${localfs};
		#
		set data(${localfs},exist) 1;
		set data(${localfs},remotefs) ${remotefs};
		set data(${localfs},fstotal) ${fstotal};
		set data(${localfs},fsused) ${fsused};
		set data(${localfs},fsavail) ${fsavail};
		set data(${localfs},fspercent) ${fspercent};
		#
		exp_continue;
	}
	timeout {
		# oops.
		local_close $spawn_id;
		return "-1 - local_df: time out during /usersbin/df.";
	}
	eof {
		# all done
		local_close $spawn_id;
	}
	}
	#
	return "0 - success";
}
#
proc local_du { data_name { filesystems "" }  { timeout 30 } { maxtries 5 } } {
	upvar $data_name data;
	#
	for {set i 0} {$i<$maxtries} {incr i} {
		if {${filesystems} != ""} {
			spawn -noecho "/usr/bin/du" "-sk" ${filesystems};
		} else {
			spawn -noecho "/usr/bin/du" "-sk";
		}
		#
		#exp_internal 1;
		expect {
		-re {[\n\r]*([0-9][0-9]*)[\r\n 	][\r\n 	]*([^\r\n 	][^\r\n 	]*)} {
			# we have data
			set kbytes $expect_out(1,string);
			set path $expect_out(2,string);
			#
			puts "\nPATH: ${path}";
			puts "SIZE (kbytes): ${kbytes}";
			#
			set data(${path}) ${kbytes};
			exp_continue;
		}
		timeout {
			# try again
			local_close $spawn_id;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
		}
		eof {
			# all done
			local_close $spawn_id;
			break;
		}
		}
		#exp_internal 0;
	}
	if {$i>=$maxtries} {
		puts "\nUnable to run /usr/bin/du.";
		return "-1 - local_du: unable to run /usr/bin/du.";
	}
	#
	return "0 - success";
}
