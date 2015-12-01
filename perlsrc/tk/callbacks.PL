#!/usr/bin/perl
use Tk;
# Main Window
my $mw = new MainWindow;
my $lab = $mw -> Label(-text=> " The Bind command ",
-font=>"ansi 12 bold") -> pack;
my $lst = $mw -> Listbox() -> pack;
$lst -> insert('end',"Don't double-click this");
$lst -> insert('end',"Don't double-click this either");
$lst -> insert('end',"Don't even think of double-clicking this");
$lst -> insert('end',"You may double-click this");
$lst -> insert('end',"No Negative Nay Nope Get the message?");
#Bind the double click event to the list box
$lst -> bind ('<Double-ButtonPress-1>', sub { double() });
my $all_keys = $mw -> Label(-justify=>"left",
-text=>"Press any of the following
Control+A
Control+Shift+A
Control+Alt+T
Right click
Control+Escape") -> pack;
#Exit when the escape key is pressed
$mw -> bind('<Key-Escape>', sub { exit });
#Shows a helping dialog box when F1 is pressed
$mw -> bind('<Key-F1>',sub { help(); });
#Binds misc keys
$mw -> bind('<Control-Key-a>', sub {
$mw -> messageBox(-message=>"You pressed Control+A, did'nt you?")
});#Control+A
$mw -> bind('<Control-Key-A>', sub {
$mw -> messageBox(-message => "Control+Shift+A, right?");
});#Control+Shift+A
$mw -> bind('<Control-Alt-Key-t>', sub {
$mw -> messageBox(-message => "Control, Alt and T");
});#Control+Alt+T
$mw -> bind('<ButtonPress-3>', sub {
$mw -> messageBox(-message => "The right way to click");
});#Right click
$mw -> bind('<Control-Key-Escape>', sub {
$mw -> messageBox(-message => "You must be a married man
What you pressed remindes married men of what they never will have
- Control or Escape");
});#Control+Escape
MainLoop;
#The helping function
sub help {
$mw -> messageBox(-message=>"Did you ask of help?
You ain't getting any
Ha Ha Ha!");
}
#This happens at double-click
sub double {
$mw -> messageBox(-message=>"You double clicked something
This script is simple - so it won't display what you clicked on
But if you want a sript that is able to do that,
write to me at binnyva\(at\)hotmail\(dot\)com
and I will send you a better script");
}

