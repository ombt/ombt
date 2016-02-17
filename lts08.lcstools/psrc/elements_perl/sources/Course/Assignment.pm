package Assignment;

sub new {
    my $type = shift;
    my $class = ref($type)||$type;
    my $cfg = shift;
    my $self = {config => $cfg};
    bless $self, $class;
    $self->_assign(@_);
    return $self;
}

sub _assign {
    my $self = shift;
    my $cfg = $self->{config};
    my ($type, $num, $score) = @_;
    my ($raw, $final, $fscore);

    $type =~ m/^(Assign|Exam)$/ or
        die "unkown assignment type: $type\n";

    $cfg->{$type}[$num] or
        die "Unrecognized $type number: $num\n";

    ($raw, $final) = @{$cfg->{$type}[$num]};
    if($type eq 'Assign'){
        $type = "Assignment $num";
    } else {
        $type = "Exam $num";
    }
    $fscore = sprintf("%.2f",$score /($raw/$final));
    $self->{type}   = $type;
    $self->{score}  = $score;
    $self->{raw}    = $raw;
    $self->{fscore} = $fscore;
    $self->{final}  = $final;
}

sub fscore {
    my $self = shift;
    return $self->{fscore};
}
sub print_report {
    my $self = shift;
    print $self->{type},": raw = ";
    print $self->{score}, "/",$self->{raw}," : Adjusted = ";
    print $self->{fscore}, "/",$self->{final},"\n";
}

1;
__END__
