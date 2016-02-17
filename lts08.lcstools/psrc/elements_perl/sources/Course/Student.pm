package Student;
use Assignment;
use strict;

sub new {
    my $type  = shift;
    my $class = ref($type)||$type;
    my $cfg  = shift;
    my $name = shift;
    my $self = {config      => $cfg, 
                name        => $name,
                assignments => 0,
               };
    bless $self, $class;
    return $self;
}

sub add_assignment {
    my $self = shift;
    my($num, $score) = @_;
    my $cfg = $self->{config};
    my $type;
    if($num =~ s/^E//){
        $type = 'Exam';
    } else {
        $type = 'Assign';
    }
    $self->{$type}[$num] = Assignment->new($cfg,$type,$num, $score);
    $self->{assignments}++;
}

sub get_assigns {
    my $self = shift;
    return grep{$_}@{$self->{Assign}};
}

sub get_exams {
    my $self = shift;
    return grep{$_}@{$self->{Exam}};
}

sub print_report {
    my $self = shift;
    my $cfg  = $self->{config};
    print $self->{name},":\n";

    unless($self->get_assigns()||$self->get_exams()){
        print "\tNo records for this student\n";
        return;
    }

    my ($ftotal,$a_count,$e_count) = (0,0,0);
    foreach my $assign ($self->get_assigns()){
        print "\t";
        $assign->print_report();
        $ftotal += $assign->fscore();
        $a_count++;
    }
    foreach my $assign ($self->get_exams()){
        print "\t";
        $assign->print_report();
        $ftotal += $assign->fscore();
        $e_count++;
    }

    if($cfg->{Assign_no} == $a_count and
       $cfg->{Exam_no}   == $e_count){
        print "\tFinal Course Grade: $ftotal/100\n";
    }else{
        print "\tIncomplete Record\n";    
    }
    print "\n";
}

1;
__END__
