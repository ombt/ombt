#!/usr/bin/perl -w
use strict;
use warnings;
use Tk;
use Tk::TableMatrix;

my $main = MainWindow->new;
$main->title("TM - in list mode");

my $xtvar;
foreach my $row  (0 .. 7){
    foreach my $col (0 .. 2){
        if ($col == 0) {
            $xtvar->{"$row,$col"} = "click row $row";
        }
        else {
            $xtvar->{"$row,$col"} = "r$row, c$col";
        }
    }
}

my $xtable = $main->Scrolled(
    'TableMatrix',
    -rows          => 8,
    -cols          => 3,
    -ipadx         => 3,
    -variable      => $xtvar,
    -selectmode    => 'single',
    -selecttype    => 'row',
    -resizeborders => 'none',
    -state         => 'disabled',
    -cursor        => 'top_left_arrow',
    -bg            => 'white',
    -scrollbars    => 'ose',
)->pack;

# Clean up if mouse leaves the widget
$xtable->bind(
    '<FocusOut>',
    sub {
        my $w = shift;
        $w->selectionClear('all');
    }
);

# Highlight the cell under the mouse
$xtable->bind(
    '<Motion>',
    sub {
        my $w  = shift;
        my $Ev = $w->XEvent;
        if ( $w->selectionIncludes( '@' . $Ev->x . "," . $Ev->y ) ) {
            Tk->break;
        }
        $w->selectionClear('all');
        $w->selectionSet( '@' . $Ev->x . "," . $Ev->y );
        Tk->break;
    }
);

# MouseButton 1 event
$xtable->bind(
    '<1>',
    sub {
        my $w = shift;
        $w->focus;
        my ($rc) = @{ $w->curselection };
        # Do something with the selection
        print " Selection: $rc\n";
    }
);

MainLoop;
