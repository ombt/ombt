#!/usr/bin/perl
use Tk;
my $mw = new MainWindow;
my $txt = $mw -> Scrolled('Text',-scrollbars=>"oe") -> pack;
$txt -> insert('end',
"Arthur: \"It's at times like this I wish I'd listened to my mother\"
Ford : \"Why, what did she say?\"
Arthur: \"I don't know, I never listened\"
Douglas Adams");
MainLoop;
The same example using han

