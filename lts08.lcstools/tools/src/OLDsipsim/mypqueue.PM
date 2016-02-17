# priority queue implementation
package mypqueue;
#
sub makepqueue {
	my $plist = {
		first => undef,
		last => undef,
		count => 0
	};
	return($plist);
}
#
sub isEmpty {
	my ($plist) = @_;
	if (defined($plist)) {
		return(!defined($plist->{first}) && !defined($plist->{last}));
	} else {
		return(0);
	}
}
#
sub enqueue {
	my ($plist, $key, $value) = @_;
	my $pos = undef;
	#
	defined($plist) or die "enqueue - undefined list.";
	defined($key) or die "enqueue - undefined key.";
	defined($value) or die "enqueue - undefined value.";
	#
	for ($pos = $plist->{first}; 
	     defined($pos) && $pos->{key} < $key; 
	     $pos = $pos->{next}) {
		# do nothing
	}
	#
	if (!defined($pos)) {
		my $pitem = {
			previous => undef,
			next => undef,
			key => $key,
			value => $value,
		};
		if (isEmpty($plist)) {
			$plist->{first} = $pitem;
			$plist->{last} = $pitem;
		} else {
			$pitem->{previous} = $plist->{last};
			$pitem->{previous}->{next} = $pitem;
			$pitem->{next} = undef;
			$plist->{last} = $pitem;
		}
		$plist->{count} += 1;
	} else {
		my $pitem = {
			previous => undef,
			next => undef,
			key => $key,
			value => $value,
		};
		#
		$pitem->{previous} = $pos->{previous};
		$pitem->{next} = $pos;
		$pos->{previous} = $pitem;
		#
		if (defined($pitem->{previous})) {
			$pitem->{previous}->{next} = $pitem;
		} else {
			$plist->{first} = $pitem;
		}
		$plist->{count} += 1;
	}
	print "enqueued - (key,value)=($key,$value)\n";
	return 1;
}
#
sub dequeue {
	my ($plist, $pkey, $pvalue) = @_;
	my $pos = undef;
	#
	defined($plist) or die "dequeue - undefined list.";
	defined($pkey) or die "dequeue - undefined pkey.";
	defined($pvalue) or die "dequeue - undefined pvalue.";
	#
	if (isEmpty($plist)) {
		return(0);
	}
	#
	if ($plist->{first} == $plist->{last}) {
		$$pkey = $plist->{first}->{key};
		$$pvalue = $plist->{first}->{value};
		$plist->{first} = undef;
		$plist->{last} = undef;
	} else {
		$$pkey = $plist->{first}->{key};
		$$pvalue = $plist->{first}->{value};
		my $psave = $plist->{first};
		$plist->{first} = $plist->{first}->{next};
		$plist->{first}->{previous} = undef;
		$psave = undef;
	}
	$plist->{count} -= 1;
	return(1);
}
#
sub front {
	my ($plist, $pkey, $pvalue) = @_;
	my $pos = undef;
	#
	defined($plist) or die "front - undefined list.";
	defined($pkey) or die "front - undefined pkey.";
	defined($pvalue) or die "front - undefined pvalue.";
	#
	if (isEmpty($plist)) {
		return(0);
	} else {
		$$pkey = $plist->{first}->{key};
		$$pvalue = $plist->{first}->{value};
		return(1);
	}
}
# exit with success
1;
