# priority queue object implementation
package mypqueue;
#
sub new {
	my $class = shift;
	my $self = {};
	$self->{first} = undef;
	$self->{last} = undef;
	$self->{count} = 0;
	bless $self, $class;
	return($self);
}
#
sub isEmpty {
	my $self = shift;
	return(!defined($self->{first}) && !defined($self->{last}));
}
#
sub first {
	my $self = shift;
	$self->{first} = shift if @_;
	return($self->{first});
}
sub last {
	my $self = shift;
	$self->{last} = shift if @_;
	return($self->{last});
}
sub count {
	my $self = shift;
	$self->{count} = shift if @_;
	return($self->{count});
}
#
sub enqueue {
	#
	# for queueing new events, start at the end.
	#
	my ($self, $data) = @_;
	my $pos = undef;
	#
	defined($data) or die "enqueue - undefined data.";
	#
	for ($pos = $self->{last}; 
	     defined($pos) && ($pos->{data}->cmp($data) > 0);
	     $pos = $pos->{previous}) {
		# do nothing
	}
	#
	if (!defined($pos)) {
		my $pitem = {
			previous => undef,
			next => undef,
			data => $data,
		};
		if ($self->isEmpty()) {
			$self->{first} = $pitem;
			$self->{last} = $pitem;
		} else {
			$pitem->{next} = $self->{first};
			$pitem->{next}->{previous} = $pitem;
			$pitem->{previous} = undef;
			$self->{first} = $pitem;
		}
		$self->{count} += 1;
	} else {
		my $pitem = {
			previous => undef,
			next => undef,
			data => $data,
		};
		#
		$pitem->{next} = $pos->{next};
		$pitem->{previous} = $pos;
		$pos->{next} = $pitem;
		#
		if (defined($pitem->{next})) {
			$pitem->{next}->{previous} = $pitem;
		} else {
			$self->{last} = $pitem;
		}
		$self->{count} += 1;
	}
	#
	return 1;
}
#
sub dequeue {
	my ($self, $pdata) = @_;
	my $pos = undef;
	#
	defined($pdata) or die "dequeue - undefined data.";
	#
	if ($self->isEmpty()) {
		return(0);
	}
	#
	$$pdata = $self->{first}->{data};
	#
	if ($self->{first} == $self->{last}) {
		$self->{first} = undef;
		$self->{last} = undef;
	} else {
		my $psave = $self->{first};
		$self->{first} = $self->{first}->{next};
		$self->{first}->{previous} = undef;
		$psave = undef;
	}
	$self->{count} -= 1;
	return(1);
}
#
sub front {
	my ($self, $pdata) = @_;
	my $pos = undef;
	#
	defined($pdata) or die "front - undefined pdata.";
	#
	if ($self->isEmpty()) {
		return(0);
	} else {
		$$pdata = $self->{first}->{data};
		return(1);
	}
}
#
sub remove {
	my ($self, $data) = @_;
	my $pos = undef;
	#
	defined($data) or die "remove - undefined key.";
	#
	if ($self->isEmpty()) {
		return(0);
	}
	#
	for ($pos = $self->{first}; 
	     defined($pos) && ($pos->{data}->cmp($data) < 0);
	     $pos = $pos->{next}) {
		# do nothing
	}
	#
	if (!defined($pos) || $pos->{data}->cmp($data) != 0) {
		return(0);
	}
	#
	if (defined($pos->{previous}) && defined($pos->{next})) {
		$pos->{previous}->{next} = $pos->{next};
		$pos->{next}->{previous} = $pos->{previous};
	} elsif (!defined($pos->{previous}) && defined($pos->{next})) {
		$self->{first} = $pos->{next};
		$pos->{next}->{previous} = undef;
	} elsif (defined($pos->{previous}) && !defined($pos->{next})) {
		$self->{last} = $pos->{previous};
		$pos->{previous}->{next} = undef;
	} else {
		$self->{first} = undef;
		$self->{last} = undef;
	}
	#
	$pos->{key} = undef;
	$pos->{value} = undef;
	$pos = undef;
	$self->{count} -= 1;
	#
	return 1;
}
#
sub dump {
	my $self = shift;
	for (my $pos=$self->{first}; defined($pos); $pos=$pos->{next}) {
		$pos->{data}->dump();
	}
}
#
# exit with success
1;
