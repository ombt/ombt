# telnet to a machine and exec cmds.
package provide lcstelnet 1.0
#
#########################################################################
#
proc telnet_to { ip spawn_id_name } {
	global username;
	global userpasswd;
	global timeout;
	#
	upvar $spawn_id_name spawn_id;
	#
	set loginseen 0;
	set savetimeout $timeout;
	set timeout 15;
	#
	spawn -noecho "/usr/bin/telnet" "-l $username" $ip;
	expect {
	-re ".*login:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*user name:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*\[Pp]assword:" {
		set loginseen 0;
		send "$userpasswd\r";
	}
	timeout {
		puts "\ntimeout during login ...";
		exit 2;
	}
	};
	#
	if {$loginseen} {
		expect {
		-re ".*\[Pp]assword:" {
			send "$userpasswd\r";
		}
		timeout {
			puts "\ntimeout during passwd ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*> " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		puts "\ntimeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok.
	}
	timeout {
		puts "\ntimeout during cd ...";
		exit 2;
	}
	};
	#
	set timeout $savetimeout;
}
#
proc interactive_telnet_to { ip spawn_id_name { telnetport 23 } } {
	global username;
	global userpasswd;
	global timeout;
	#
	upvar $spawn_id_name spawn_id;
	#
	set loginseen 0;
	set savetimeout $timeout;
	set timeout 15;
	#
	spawn -noecho "/usr/bin/telnet" "-l $username" $ip $telnetport;
	expect {
	-re ".*login:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*user name:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*\[Pp]assword:" {
		set loginseen 0;
		send "$userpasswd\r";
	}
	timeout {
		puts "\ntimeout during login ...";
		exit 2;
	}
	};
	#
	if {$loginseen} {
		expect {
		-re ".*\[Pp]assword:" {
			send "$userpasswd\r";
		}
		timeout {
			puts "\ntimeout during passwd ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*# " {
		# ok
	}
	timeout {
		puts "\ntimeout waiting for prompt ...";
		exit 2;
	}
	};
	#
	set timeout $savetimeout;
}
#
proc remote_exec { id cmd { localtimeout 30 } } {
	#
	global timeout;
	set savetimeout $timeout;
	set timeout $localtimeout;
	#
	send -i $id "$cmd\r"
	#
	expect {
	-i $id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		# ok
		puts "\ntimeout during $cmd ...";
		return 2;
	}
	};
	#
	set timeout $savetimeout;
	return 0;
}
#
proc close_telnet { id } {
	catch { close $id; } ignore;
}
