#!/usr/bin/perl -w
use strict;
use LWP::Simple;
use GIFgraph::mixed;
my $root_dir = '/home/ajohnson/STOCK/portfol';
my $stock_file = "$root_dir/stock.conf";
my @Symbols;
open(STOCK, $stock_file) || die "can't open file: $!";
while( <STOCK> ) {
    last if m/^Purchases:/;
    chomp;
    push @Symbols, $1 if m/^CODE\s+([\w\.]+)/;
}
close STOCK;
foreach my $symbol (@Symbols) {
    my $quote = fetch_quote($symbol);
    write_quote($symbol, $quote);
    
    my @data  = read_data($symbol);
    make_gif($symbol, @data);
}

sub fetch_quote {
    my $symbol = shift;
    my $format = 'd1ohgl1v';
    my $url    = 'http://quote.yahoo.com/d/';
    my $quote  = get("$url/quotes.csv?s=$symbol&f=$format&e=.csv");
    $quote =~ s/\s+$/\n/; # replace line ending with newline
    return $quote;
}
sub write_quote {
    my ($symbol, $quote) = @_;
    open(DBASE, ">>$root_dir/$symbol.csv") || die "can't: $!";
    print DBASE $quote;
    close DBASE;
}
sub read_data {
    my $symbol = shift;
    my @data;
    open(FILE, "$root_dir/$symbol.csv")||die "can't open file: $!";
    $_ = <FILE>; # throw out first line
    while ( <FILE> ) {
        my @fields = split /,/;
        push @{$data[0]},$fields[0];
        push @{$data[1]},$fields[4];
        push @{$data[2]},$fields[5];
    }
    close FILE;
    @{$data[0]} = map{s/^"|"$//g;$_} @{$data[0]};
    return @data;
}
sub make_gif {
    my($title, @data) = @_;

    my $graph = GIFgraph::mixed->new();
    $graph->set_legend('Close Price', 'Volume Traded');

    $graph->set(
        title             => $title,
        two_axes          => 1,
        types             => ['lines', 'bars'],
        x_label           => 'Date',
        y1_label          => 'Close Price',
        y2_label          => 'Volume Traded',
        y_min_value       => 0,
        x_label_position  => 1/2,
        x_labels_vertical => 1,
        x_label_skip      => 5,
        t_margin          => 10,
        b_margin          => 10,
        r_margin          => 10,
        l_margin          => 10,
        legend_placement  => 'BC',
    );

    $graph->plot_to_gif("$title.gif", \@data);
}
