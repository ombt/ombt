#!/usr/bin/perl -w
use strict;
use Course;

@ARGV || die "You must supply a course name.\n";

my $class = Course->new($ARGV[0]);
while(<>){
    chomp;
    $class->add_student_record(split /:/);
}
$class->print_report();
__END__
