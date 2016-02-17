#!/usr/bin/perl -w
use strict;


sub insertion_sort {
    my $in = \@_;
    for(my $j = 1; $j < @_; $j++){
        my $val = $in->[$j];
        my $i = ($j - 1);
        while($i >= 0 and $in->[$i] > $val){
            $in->[$i+1] = $in->[$i];
            $i = $i - 1;
        }
        $in->[$i + 1] = $val;
    }
}


sub selection_sort {
    my $in = \@_;
    my $upper = $#_;
    for(my $i = $upper; $i; $i--){
        my $max = $i;
        for(my $j = 0;$j < $i; $j++) {
            $max = $j if $in->[$j] > $in->[$max];
        }
        ($in->[$i], $in->[$max]) = ($in->[$max], $in->[$i]);
    }
}

sub bubble_sort1 {
    my $in = \@_;
    my $upper = $#_;
    my $swaps = 1;
    while($upper && $swaps){
        $swaps = 0;
        for(my $i = 0; $i < $upper; $i++){
            if($in->[$i] > $in->[$i+1]){
                ($in->[$i], $in->[$i+1]) = ($in->[$i+1], $in->[$i]);
                $swaps++;
            }
        }
    }
}

sub bubble_sort2 {
    return unless @_;
    my $in  = \@_;
    my $low = 0;
    my $up  = $#_;
    my $direction = 1;
    my $swaps = 1;
    while($swaps and $low <= $up){
        $swaps = 0;
        if ($direction) {
            for(my $i = 0; $i < $up; $i++){
                if($in->[$i] > $in->[$i+1]){
                    ($in->[$i], $in->[$i+1]) = ($in->[$i+1], $in->[$i]);
                    $swaps++;
                }
            }
            $up--;
        } else {
            for(my $i = $up; $i > $low; $i--){
                if($in->[$i] > $in->[$i+1]){
                    ($in->[$i], $in->[$i+1]) = ($in->[$i+1], $in->[$i]);
                    $swaps++;
                }
            }
            $low++;
        }
        if($direction){
            $direction = 0;
        }else{
            $direction = 1;
        }
    }
}

sub heap_sort {
    my $heap = \@_;
    unshift @$heap, scalar @_;
    for(my $i = int($heap->[0] / 2) ; $i >= 1; $i--){
        pushdown($i,$heap);
    }

    for(my $i = $heap->[0]; $i >= 2; $i--){
        ($heap->[1], $heap->[$i]) = ($heap->[$i], $heap->[1]);
        $heap->[0]--;
        pushdown(1,$heap);
    }

    shift @$heap;
}

sub pushdown {
    my ($i, $heap) = @_;
    my $size = $heap->[0];
    while($i <= $size/2){
        my $child = 2 * $i;
        if ($child < $size and $heap->[$child] < $heap->[$child+1]){
            $child++;
        }
        if ($heap->[$i] >= $heap->[$child]) { last }
        ($heap->[$i], $heap->[$child]) = ($heap->[$child], $heap->[$i]);
        $i = $child;
    }
}

