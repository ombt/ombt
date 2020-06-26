# ftp files to and from.
package provide lcsftp 1.0
#
proc send_ftp_cmd { cmd { tmout 0 } } {
	global timeout;
	global spawn_id;
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	send "$cmd\r";
	expect {
	"ftp> " {
		# nothing to do
	}
	timeout {
		puts "ftp error: time out waiting for cmd \"$cmd\"";
		exit 2;
	}
	}
}
#
proc ftp_login { machine } {
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	global env;
	#
	spawn -noecho $env(LCSTOOLSBIN)/suid "/usr/bin/ftp" $machine;
	expect {
	"Name" { send "$username\r"; }
	timeout { puts "\ntimeout during ftp login ..."; exit 2; }
	};
	#
	expect {
	"Password:" { send "$userpasswd\r"; }
	timeout { puts "\ntimeout during ftp passwd ..."; exit 2; }
	};
	#
	expect {
	"ftp> " { # nothing to do }
	timeout { puts "ftp error: time out waiting for prompt."; exit 2; }
	}
	#
	send_ftp_cmd "!id" 20;
}
#
proc ftp_create_local_file { localfile } {
	if {[file exists $localfile]} {                            
		file delete -force -- $tofile;          
	}                                               
	set infd [open $localfile "w" 0777 ];
	close $infd;
}
#
proc ftp_chmod { dname dperms } {
	if {[file owned $dname]} {
		system /usr/bin/chmod $dperms $dname;
	}
}
#
proc ftp_get_binary_file { machine remotefile localfile { fileperms 0 } } {
	#
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	#
	set savetimeout $timeout;
	set timeout 60;
	#
	ftp_chmod [file dirname $localfile] 0777;
	#
	ftp_login $machine;
	#
	send_ftp_cmd "binary" 60;
	send_ftp_cmd "get $remotefile $localfile" -1;
	if {$fileperms != 0} {
		send_ftp_cmd "!chmod $fileperms $localfile" 60;
	}
	send_ftp_cmd "bye" 60;
	#
	set timeout $savetimeout;
	catch { close; wait; } ignore;
	#
	ftp_chmod [file dirname $localfile] 0755;
}
#
proc ftp_get_ascii_file { machine remotefile localfile { fileperms 0 } } {
	#
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	#
	set savetimeout $timeout;
	set timeout 60;
	#
	ftp_chmod [file dirname $localfile] 0777;
	ftp_login $machine;
	#
	send_ftp_cmd "ascii" 60;
	send_ftp_cmd "get $remotefile $localfile" -1;
	if {$fileperms != 0} {
		send_ftp_cmd "!chmod $fileperms $localfile" 60;
	}
	send_ftp_cmd "bye" 60;
	#
	set timeout $savetimeout;
	catch { close; wait; } ignore;
	#
	ftp_chmod [file dirname $localfile] 0755;
}
#
proc ftp_put_binary_file { machine localfile remotefile } {
	#
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	#
	set savetimeout $timeout;
	set timeout 60;
	#
	ftp_login $machine;
	#
	send_ftp_cmd "binary" 60;
	send_ftp_cmd "put $localfile $remotefile" -1;
	send_ftp_cmd "bye" 60;
	#
	set timeout $savetimeout;
	catch { close; wait; } ignore;
}
#
proc ftp_put_ascii_file { machine localfile remotefile } {
	#
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	#
	set savetimeout $timeout;
	set timeout 60;
	#
	ftp_login $machine;
	#
	send_ftp_cmd "ascii" 60;
	send_ftp_cmd "put $localfile $remotefile" -1;
	send_ftp_cmd "bye" 60;
	#
	set timeout $savetimeout;
	catch { close; wait; } ignore;
}
#
proc interactive_ftp_login { machine spawn_id_name } {
	global username;
	global userpasswd;
	global timeout;
	global spawn_id;
	global env;
	#
	upvar $spawn_id_name spawn_id;
	#
	spawn -noecho "/usr/bin/ftp" $machine;
	expect {
	"Name" { send "$username\r"; }
	timeout { puts "\ntimeout during ftp login ..."; exit 2; }
	};
	#
	expect {
	"Password:" { send "$userpasswd\r"; }
	timeout { puts "\ntimeout during ftp passwd ..."; exit 2; }
	};
	#
	expect {
	"ftp> " { # nothing to do }
	timeout { puts "ftp error: time out waiting for prompt."; exit 2; }
	}
	#
	send "!id\r";
	expect {
	"ftp> " {
		# nothing to do
	}
	timeout {
		puts "ftp error: time out waiting for cmd \"$cmd\"";
		exit 2;
	}
	}
}
#
proc close_ftp { id } {
	catch { close $id; } ignore;
}

#
