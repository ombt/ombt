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
# new frame
#
my $frm = $mw->Frame()->pack();
#
my $lab = $mw->Label(-text => "Enter Name:")->pack();
my $ent = $frm->Entry()-> pack();
#
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
	$ent->insert(0, "Hello, ");
}
#
exit 0;
