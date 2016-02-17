#!/usr/bin/perl -w
use strict;

# fibonacci stream generator:
sub new_fib_stream {
    my ($current, $next) = (0, 1);
    return sub{
                  my $fib = $current;
                  ($current, $next) = ($next, $current + $next);
                  return $fib;
               };
}

# create two new fibonacci streams
my $fib_stream1 = new_fib_stream();
my $fib_stream2 = new_fib_stream();

# print out first 5 fibonacci numbers from stream 1
foreach (1..5) {
    print $fib_stream1->(), "\n";
}

# print out first 10 fibonaci numbers from stream 2
foreach (1..10) {
    print $fib_stream2->(), " ";
}
print "\n";

# print out next 5 fibonacci numbers in stream 1
foreach (1..5) {
    print $fib_stream1->(), "\n";
}
