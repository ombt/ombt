package Stack;
use strict;
use Carp;
sub new {
    my $type = shift;
    my $class = ref($type) || $type;
    my $size = shift;
    my $self = [$size];
    return bless $self, $class;
}

sub is_empty {
    my $self = shift;
    return !$#$self;
}
sub is_full {
    my $self = shift;
    return 0 unless $self->[0];
    return ($#$self == $self->[0]);
}

sub push {
    my $self = shift;
    my $item = shift;
    if($self->is_full()){
        carp ref($self)," is full:";
        return;
    }
    push @$self, $item;
}

sub pop {
    my $self = shift;
    if($self->is_empty()){
        carp ref($self), " is empty:";
        return;
    }else{
        return pop @$self;
    }
}
sub top {
    my $self = shift;
    if($self->is_empty()){
        carp ref($self)," is empty:";
        return;
    }
    return $self->[$#$self];
}

1;
__END__
