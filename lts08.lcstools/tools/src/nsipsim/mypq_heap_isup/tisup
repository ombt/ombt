#!/opt/exp/bin/perl -w
#
use Carp;
use lib "$ENV{SIPSIMHOME}/bin";
use lib "$ENV{SIPSIMHOME}/lib";
#
use myisup;
#
sub printit {
	my ($msg) = @_;
	print STDERR ${msg};
}
#
my $isup = "myisup"->new(\&printit);
#
my $fixed = [];
$fixed->[0][0] = 0b11111111;
$fixed->[1][0] = 0b11111111;
$fixed->[1][1] = 0b11111111;
$fixed->[2][0] = 0b11111111;

#
my $variable = [];
$variable->[0][0] = 0b11111111;
$variable->[0][1] = 0b11111111;
$variable->[0][2] = 0b11111111;
$variable->[0][3] = 0b11111111;
$variable->[1][0] = 0b11111111;
$variable->[1][1] = 0b11111111;
$variable->[1][2] = 0b11111111;
$variable->[1][3] = 0b11111111;
$variable->[1][4] = 0b11111111;
#
my $optional = [];
$optional->[0]{id} = 0b00000011;
$optional->[0]{data}[0] = 0b11111111;
$optional->[0]{data}[1] = 0b11111111;
$optional->[0]{data}[2] = 0b11111111;
$optional->[1]{id} = 0b00111101;
$optional->[1]{data}[0] = 0b11111111;
$optional->[1]{data}[1] = 0b11111111;
$optional->[1]{data}[2] = 0b11111111;
$optional->[1]{data}[3] = 0b11111111;
$optional->[1]{data}[4] = 0b11111111;
$optional->[1]{data}[5] = 0b11111111;
$optional->[2]{id} = 0b00101000;
$optional->[2]{data}[0] = 0b11111111;
$optional->[2]{data}[1] = 0b11111111;
$optional->[2]{data}[2] = 0b11111111;
$optional->[2]{data}[3] = 0b11111111;
$optional->[2]{data}[4] = 0b11111111;
$optional->[2]{data}[5] = 0b11111111;
#
my $msgid = 1;
my $datablock = 0;
#
$isup->makemsg("ansi", \$datablock, $msgid, $fixed, $variable, $optional);
#
$isup->decode("ansi", $datablock);
#
exit 0;
