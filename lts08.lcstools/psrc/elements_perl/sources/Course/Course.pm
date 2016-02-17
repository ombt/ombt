package Course;
use Student;
use strict;

sub new {
    my $type  = shift;
    my $class = ref($type)||$type;
    my $course = shift;
    my $self = { course   => $course,
                 students => {},
                 number   => 0,
               };
    bless $self, $class;
    $self->_configure_course();
    return $self;
}

sub _configure_course {
    my $self = shift;
    my $cfg_file = $self->{course} . '.cfg';
    my %cfg;
    open(CFG,$cfg_file)||die "can't open $cfg_file: $!";
    while(<CFG>){
        chomp;
        my ($type, @data) = split /:/;
        $cfg{$type}[$data[0]] = [ @data[1,2] ];
        $cfg{"$type".'_no'}++;
    }
    close CFG;
    my $stud_file = $self->{course} . '.std';
    open(STD,$stud_file) || die "can't open $stud_file: $!";
    while(<STD>){
        chomp(my $name = $_);
        $self->{students}{$name} = Student->new(\%cfg,$name);
        $self->{number}++;
    }
    close STD;
}


sub course {
    my $self = shift;
    return $self->{course};
}

sub number {
    my $self = shift;
    return $self->{number};
}

sub student {
    my $self = shift;
    my $name = shift;
    return $self->{students}{$name}||undef;
}

sub add_student_record {
    my $self = shift;
    my $name = shift;
    my $student = $self->student($name);
    $student->add_assignment(@_);
}

sub list {
    my $self = shift;
    my @list = map { $_->[0] }
               sort{ $a->[2] cmp $b->[2] }
               map { [$_,split] } keys %{$self->{students}};
    return @list;
}

sub print_report {
    my $self = shift;
    my $course = $self->course();
    my $number = $self->number();
    print "Class Report: course = $course: students = $number\n";
    foreach my $name ( $self->list() ){
        $self->student($name)->print_report();
    }
}


1;
