# db unity cmds
package provide db 1.0
#
proc dbselect { obufname relation {whereclause ""} {fields ""} } {
	global env;
	upvar $obufname obuf;
	#
	if {[info exists $obufname]} {
		unset $obufname;
	}
	#
	if {![info exists env(LCSHUGDATA)]} {
		return "-1 - dbselect: LCSHUGDATA not defined."
	}
	#
	set cwd [pwd];
	cd $env(LCSHUGDATA);
	#
	set timeout 30;
	if {$whereclause == ""} {
		eval spawn -noecho "/opt/exp/lib/unity/bin/uselect" -q $fields from $relation;
	} else {
		eval spawn -noecho "/opt/exp/lib/unity/bin/uselect" -q $fields from $relation where $whereclause;
	}
	expect {
	-re {(.*uselect.*)\r\n} {
		puts "\ndbselect: $expect_out(1,string)";
		if {[info exists $obufname]} {
			unset $obufname;
		}
		catch { close; wait; } ignore;
		cd $cwd;
		return "-1 - dbselect: uselect error.";
	}
	-re {([^\r\n]*)[\r\n][\r\n]*} {
		lappend obuf $expect_out(1,string);
		exp_continue;
	}
	eof {
		# done
		catch { close; wait; } ignore;
		cd $cwd;
		return "0 - success";
	}
	timeout {
		puts "\ndbselect: timeout during 'uselect' ...";
		if {[info exists $obufname]} {
			unset $obufname;
		}
		catch { close; wait; } ignore;
		cd $cwd;
		return "-1 - dbselect: timeout during 'uselect'.";
	}
	};
	# 
	catch { close; wait; } ignore;
	#
	cd $cwd;
	return "0 - success";
}
#
proc dbinsert { relation nmvalname } {
	global env;
	upvar $nmvalname namevalue;
	#
	if {![info exists env(LCSHUGDATA)]} {
		puts "dbinsert: LCSHUGDATA not defined."
		return "-1 - dbinsert: LCSHUGDATA not defined."
	}
	#
	set cwd [pwd];
	cd $env(LCSHUGDATA);
	#
	set tuple "";
	set Dfd [open D${relation} "r"];
	while {[gets $Dfd line] != -1} {
		set fldinfo [split $line " \t"];
		set fldname [lindex $fldinfo 0];
		set flddelimiter [lindex $fldinfo 1];
		#
		if {[info exists namevalue($fldname)]} {
			set fldvalue $namevalue($fldname);
		} else {
			set fldvalue "";
		}
		#
		switch -regexp -- $flddelimiter {
		{^t\\t$} {
			append tuple "$fldvalue" "\t";
		}
		{^t\\n$} {
			append tuple "$fldvalue" "\n";
		}
		{^t;$} {
			append tuple "$fldvalue" ";";
		}
		default {
			catch { close $Dfd; } ignore;
			puts "dbinsert: unsupported delimiter <$flddelimiter>.";
			cd $cwd;
			return "-1 - dbinsert: unsupported delimiter <$flddelimiter>.";
		}
		}
	}
	#
	catch { close $Dfd; } ignore;
	#
	append tmpfile "/tmp/dbinsert" [exp_pid];
	set tmpfd [open ${tmpfile} "w"];
	puts -nonewline $tmpfd "$tuple";
	catch { close $tmpfd; } ignore;
	#
	system "/opt/exp/lib/unity/bin/insert" -q in $relation from $tmpfile;
	#
	file delete -force -- $tmpfile;
	#
	cd $cwd;
	return "0 - success";
}
#
proc dbdelete { relation whereclause } {
	global env;
	#
	if {![info exists env(LCSHUGDATA)]} {
		puts "dbdelete: LCSHUGDATA not defined."
		return "-1 - dbdelete: LCSHUGDATA not defined."
	}
	#
	set cwd [pwd];
	cd $env(LCSHUGDATA);
	set timeout 30;
	eval spawn -noecho "/opt/exp/lib/unity/bin/udelete" -q from $relation where $whereclause;
	expect {
	-re {(.*udelete.*)\r\n} {
		puts "\ndbdelete: $expect_out(1,string)";
		catch { close; wait; } ignore;
		cd $cwd;
		return "-1 - dbdelete: udelete failed.";
	}
	eof {
		# done
		catch { close; wait; } ignore;
		cd $cwd;
		return "0 - success";
	}
	timeout {
		puts "\ndbdelete: timeout during 'udelete' ...";
		catch { close; wait; } ignore;
		cd $cwd;
		return "-1 - dbdelete: timeout during 'udelete'.";
	}
	};
	# 
	catch { close; wait; } ignore;
	cd $cwd;
	#
	return "0 - success";
}
#
