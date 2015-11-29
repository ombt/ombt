#!/usr/bin/perl -w
#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
#
use Getopt::Std;
#
# data
#
my %loginrejectioncodes = (
	"A" => "Not Authorized. There was an invalid username and password combination in the Login Request Message.",
	"S" => "Session not available. The Requested Session in the Login Request Packet was either invalid or not available."
);
#
my %side_enums = (
	"B" => "Buy", "S" => "Sell", "T" => "Short", "E" => "Short" );
my %display_enums = (
	"Y" => "Anonymous-Price to Comply", "N" => "Non-Displayed", "A" => "Attributable-Price to Display", "J" => "Imbalance Only" );
my %peg_type_enums = (
	"M" => "Midpoint", "N" => "No Peg", "P" => "Market", "R" => "Primary" );
my %inter_market_sweep_eligibility_enums = (
	"Y" => "elgible", "N" => "not elgibile" );
my %cross_type_enums = (
	"O" => "opening cross", "C" => "closing crosss", "I" => "intraday cross", "N" => "immediately live" );
#
my %rejected_order_reasons_enums = (
	"Y" => "No Shares Found for Routing",
	"C" => "NASDAQ is Closed",
	"I" => "Invalid Order Side",
	"E" => "Invalid Peg",
	"L" => "Invalid Firm",
	"Z" => "Quantity Exceeds Threshold",
	"O" => "Other. A reason not contemplated in this version of RASH.",
	"B" => "Quote not available for pegged order.",
	"P" => "Pegging Not Allowed",
	"X" => "Invalid Price",
	"G" => "Destination Not Available",
	"J" => "Processing Error",
	"N" => "Invalid Routing Instructions",
	"D" => "Invalid Display value",
	"M" => "Outside of Permitted Times for Clearing Destination",
	"H" => "Security is Halted",
	"S" => "Invalid Symbol",
	"Q" => "Invalid Order Quantity",
	"K" => "Invalid Minimum Quantity",
	"W" => "Invalid Destination",
	"A" => "Advance Features Not Allowed",
	"U" => "Possible Duplicate Order",
	"V" => "Invalid Order Type",
	"T" => "Test Mode",
	"R" => "Routing Not Allowed",
	"a" => "invalid message format",
	"b" => "invalid symbol",
	"c" => "invalid account",
	"d" => "short sale violation",
	"e" => "order rejected due to ISO order check",
	"f" => "order rejected due to GTC oreder check",
	"g" => "order rejected due to pre-market order check",
	"h" => "order rejected due to post-market order check",
	"i" => "order rejected due to the delayed checking flag turned off",
	"j" => "order rejected due to notional value threshold exceeded",
	"k" => "order rejected due to max order share threshold exceeded",
	"l" => "order rejected due to max order value threshold exceeded",
	"m" => "order rejected due to previous command to reject all orders",
	"n" => "order rejected due to invalid price",
	"o" => "Order rejected due to symbol not listed on easy to borrow list",
	"p" => "PRM is Not Available",
	"q" => "Invalid Message",
	"r" => "Snap Is In Process",
	"s" => "Symbol Halted",
	"t" => "Order Rejected Due To On Open",
	"u" => "Order Rejected Due To On Close",
	"v" => "Order Rejected Due To Program Trading",
	"w" => "Over Daily Trade Limit",
	"x" => "Over Daily Trade Value",
	"y" => "Over Daily Order Limit",
	"z" => "Over Daily Order Market Value",
	"1" => "Over Daily Open Order Limit",
	"2" => "Over Daily Open Order Market Value",
	"3" => "Over Daily Symbol Trade Limit",
	"4" => "Over Daily Symbol Trade Value",
	"5" => "Over Daily Symbol Order Limit",
	"6" => "Over Daily Symbol Order Market Value",
	"7" => "Over Daily Symbol Open Order Limit",
	"8" => "Over Daily Symbol Open Order Market Value",
	"9" => "Over Daily Trade Net Limit",
	"." => "Over Daily Open Order Net Limit",
	"-" => "Over Daily Open Order Net Market Value",
	"+" => "Over Daily Symbol Trade Net Limit",
	"=" => "Over Daily Symbol Trade Net Value",
	"&" => "Daily Symbol Order Net Limit",
	"^" => "Over Daily Symbol Order Market Net Value",
	"!" => "Over Daily Symbol Open Order Net Limit",
	"~" => "Over Daily Symbol Open Order Net Market Value",
	")" => "Over Daily Order Net Market Value",
	"(" => "Over Daily Order Net Limit",
	"}" => "Over Daily Trade Net Value",
	"{" => "Order rejected due to symbol not listed on restricted list",
);
#
my %liquidity_enums = (
	"A" => "Added",
	"R" => "Removed",
	"X" => "Routed",
	"D" => "DOT",
	"F" => "Added or Opening Trade (on NYSE)",
	"G" => "Odd Lot or On-Close order (on NYSE)",
	"J" => "Non-displayed and added liquidity",
	"O" => "Open Cross (billable)",
	"M" => "Open Cross (non-billable)",
	"C" => "Closing Cross (billable)",
	"L" => "Closing Cross (non-billable)",
	"H" => "Halt/IPO Cross (billable)",
	"K" => "Halt/IPO Cross (non-billable)",
	"I" => "Intraday/Post-Market Cross",
	"Y" => "Re-Routed by NYSE",
);
#
my %broken_trade_reason_enums = (
	"E" => "Erroneous. The trade was deemed clearly erroneous",
	"C" => "Consent. The two parties mutually agreed to break the trade",
	"S" => "Supervisory. The trade was manually broken by a NASDAQ supervisory terminal.",
	"X" => "External. The trade was broken by an external third party.",
);
#
my %incomingmsgs = (
	"S" => {
		name => "System Event Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Event Code", offset => 9,
			length => 1 },
		]
	},
	"A" => {
		name => "Accepted Order Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 9,
			length => 14 },
		{ name => "Side", offset => 23, 
			length => 1, format => sub {
				return enum_formatter(shift, \%side_enums); } },
		{ name => "Shares/OrderQty", offset => 24,
			length => 6, format => \&integer_formatter },
		{ name => "Stock Symbol", offset => 30,
			length => 6 },
		{ name => "Price", offset => 36,
			length => 10, format => \&price_formatter },
		{ name => "Time In Force", offset => 46,
			length => 5, format => \&integer_formatter },
		{ name => "Firm/Client ID", offset => 51,
			length => 4 },
		{ name => "Display", offset => 55,
			length => 1, format => sub {
				return enum_formatter(shift, \%display_enums); } },
		{ name => "Order Reference Number", offset => 56,
			length => 9 },
		{ name => "MinQty", offset => 65,
			length => 6, format => \&integer_formatter },
		{ name => "Max Floor", offset => 71,
			length => 6, format => \&integer_formatter },
		{ name => "Peg Type", offset => 77,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Peg Difference Sign", offset => 78,
			length => 1 },
		{ name => "Peg Difference", offset => 79,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Price", offset => 89,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Peg Type", offset => 99,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Discretion Peg Difference Sign", offset => 100,
			length => 1 },
		{ name => "Discretion Peg Difference", offset => 101,
			length => 10, format => \&price_formatter },
		{ name => "Capacity/Rule 80A Indicator", offset => 111,
			length => 1 },
		{ name => "Random Reserve", offset => 112,
			length => 6, format => \&integer_formatter },
		{ name => "Route Dest/Exec Broker", offset => 118,
			length => 4 },
		{ name => "Cust/Terminal ID/Sender SubID", offset => 122,
			length => 32 },
		]
	},
	"R" => {
		name => "Accepted Order Message with Cross Functionality",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 9,
			length => 14 },
		{ name => "Side", offset => 23,
			length => 1, format => sub {
				return enum_formatter(shift, \%side_enums); } },
		{ name => "Shares/OrderQty", offset => 24,
			length => 6, format => \&integer_formatter },
		{ name => "Stock Symbol", offset => 30,
			length => 6 },
		{ name => "Price", offset => 36,
			length => 10, format => \&price_formatter },
		{ name => "Time In Force", offset => 46,
			length => 5, format => \&integer_formatter },
		{ name => "Firm/Client ID", offset => 51,
			length => 4 },
		{ name => "Display", offset => 55,
			length => 1, format => sub {
				return enum_formatter(shift, \%display_enums); } },
		{ name => "Order Reference Number", offset => 56,
			length => 9 },
		{ name => "MinQty", offset => 65,
			length => 6, format => \&integer_formatter },
		{ name => "Max Floor", offset => 71,
			length => 6, format => \&integer_formatter },
		{ name => "Peg Type", offset => 77,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Peg Difference Sign", offset => 78,
			length => 1 },
		{ name => "Peg Difference", offset => 79,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Price", offset => 89,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Peg Type", offset => 99,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Discretion Peg Difference Sign", offset => 100,
			length => 1 },
		{ name => "Discretion Peg Difference", offset => 101,
			length => 10, format => \&price_formatter },
		{ name => "Capacity/Rule 80A Indicator", offset => 111,
			length => 1 },
		{ name => "Random Reserve", offset => 112,
			length => 6, format => \&integer_formatter },
		{ name => "Route Dest/Exec Broker", offset => 118,
			length => 4 },
		{ name => "Cust/Terminal ID/Sender SubID", offset => 122,
			length => 32 },
		{ name => "Inter-market sweep elgibility", offset => 137,
			length => 1, format => sub {
				return enum_formatter(shift, \%inter_market_sweep_eligibility_enums); } },
		{ name => "Cross type", offset => 138,
			length => 1, format => sub {
				return enum_formatter(shift, \%cross_type_enums); } },
		]
	},
	"C" => {
		name => "Canceled Order Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 9,
			length => 14 },
		{ name => "Shares", offset => 23,
			length => 6, format => \&integer_formatter }
		]
	},
	"J" => {
		name => "Rejected Order Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 9,
			length => 14 },
		{ name => "Reason", offset => 23,
			length => 1, format => sub {
				return enum_formatter(shift, \%rejected_order_reasons_enums); } },
		]
	},
	"E" => {
		name => "Executed Order Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 9,
			length => 14 },
		{ name => "Shares", offset => 23,
			length => 6, format => \&integer_formatter },
		{ name => "Price", offset => 29,
			length => 10, format => \&price_formatter },
		{ name => "Liquidity", offset => 39,
			length => 1, format => sub {
				return enum_formatter(shift, \%liquidity_enums); } },
		{ name => "Match number", offset => 40,
			length => 9 },
		]
	},
	"B" => {
		name => "Broken Trade Message",
		fields => [
		{ name => "Time Stamp (msec past midnight)", offset => 0,
			length => 8, format => \&timestamp_formatter },
		{ name => "Message Type", offset => 8,
			length => 1 },
		{ name => "Order Token", offset => 9,
			length => 14 },
		{ name => "Match Number", offset => 3,
			length => 9, format => \&integer_formatter },
		{ name => "Reason", offset => 2,
			length => 1, format => sub {
				return enum_formatter(shift, \%broken_trade_reason_enums); } },
		]
	},
);
#
my %outgoingmsgs = (
	"O" => {
		name => "Enter Order Message",
		fields => [
		{ name => "Message Type", offset => 0,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 1,
			length => 14 },
		{ name => "Side", offset => 15,
			length => 1, format => sub {
				return enum_formatter(shift, \%side_enums); } },
		{ name => "Shares/OrderQty", offset => 16,
			length => 6, format => \&integer_formatter },
		{ name => "Stock Symbol", offset => 22,
			length => 6 },
		{ name => "Price", offset => 28,
			length => 10, format => \&price_formatter },
		{ name => "Time In Force", offset => 38,
			length => 5, format => \&integer_formatter },
		{ name => "Firm/Client ID", offset => 43,
			length => 4 },
		{ name => "Display", offset => 47,
			length => 1, format => sub {
				return enum_formatter(shift, \%display_enums); } },
		{ name => "MinQty", offset => 48,
			length => 6, format => \&integer_formatter },
		{ name => "Max Floor", offset => 54,
			length => 6, format => \&integer_formatter },
		{ name => "Peg Type", offset => 60,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Peg Difference Sign", offset => 61,
			length => 1 },
		{ name => "Peg Difference", offset => 62,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Price", offset => 72,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Peg Type", offset => 82,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Discretion Peg Difference Sign", offset => 83,
			length => 1 },
		{ name => "Discretion Peg Difference", offset => 84,
			length => 10, format => \&price_formatter },
		{ name => "Capacity/Rule 80A Indicator", offset => 94,
			length => 1 },
		{ name => "Random Reserve", offset => 95,
			length => 6, format => \&integer_formatter },
		{ name => "Route Dest/Exec Broker", offset => 101,
			length => 4 },
		{ name => "Cust/Terminal ID/Sender SubID", offset => 105,
			length => 32 },
		]
	},
	"Q" => {
		name => "Enter Order Message with Cross Functionality",
		fields => [
		{ name => "Message Type", offset => 0,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 1,
			length => 14 },
		{ name => "Side", offset => 15,
			length => 1, format => sub {
				return enum_formatter(shift, \%side_enums); } },
		{ name => "Shares/OrderQty", offset => 16,
			length => 6, format => \&integer_formatter },
		{ name => "Stock Symbol", offset => 22,
			length => 6 },
		{ name => "Price", offset => 28,
			length => 10, format => \&price_formatter },
		{ name => "Time In Force", offset => 38,
			length => 5, format => \&integer_formatter },
		{ name => "Firm/Client ID", offset => 43,
			length => 4 },
		{ name => "Display", offset => 47,
			length => 1, format => sub {
				return enum_formatter(shift, \%display_enums); } },
		{ name => "MinQty", offset => 48,
			length => 6, format => \&integer_formatter },
		{ name => "Max Floor", offset => 54,
			length => 6, format => \&integer_formatter },
		{ name => "Peg Type", offset => 60,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Peg Difference Sign", offset => 61,
			length => 1 },
		{ name => "Peg Difference", offset => 62,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Price", offset => 72,
			length => 10, format => \&price_formatter },
		{ name => "Discretion Peg Type", offset => 82,
			length => 1, format => sub {
				return enum_formatter(shift, \%peg_type_enums); } },
		{ name => "Discretion Peg Difference Sign", offset => 83,
			length => 1 },
		{ name => "Discretion Peg Difference", offset => 84,
			length => 10, format => \&price_formatter },
		{ name => "Capacity/Rule 80A Indicator", offset => 94,
			length => 1 },
		{ name => "Random Reserve", offset => 95,
			length => 6, format => \&integer_formatter },
		{ name => "Route Dest/Exec Broker", offset => 101,
			length => 4 },
		{ name => "Cust/Terminal ID/Sender SubID", offset => 105,
			length => 32 },
		{ name => "Inter-market sweep elgibility", offset => 137,
			length => 1, format => sub {
				return enum_formatter(shift, \%inter_market_sweep_eligibility_enums); } },
		{ name => "Cross type", offset => 138,
			length => 1, format => sub {
				return enum_formatter(shift, \%cross_type_enums); } },
		]
	},
	"X" => {
		name => "Cancel Order Message",
		fields => [
		{ name => "Message Type", offset => 0,
			length => 1 },
		{ name => "Order Token/Client Order ID", offset => 1,
			length => 14 },
		{ name => "Shares", offset => 15,
			length => 6, format => \&integer_formatter }
		]
	},
);
#
my %packettypes = (
	"+" => { name => "Debug Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			print "Soup TCP Debug Msg: $payload\n";
		}
	},
	"A" => { name => "Login Accepted Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			my $session = substr($payload, 1, 10);
			my $sequencenumber = substr($payload, 11, 10);
			print "Soup TCP Session: $session\n";
			print "Soup TCP Sequence Number: $sequencenumber\n";
		}
	},
	"J" => { name => "Login Rejected Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			my $rejectreasonsode = substr($payload, 1, 1);
			print "Soup TCP Rejection Reason Code: $rejectreasoncode\n";
			if (exists($loginrejectioncodes{$rejectreasoncode})) {
				print "$loginrejectioncodes{$rejectreasoncode}";
			}
		}
	},
	"S" => { name => "Sequenced Data Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			my $rashpayload = substr($payload, 1);
			handle_rash_payload($msgdir, $rashpayload);
		}
	},
	"H" => { name => "Server Heartbeat Packet", ignore => 1,
		handler => sub {
			my ($msgdir, $payload) = @_;
			print "No payload for this Soup TCP message type.\n";
		}
	},
	"L" => { name => "Login Request Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			my $username = substr($payload, 1, 6);
			my $password = substr($payload, 7, 10);
			my $requestedsession = substr($payload, 17, 10);
			my $requestedsequencenumber = substr($payload, 27, 10);
			print "Soup TCP User Name: $username\n";
			print "Soup TCP Password: $password\n";
			print "Soup TCP Requested Session: $requestedsession\n";
			print "Soup TCP Requested Sequence Number: $requestedsequencenumber\n";
		}
	},
	"U" => { name => "Unsequenced Data Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			my $rashpayload = substr($payload, 1);
			handle_rash_payload($msgdir, $rashpayload);
		}
	},
	"R" => { name => "Client Heartbeat Packet", ignore => 1,
		handler => sub {
			my ($msgdir, $payload) = @_;
			print "No payload for this Soup TCP message type.\n";
		}
	},
	"O" => { name => "Logout Request Packet", ignore => 0,
		handler => sub {
			my ($msgdir, $payload) = @_;
			print "No payload for this Soup TCP message type.\n";
		}
	}
);
#
sub usage {
	print "usage: rp [-?] [-V] [-A] [rashlogfile [...]]\n"
}
#
sub enum_formatter {
	my ($enum, $enumdict) = @_;
	chomp($enum);
	if (exists($enumdict->{$enum})) {
		return $enumdict->{$enum};
	} else {
		return $enum;
	}
}
sub price_formatter {
	my ($rawvalue) = @_;
	my ($dollars,$cents) = $rawvalue =~ /(......)(....)/;
	$dollars =~ s/^0+//;
	if (!$dollars) {
		$dollars = 0;
	}
	return "\$$dollars.$cents";
}
#
sub timestamp_formatter {
	my ($rawvalue) = @_;
	$rawvalue /= 1000;
	my $hours = int($rawvalue/3600);
	my $minutes = int(($rawvalue%3600)/60);
	my $seconds = int(($rawvalue%3600)%60);
	return sprintf("%02d:%02d:%02d", $hours,$minutes,$seconds);
}
#
sub integer_formatter {
	my ($rawvalue) = @_;
	$rawvalue =~ s/^0+//;
	if (!$rawvalue) { $rawvalue = 0; }
	return $rawvalue;
}
#
sub parsemsg {
	my ($rashpayload, $msgdict) = @_;
	print "Parsing Msg: ${$msgdict}{name}\n";
	foreach $field (@{${$msgdict}{fields}}) {
		if ($verbose) {
			print "Field Name: $field->{name}\n";
			print "Field Offset: $field->{offset}\n";
			print "Field Length: $field->{length}\n";
			my $value = substr($rashpayload, $field->{offset}, $field->{length});
			print "Field Value: $value\n";
			if (exists($field->{format})) {
				my $formatter = $field->{format};
				my $formattedvalue = &{${formatter}}($value);
				print "Formatted Field Value: $formattedvalue\n";
			}
		} else {
			print "$field->{name}: ";
			my $value = substr($rashpayload, $field->{offset}, $field->{length});
			print "$value";
			if (exists($field->{format})) {
				my $formatter = $field->{format};
				my $formattedvalue = &{${formatter}}($value);
				print " ($formattedvalue)";
			}
			print "\n";
		}
	}
}
#
sub handle_rash_payload {
	my ($msgdir, $rashpayload) = @_;
	if ($msgdir eq 'R') {
		my $messagetype = substr($rashpayload, 8, 1);
		if (exists($incomingmsgs{$messagetype})) {
			parsemsg($rashpayload, \%{$incomingmsgs{$messagetype}});
		} else {
			print "WARNING: unknown received msg type: $messagetype\n";
		}
	} elsif ($msgdir eq 'S') {
		my $messagetype = substr($rashpayload, 0, 1);
		if (exists($outgoingmsgs{$messagetype})) {
			parsemsg($rashpayload, \%{$outgoingmsgs{$messagetype}});
		} else {
			print "WARNING: unknown sent msg type: $messagetype\n";
		}
	} else {
		print "WARNING: unknown msg direction: $msgdir\n";
	}
}
#
sub parsefile {
	my ($infd, $allmsgs) = @_;
	#
	while (defined($rec = <$infd>)) {
		chomp($rec);
		#
		$rec =~ m/^([RS])\s(\S+)\s(\S+)\s(\S+)\s(\S+)\s(.*)$/;
		my $msgdir = ${1};
		my $mmddyy = ${2};
		my $hhmmss = ${3};
		my $seqno = ${4};
		my $xxx = ${5};
		my $souptcppayload = ${6};
		#
		my $souptcptype = substr($souptcppayload, 0, 1);
		#
		if (exists($packettypes{$souptcptype})) {
			next if (!$allmsgs && $packettypes{$souptcptype}->{ignore});
			if ($msgdir eq 'R') {
				print "\n<<<<==== Receiving Msg <<<<====\n";
			} elsif ($msgdir eq 'S') {
				print "\n====>>>> Sending Msg ====>>>>\n";
			} else {
				next;
			}
			print "Date: $mmddyy $hhmmss\n";
			print "Seq No: $seqno\n";
			print "Soup TCP Packet Type: $packettypes{$souptcptype}->{name}\n";
			if ($verbose) {
				print "Soup TCP Payload: $souptcppayload\n";
			}
			my $handler = $packettypes{$souptcptype}->{handler};
			&{${handler}}($msgdir, $souptcppayload);
		} else {
			print "Date: $mmddyy $hhmmss\n";
			print "Seq No: $seqno\n";
			print "Soup TCP Payload: $souptcppayload\n";
			print "Unknown Soup TCP type: ($souptcptype)\n";
			print "WARNING: Skipping msg.\n";
		}
	 }
}
#
$verbose = 0;
my $allflag = 0;
my %opts;
if (getopts('?AV', \%opts) != 1) {
	usage();
	exit 2;
}
#
foreach my $opt (%opts) {
	if ($opt eq "?") {
		usage();
		exit 0;
	} elsif ($opt eq "V") {
		$verbose = 1;
	} elsif ($opt eq "A") {
		$allflag = 1;
	}
}
#
if ($#ARGV < 0) {
	parsefile(\*STDIN);
} else {
	foreach my $rashlogfile (@ARGV) {
		print "\nLOG FILE: $rashlogfile \n";
		open(INFD, "<".$rashlogfile) or 
			die "can't open $rashlogfile: $!";
		parsefile(\*INFD, $allflag);
		close(INFD);
	}
}
#
exit 0;



