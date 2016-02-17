#!/opt/exp/bin/perl -w
%h = (
	A => {
		'Aa' => 1,
		'Ab' => 2,
	},
	B => {
		'Ba' => 1,
		'Bb' => 2,
	},
	C => {
		'Ca' => 1,
		'Cb' => 2,
	},
);
#
@a = %h;
#
print STDERR "a = @a\n";
#
exit 0;
