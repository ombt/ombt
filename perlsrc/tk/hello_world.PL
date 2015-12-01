#!/usr/bin/perl -w
#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
use strict;
#
use Tk;
#
my $mw;
#
sub exitProgram {
	$mw->messageBox(-message=>"Good Bye");
	exit 0;
}
#
# Main Window
#
$mw = new MainWindow;
#
my $label = $mw->Label(-text   => "Hello World",
                       -font   => "courierfont",
                       -relief => "raised");
$label->pack(-side=>"left", -anchor=>'w');
#
my $button = $mw->Button(-text    => "Quit",
                         -command => sub { exit 0; })->pack();
my $button2 = $mw->Button(-text    => "Quit2",
                          -command => \&exitProgram)->pack();
#
MainLoop;
#
exit 0;
