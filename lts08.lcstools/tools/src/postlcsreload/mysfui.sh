#!/bin/expect
#
log_user 1;
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set iom [lindex $argv $arg];
	puts "\nRunning SFUI for IOM $iom";
	#
	spawn -noecho "/Telica/swCPU/CurrRel/system/sfui";
	#
	expect {
	-re {sfui> } {
		send "port ${iom} set\r";
	}
	timeout {
		puts "\nERROR: timeout during sfui of IOM $iom. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	eof {
		puts "\nERROR: EOF during sfui of IOM $iom. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	}
	#
	expect {
	-re {sfui> } {
		send "mlw ${iom} 0x50000030 0x2\r";
	}
	timeout {
		puts "\nERROR: timeout during port. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	eof {
		puts "\nERROR: eof during port. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	}
	#
	expect {
	-re {sfui> } {
		send "quit\r";
	}
	timeout {
		puts "\nERROR: timeout during mlw. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	eof {
		puts "\nERROR: eof during mlw. Goto next IOM.";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait -nowait; } ignore;
		continue;
	}
	}
	#
	sleep 2;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
}
#
exit 0;
