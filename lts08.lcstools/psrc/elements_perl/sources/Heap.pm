package Heap;
use strict;

my %comp = ( str  => sub {return $_[0] cmp $_[1]},
             rstr => sub {return $_[1] cmp $_[0]},
             num  => sub {return $_[0] <=> $_[1]},
             rnum => sub {return $_[1] <=> $_[0]},
           );

sub new {
    my $this = shift;
    my $class = ref($this)||$this;
    my $comp = ( shift || 'num' );
    my $self = [ { size  => 0,
                   comp  => $comp{$comp},
                 },
               ];
    return bless $self,$class;
}

sub insert {
    my $self = shift;
    my $key  = shift;
    my $data = defined($_[0])? shift : $key;
    my $node = { key  => $key,
                 data => $data,
               };
    $self->_insert_node($node);
}


sub _insert_node {
    my ($self, $node) = @_;
    my $i = ++$self->[0]{size};
    my $comp = $self->[0]{comp};
    while ( $i > 1 and 
            $comp->($self->[parent($i)]{key}, $node->{key})<=0 ) {
        $self->[$i] = $self->[parent($i)];
        $i = parent($i);
    }
    $self->[$i] = $node;
}

sub top {
    my $self = shift;
    return $self->[1]{data};
}

sub extract_top {
    my $self = shift;
    if ($self->[0]{size} < 1) {
        return;
    }
    my $top = $self->[1]{data};
    my $node = pop @$self;
    $self->[0]{size}--;
    if ($self->[0]{size} > 0) {
        $self->[1] = $node;
        $self->_pushdown(1);
    }
    return $top;
}

sub _pushdown {
    my ($self, $i) = @_;
    my $size = $self->[0]{size};
    my $comp = $self->[0]{comp};
    while( $i <= $size/2 ) {
        my $kid = left($i);
        if ( $kid < $size and 
             $comp->($self->[$kid]{key},$self->[$kid +1]{key}) < 0)
          {             
            $kid++;
          }
        last if $comp->($self->[$i]{key},$self->[$kid]{key}) >= 0;
        ($self->[$i],$self->[$kid]) = ($self->[$kid], $self->[$i]);
        $i = $kid;
    }
    
}

sub parent { return int($_[0] / 2) }
sub left   { return $_[0] * 2 }
sub right  { return $_[0] * 2 + 1 }

1;
__END__
