#!/usr/bin/perl
use Tk;
use strict;
# Main Window
my $mw = new MainWindow;
my $response = $mw -> messageBox(-message=>"Really quit?",-type=>'yesno',-icon=>'question');

if( $response eq "yes" ) {
    exit
}
else {
    $mw -> messageBox(-message=>"I know you like this application!", -type=>"ok");
}

MainLoop;
