#!/opt/exp/bin/tclsh
#
package require Expect
#
global env;
#
puts "\nGDB Front-End:\n";
#
set debugfile [lindex $argv 0];
set corefile [lindex $argv 1];
set analyzeframes [lindex $argv 2];
set analyzethreads [lindex $argv 3];
#
if {![info exists analyzethreads] || [string length "${analyzethreads}"] == 0} {
	set analyzethreads "no";
}
if {![info exists analyzeframes] || [string length "${analyzeframes}"] == 0} {
	set analyzeframes "no";
}
#
puts "\nDebug File: ${debugfile}";
puts "Core File : ${corefile}";
puts "Frames    : ${analyzeframes} \n";
puts "Threads   : ${analyzethreads} \n";
#
log_user 1;
set timeout 60;
#
spawn -noecho gdb ${debugfile} ${corefile};
#
expect {
-re {.*[\r\n]*\(gdb\)[\t ]*} {
	puts "\n==========================================================\n";
	puts "\nSending backtrace ... \n";
	send "bt\r";
}
timeout {
	puts "\nERROR: time out while waiting for gdb to initialize.\n";
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
eof {
	puts "\nERROR: EOF while waiting for gdb to initialize.\n";
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
}
#
set maxstackframe 0;
#
expect {
-re {.*[\r\n][^#]*#([0-9][0-9]*)  *0x[0-9a-fA-F]} {
	set maxstackframe $expect_out(1,string);
	exp_continue;
}
-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
	puts "\nSending <CR> to continue ... \n";
	send "\r";
	exp_continue;
}
-re {.*[\r\n]*\(gdb\)[\t ]*} {
	puts "\nMax Stack Frame is -- ${maxstackframe}\n";
	puts "\n==========================================================\n";
	puts "\nSending info threads ... \n";
	send "info threads\r";
}
timeout {
	puts "\nERROR: time out while waiting for bt to complete.\n";
	catch { send "quit\r"; sleep 0.5; } ignore;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
eof {
	puts "\nERROR: EOF while waiting for bt to complete.\n";
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
}
#
set gdbthreads(maxgdbid) 0;
#
expect {
-re {.*[\r\n][^0-9]*([0-9][0-9]*)[	 ]*process[	 ]*([0-9][0-9]*)[	 ]*thread[	 ]*([0-9][0-9]*)[	 ]*(0x[0-9a-fA-F][0-9a-fA-F]*)} {
	set gdbid $expect_out(1,string);
	set process $expect_out(2,string);
	set thread $expect_out(3,string);
	set address $expect_out(4,string);
	#
	# puts "\nGBDID - ${gdbid}";
	# puts "PROCESS - ${process}";
	# puts "THREAD - ${thread}";
	# puts "ADDRESS - ${address}";
	#
	set gdbthreads(${gdbid},process) ${process};
	set gdbthreads(${gdbid},thread) ${thread};
	set gdbthreads(${gdbid},address) ${address};
	#
	if {$gdbthreads(maxgdbid) <= 0} {
		set gdbthreads(maxgdbid) ${gdbid};
	}
	#
	exp_continue;
}
-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
	puts "\nSending <CR> to continue ... \n";
	send "\r";
	exp_continue;
}
-re {.*[\r\n]*\(gdb\)[\t ]*} {
	# ok
}
timeout {
	puts "\nERROR: time out while waiting for info thread to complete.\n";
	catch { send "quit\r"; sleep 0.5; } ignore;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
eof {
	puts "\nERROR: EOF while waiting for info thread to complete.\n";
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	exit 2;
}
}
#
puts "\nMax Thread GDB ID -- $gdbthreads(maxgdbid)\n";
#
set cmd 0;
set cmds([incr cmd]) "dumplog";
set cmds([incr cmd]) "info registers";
set cmds([incr cmd]) "thread apply all backtrace";
#
for {set icmd 1} {[info exists cmds(${icmd})]} {incr icmd} {
	set cmd $cmds(${icmd});
	#
	puts "\n==========================================================\n";
	puts "\nSending ${cmd} ... \n";
	send "${cmd}\r";
	#
	expect {
	-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
		puts "\nSending <CR> to continue ... \n";
		send "\r";
		exp_continue;
	}
	-re {.*[\r\n]*\(gdb\)[\t ]*} {
		# ok
	}
	timeout {
		puts "\nERROR: time out while waiting for '${cmd}' to complete.\n";
		catch { send "quit\r"; sleep 0.5; } ignore;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		exit 2;
	}
	eof {
		puts "\nERROR: EOF while waiting for '${cmd}' to complete.\n";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		exit 2;
	}
	}
}
#
if {"${analyzeframes}" == "yes"} {
	set cmd 0;
	set stackcmds([incr cmd]) "frame %d";
	set stackcmds([incr cmd]) "info frame";
	set stackcmds([incr cmd]) "info args";
	set stackcmds([incr cmd]) "info locals";
	#
	for {set stackframe 0} {${stackframe}<=${maxstackframe}} {incr stackframe} {
		puts "\n==========================================================\n";
		puts "\nFrame ${stackframe} ... \n";
		for {set stackcmd 1} {[info exists stackcmds(${stackcmd})]} {incr stackcmd} {
			set cmd $stackcmds(${stackcmd});
			#
			set fcmd [format ${cmd} ${stackframe}];
			puts "\n----------------------------------------------------------\n";
			puts "\nSending ${fcmd} ... \n";
			send "${fcmd}\r";
			#
			expect {
			-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
				puts "\nSending <CR> to continue ... \n";
				send "\r";
				exp_continue;
			}
			-re {.*[\r\n]*\(gdb\)[\t ]*} {
				# ok
			}
			timeout {
				puts "\nERROR: time out while waiting for '${cmd}' to complete.\n";
				catch { send "quit\r"; sleep 0.5; } ignore;
				catch { close; } ignore;
				catch { exec kill -9 [exp_pid]; } ignore;
				catch { wait -nowait; } ignore;
				exit 2;
			}
			eof {
				puts "\nERROR: EOF while waiting for '${cmd}' to complete.\n";
				catch { close; } ignore;
				catch { exec kill -9 [exp_pid]; } ignore;
				catch { wait -nowait; } ignore;
				exit 2;
			}
			}
		}
	}
}
#
# analyze threads 
#
if {"${analyzethreads}" != "yes"} {
	puts "\n==========================================================\n";
	puts "\nSending quit ... \n";
	send "quit\r";
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	#
	puts "\nDone.\n";
	exit 0;
} elseif {$gdbthreads(maxgdbid) <= 0} {
	puts "\n==========================================================\n";
	puts "\nNo threads found to analyze ... \n";
	puts "\n==========================================================\n";
	puts "\nSending quit ... \n";
	send "quit\r";
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	#
	puts "\nDone.\n";
	exit 0;
}
#
set cmd 0;
set threadcmds([incr cmd]) "thread %d";
set threadcmds([incr cmd]) "thread apply %d bt";
#
set cmd 0;
set threadstackcmds([incr cmd]) "thread apply %d frame %d";
set threadstackcmds([incr cmd]) "thread apply %d info frame";
set threadstackcmds([incr cmd]) "thread apply %d info args";
set threadstackcmds([incr cmd]) "thread apply %d info locals";
#
set maxgdbid $gdbthreads(maxgdbid);
#
for {set gdbid 1} {${gdbid}<=${maxgdbid}} {incr gdbid} {
	puts "\n==========================================================\n";
	puts "\nThread ${gdbid} ... \n";
	for {set threadcmd 1} {[info exists threadcmds(${threadcmd})]} {incr threadcmd} {
		set cmd $threadcmds(${threadcmd});
		#
		set fcmd [format ${cmd} ${gdbid}];
		puts "\n----------------------------------------------------------\n";
		puts "\nSending ${fcmd} ... \n";
		send "${fcmd}\r";
		#
		expect {
		-re {.*[\r\n][^#]*#([0-9][0-9]*)  *0x[0-9a-fA-F]} {
			set maxstackframe $expect_out(1,string);
			exp_continue;
		}
		-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
			puts "\nSending <CR> to continue ... \n";
			send "\r";
			exp_continue;
		}
		-re {.*[\r\n]*\(gdb\)[\t ]*} {
			# ok
		}
		timeout {
			puts "\nERROR: time out while waiting for '${cmd}' to complete.\n";
			catch { send "quit\r"; sleep 0.5; } ignore;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait -nowait; } ignore;
			exit 2;
		}
		eof {
			puts "\nERROR: EOF while waiting for '${cmd}' to complete.\n";
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait -nowait; } ignore;
			exit 2;
		}
		}
	}
	# thread stack commands for this thread
	for {set threadstackframe 0} {${threadstackframe}<=${maxstackframe}} {incr threadstackframe} {
		puts "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
		puts "\nThread ${gdbid} Frame ${threadstackframe} ... \n";
		for {set threadstackcmd 1} {[info exists threadstackcmds(${threadstackcmd})]} {incr threadstackcmd} {
			set cmd $threadstackcmds(${threadstackcmd});
			#
			set fcmd [format ${cmd} ${gdbid} ${threadstackframe}];
			puts "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
			puts "\nSending ${fcmd} ... \n";
			send "${fcmd}\r";
			#
			expect {
			-re {.*[\r\n]*---.*Type.*to *continue.*or.*q.*to.*quit.*---} {
				puts "\nSending <CR> to continue ... \n";
				send "\r";
				exp_continue;
			}
			-re {.*[\r\n]*\(gdb\)[\t ]*} {
				# ok
			}
			timeout {
				puts "\nERROR: time out while waiting for '${cmd}' to complete.\n";
				catch { send "quit\r"; sleep 0.5; } ignore;
				catch { close; } ignore;
				catch { exec kill -9 [exp_pid]; } ignore;
				catch { wait -nowait; } ignore;
				exit 2;
			}
			eof {
				puts "\nERROR: EOF while waiting for '${cmd}' to complete.\n";
				catch { close; } ignore;
				catch { exec kill -9 [exp_pid]; } ignore;
				catch { wait -nowait; } ignore;
				exit 2;
			}
			}
		}
	}
}
#
puts "\n==========================================================\n";
puts "\nSending quit ... \n";
send "quit\r";
#
catch { close; } ignore;
catch { exec kill -9 [exp_pid]; } ignore;
catch { wait -nowait; } ignore;
#
puts "\nDone.\n";
exit 0;
