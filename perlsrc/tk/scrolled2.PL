#!/usr/bin/perl
use Tk;
my $mw = new MainWindow;
my $textarea = $mw -> Frame();
my $txt = $textarea -> Text(-width=>40, -height=>10);
my $srl_y = $textarea -> Scrollbar(-orient=>'v',-command=>[yview => $txt]);
my $srl_x = $textarea -> Scrollbar(-orient=>'h',-command=>[xview => $txt]);
$txt -> configure(-yscrollcommand=>['set', $srl_y],
-xscrollcommand=>['set',$srl_x]);
$txt -> insert('end',
"Arthur: \"It's at times like this I wish I'd listened to my mother\"
Ford : \"Why, what did she say?\"
Arthur: \"I don't know, I never listened\"
Douglas Adams");
#Geometry
$txt -> grid(-row=>1,-column=>1);
$srl_y -> grid(-row=>1,-column=>2,-sticky=>"ns");
$srl_x -> grid(-row=>2,-column=>1,-sticky=>"ew");
$textarea -> grid(-row=>1,-column=>1,-columnspan=>2);
MainLoop;

