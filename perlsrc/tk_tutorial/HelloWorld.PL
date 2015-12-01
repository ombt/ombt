#!/usr/bin/perl
use Tk;
# Main Window
$mw = new MainWindow;
$label = $mw -> Label(-text=>"Hello World") -> pack();
$button = $mw -> Button(-text => "Quit", 
		-command => sub { exit }) -> pack();
MainLoop;
