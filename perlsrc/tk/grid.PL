#!/usr/bin/perl
#
use Tk;
#
# main window
#
my $mw = new MainWindow;
#
# child of main window
#
my $frm_name = $mw->Frame();
#
# children of frame
#
my $lab = $frm_name->Label(-text => "Name:");
my $ent = $frm_name->Entry();
#
# children of main window
#
my $but = $mw->Button(-text    => "Push Me", 
                      -command => \&push_button);
my $textarea = $mw->Frame();
#
# children of text area
#
my $txt   = $textarea->Text(-width => 40, 
                            -height=>10);
my $srl_y = $textarea->Scrollbar(-orient => 'v',
                                 -command=>[yview => $txt]);
my $srl_x = $textarea->Scrollbar(-orient => 'h',
                                 -command=>[xview => $txt]);
$txt -> configure(-yscrollcommand =>['set', $srl_y],
                  -xscrollcommand=>['set', $srl_x]);
#
# set up grid in each frame or window.
#
$lab->grid(-row    => 1,
           -column => 1);
$ent->grid(-row    => 1,
           -column => 2);
$frm_name->grid(-row        => 1,
                -column     => 1, 
                -columnspan => 2);
$but->grid(-row        => 4,
           -column     => 1,
           -columnspan => 2);
$txt->grid(-row    => 1,
           -column => 1);
$srl_y->grid(-row    => 1,
             -column => 2,
             -sticky => "ns");
$srl_x->grid(-row    => 2,
             -column => 1,
             -sticky => "ew");
$textarea->grid(-row        => 5,
                -column     => 1,
                -columnspan => 2);
#
MainLoop;
#
sub push_button {
	my $name = $ent->get();
	$txt->insert('end', "Hello, $name");
}

