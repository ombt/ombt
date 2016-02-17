# double-ended queue -- acts like a stack and a queue.
package provide dequeue 1.0
#
proc isdeqempty { deq_name } {
	upvar $deq_name deq;
	#
	if {$deq(left) >= $deq(right)} {
		return 1;
	} else {
		return 0;
	}
}
#
proc depth { deq_name } {
	upvar $deq_name deq;
	return [expr $deq(right) - $deq(left)];
}
#
proc initdeq { deq_name } {
	upvar $deq_name deq;
	set deq(left) 0;
	set deq(right) 0;
	return "0 - success";
}
#
proc enqueue { deq_name item } {
	upvar $deq_name deq;
	#
	set deq($deq(right)) $item;
	incr deq(right);
	#
	return "0 - success";
}
proc dequeue { deq_name item_name } {
	upvar $deq_name deq;
	upvar $item_name item;
	#
	if {[isdeqempty deq]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $deq($deq(left));
	incr deq(left);
	#
	return "0 - success";
}
proc push { deq_name item } {
	upvar $deq_name deq;
	#
	set deq($deq(right)) $item;
	incr deq(right);
	#
	return "0 - success";
}
proc pop { deq_name item_name } {
	upvar $deq_name deq;
	upvar $item_name item;
	#
	if {[isdeqempty deq]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	incr deq(right) -1;
	set item $deq($deq(right));
	#
	return "0 - success";
}
proc peeklhs { deq_name item_name } {
	upvar $deq_name deq;
	upvar $item_name item;
	#
	if {[isdeqempty deq]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $deq($deq(left));
	#
	return "0 - success";
}
proc peekrhs { deq_name item_name } {
	upvar $deq_name deq;
	upvar $item_name item;
	#
	if {[isdeqempty deq]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $deq([expr $deq(right) - 1]);
	#
	return "0 - success";
}
#
