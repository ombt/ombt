#!/usr/bin/perl -w
use strict;
use Course;

@ARGV || die "You must supply a course name.\n";

my $class = Course->new($ARGV[0]);
while(<>){
    chomp;
    $class->add_student_record(split /:/);
}

while(1){
    print "Enter a student name [or 'l' for list; 'q' to quit]: ";
    chomp(my $name = <STDIN>);
    exit if $name =~ m/^q$/;
    print join("\n",$class->list()),"\n" and next if $name eq 'l';
    if($class->student($name)){
        $class->student($name)->print_report();
    }else{
        print "no student by that name\n";
    }
}


