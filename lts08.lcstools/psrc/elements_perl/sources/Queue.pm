package Queue;
use strict;
use Carp;
use Stack;
use vars '@ISA';  # declare @ISA as package global
@ISA = qw(Stack); # inherit from Stack


sub enqueue {
    shift->push(shift);
}

sub dequeue {
    my $self = shift;
    if($self->is_empty()){
        carp ref($self)," is empty:";
        return;
    }
    return splice(@$self,1,1);
}

sub front {
    my $self = shift;
    if($self->is_empty()){
        carp ref($self)," is empty:";
        return;
    }
    return $self->[1];
}

1;
__END__
