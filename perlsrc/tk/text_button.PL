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
# Main Window
#
my $mw = new MainWindow;
#
# build GUI area
#
#GUI Building Area
my $ent = $mw->Entry()-> pack();
my $but = $mw-> Button(-text => "Push Me",
                       -command =>\&push_button);
$but -> pack();
#
# start main gui loop.
#
MainLoop;
#
# This is executed when the button is pressed
#
sub push_button {
	$ent->insert('end', "Hello");
}
#
exit 0;
