# ISUP msg encoding/decoding class for encapsulating ISUP in SIP.
#
package myisup;
#
use Carp;
#
# parameter encoding/decoding functions
#
sub forwardnetworkspecificinformation {
	confess "barf: $!";
}
sub CLLIcode {
	confess "barf: $!";
}
sub accesstransport {
	confess "barf: $!";
}
sub applicationtransport {
	confess "barf: $!";
}
sub automaticcongestionlevel {
	confess "barf: $!";
}
sub backwardcallindicators {
	confess "barf: $!";
}
sub businessgroup {
	confess "barf: $!";
}
sub calledpartynumber {
	confess "barf: $!";
}
sub callinggeodeticlocation {
	confess "barf: $!";
}
sub callingpartynumber {
	confess "barf: $!";
}
sub callingpartyscategory {
	confess "barf: $!";
}
sub callmodificationindicators {
	confess "barf: $!";
}
sub callreference {
	confess "barf: $!";
}
sub carrierindication {
	confess "barf: $!";
}
sub carrierselectioninformation {
	confess "barf: $!";
}
sub carrierserviceprovideridentification {
	confess "barf: $!";
}
sub causeindicattors {
	confess "barf: $!";
}
sub chargenumber {
	confess "barf: $!";
}
sub circuitassignmentmap {
	confess "barf: $!";
}
sub circuitgroupcharacteristicindicator {
	confess "barf: $!";
}
sub circuitgroupsupervisionmessagetypeindicator {
	confess "barf: $!";
}
sub circuitidentificationname {
	confess "barf: $!";
}
sub circuitstateindicator {
	confess "barf: $!";
}
sub circuitvalidationresponseindicator {
	confess "barf: $!";
}
sub closedusergroupcheckresponseindicators {
	confess "barf: $!";
}
sub closedusergroupinterlockcode {
	confess "barf: $!";
}
sub commonlanguagelocationidentifiercode {
	confess "barf: $!";
}
sub connectednumber {
	confess "barf: $!";
}
sub connectionrequest {
	confess "barf: $!";
}
sub continuityindicators {
	confess "barf: $!";
}
sub egress {
	confess "barf: $!";
}
sub endofoptionalparameters {
	confess "barf: $!";
}
sub eventinformationindicators {
	confess "barf: $!";
}
sub facilityindicator {
	confess "barf: $!";
}
sub facilityinformationindicators {
	confess "barf: $!";
}
sub forwardcallindicators {
	confess "barf: $!";
}
sub genericaddress {
	confess "barf: $!";
}
sub genericdigits {
	confess "barf: $!";
}
sub genericname {
	confess "barf: $!";
}
sub hopcounter {
	confess "barf: $!";
}
sub index {
	confess "barf: $!";
}
sub informationindicators {
	confess "barf: $!";
}
sub informationrequestindicators {
	confess "barf: $!";
}
sub jurisdiction {
	confess "barf: $!";
}
sub localserviceprovideridentification {
	confess "barf: $!";
}
sub messagecompatibilityinformation {
	confess "barf: $!";
}
sub natureofconnectionindicators {
	confess "barf: $!";
}
sub networkmanagementcontrols {
	confess "barf: $!";
}
sub networkspecificfacility {
	confess "barf: $!";
}
sub networktransport {
	confess "barf: $!";
}
sub notificationindicator {
	confess "barf: $!";
}
sub operatorservicesinformation {
	confess "barf: $!";
}
sub optionalbackwardcallindicators {
	confess "barf: $!";
}
sub optionalforwardcallindicators {
	confess "barf: $!";
}
sub originalcallednumber {
	confess "barf: $!";
}
sub originatinglineinformation {
	confess "barf: $!";
}
sub outgoingtrunkgroupnumber {
	confess "barf: $!";
}
sub parametercompatibilityinformation {
	confess "barf: $!";
}
sub pivotcapabiltiy {
	confess "barf: $!";
}
sub pivotcounter {
	confess "barf: $!";
}
sub pivotroutingbackwardinformation {
	confess "barf: $!";
}
sub pivotroutingforwardinformation {
	confess "barf: $!";
}
sub pivotroutingindicator {
	confess "barf: $!";
}
sub pivotstatus {
	confess "barf: $!";
}
sub precedence {
	confess "barf: $!";
}
sub rangeandstatus {
	confess "barf: $!";
}
sub redirectbackwardinformation {
	confess "barf: $!";
}
sub redirectcapability {
	confess "barf: $!";
}
sub redirectcounter {
	confess "barf: $!";
}
sub redirectforwardinformation {
	confess "barf: $!";
}
sub redirectingnumber {
	confess "barf: $!";
}
sub redirectioninformation {
	confess "barf: $!";
}
sub redirectionnumber {
	confess "barf: $!";
}
sub redirectstatus {
	confess "barf: $!";
}
sub remoteoperations {
	confess "barf: $!";
}
sub serviceactivation {
	confess "barf: $!";
}
sub servicecodeindicator {
	confess "barf: $!";
}
sub signalingportcode {
	confess "barf: $!";
}
sub specialprocessingrequest {
	confess "barf: $!";
}
sub subsequentnumber {
	confess "barf: $!";
}
sub suspendresumeindicators {
	confess "barf: $!";
}
sub transactionrequest {
	confess "barf: $!";
}
sub transitnetworkselection {
	confess "barf: $!";
}
sub transmissionmediumrequirements {
	confess "barf: $!";
}
sub transmissionmediumused {
	confess "barf: $!";
}
sub usertouserindicators {
	confess "barf: $!";
}
sub usertouserinformation {
	confess "barf: $!";
}
sub userserviceinformation {
	confess "barf: $!";
}
sub userserviceinformationprime {
	confess "barf: $!";
}
#
# constructor
#
sub new {
	my $invoker = shift;
	my $class = ref($invoker) || $invoker;
	#
	my ($printfunc) = @_;
	#
	my $self = {
		print => $printfunc,
		ansi => {
			msgs => {
			0b00000001 => {
				name => "Initial Address (IAM)",
				fixed => [
					{
						code => 0b00000110,
						name => "nature of connection indicators",
						bytes => 1,
					},
					{
						code => 0b00000111,
						name => "forward call indicators",
						bytes => 2,
					},
					{
						code => 0b00001001,
						name => "calling partys category",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00011101,
						name => "user service information",
						minbytes => 3,
						maxbytes => 255,
					},
					{
						code => 0b00000100,
						name => "called party number",
						minbytes => 2,
						maxbytes => 11,
					},
				],
				optional => {
					0b00011101 => {
						name => "user service information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b10000001 => {
						name => "calling geodetic location",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11000101 => {
						name => "carrier identification",
						minbyte => 5,
						maxbyte => 5,
					},
					0b11101110 => {
						name => "carrier selection information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11100010 => {
						name => "carrier service provider identification",
						minbyte => 2,
						maxbyte => 255,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11000011 => {
						name => "egress service",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000000 => {
						name => "generic address",
						minbyte => 6,
						maxbyte => 13,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11000111 => {
						name => "generic name",
						minbyte => 3,
						maxbyte => 18,
					},
					0b00111101 => {
						name => "hop counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001110 => {
						name => "information request indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000100 => {
						name => "jurisdiction",
						minbyte => 2,
						maxbyte => 5,
					},
					0b11100100 => {
						name => "local service provider identification",
						minbyte => 2,
						maxbyte => 255,
					},
					0b01011011 => {
						name => "network management controls",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11000010 => {
						name => "operator services information",
						minbyte => 3,
						maxbyte => 8,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101000 => {
						name => "original called number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11101010 => {
						name => "originating line information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111011 => {
						name => "pivot capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111010 => {
						name => "precedence",
						minbyte => 8,
						maxbyte => 8,
					},
					0b01001110 => {
						name => "redirect capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001011 => {
						name => "redirect forward information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110011 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11101100 => {
						name => "service code indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11101101 => {
						name => "special processing request",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11100011 => {
						name => "transaction request",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00100011 => {
						name => "transit network selection",
						minbyte => 5,
						maxbyte => 6,
					},
					0b00110000 => {
						name => "user service information prime",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00000011 => {
				name => "Information Request (INR)",
				fixed => [
					{
						code => 0b00001110,
						name => "information request indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
				}
			},
			0b00000100 => {
				name => "Information (INF)",
				fixed => [
					{
						code => 0b00001111,
						name => "information indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00001001 => {
						name => "calling partys category",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11101010 => {
						name => "originating line information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00000101 => {
				name => "Continuity (COT)",
				fixed => [
					{
						code => 0b00010000,
						name => "continuity indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b00000110 => {
				name => "Address Complete (ACM)",
				fixed => [
					{
						code => 0b00010001,
						name => "backward call indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00000111 => {
				name => "Connect (CON)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00001000 => {
				name => "Forward Transfer (FOT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00001001 => {
				name => "Answer (ANM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00010001 => {
						name => "backward call indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00001100 => {
				name => "Release (REL)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 3,
						maxbytes => 255,
					},
				],
				optional => {
					000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00100111 => {
						name => "automatic congestion level",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b11000000 => {
						name => "generic address",
						minbyte => 6,
						maxbyte => 13,
					},
					0b10001100 => {
						name => "redirect backward information",
						minbyte => 5,
						maxbyte => 11,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00110011 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00001110 => {
				name => "Resume (RES)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010001 => {
				name => "Continuity Check Report (CCR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
				}
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011100 => {
				name => "Call Modification Request (CMR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011101 => {
				name => "Call Modification Completed (CMC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011110 => {
				name => "Call Modification Reject (CMRJ)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111100 => {
						name => "pivot routing indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
				}
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100001 => {
				name => "Facility Rejected (FRJ)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100010 => {
				name => "Facility Deactivated (FAD)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100011 => {
				name => "Facility Information (FAI)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100101 => {
				name => "Closed User Group Selection and validation Request i(CSVR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100110 => {
				name => "Closed User Group Selection and Validation Response (CSVS)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100111 => {
				name => "Delayed Release (DRS)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRA)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
				},
			},
			0b00101010 => {
				name => "Circuit Query (CQM)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
				},
			},
			0b00101011 => {
				name => "Circuit Query Response (CQR)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
					{
						code => 0b00100110,
						name => "circuit state indicator",
						minbyte => 2,
						maxbyte => 25,
					},
				],
				optional => {
				}
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				fixed => [
					{
						code => 0b00100100,
						name => "event information",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00010001 => {
						name => "backward call indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				fixed => [
					{
						code => 0b00100100,
						name => "event information",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110000 => {
				name => "Overload (OLM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110011 => {
				name => "Facility (FAC)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
				}
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b11101001 => {
				name => "Circuit Reservation Acknowledgement (CRA)",
				fixed => [
					{
						code => 0b00000110,
						name => "nature of connection indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101010 => {
				name => "Circuit Reservation (CRM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101011 => {
				name => "Circuit Validation Response (CVR)",
				fixed => [
					{
						code => 0b11100110,
						name => "circuit validation response indicator",
						bytes => 1,
					},
					{
						code => 0b11100101,
						name => "circuit group characteristic indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b11101000 => {
						name => "circuit identification name (sending end)",
						minbyte => 28,
						maxbyte => 28,
					},
					0b11101001 => {
						name => "common language location identification (CLLI) code (sending end)",
						minbyte => 13,
						maxbyte => 13,
					},
				}
			},
			0b11101100 => {
				name => "Circuit Validation Test (CVT)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101101 => {
				name => "Exit (EXM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
				}
			},
			},
			rawmsgs => {
			0b00000001 => {
				name => "Initial Address (IAM)",
				value => 0x0,
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				value => 0x0,
			},
			0b00000011 => {
				name => "Information Request (INR)",
				value => 0x0,
			},
			0b00000100 => {
				name => "Information (INF)",
				value => 0x0,
			},
			0b00000101 => {
				name => "Continuity (COT)",
				value => 0x0,
			},
			0b00000110 => {
				name => "Address Complete (ACM)",
				value => 0x0,
			},
			0b00000111 => {
				name => "Connect (CON)",
				value => 0x0,
			},
			0b00001000 => {
				name => "Forward Transfer (FOT)",
				value => 0x0,
			},
			0b00001001 => {
				name => "Answer (ANM)",
				value => 0x0,
			},
			0b00001100 => {
				name => "Release (REL)",
				value => 0x0,
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				value => 0x0,
			},
			0b00001110 => {
				name => "Resume (RES)",
				value => 0x0,
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				value => 0x0,
			},
			0b00010001 => {
				name => "Continuity Check Report (CCR)",
				value => 0x0,
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				value => 0x0,
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				value => 0x0,
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				value => 0x0,
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				value => 0x0,
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				value => 0x0,
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				value => 0x0,
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				value => 0x0,
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				value => 0x0,
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				value => 0x0,
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				value => 0x0,
			},
			0b00011100 => {
				name => "Call Modification Request (CMR)",
				value => 0x0,
			},
			0b00011101 => {
				name => "Call Modification Completed (CMC)",
				value => 0x0,
			},
			0b00011110 => {
				name => "Call Modification Reject (CMRJ)",
				value => 0x0,
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				value => 0x0,
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				value => 0x0,
			},
			0b00100001 => {
				name => "Facility Rejected (FRJ)",
				value => 0x0,
			},
			0b00100010 => {
				name => "Facility Deactivated (FAD)",
				value => 0x0,
			},
			0b00100011 => {
				name => "Facility Information (FAI)",
				value => 0x0,
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				value => 0x0,
			},
			0b00100101 => {
				name => "Closed User Group Selection and validation Request i(CSVR)",
				value => 0x0,
			},
			0b00100110 => {
				name => "Closed User Group Selection and Validation Response (CSVS)",
				value => 0x0,
			},
			0b00100111 => {
				name => "Delayed Release (DRS)",
				value => 0x0,
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				value => 0x0,
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRA)",
				value => 0x0,
			},
			0b00101010 => {
				name => "Circuit Query (CQM)",
				value => 0x0,
			},
			0b00101011 => {
				name => "Circuit Query Response (CQR)",
				value => 0x0,
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				value => 0x0,
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				value => 0x0,
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				value => 0x0,
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				value => 0x0,
			},
			0b00110000 => {
				name => "Overload (OLM)",
				value => 0x0,
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				value => 0x0,
			},
			0b00110011 => {
				name => "Facility (FAC)",
				value => 0x0,
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				value => 0x0,
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				value => 0x0,
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				value => 0x0,
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				value => 0x0,
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				value => 0x0,
			},
			0b11101001 => {
				name => "Circuit Reservation Acknowledgement (CRA)",
				value => 0x0,
			},
			0b11101010 => {
				name => "Circuit Reservation (CRM)",
				value => 0x0,
			},
			0b11101011 => {
				name => "Circuit Validation Response (CVR)",
				value => 0x0,
			},
			0b11101100 => {
				name => "Circuit Validation Test (CVT)",
				value => 0x0,
			},
			0b11101101 => {
				name => "Exit (EXM)",
				value => 0x0,
			},
			},
			msgids => {
			"Initial Address (IAM)" => 0b00000001,
			"Subsequent Address (SAM)" => 0b00000010,
			"Information Request (INR)" => 0b00000011,
			"Information (INF)" => 0b00000100,
			"Continuity (COT)" => 0b00000101,
			"Address Complete (ACM)" => 0b00000110,
			"Connect (CON)" => 0b00000111,
			"Forward Transfer (FOT)" => 0b00001000,
			"Answer (ANM)" => 0b00001001,
			"Release (REL)" => 0b00001100,
			"Suspend (SUS)" => 0b00001101,
			"Resume (RES)" => 0b00001110,
			"Release Complete (RLC)" => 0b00010000,
			"Continuity Check Report (CCR)" => 0b00010001,
			"Reset Circuit (RSC)" => 0b00010010,
			"Blocking (BLO)" => 0b00010011,
			"Unblocking (UBL)" => 0b00010100,
			"Blocking Acknowledgement (BLA)" => 0b00010101,
			"Unblocking Acknowledgement (UBA)" => 0b00010110,
			"Circuit Group Reset (GRS)" => 0b00010111,
			"Circuit Group Blocking (CGB)" => 0b00011000,
			"Circuit Group Unblocking (CGU)" => 0b00011001,
			"Circuit Group Blocking Acknowledgement (CGBA)" => 0b00011010,
			"Circuit Group Unblocking Acknowledgement (CGUA)" => 0b00011011,
			"Call Modification Request (CMR)" => 0b00011100,
			"Call Modification Completed (CMC)" => 0b00011101,
			"Call Modification Reject (CMRJ)" => 0b00011110,
			"Facility Request (FAR)" => 0b00011111,
			"Facility Accepted (FAA)" => 0b00100000,
			"Facility Rejected (FRJ)" => 0b00100001,
			"Facility Deactivated (FAD)" => 0b00100010,
			"Facility Information (FAI)" => 0b00100011,
			"Loop Back Acknowledgement (LPA)" => 0b00100100,
			"Closed User Group Selection and validation Request i(CSVR)" => 0b00100101,
			"Closed User Group Selection and Validation Response (CSVS)" => 0b00100110,
			"Delayed Release (DRS)" => 0b00100111,
			"Pass Along (PAM)" => 0b00101000,
			"Circuit Group Reset Acknowledgement (GRA)" => 0b00101001,
			"Circuit Query (CQM)" => 0b00101010,
			"Circuit Query Response (CQR)" => 0b00101011,
			"Call Progress (CPG)" => 0b00101100,
			"User-to-user Information (USR)" => 0b00101101,
			"Unequipped Circuit Identification Code (UCIC)" => 0b00101110,
			"Confusion (CFN)" => 0b00101111,
			"Overload (OLM)" => 0b00110000,
			"Network Resource Management (NRM)" => 0b00110010,
			"Facility (FAC)" => 0b00110011,
			"User Part Test (UPT)" => 0b00110100,
			"User Part Available (UPA)" => 0b00110101,
			"Segmentation (SEG)" => 0b00111000,
			"Application Transport (APT)" => 0b01000001,
			"Pre-Release Information (PRI)" => 0b01000010,
			"Circuit Reservation Acknowledgement (CRA)" => 0b11101001,
			"Circuit Reservation (CRM)" => 0b11101010,
			"Circuit Validation Response (CVR)" => 0b11101011,
			"Circuit Validation Test (CVT)" => 0b11101100,
			"Exit (EXM)" => 0b11101101,
			},
			parameters => {
				"end[\\s]*of[\\s]*optional[\\s]*parameters" => {
					id => b00000000,
					codec => \&endofoptionalparameters
				},
				"call[\\s]*reference" => {
					id => b00000001,
					codec => \&callreference
				},
				"transmission[\\s]*medium[\\s]*requirements" => {
					id => b00000010,
					codec => \&transmissionmediumrequirements
				},
				"access[\\s]*transport" => {
					id => b00000011,
					codec => \&accesstransport
				},
				"called[\\s]*party[\\s]*number" => {
					id => b00000100,
					codec => \&calledpartynumber
				},
				"subsequent[\\s]*number" => {
					id => b00000101,
					codec => \&subsequentnumber
				},
				"nature[\\s]*of[\\s]*connection[\\s]*indicators" => {
					id => b00000110,
					codec => \&natureofconnectionindicators
				},
				"forward[\\s]*call[\\s]*indicators" => {
					id => b00000111,
					codec => \&forwardcallindicators
				},
				"optional[\\s]*forward[\\s]*call[\\s]*indicators" => {
					id => b00001000,
					codec => \&optionalforwardcallindicators
				},
				"calling[\\s]*partys[\\s]*category" => {
					id => b00001001,
					codec => \&callingpartyscategory
				},
				"calling[\\s]*party[\\s]*number" => {
					id => b00001010,
					codec => \&callingpartynumber
				},
				"redirecting[\\s]*number" => {
					id => b00001011,
					codec => \&redirectingnumber
				},
				"redirection[\\s]*number" => {
					id => b00001100,
					codec => \&redirectionnumber
				},
				"connection[\\s]*request" => {
					id => b00001101,
					codec => \&connectionrequest
				},
				"information[\\s]*request[\\s]*indicators" => {
					id => b00001110,
					codec => \&informationrequestindicators
				},
				"information[\\s]*indicators" => {
					id => b00001111,
					codec => \&informationindicators
				},
				"continuity[\\s]*indicators" => {
					id => b00010000,
					codec => \&continuityindicators
				},
				"backward[\\s]*call[\\s]*indicators" => {
					id => b00010001,
					codec => \&backwardcallindicators
				},
				"cause[\\s]*indicattors" => {
					id => b00010010,
					codec => \&causeindicattors
				},
				"redirection[\\s]*information" => {
					id => b00010011,
					codec => \&redirectioninformation
				},
				"circuit[\\s]*group[\\s]*supervision[\\s]*message[\\s]*type[\\s]*indicator" => {
					id => b00010101,
					codec => \&circuitgroupsupervisionmessagetypeindicator
				},
				"range[\\s]*and[\\s]*status" => {
					id => b00010110,
					codec => \&rangeandstatus
				},
				"call[\\s]*modification[\\s]*indicators" => {
					id => b00010111,
					codec => \&callmodificationindicators
				},
				"facility[\\s]*indicator" => {
					id => b00011000,
					codec => \&facilityindicator
				},
				"facility[\\s]*information[\\s]*indicators" => {
					id => b00011001,
					codec => \&facilityinformationindicators
				},
				"closed[\\s]*user[\\s]*groupinterlock[\\s]*code" => {
					id => b00011010,
					codec => \&closedusergroupinterlockcode
				},
				"index" => {
					id => b00011011,
					codec => \&index
				},
				"closed[\\s]*user[\\s]*group[\\s]*check[\\s]*response[\\s]*indicators" => {
					id => b00011100,
					codec => \&closedusergroupcheckresponseindicators
				},
				"user[\\s]*service[\\s]*information" => {
					id => b00011101,
					codec => \&userserviceinformation
				},
				"signaling[\\s]*port[\\s]*code" => {
					id => b00011110,
					codec => \&signalingportcode
				},
				"user-to-user[\\s]*information" => {
					id => b00100000,
					codec => \&usertouserinformation
				},
				"connected[\\s]*number" => {
					id => b00100001,
					codec => \&connectednumber
				},
				"suspend[\\s]*resume[\\s]*indicators" => {
					id => b00100010,
					codec => \&suspendresumeindicators
				},
				"transit[\\s]*network[\\s]*selection" => {
					id => b00100011,
					codec => \&transitnetworkselection
				},
				"event[\\s]*information[\\s]*indicators" => {
					id => b00100100,
					codec => \&eventinformationindicators
				},
				"circuit[\\s]*assignment[\\s]*map" => {
					id => b00100101,
					codec => \&circuitassignmentmap
				},
				"circuit[\\s]*state[\\s]*indicator" => {
					id => b00100110,
					codec => \&circuitstateindicator
				},
				"automatic[\\s]*congestion[\\s]*level" => {
					id => b00100111,
					codec => \&automaticcongestionlevel
				},
				"original[\\s]*called[\\s]*number" => {
					id => b00101000,
					codec => \&originalcallednumber
				},
				"optional[\\s]*backward[\\s]*call[\\s]*indicators" => {
					id => b00101001,
					codec => \&optionalbackwardcallindicators
				},
				"user-to-user[\\s]*indicators" => {
					id => b00101010,
					codec => \&usertouserindicators
				},
				"network[\\s]*specific[\\s]*facility" => {
					id => b00101111,
					codec => \&networkspecificfacility
				},
				"user[\\s]*service[\\s]*information[\\s]*prime" => {
					id => b00110000,
					codec => \&userserviceinformationprime
				},
				"remote[\\s]*operations" => {
					id => b00110010,
					codec => \&remoteoperations
				},
				"service[\\s]*activation" => {
					id => b00110011,
					codec => \&serviceactivation
				},
				"transmission[\\s]*medium[\\s]*used" => {
					id => b00110101,
					codec => \&transmissionmediumused
				},
				"message[\\s]*compatibility[\\s]*information" => {
					id => b00111000,
					codec => \&messagecompatibilityinformation
				},
				"parameter[\\s]*compatibility[\\s]*information" => {
					id => b00111001,
					codec => \&parametercompatibilityinformation
				},
				"precedence" => {
					id => b00111010,
					codec => \&precedence
				},
				"hop[\\s]*counter" => {
					id => b00111101,
					codec => \&hopcounter
				},
				"redirect[\\s]*capability" => {
					id => b01001110,
					codec => \&redirectcapability
				},
				"network[\\s]*management[\\s]*controls" => {
					id => b01011011,
					codec => \&networkmanagementcontrols
				},
				"redirect[\\s]*counter" => {
					id => b01110111,
					codec => \&redirectcounter
				},
				"application[\\s]*transport" => {
					id => b01111000,
					codec => \&applicationtransport
				},
				"pivot[\\s]*capabiltiy" => {
					id => b01111011,
					codec => \&pivotcapabiltiy
				},
				"pivot[\\s]*routing[\\s]*indicator" => {
					id => b01111100,
					codec => \&pivotroutingindicator
				},
				"calling[\\s]*geodetic[\\s]*location" => {
					id => b10000001,
					codec => \&callinggeodeticlocation
				},
				"pivot[\\s]*status" => {
					id => b10000110,
					codec => \&pivotstatus
				},
				"pivot[\\s]*counter" => {
					id => b10000111,
					codec => \&pivotcounter
				},
				"pivot[\\s]*routing[\\s]*forward[\\s]*information" => {
					id => b10001000,
					codec => \&pivotroutingforwardinformation
				},
				"pivot[\\s]*routing[\\s]*backward[\\s]*information" => {
					id => b10001001,
					codec => \&pivotroutingbackwardinformation
				},
				"redirect[\\s]*status" => {
					id => b10001010,
					codec => \&redirectstatus
				},
				"redirect[\\s]*forward[\\s]*information" => {
					id => b10001011,
					codec => \&redirectforwardinformation
				},
				"redirect[\\s]*backward[\\s]*information" => {
					id => b10001100,
					codec => \&redirectbackwardinformation
				},
				"generic[\\s]*address" => {
					id => b11000000,
					codec => \&genericaddress
				},
				"generic[\\s]*digits" => {
					id => b11000001,
					codec => \&genericdigits
				},
				"operator[\\s]*services[\\s]*information" => {
					id => b11000010,
					codec => \&operatorservicesinformation
				},
				"egress" => {
					id => b11000011,
					codec => \&egress
				},
				"jurisdiction" => {
					id => b11000100,
					codec => \&jurisdiction
				},
				"carrier[\\s]*indication" => {
					id => b11000101,
					codec => \&carrierindication
				},
				"business[\\s]*group" => {
					id => b11000110,
					codec => \&businessgroup
				},
				"generic[\\s]*name" => {
					id => b11000111,
					codec => \&genericname
				},
				"notification[\\s]*indicator" => {
					id => b11100001,
					codec => \&notificationindicator
				},
				"carrier[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100010,
					codec => \&carrierserviceprovideridentification
				},
				"transaction[\\s]*request" => {
					id => b11100011,
					codec => \&transactionrequest
				},
				"local[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100100,
					codec => \&localserviceprovideridentification
				},
				"circuit[\\s]*group[\\s]*characteristic[\\s]*indicator" => {
					id => b11100101,
					codec => \&circuitgroupcharacteristicindicator
				},
				"circuit[\\s]*validation[\\s]*response[\\s]*indicator" => {
					id => b11100110,
					codec => \&circuitvalidationresponseindicator
				},
				"outgoing[\\s]*trunk[\\s]*group[\\s]*number" => {
					id => b11100111,
					codec => \&outgoingtrunkgroupnumber
				},
				"circuit[\\s]*identification[\\s]*name" => {
					id => b11101000,
					codec => \&circuitidentificationname
				},
				"CLLI[\\s]*code" => {
					id => b11101001,
					codec => \&CLLIcode
				},
				"common[\\s]*language[\\s]*location[\\s]*identifier[\\s]*code" => {
					id => b11101001,
					codec => \&commonlanguagelocationidentifiercode
				},
				"originating[\\s]*line[\\s]*information" => {
					id => b11101010,
					codec => \&originatinglineinformation
				},
				"charge[\\s]*number" => {
					id => b11101011,
					codec => \&chargenumber
				},
				"service[\\s]*code[\\s]*indicator" => {
					id => b11101100,
					codec => \&servicecodeindicator
				},
				"special[\\s]*processing[\\s]*request" => {
					id => b11101101,
					codec => \&specialprocessingrequest
				},
				"carrier[\\s]*selection[\\s]*information" => {
					id => b11101110,
					codec => \&carrierselectioninformation
				},
				"network[\\s]*transport" => {
					id => b11101111,
					codec => \&networktransport
				},
			},
		},
		ansi00 => {
			msgs => {
			0b00000001 => {
				name => "Initial Address (IAM)",
				fixed => [
					{
						code => 0b00000110,
						name => "nature of connection indicators",
						bytes => 1,
					},
					{
						code => 0b00000111,
						name => "forward call indicators",
						bytes => 2,
					},
					{
						code => 0b00001001,
						name => "calling partys category",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00011101,
						name => "user service information",
						minbytes => 3,
						maxbytes => 255,
					},
					{
						code => 0b00000100,
						name => "called party number",
						minbytes => 4,
						maxbytes => 11,
					},
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11110001 => {
						name => "alliance screening",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b10000001 => {
						name => "calling geodetic location",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11000101 => {
						name => "carrier identification",
						minbyte => 5,
						maxbyte => 5,
					},
					0b11101110 => {
						name => "carrier selection information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11100010 => {
						name => "carrier service provider identification",
						minbyte => 2,
						maxbyte => 255,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b00011010 => {
						name => "closed user group interlock code",
						minbyte => 6,
						maxbyte => 6,
					},
					0b11110010 => {
						name => "dialed number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11111100 => {
						name => "DNHR indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000011 => {
						name => "egress service",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11111000 => {
						name => "forward network specific information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000000 => {
						name => "generic address",
						minbyte => 6,
						maxbyte => 13,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11000111 => {
						name => "generic name",
						minbyte => 3,
						maxbyte => 18,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111101 => {
						name => "hop counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001110 => {
						name => "information request indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000100 => {
						name => "jurisdiction",
						minbyte => 2,
						maxbyte => 5,
					},
					0b11100100 => {
						name => "local service provider identification",
						minbyte => 2,
						maxbyte => 255,
					},
					0b00111010 => {
						name => "MLPP precedence",
						minbyte => 6,
						maxbyte => 6,
					},
					0b01011011 => {
						name => "network management controls",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11111101 => {
						name => "network management information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11110110 => {
						name => "network switch number",
						minbyte => 5,
						maxbyte => 5,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11000010 => {
						name => "operator services information",
						minbyte => 3,
						maxbyte => 8,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101000 => {
						name => "original called number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11101010 => {
						name => "originating line information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111011 => {
						name => "pivot capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01001110 => {
						name => "redirect capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001011 => {
						name => "redirect forward information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b11111010 => {
						name => "routing information indicator",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110011 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11101100 => {
						name => "service code indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11101101 => {
						name => "special processing request",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11100011 => {
						name => "transaction request",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00100011 => {
						name => "transit network selection",
						minbyte => 5,
						maxbyte => 6,
					},
					0b00110000 => {
						name => "user service information prime",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00000011 => {
				name => "Information Request (INR)",
				fixed => [
					{
						code => 0b00001110,
						name => "information request indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
				}
			},
			0b00000100 => {
				name => "Information (INF)",
				fixed => [
					{
						code => 0b00001111,
						name => "information indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001001 => {
						name => "calling partys category",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00100001 => {
						name => "connected number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11101010 => {
						name => "originating line information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00000101 => {
				name => "Continuity (COT)",
				fixed => [
					{
						code => 0b00010000,
						name => "continuity indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b00000110 => {
				name => "Address Complete (ACM)",
				fixed => [
					{
						code => 0b00010001,
						name => "backward call indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 131,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00100111 => {
						name => "automatic congestion control level",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11110101 => {
						name => "hard-to-reach indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11110110 => {
						name => "network switch number",
						minbyte => 2,
						maxbyte => 2,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00000111 => {
				name => "Connect (CON)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00001000 => {
				name => "Forward Transfer (FOT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00001001 => {
				name => "Answer (ANM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 131,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00010001 => {
						name => "backward call indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b01001101 => {
						name => "backward GVNS",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00100001 => {
						name => "connected number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00001100 => {
				name => "Release (REL)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 3,
						maxbytes => 255,
					},
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11110111 => {
						name => "announcement indicator",
						minbyte => 1,
						maxbyte => 1,
					},
					0b00100111 => {
						name => "automatic congestion level",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b00011010 => {
						name => "closed user group interlock code",
						minbyte => 6,
						maxbyte => 6,
					},
					0b11000000 => {
						name => "generic address",
						minbyte => 6,
						maxbyte => 13,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b11110101 => {
						name => "hard-to-reach indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001100 => {
						name => "redirect backward information",
						minbyte => 5,
						maxbyte => 11,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00011110 => {
						name => "signaling point code",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110011 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				},
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00001110 => {
				name => "Resume (RES)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
				}
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
				}
			},
			0b00010001 => {
				name => "Continuity Check Report (CCR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
				}
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011100 => {
				name => "Call Modification Request (CMR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011101 => {
				name => "Call Modification Completed (CMC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011110 => {
				name => "Call Modification Reject (CMRJ)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				fixed => [
					{
						code => 0b00011000,
						name => "facility indicator",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 131,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00000100 => {
						name => "called party number",
						minbytes => 5,
						maxbytes => 12,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100001 => {
				name => "Facility Rejected (FRJ)",
				fixed => [
					{
						code => 0b00011000,
						name => "facility indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00000100 => {
						name => "called party number",
						minbytes => 5,
						maxbytes => 12,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00100010 => {
				name => "Facility Deactivated (FAD)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100011 => {
				name => "Facility Information (FAI)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100101 => {
				name => "Closed User Group Selection and validation Request i(CSVR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100110 => {
				name => "Closed User Group Selection and Validation Response (CSVS)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00100111 => {
				name => "Delayed Release (DRS)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRA)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 6,
					},
				},
			},
			0b00101010 => {
				name => "Circuit Query (CQM)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				},
			},
			0b00101011 => {
				name => "Circuit Query Response (CQR)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
					{
						code => 0b00100110,
						name => "circuit state indicator",
						minbyte => 2,
						maxbyte => 25,
					},
				],
				optional => {
				}
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				fixed => [
					{
						code => 0b00100100,
						name => "event information",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 131,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00010001 => {
						name => "backward call indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000110 => {
						name => "business group",
						minbyte => 9,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11110100 => {
						name => "generic operation",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00001111 => {
						name => "information indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11110110 => {
						name => "network switch number",
						minbyte => 2,
						maxbyte => 2,
					},
					0b11101111 => {
						name => "network transport",
						minbyte => 6,
						maxbyte => 255,
					},
					0b11100001 => {
						name => "notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				fixed => [
					{
						code => 0b00100100,
						name => "event information",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
				],
				optional => {
				}
			},
			0b00110000 => {
				name => "Overload (OLM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110001 => {
				name => "Charge Information (CHGI)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110011 => {
				name => "Facility (FAC)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 1,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
				}
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110110 => {
				name => "Identification Request (IRQ)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110111 => {
				name => "Identification Response (IRS)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b01000000 => {
				name => "Loop Prevention (LPR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b01000011 => {
				name => "Subsequent Directory Number (SDN)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101001 => {
				name => "Circuit Reservation Acknowledgement (CRA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101010 => {
				name => "Circuit Reservation (CRM)",
				fixed => [
					{
						code => 0b00000110,
						name => "nature of connection indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101011 => {
				name => "Circuit Validation Response (CVR)",
				fixed => [
					{
						code => 0b11100110,
						name => "circuit validation response indicator",
						bytes => 1,
					},
					{
						code => 0b11100101,
						name => "circuit group characteristic indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b11101000 => {
						name => "circuit identification name (sending end)",
						minbyte => 28,
						maxbyte => 28,
					},
					0b11101001 => {
						name => "common language location identification (CLLI) code (sending end)",
						minbyte => 13,
						maxbyte => 13,
					},
				}
			},
			0b11101100 => {
				name => "Circuit Validation Test (CVT)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11101101 => {
				name => "Exit (EXM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b11100111 => {
						name => "outgoing trunk group number",
						minbyte => 3,
						maxbyte => 5,
					},
				}
			},
			0b11110000 => {
				name => "Crankback (CRB)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b11110101 => {
						name => "hard-to-reach indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100111 => {
						name => "automatic congestion level",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b11110001 => {
				name => "Embedded Pass Along (EPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11110010 => {
				name => "Second Start Dial (SSD)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b11110011 => {
				name => "Virtual Connection Request (VCR)",
				fixed => [
					{
						code => 0b00000111,
						name => "forward call indicators",
						bytes => 2,
					},
					{
						code => 0b00001001,
						name => "calling partys category",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00001101,
						name => "connection request",
						minbyte => 9,
						maxbyte => 10,
					},
					{
						code => 0b00000100,
						name => "called party number",
						minbytes => 4,
						maxbytes => 11,
					},
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 8,
						maxbyte => 8,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11101011 => {
						name => "charge number",
						minbyte => 3,
						maxbyte => 12,
					},
					0b11110010 => {
						name => "dialed number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b11111100 => {
						name => "DNHR indicators",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11111101 => {
						name => "network management information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11110110 => {
						name => "network switch number",
						minbyte => 5,
						maxbyte => 5,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11111010 => {
						name => "routing information indicator",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b11110100 => {
				name => "RSVP (RSVP)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b11110101 => {
				name => "RSVP Reply (RSVR)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
				}
			},
			0b11111101 => {
				name => "SDN SID/ANI Increment (SSAI)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			},
			rawmsgs => {
			0b00000001 => {
				name => "Initial Address (IAM)",
				value => 0x0,
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				value => 0x0,
			},
			0b00000011 => {
				name => "Information Request (INR)",
				value => 0x0,
			},
			0b00000100 => {
				name => "Information (INF)",
				value => 0x0,
			},
			0b00000101 => {
				name => "Continuity (COT)",
				value => 0x0,
			},
			0b00000110 => {
				name => "Address Complete (ACM)",
				value => 0x0,
			},
			0b00000111 => {
				name => "Connect (CON)",
				value => 0x0,
			},
			0b00001000 => {
				name => "Forward Transfer (FOT)",
				value => 0x0,
			},
			0b00001001 => {
				name => "Answer (ANM)",
				value => 0x0,
			},
			0b00001100 => {
				name => "Release (REL)",
				value => 0x0,
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				value => 0x0,
			},
			0b00001110 => {
				name => "Resume (RES)",
				value => 0x0,
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				value => 0x0,
			},
			0b00010001 => {
				name => "Continuity Check Report (CCR)",
				value => 0x0,
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				value => 0x0,
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				value => 0x0,
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				value => 0x0,
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				value => 0x0,
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				value => 0x0,
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				value => 0x0,
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				value => 0x0,
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				value => 0x0,
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				value => 0x0,
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				value => 0x0,
			},
			0b00011100 => {
				name => "Call Modification Request (CMR)",
				value => 0x0,
			},
			0b00011101 => {
				name => "Call Modification Completed (CMC)",
				value => 0x0,
			},
			0b00011110 => {
				name => "Call Modification Reject (CMRJ)",
				value => 0x0,
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				value => 0x0,
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				value => 0x0,
			},
			0b00100001 => {
				name => "Facility Rejected (FRJ)",
				value => 0x0,
			},
			0b00100010 => {
				name => "Facility Deactivated (FAD)",
				value => 0x0,
			},
			0b00100011 => {
				name => "Facility Information (FAI)",
				value => 0x0,
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				value => 0x0,
			},
			0b00100101 => {
				name => "Closed User Group Selection and validation Request i(CSVR)",
				value => 0x0,
			},
			0b00100110 => {
				name => "Closed User Group Selection and Validation Response (CSVS)",
				value => 0x0,
			},
			0b00100111 => {
				name => "Delayed Release (DRS)",
				value => 0x0,
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				value => 0x0,
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRA)",
				value => 0x0,
			},
			0b00101010 => {
				name => "Circuit Query (CQM)",
				value => 0x0,
			},
			0b00101011 => {
				name => "Circuit Query Response (CQR)",
				value => 0x0,
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				value => 0x0,
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				value => 0x0,
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				value => 0x0,
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				value => 0x0,
			},
			0b00110000 => {
				name => "Overload (OLM)",
				value => 0x0,
			},
			0b00110001 => {
				name => "Charge Information (CHGI)",
				value => 0x0,
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				value => 0x0,
			},
			0b00110011 => {
				name => "Facility (FAC)",
				value => 0x0,
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				value => 0x0,
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				value => 0x0,
			},
			0b00110110 => {
				name => "Identification Request (IRQ)",
				value => 0x0,
			},
			0b00110111 => {
				name => "Identification Response (IRS)",
				value => 0x0,
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				value => 0x0,
			},
			0b01000000 => {
				name => "Loop Prevention (LPR)",
				value => 0x0,
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				value => 0x0,
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				value => 0x0,
			},
			0b01000011 => {
				name => "Subsequent Directory Number (SDN)",
				value => 0x0,
			},
			0b11101001 => {
				name => "Circuit Reservation Acknowledgement (CRA)",
				value => 0x0,
			},
			0b11101010 => {
				name => "Circuit Reservation (CRM)",
				value => 0x0,
			},
			0b11101011 => {
				name => "Circuit Validation Response (CVR)",
				value => 0x0,
			},
			0b11101100 => {
				name => "Circuit Validation Test (CVT)",
			},
			0b11101101 => {
				name => "Exit (EXM)",
				value => 0x0,
			},
			0b11110000 => {
				name => "Crankback (CRB)",
				value => 0x0,
			},
			0b11110001 => {
				name => "Embedded Pass Along (EPA)",
				value => 0x0,
			},
			0b11110010 => {
				name => "Second Start Dial (SSD)",
				value => 0x0,
			},
			0b11110011 => {
				name => "Virtual Connection Request (VCR)",
				value => 0x0,
			},
			0b11110100 => {
				name => "RSVP (RSVP)",
				value => 0x0,
			},
			0b11110101 => {
				name => "RSVP Reply (RSVR)",
				value => 0x0,
			},
			0b11111101 => {
				name => "SDN SID/ANI Increment (SSAI)",
				value => 0x0,
			},
			},
			msgids => {
			"Initial Address (IAM)" => 0b00000001,
			"Subsequent Address (SAM)" => 0b00000010,
			"Information Request (INR)" => 0b00000011,
			"Information (INF)" => 0b00000100,
			"Continuity (COT)" => 0b00000101,
			"Address Complete (ACM)" => 0b00000110,
			"Connect (CON)" => 0b00000111,
			"Forward Transfer (FOT)" => 0b00001000,
			"Answer (ANM)" => 0b00001001,
			"Release (REL)" => 0b00001100,
			"Suspend (SUS)" => 0b00001101,
			"Resume (RES)" => 0b00001110,
			"Release Complete (RLC)" => 0b00010000,
			"Continuity Check Report (CCR)" => 0b00010001,
			"Reset Circuit (RSC)" => 0b00010010,
			"Blocking (BLO)" => 0b00010011,
			"Unblocking (UBL)" => 0b00010100,
			"Blocking Acknowledgement (BLA)" => 0b00010101,
			"Unblocking Acknowledgement (UBA)" => 0b00010110,
			"Circuit Group Reset (GRS)" => 0b00010111,
			"Circuit Group Blocking (CGB)" => 0b00011000,
			"Circuit Group Unblocking (CGU)" => 0b00011001,
			"Circuit Group Blocking Acknowledgement (CGBA)" => 0b00011010,
			"Circuit Group Unblocking Acknowledgement (CGUA)" => 0b00011011,
			"Call Modification Request (CMR)" => 0b00011100,
			"Call Modification Completed (CMC)" => 0b00011101,
			"Call Modification Reject (CMRJ)" => 0b00011110,
			"Facility Request (FAR)" => 0b00011111,
			"Facility Accepted (FAA)" => 0b00100000,
			"Facility Rejected (FRJ)" => 0b00100001,
			"Facility Deactivated (FAD)" => 0b00100010,
			"Facility Information (FAI)" => 0b00100011,
			"Loop Back Acknowledgement (LPA)" => 0b00100100,
			"Closed User Group Selection and validation Request i(CSVR)" => 0b00100101,
			"Closed User Group Selection and Validation Response (CSVS)" => 0b00100110,
			"Delayed Release (DRS)" => 0b00100111,
			"Pass Along (PAM)" => 0b00101000,
			"Circuit Group Reset Acknowledgement (GRA)" => 0b00101001,
			"Circuit Query (CQM)" => 0b00101010,
			"Circuit Query Response (CQR)" => 0b00101011,
			"Call Progress (CPG)" => 0b00101100,
			"User-to-user Information (USR)" => 0b00101101,
			"Unequipped Circuit Identification Code (UCIC)" => 0b00101110,
			"Confusion (CFN)" => 0b00101111,
			"Overload (OLM)" => 0b00110000,
			"Charge Information (CHGI)" => 0b00110001,
			"Network Resource Management (NRM)" => 0b00110010,
			"Facility (FAC)" => 0b00110011,
			"User Part Test (UPT)" => 0b00110100,
			"User Part Available (UPA)" => 0b00110101,
			"Identification Request (IRQ)" => 0b00110110,
			"Identification Response (IRS)" => 0b00110111,
			"Segmentation (SEG)" => 0b00111000,
			"Loop Prevention (LPR)" => 0b01000000,
			"Application Transport (APT)" => 0b01000001,
			"Pre-Release Information (PRI)" => 0b01000010,
			"Subsequent Directory Number (SDN)" => 0b01000011,
			"Circuit Reservation Acknowledgement (CRA)" => 0b11101001,
			"Circuit Reservation (CRM)" => 0b11101010,
			"Circuit Validation Response (CVR)" => 0b11101011,
			"Circuit Validation Test (CVT)" => 0b11101100,
			"Exit (EXM)" => 0b11101101,
			"Crankback (CRB)" => 0b11110000,
			"Embedded Pass Along (EPA)" => 0b11110001,
			"Second Start Dial (SSD)" => 0b11110010,
			"Virtual Connection Request (VCR)" => 0b11110011,
			"RSVP (RSVP)" => 0b11110100,
			"RSVP Reply (RSVR)" => 0b11110101,
			"SDN SID/ANI Increment (SSAI)" => 0b11111101,
			},
			parameters => {
				"forward[\\s]*network[\\s]*specific[\\s]*information" => {
					id => 0b11111000,
					codec => \&forwardnetworkspecificinformation
				},
				"end[\\s]*of[\\s]*optional[\\s]*parameters" => {
					id => b00000000,
					codec => \&endofoptionalparameters
				},
				"call[\\s]*reference" => {
					id => b00000001,
					codec => \&callreference
				},
				"transmission[\\s]*medium[\\s]*requirements" => {
					id => b00000010,
					codec => \&transmissionmediumrequirements
				},
				"access[\\s]*transport" => {
					id => b00000011,
					codec => \&accesstransport
				},
				"called[\\s]*party[\\s]*number" => {
					id => b00000100,
					codec => \&calledpartynumber
				},
				"subsequent[\\s]*number" => {
					id => b00000101,
					codec => \&subsequentnumber
				},
				"nature[\\s]*of[\\s]*connection[\\s]*indicators" => {
					id => b00000110,
					codec => \&natureofconnectionindicators
				},
				"forward[\\s]*call[\\s]*indicators" => {
					id => b00000111,
					codec => \&forwardcallindicators
				},
				"optional[\\s]*forward[\\s]*call[\\s]*indicators" => {
					id => b00001000,
					codec => \&optionalforwardcallindicators
				},
				"calling[\\s]*partys[\\s]*category" => {
					id => b00001001,
					codec => \&callingpartyscategory
				},
				"calling[\\s]*party[\\s]*number" => {
					id => b00001010,
					codec => \&callingpartynumber
				},
				"redirecting[\\s]*number" => {
					id => b00001011,
					codec => \&redirectingnumber
				},
				"redirection[\\s]*number" => {
					id => b00001100,
					codec => \&redirectionnumber
				},
				"connection[\\s]*request" => {
					id => b00001101,
					codec => \&connectionrequest
				},
				"information[\\s]*request[\\s]*indicators" => {
					id => b00001110,
					codec => \&informationrequestindicators
				},
				"information[\\s]*indicators" => {
					id => b00001111,
					codec => \&informationindicators
				},
				"continuity[\\s]*indicators" => {
					id => b00010000,
					codec => \&continuityindicators
				},
				"backward[\\s]*call[\\s]*indicators" => {
					id => b00010001,
					codec => \&backwardcallindicators
				},
				"cause[\\s]*indicattors" => {
					id => b00010010,
					codec => \&causeindicattors
				},
				"redirection[\\s]*information" => {
					id => b00010011,
					codec => \&redirectioninformation
				},
				"circuit[\\s]*group[\\s]*supervision[\\s]*message[\\s]*type[\\s]*indicator" => {
					id => b00010101,
					codec => \&circuitgroupsupervisionmessagetypeindicator
				},
				"range[\\s]*and[\\s]*status" => {
					id => b00010110,
					codec => \&rangeandstatus
				},
				"call[\\s]*modification[\\s]*indicators" => {
					id => b00010111,
					codec => \&callmodificationindicators
				},
				"facility[\\s]*indicator" => {
					id => b00011000,
					codec => \&facilityindicator
				},
				"facility[\\s]*information[\\s]*indicators" => {
					id => b00011001,
					codec => \&facilityinformationindicators
				},
				"closed[\\s]*user[\\s]*groupinterlock[\\s]*code" => {
					id => b00011010,
					codec => \&closedusergroupinterlockcode
				},
				"index" => {
					id => b00011011,
					codec => \&index
				},
				"closed[\\s]*user[\\s]*group[\\s]*check[\\s]*response[\\s]*indicators" => {
					id => b00011100,
					codec => \&closedusergroupcheckresponseindicators
				},
				"user[\\s]*service[\\s]*information" => {
					id => b00011101,
					codec => \&userserviceinformation
				},
				"signaling[\\s]*port[\\s]*code" => {
					id => b00011110,
					codec => \&signalingportcode
				},
				"user-to-user[\\s]*information" => {
					id => b00100000,
					codec => \&usertouserinformation
				},
				"connected[\\s]*number" => {
					id => b00100001,
					codec => \&connectednumber
				},
				"suspend[\\s]*resume[\\s]*indicators" => {
					id => b00100010,
					codec => \&suspendresumeindicators
				},
				"transit[\\s]*network[\\s]*selection" => {
					id => b00100011,
					codec => \&transitnetworkselection
				},
				"event[\\s]*information[\\s]*indicators" => {
					id => b00100100,
					codec => \&eventinformationindicators
				},
				"circuit[\\s]*assignment[\\s]*map" => {
					id => b00100101,
					codec => \&circuitassignmentmap
				},
				"circuit[\\s]*state[\\s]*indicator" => {
					id => b00100110,
					codec => \&circuitstateindicator
				},
				"automatic[\\s]*congestion[\\s]*level" => {
					id => b00100111,
					codec => \&automaticcongestionlevel
				},
				"original[\\s]*called[\\s]*number" => {
					id => b00101000,
					codec => \&originalcallednumber
				},
				"optional[\\s]*backward[\\s]*call[\\s]*indicators" => {
					id => b00101001,
					codec => \&optionalbackwardcallindicators
				},
				"user-to-user[\\s]*indicators" => {
					id => b00101010,
					codec => \&usertouserindicators
				},
				"network[\\s]*specific[\\s]*facility" => {
					id => b00101111,
					codec => \&networkspecificfacility
				},
				"user[\\s]*service[\\s]*information[\\s]*prime" => {
					id => b00110000,
					codec => \&userserviceinformationprime
				},
				"remote[\\s]*operations" => {
					id => b00110010,
					codec => \&remoteoperations
				},
				"service[\\s]*activation" => {
					id => b00110011,
					codec => \&serviceactivation
				},
				"transmission[\\s]*medium[\\s]*used" => {
					id => b00110101,
					codec => \&transmissionmediumused
				},
				"message[\\s]*compatibility[\\s]*information" => {
					id => b00111000,
					codec => \&messagecompatibilityinformation
				},
				"parameter[\\s]*compatibility[\\s]*information" => {
					id => b00111001,
					codec => \&parametercompatibilityinformation
				},
				"precedence" => {
					id => b00111010,
					codec => \&precedence
				},
				"hop[\\s]*counter" => {
					id => b00111101,
					codec => \&hopcounter
				},
				"redirect[\\s]*capability" => {
					id => b01001110,
					codec => \&redirectcapability
				},
				"network[\\s]*management[\\s]*controls" => {
					id => b01011011,
					codec => \&networkmanagementcontrols
				},
				"redirect[\\s]*counter" => {
					id => b01110111,
					codec => \&redirectcounter
				},
				"application[\\s]*transport" => {
					id => b01111000,
					codec => \&applicationtransport
				},
				"pivot[\\s]*capabiltiy" => {
					id => b01111011,
					codec => \&pivotcapabiltiy
				},
				"pivot[\\s]*routing[\\s]*indicator" => {
					id => b01111100,
					codec => \&pivotroutingindicator
				},
				"calling[\\s]*geodetic[\\s]*location" => {
					id => b10000001,
					codec => \&callinggeodeticlocation
				},
				"pivot[\\s]*status" => {
					id => b10000110,
					codec => \&pivotstatus
				},
				"pivot[\\s]*counter" => {
					id => b10000111,
					codec => \&pivotcounter
				},
				"pivot[\\s]*routing[\\s]*forward[\\s]*information" => {
					id => b10001000,
					codec => \&pivotroutingforwardinformation
				},
				"pivot[\\s]*routing[\\s]*backward[\\s]*information" => {
					id => b10001001,
					codec => \&pivotroutingbackwardinformation
				},
				"redirect[\\s]*status" => {
					id => b10001010,
					codec => \&redirectstatus
				},
				"redirect[\\s]*forward[\\s]*information" => {
					id => b10001011,
					codec => \&redirectforwardinformation
				},
				"redirect[\\s]*backward[\\s]*information" => {
					id => b10001100,
					codec => \&redirectbackwardinformation
				},
				"generic[\\s]*address" => {
					id => b11000000,
					codec => \&genericaddress
				},
				"generic[\\s]*digits" => {
					id => b11000001,
					codec => \&genericdigits
				},
				"operator[\\s]*services[\\s]*information" => {
					id => b11000010,
					codec => \&operatorservicesinformation
				},
				"egress" => {
					id => b11000011,
					codec => \&egress
				},
				"jurisdiction" => {
					id => b11000100,
					codec => \&jurisdiction
				},
				"carrier[\\s]*indication" => {
					id => b11000101,
					codec => \&carrierindication
				},
				"business[\\s]*group" => {
					id => b11000110,
					codec => \&businessgroup
				},
				"generic[\\s]*name" => {
					id => b11000111,
					codec => \&genericname
				},
				"notification[\\s]*indicator" => {
					id => b11100001,
					codec => \&notificationindicator
				},
				"carrier[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100010,
					codec => \&carrierserviceprovideridentification
				},
				"transaction[\\s]*request" => {
					id => b11100011,
					codec => \&transactionrequest
				},
				"local[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100100,
					codec => \&localserviceprovideridentification
				},
				"circuit[\\s]*group[\\s]*characteristic[\\s]*indicator" => {
					id => b11100101,
					codec => \&circuitgroupcharacteristicindicator
				},
				"circuit[\\s]*validation[\\s]*response[\\s]*indicator" => {
					id => b11100110,
					codec => \&circuitvalidationresponseindicator
				},
				"outgoing[\\s]*trunk[\\s]*group[\\s]*number" => {
					id => b11100111,
					codec => \&outgoingtrunkgroupnumber
				},
				"circuit[\\s]*identification[\\s]*name" => {
					id => b11101000,
					codec => \&circuitidentificationname
				},
				"CLLI[\\s]*code" => {
					id => b11101001,
					codec => \&CLLIcode
				},
				"common[\\s]*language[\\s]*location[\\s]*identifier[\\s]*code" => {
					id => b11101001,
					codec => \&commonlanguagelocationidentifiercode
				},
				"originating[\\s]*line[\\s]*information" => {
					id => b11101010,
					codec => \&originatinglineinformation
				},
				"charge[\\s]*number" => {
					id => b11101011,
					codec => \&chargenumber
				},
				"service[\\s]*code[\\s]*indicator" => {
					id => b11101100,
					codec => \&servicecodeindicator
				},
				"special[\\s]*processing[\\s]*request" => {
					id => b11101101,
					codec => \&specialprocessingrequest
				},
				"carrier[\\s]*selection[\\s]*information" => {
					id => b11101110,
					codec => \&carrierselectioninformation
				},
				"network[\\s]*transport" => {
					id => b11101111,
					codec => \&networktransport
				},
			},
		},
		itu => {
			msgs => {
			0b00000110 => {
				name => "Address Complete (ACM)",
				fixed => [
					{
						code => 0b00010001,
						name => "backward call indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101110 => {
						name => "access delivery information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110110 => {
						name => "call diversion information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01000000 => {
						name => "redirection number restriction",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110010 => {
						name => "conference treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01110100 => {
						name => "UID action indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01111010 => {
						name => "CCNR possible indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10000010 => {
						name => "HTR information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b10001001 => {
						name => "pivot routing backward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
				},
			},
			0b00001001 => {
				name => "Answer (ANM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b00100001 => {
						name => "connected number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101110 => {
						name => "access delivery information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b01001101 => {
						name => "backward GVNS",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101101 => {
						name => "call history information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium used",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00001100 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00110011 => {
						name => "sevice activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01000000 => {
						name => "redirection number restriction",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110011 => {
						name => "display information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01110010 => {
						name => "conference treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b10001001 => {
						name => "pivot routing backward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
				},
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				fixed => [
					{
						code => 0b00100100,
						name => "event information",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00010010 => {
						name => "cause indicators",
						minbytes => 4,
						maxbytes => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101110 => {
						name => "access delivery information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110110 => {
						name => "call diversion information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01000000 => {
						name => "redirection number restriction",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01000101 => {
						name => "call transfer number",
						minbyte => 4,
						maxbyte => 7,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100001 => {
						name => "connected number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b01001101 => {
						name => "backward GVNS",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00101101 => {
						name => "call history information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b01110010 => {
						name => "conference treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01110100 => {
						name => "UID action indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01111010 => {
						name => "CCNR possible indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001001 => {
						name => "pivot routing backward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
				},
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 3,
						maxbyte => 34,
					},
				],
				optional => {
				}
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 3,
						maxbyte => 34,
					},
				],
				optional => {
				}
			},
			0b00101010 => {
				name => "Circuit Group Query (CGQ)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 255,
					},
				],
				optional => {
				},
			},
			0b00101011 => {
				name => "Circuit Group Query Response (CGQR)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 255,
					},
					{
						code => 0b00100110,
						name => "circuit state indicator",
						minbyte => 2,
						maxbyte => 33,
					},
				],
				optional => {
				}
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 255,
					},
				],
				optional => {
				}
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRSA)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 2,
						maxbyte => 5,
					},
				],
				optional => {
				}
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 3,
						maxbyte => 34,
					},
				],
				optional => {
				}
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				fixed => [
					{
						code => 0b00010101,
						name => "circuit group supervision type indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010110,
						name => "range and status",
						minbyte => 3,
						maxbyte => 34,
					},
				],
				optional => {
				}
			},
			0b00110001 => {
				name => "Charge Information (CHGI)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 3,
						maxbytes => 255,
					},
				],
				optional => {
				}
			},
			0b00000111 => {
				name => "Connect (CON)",
				fixed => [
					{
						code => 0b00010001,
						name => "backward call indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01001101 => {
						name => "backward GVNS",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00100001 => {
						name => "connected number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00110101 => {
						name => "transmission medium user",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101110 => {
						name => "access delivery information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101101 => {
						name => "call history information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01110010 => {
						name => "conference treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b10000010 => {
						name => "HTR information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b10001001 => {
						name => "pivot routing backward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
				},
			},
			0b00000101 => {
				name => "Continuity (COT)",
				fixed => [
					{
						code => 0b00010000,
						name => "continuity indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010001 => {
				name => "Continuity Check Request (CCR)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110011 => {
				name => "Facility (FAC)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01000101 => {
						name => "call transfer number",
						minbyte => 4,
						maxbyte => 7,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b01111100 => {
						name => "pivot routing indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10000110 => {
						name => "pivot routing indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10000111 => {
						name => "pivot counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001001 => {
						name => "pivot routing backward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				fixed => [
					{
						code => 0b00011000,
						name => "facility indicator",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 7,
						maxbyte => 9,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00100001 => {
				name => "Facility Reject (FRJ)",
				fixed => [
					{
						code => 0b00011000,
						name => "facility indicator",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 3,
						maxbytes => 255,
					},
				],
				optional => {
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				fixed => [
					{
						code => 0b00011000,
						name => "facility indicator",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 7,
						maxbyte => 9,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00001000 => {
				name => "Forward Transfer (FT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
				}
			},
			0b00110110 => {
				name => "Identification Request (IRQ)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111011 => {
						name => "MCID request indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00110111 => {
				name => "Identification Response (IRS)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111011 => {
						name => "MCID request indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 4,
						maxbyte => 7,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01110001 => {
						name => "charged party identification",
						minbyte => 3,
						maxbyte => 255,
					},
				}
			},
			0b00000100 => {
				name => "Information (INF)",
				fixed => [
					{
						code => 0b00001111,
						name => "information indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00001001 => {
						name => "calling partys category",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 4,
						maxbyte => 7,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 7,
						maxbyte => 9,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00000011 => {
				name => "Information Request (INR)",
				fixed => [
					{
						code => 0b00001110,
						name => "information request indicators",
						bytes => 2,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00000001 => {
				name => "Initial Address (IAM)",
				fixed => [
					{
						code => 0b00000110,
						name => "nature of connection indicators",
						bytes => 1,
					},
					{
						code => 0b00000111,
						name => "forward call indicators",
						bytes => 2,
					},
					{
						code => 0b00001001,
						name => "calling partys category",
						bytes => 1,
					},
					{
						code => 0b00000010,
						name => "transmission medium requirement",
						bytes => 1,
					},
				],
				variable => [
					{
						code => 0b00000100,
						name => "called party number",
						minbytes => 4,
						maxbytes => 255,
					},
				],
				optional => {
					0b00100011 => {
						name => "transit network selection",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
					0b00001010 => {
						name => "calling party number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00001011 => {
						name => "redirecting number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00011010 => {
						name => "closed user group interlock code",
						minbyte => 6,
						maxbyte => 6,
					},
					0b00001101 => {
						name => "connection request",
						minbyte => 7,
						maxbyte => 9,
					},
					0b00101000 => {
						name => "original called number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00011101 => {
						name => "user service information",
						minbyte => 4,
						maxbyte => 13,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00110001 => {
						name => "propagation delay counter",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110000 => {
						name => "user service information prime",
						minbyte => 4,
						maxbyte => 13,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101011 => {
						name => "origination ISC point code",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00110100 => {
						name => "user teleservice information",
						minbyte => 4,
						maxbyte => 5,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00110010 => {
						name => "service activation",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00100101 => {
						name => "circuit assignment map",
						minbyte => 6,
						maxbyte => 7,
					},
					0b01100101 => {
						name => "correlation id",
						minbyte => 6,
						maxbyte => 7,
					},
					0b00110110 => {
						name => "call diversion information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01101111 => {
						name => "called IN number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b01110000 => {
						name => "call offering treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01110010 => {
						name => "conference treatment indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01100110 => {
						name => "SCF id",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01110101 => {
						name => "UID capability indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111101 => {
						name => "hop counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111001 => {
						name => "collect call request",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01111011 => {
						name => "pivot capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01111101 => {
						name => "called directgory number",
						minbyte => 5,
						maxbyte => 255,
					},
					0b01111111 => {
						name => "original called IN number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b10000001 => {
						name => "calling geodetic location",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10000100 => {
						name => "network routing number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b10000101 => {
						name => "quality on release capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10000111 => {
						name => "pivot counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001000 => {
						name => "pivot routing forward indicator",
						minbyte => 3,
						maxbyte => 255,
					},
					0b01001110 => {
						name => "redirect capability",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001010 => {
						name => "redirect status",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001011 => {
						name => "redirect forward information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b10001101 => {
						name => "number portability forward information",
						minbyte => 1,
						maxbyte => 255,
					},
				},
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b01000000 => {
				name => "Loop Prevention (LPP)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b01000011 => {
						name => "call transfer reference",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01000100 => {
						name => "lopp prevention indicators",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00110111 => {
						name => "echo control information",
						minbyte => 3,
						maxbyte => 3,
					},
				}
			},
			0b00110000 => {
				name => "Overload (OLM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b01111000 => {
						name => "application transport",
						minbyte => 5,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101001 => {
						name => "optional backward call indicators",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00001000 => {
						name => "optional forward call indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00001100 => {
				name => "Release (REL)",
				fixed => [
				],
				variable => [
					{
						code => 0b00010010,
						name => "cause indicators",
						minbytes => 3,
						maxbytes => 255,
					},
				],
				optional => {
					0b00010011 => {
						name => "redirection information",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00001011 => {
						name => "redirection number",
						minbyte => 5,
						maxbyte => 12,
					},
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00011110 => {
						name => "signalling point code",
						minbyte => 4,
						maxbyte => 4,
					},
					0b00100000 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b00100111 => {
						name => "automatic congestion level",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00101111 => {
						name => "network specific facility",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101110 => {
						name => "access delivery information",
						minbyte => 3,
						maxbyte => 3,
					},
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user indicators",
						minbyte => 3,
						maxbyte => 3,
					},
					0b01110011 => {
						name => "display information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00110010 => {
						name => "remote operations",
						minbyte => 8,
						maxbyte => 255,
					},
					0b10000010 => {
						name => "HTR information",
						minbyte => 4,
						maxbyte => 255,
					},
					0b01110111 => {
						name => "redirect counter",
						minbyte => 3,
						maxbyte => 3,
					},
					0b10001100 => {
						name => "redirect backward information",
						minbyte => 5,
						maxbyte => 11,
					},
				},
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00010010 => {
						name => "cause indicators",
						minbytes => 5,
						maxbytes => 6,
					},
				}
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00001110 => {
				name => "Resume (RES)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
				}
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
					0b00101010 => {
						name => "user-to-user information",
						minbyte => 3,
						maxbyte => 131,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
					0b11000001 => {
						name => "generic digits",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00101100 => {
						name => "generic notification indicator",
						minbyte => 3,
						maxbyte => 3,
					},
					0b11000000 => {
						name => "generic number",
						minbyte => 5,
						maxbyte => 255,
					},
				}
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00000101 => {
						name => "subsequent number",
						minbyte => 4,
						maxbyte => 255,
					},
					0b00111000 => {
						name => "message compatibility information",
						minbyte => 3,
						maxbyte => 255,
					},
				}
			},
			0b01000011 => {
				name => "Subsequent Directory Number (SDN)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				fixed => [
					{
						code => 0b00100010,
						name => "suspend/resume indicators",
						bytes => 1,
					},
				],
				variable => [
				],
				optional => {
					0b00000001 => {
						name => "call reference",
						minbyte => 7,
						maxbyte => 7,
					},
				}
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				fixed => [
				],
				variable => [
				],
				optional => {
				}
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				fixed => [
				],
				variable => [
				],
				optional => {
					0b00111001 => {
						name => "parameter compatibility information",
						minbyte => 4,
						maxbyte => 255,
					},
				}
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				fixed => [
				],
				variable => [
					{
						code => 0b00100000,
						name => "user-to-user information",
						minbyte => 2,
						maxbyte => 130,
					},
				],
				optional => {
					0b000000011 => {
						name => "access transport",
						minbyte => 3,
						maxbyte => 255,
					},
				}
			},
			},
			rawmsgs => {
			0b00000110 => {
				name => "Address Complete (ACM)",
				value => 0x0,
			},
			0b00001001 => {
				name => "Answer (ANM)",
				value => 0x0,
			},
			0b01000001 => {
				name => "Application Transport (APT)",
				value => 0x0,
			},
			0b00010011 => {
				name => "Blocking (BLO)",
				value => 0x0,
			},
			0b00010101 => {
				name => "Blocking Acknowledgement (BLA)",
				value => 0x0,
			},
			0b00101100 => {
				name => "Call Progress (CPG)",
				value => 0x0,
			},
			0b00011000 => {
				name => "Circuit Group Blocking (CGB)",
				value => 0x0,
			},
			0b00011010 => {
				name => "Circuit Group Blocking Acknowledgement (CGBA)",
				value => 0x0,
			},
			0b00101010 => {
				name => "Circuit Group Query (CGQ)",
				value => 0x0,
			},
			0b00101011 => {
				name => "Circuit Group Query Response (CGQR)",
				value => 0x0,
			},
			0b00010111 => {
				name => "Circuit Group Reset (GRS)",
				value => 0x0,
			},
			0b00101001 => {
				name => "Circuit Group Reset Acknowledgement (GRSA)",
				value => 0x0,
			},
			0b00011001 => {
				name => "Circuit Group Unblocking (CGU)",
				value => 0x0,
			},
			0b00011011 => {
				name => "Circuit Group Unblocking Acknowledgement (CGUA)",
				value => 0x0,
			},
			0b00110001 => {
				name => "Charge Information (CHGI)",
				value => 0x0,
			},
			0b00101111 => {
				name => "Confusion (CFN)",
				value => 0x0,
			},
			0b00000111 => {
				name => "Connect (CON)",
				value => 0x0,
			},
			0b00000101 => {
				name => "Continuity (COT)",
				value => 0x0,
			},
			0b00010001 => {
				name => "Continuity Check Request (CCR)",
				value => 0x0,
			},
			0b00110011 => {
				name => "Facility (FAC)",
				value => 0x0,
			},
			0b00100000 => {
				name => "Facility Accepted (FAA)",
				value => 0x0,
			},
			0b00100001 => {
				name => "Facility Reject (FRJ)",
				value => 0x0,
			},
			0b00011111 => {
				name => "Facility Request (FAR)",
				value => 0x0,
			},
			0b00001000 => {
				name => "Forward Transfer (FT)",
				value => 0x0,
			},
			0b00110110 => {
				name => "Identification Request (IRQ)",
				value => 0x0,
			},
			0b00110111 => {
				name => "Identification Response (IRS)",
				value => 0x0,
			},
			0b00000100 => {
				name => "Information (INF)",
				value => 0x0,
			},
			0b00000011 => {
				name => "Information Request (INR)",
				value => 0x0,
			},
			0b00000001 => {
				name => "Initial Address (IAM)",
				value => 0x0,
			},
			0b00100100 => {
				name => "Loop Back Acknowledgement (LPA)",
				value => 0x0,
			},
			0b01000000 => {
				name => "Loop Prevention (LPP)",
				value => 0x0,
			},
			0b00110010 => {
				name => "Network Resource Management (NRM)",
				value => 0x0,
			},
			0b00110000 => {
				name => "Overload (OLM)",
				value => 0x0,
			},
			0b00101000 => {
				name => "Pass Along (PAM)",
				value => 0x0,
			},
			0b01000010 => {
				name => "Pre-Release Information (PRI)",
				value => 0x0,
			},
			0b00001100 => {
				name => "Release (REL)",
				value => 0x0,
			},
			0b00010000 => {
				name => "Release Complete (RLC)",
				value => 0x0,
			},
			0b00010010 => {
				name => "Reset Circuit (RSC)",
				value => 0x0,
			},
			0b00001110 => {
				name => "Resume (RES)",
				value => 0x0,
			},
			0b00111000 => {
				name => "Segmentation (SEG)",
				value => 0x0,
			},
			0b00000010 => {
				name => "Subsequent Address (SAM)",
				value => 0x0,
			},
			0b01000011 => {
				name => "Subsequent Directory Number (SDN)",
				value => 0x0,
			},
			0b00001101 => {
				name => "Suspend (SUS)",
				value => 0x0,
			},
			0b00010100 => {
				name => "Unblocking (UBL)",
				value => 0x0,
			},
			0b00010110 => {
				name => "Unblocking Acknowledgement (UBA)",
				value => 0x0,
			},
			0b00101110 => {
				name => "Unequipped Circuit Identification Code (UCIC)",
				value => 0x0,
			},
			0b00110101 => {
				name => "User Part Available (UPA)",
				value => 0x0,
			},
			0b00110100 => {
				name => "User Part Test (UPT)",
				value => 0x0,
			},
			0b00101101 => {
				name => "User-to-user Information (USR)",
				value => 0x0,
			},
			},
			msgids => {
			"Address Complete (ACM)" => 0b00000110,
			"Answer (ANM)" => 0b00001001, 
			"Application Transport (APT)" => 0b01000001,
			"Blocking (BLO)" => 0b00010011,
			"Blocking Acknowledgement (BLA)" => 0b00010101,
			"Call Progress (CPG)" => 0b00101100,
			"Circuit Group Blocking (CGB)" => 0b00011000,
			"Circuit Group Blocking Acknowledgement (CGBA)" => 0b00011010,
			"Circuit Group Query (CGQ)" => 0b00101010,
			"Circuit Group Query Response (CGQR)" => 0b00101011,
			"Circuit Group Reset (GRS)" => 0b00010111,
			"Circuit Group Reset Acknowledgement (GRSA)" => 0b00101001,
			"Circuit Group Unblocking (CGU)" => 0b00011001,
			"Circuit Group Unblocking Acknowledgement (CGUA)" => 0b00011011,
			"Charge Information (CHGI)" => 0b00110001,
			"Confusion (CFN)" => 0b00101111,
			"Connect (CON)" => 0b00000111,
			"Continuity (COT)" => 0b00000101,
			"Continuity Check Request (CCR)" => 0b00010001,
			"Facility (FAC)" => 0b00110011,
			"Facility Accepted (FAA)" => 0b00100000,
			"Facility Reject (FRJ)" => 0b00100001,
			"Facility Request (FAR)" => 0b00011111,
			"Identification Request (IRQ)" => 0b00110110,
			"Identification Response (IRS)" => 0b00110111,
			"Information (INF)" => 0b00000100,
			"Information Request (INR)" => 0b00000011,
			"Initial Address (IAM)" => 0b00000001,
			"Loop Back Acknowledgement (LPA)" => 0b00100100,
			"Loop Prevention (LPP)" => 0b01000000,
			"Network Resource Management (NRM)" => 0b00110010,
			"Overload (OLM)" => 0b00110000,
			"Pass Along (PAM)" => 0b00101000,
			"Pre-Release Information (PRI)" => 0b01000010,
			"Release (REL)" => 0b00001100,
			"Release Complete (RLC)" => 0b00010000,
			"Reset Circuit (RSC)" => 0b00010010,
			"Resume (RES)" => 0b00001110,
			"Segmentation (SEG)" => 0b00111000,
			"Subsequent Address (SAM)" => 0b00000010,
			"Subsequent Directory Number (SDN)" => 0b01000011,
			"Suspend (SUS)" => 0b00001101,
			"Unblocking (UBL)" => 0b00010100,
			"Unblocking Acknowledgement (UBA)" => 0b00010110,
			"Unequipped Circuit Identification Code (UCIC)" => 0b00101110,
			"User Part Available (UPA)" => 0b00110101,
			"User Part Test (UPT)" => 0b00110100,
			"User-to-user Information (USR)" => 0b00101101,
			},
			parameters => {
				"end[\\s]*of[\\s]*optional[\\s]*parameters" => {
					id => b00000000,
					codec => \&endofoptionalparameters
				},
				"call[\\s]*reference" => {
					id => b00000001,
					codec => \&callreference
				},
				"transmission[\\s]*medium[\\s]*requirements" => {
					id => b00000010,
					codec => \&transmissionmediumrequirements
				},
				"access[\\s]*transport" => {
					id => b00000011,
					codec => \&accesstransport
				},
				"called[\\s]*party[\\s]*number" => {
					id => b00000100,
					codec => \&calledpartynumber
				},
				"subsequent[\\s]*number" => {
					id => b00000101,
					codec => \&subsequentnumber
				},
				"nature[\\s]*of[\\s]*connection[\\s]*indicators" => {
					id => b00000110,
					codec => \&natureofconnectionindicators
				},
				"forward[\\s]*call[\\s]*indicators" => {
					id => b00000111,
					codec => \&forwardcallindicators
				},
				"optional[\\s]*forward[\\s]*call[\\s]*indicators" => {
					id => b00001000,
					codec => \&optionalforwardcallindicators
				},
				"calling[\\s]*partys[\\s]*category" => {
					id => b00001001,
					codec => \&callingpartyscategory
				},
				"calling[\\s]*party[\\s]*number" => {
					id => b00001010,
					codec => \&callingpartynumber
				},
				"redirecting[\\s]*number" => {
					id => b00001011,
					codec => \&redirectingnumber
				},
				"redirection[\\s]*number" => {
					id => b00001100,
					codec => \&redirectionnumber
				},
				"connection[\\s]*request" => {
					id => b00001101,
					codec => \&connectionrequest
				},
				"information[\\s]*request[\\s]*indicators" => {
					id => b00001110,
					codec => \&informationrequestindicators
				},
				"information[\\s]*indicators" => {
					id => b00001111,
					codec => \&informationindicators
				},
				"continuity[\\s]*indicators" => {
					id => b00010000,
					codec => \&continuityindicators
				},
				"backward[\\s]*call[\\s]*indicators" => {
					id => b00010001,
					codec => \&backwardcallindicators
				},
				"cause[\\s]*indicattors" => {
					id => b00010010,
					codec => \&causeindicattors
				},
				"redirection[\\s]*information" => {
					id => b00010011,
					codec => \&redirectioninformation
				},
				"circuit[\\s]*group[\\s]*supervision[\\s]*message[\\s]*type[\\s]*indicator" => {
					id => b00010101,
					codec => \&circuitgroupsupervisionmessagetypeindicator
				},
				"range[\\s]*and[\\s]*status" => {
					id => b00010110,
					codec => \&rangeandstatus
				},
				"call[\\s]*modification[\\s]*indicators" => {
					id => b00010111,
					codec => \&callmodificationindicators
				},
				"facility[\\s]*indicator" => {
					id => b00011000,
					codec => \&facilityindicator
				},
				"facility[\\s]*information[\\s]*indicators" => {
					id => b00011001,
					codec => \&facilityinformationindicators
				},
				"closed[\\s]*user[\\s]*groupinterlock[\\s]*code" => {
					id => b00011010,
					codec => \&closedusergroupinterlockcode
				},
				"index" => {
					id => b00011011,
					codec => \&index
				},
				"closed[\\s]*user[\\s]*group[\\s]*check[\\s]*response[\\s]*indicators" => {
					id => b00011100,
					codec => \&closedusergroupcheckresponseindicators
				},
				"user[\\s]*service[\\s]*information" => {
					id => b00011101,
					codec => \&userserviceinformation
				},
				"signaling[\\s]*port[\\s]*code" => {
					id => b00011110,
					codec => \&signalingportcode
				},
				"user-to-user[\\s]*information" => {
					id => b00100000,
					codec => \&usertouserinformation
				},
				"connected[\\s]*number" => {
					id => b00100001,
					codec => \&connectednumber
				},
				"suspend[\\s]*resume[\\s]*indicators" => {
					id => b00100010,
					codec => \&suspendresumeindicators
				},
				"transit[\\s]*network[\\s]*selection" => {
					id => b00100011,
					codec => \&transitnetworkselection
				},
				"event[\\s]*information[\\s]*indicators" => {
					id => b00100100,
					codec => \&eventinformationindicators
				},
				"circuit[\\s]*assignment[\\s]*map" => {
					id => b00100101,
					codec => \&circuitassignmentmap
				},
				"circuit[\\s]*state[\\s]*indicator" => {
					id => b00100110,
					codec => \&circuitstateindicator
				},
				"automatic[\\s]*congestion[\\s]*level" => {
					id => b00100111,
					codec => \&automaticcongestionlevel
				},
				"original[\\s]*called[\\s]*number" => {
					id => b00101000,
					codec => \&originalcallednumber
				},
				"optional[\\s]*backward[\\s]*call[\\s]*indicators" => {
					id => b00101001,
					codec => \&optionalbackwardcallindicators
				},
				"user-to-user[\\s]*indicators" => {
					id => b00101010,
					codec => \&usertouserindicators
				},
				"network[\\s]*specific[\\s]*facility" => {
					id => b00101111,
					codec => \&networkspecificfacility
				},
				"user[\\s]*service[\\s]*information[\\s]*prime" => {
					id => b00110000,
					codec => \&userserviceinformationprime
				},
				"remote[\\s]*operations" => {
					id => b00110010,
					codec => \&remoteoperations
				},
				"service[\\s]*activation" => {
					id => b00110011,
					codec => \&serviceactivation
				},
				"transmission[\\s]*medium[\\s]*used" => {
					id => b00110101,
					codec => \&transmissionmediumused
				},
				"message[\\s]*compatibility[\\s]*information" => {
					id => b00111000,
					codec => \&messagecompatibilityinformation
				},
				"parameter[\\s]*compatibility[\\s]*information" => {
					id => b00111001,
					codec => \&parametercompatibilityinformation
				},
				"precedence" => {
					id => b00111010,
					codec => \&precedence
				},
				"hop[\\s]*counter" => {
					id => b00111101,
					codec => \&hopcounter
				},
				"redirect[\\s]*capability" => {
					id => b01001110,
					codec => \&redirectcapability
				},
				"network[\\s]*management[\\s]*controls" => {
					id => b01011011,
					codec => \&networkmanagementcontrols
				},
				"redirect[\\s]*counter" => {
					id => b01110111,
					codec => \&redirectcounter
				},
				"application[\\s]*transport" => {
					id => b01111000,
					codec => \&applicationtransport
				},
				"pivot[\\s]*capabiltiy" => {
					id => b01111011,
					codec => \&pivotcapabiltiy
				},
				"pivot[\\s]*routing[\\s]*indicator" => {
					id => b01111100,
					codec => \&pivotroutingindicator
				},
				"calling[\\s]*geodetic[\\s]*location" => {
					id => b10000001,
					codec => \&callinggeodeticlocation
				},
				"pivot[\\s]*status" => {
					id => b10000110,
					codec => \&pivotstatus
				},
				"pivot[\\s]*counter" => {
					id => b10000111,
					codec => \&pivotcounter
				},
				"pivot[\\s]*routing[\\s]*forward[\\s]*information" => {
					id => b10001000,
					codec => \&pivotroutingforwardinformation
				},
				"pivot[\\s]*routing[\\s]*backward[\\s]*information" => {
					id => b10001001,
					codec => \&pivotroutingbackwardinformation
				},
				"redirect[\\s]*status" => {
					id => b10001010,
					codec => \&redirectstatus
				},
				"redirect[\\s]*forward[\\s]*information" => {
					id => b10001011,
					codec => \&redirectforwardinformation
				},
				"redirect[\\s]*backward[\\s]*information" => {
					id => b10001100,
					codec => \&redirectbackwardinformation
				},
				"generic[\\s]*address" => {
					id => b11000000,
					codec => \&genericaddress
				},
				"generic[\\s]*digits" => {
					id => b11000001,
					codec => \&genericdigits
				},
				"operator[\\s]*services[\\s]*information" => {
					id => b11000010,
					codec => \&operatorservicesinformation
				},
				"egress" => {
					id => b11000011,
					codec => \&egress
				},
				"jurisdiction" => {
					id => b11000100,
					codec => \&jurisdiction
				},
				"carrier[\\s]*indication" => {
					id => b11000101,
					codec => \&carrierindication
				},
				"business[\\s]*group" => {
					id => b11000110,
					codec => \&businessgroup
				},
				"generic[\\s]*name" => {
					id => b11000111,
					codec => \&genericname
				},
				"notification[\\s]*indicator" => {
					id => b11100001,
					codec => \&notificationindicator
				},
				"carrier[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100010,
					codec => \&carrierserviceprovideridentification
				},
				"transaction[\\s]*request" => {
					id => b11100011,
					codec => \&transactionrequest
				},
				"local[\\s]*service[\\s]*provider[\\s]*identification" => {
					id => b11100100,
					codec => \&localserviceprovideridentification
				},
				"circuit[\\s]*group[\\s]*characteristic[\\s]*indicator" => {
					id => b11100101,
					codec => \&circuitgroupcharacteristicindicator
				},
				"circuit[\\s]*validation[\\s]*response[\\s]*indicator" => {
					id => b11100110,
					codec => \&circuitvalidationresponseindicator
				},
				"outgoing[\\s]*trunk[\\s]*group[\\s]*number" => {
					id => b11100111,
					codec => \&outgoingtrunkgroupnumber
				},
				"circuit[\\s]*identification[\\s]*name" => {
					id => b11101000,
					codec => \&circuitidentificationname
				},
				"CLLI[\\s]*code" => {
					id => b11101001,
					codec => \&CLLIcode
				},
				"common[\\s]*language[\\s]*location[\\s]*identifier[\\s]*code" => {
					id => b11101001,
					codec => \&commonlanguagelocationidentifiercode
				},
				"originating[\\s]*line[\\s]*information" => {
					id => b11101010,
					codec => \&originatinglineinformation
				},
				"charge[\\s]*number" => {
					id => b11101011,
					codec => \&chargenumber
				},
				"service[\\s]*code[\\s]*indicator" => {
					id => b11101100,
					codec => \&servicecodeindicator
				},
				"special[\\s]*processing[\\s]*request" => {
					id => b11101101,
					codec => \&specialprocessingrequest
				},
				"carrier[\\s]*selection[\\s]*information" => {
					id => b11101110,
					codec => \&carrierselectioninformation
				},
				"network[\\s]*transport" => {
					id => b11101111,
					codec => \&networktransport
				},
			},
		},
	};
	#
	bless $self, $class;
	#
	return($self);
}
#
# decode mandatory FIX part of ISUP msgs
#
sub decodeFixedParam {
	my ($self, $prawbytes, $poffset, $pdict) = @_;
	#
	$self->{print}->("\tFIXED FIELD NAME: ");
	$self->{print}->($pdict->{name});
	$self->{print}->("\n");
	#
	$self->{print}->("\tFIXED FIELD VALUE: ");
	for (my $i=0; $i<$pdict->{bytes}; ${i}++) {
		$self->{print}->(sprintf("%02x", $prawbytes->[$$poffset]));
		${${poffset}}++;
	}
	$self->{print}->("\n");
}
#
# decode mandatory VARIABLE part of ISUP msgs
#
sub decodeVariableParam {
	my ($self, $prawbytes, $poffset, $pdict) = @_;
	#
	$self->{print}->("\tVARIABLE FIELD NAME: ");
	$self->{print}->($pdict->{name});
	$self->{print}->("\n");
	#
	$self->{print}->("\tVARIABLE FIELD VALUE: ");
	my $loffset = $$poffset+$prawbytes->[$$poffset];
	my $vlen = $prawbytes->[$loffset];
	my $voffset = $loffset + 1;
	#
	for (my $i=0; $i<$vlen; ${i}++) {
		$self->{print}->(sprintf("%02x", $prawbytes->[$voffset]));
		${voffset}++;
	}
	${${poffset}}++;
	$self->{print}->("\n");
}
#
# decode OPTIONAL part of ISUP msgs
#
sub decodeOptionalParam {
	my ($self, $prawbytes, $poffset, $pdict) = @_;
	#
	my $optparamid = $prawbytes->[$$poffset];
	if (exists($pdict->{${optparamid}})) {
		my $name = $pdict->{$optparamid}->{name};
		$self->{print}->("\tOPTIONAL FIELD NAME: ");
		$self->{print}->(${name});
		$self->{print}->("\n");
	} else {
		# parameter does is not known. don't print name.
		my $hexoptparamid = sprintf("%02x", ${optparamid});
		$self->{print}->("\tUNKNOWN PARAMETER ID: ${optparamid},${hexoptparamid}\n");
	}
	#
	$self->{print}->("\tOPTIONAL FIELD VALUE: ");
	$$poffset += 1;
	my $optlen = $prawbytes->[$$poffset];
	$$poffset += 1;
	#
	for (my $i=0; $i<$optlen; ${i}++) {
		$self->{print}->(sprintf("%02x", $prawbytes->[$$poffset]));
		${${poffset}}++;
	}
	$self->{print}->("\n");
}
#
# decode ISUP msgs
#
sub decode {
	my ($self, $protocol, $rawmsg) = @_;
	#
	confess "$protocol does not exist: $!" 
		unless exists($self->{${protocol}});
	#
	my @rawbytes = unpack "C*", $rawmsg;
	#
	my $offset = 0;
	my $msgid = ${rawbytes}[$offset];
	#
	if (exists($self->{${protocol}}{msgs}{$msgid})) {
		$self->{print}->("\n");
		$self->{print}->("MSG TYPE: ");
		$self->{print}->($msgid);
		$self->{print}->("\n");
		$self->{print}->("MSG NAME: ");
		$self->{print}->($self->{${protocol}}{msgs}{$msgid}{name});
		$self->{print}->("\n");
	} else {
		$self->{print}->("\n");
		my $hexmsgid = sprintf("%02x", ${msgid});
		$self->{print}->("UNKNOWN MSG TYPE: ${msgid},${hexmsgid}\n");
		return;
	}
	#
	my $name = $self->{${protocol}}{msgs}{$msgid}{name};
	my $fixed = $self->{${protocol}}{msgs}{$msgid}{fixed};
	my $variable = $self->{${protocol}}{msgs}{$msgid}{variable};
	my $optional = $self->{${protocol}}{msgs}{$msgid}{optional};
	#
	if (scalar($self->{${protocol}}{msgs}{$msgid}{optional}) == 0) {
		# no optional parameters allowed.
		$optional = undef;
	}
	#
	${offset}++;
	for (my $fidx=0; $fidx<scalar(@{$fixed}); ${fidx}++) {
		$self->decodeFixedParam(\@rawbytes, \$offset, 
				${fixed}->[$fidx]);
	}
	#
	for (my $vidx=0; $vidx<scalar(@{$variable}); ${vidx}++) {
		$self->decodeVariableParam(\@rawbytes, \$offset, 
				${variable}->[$vidx]);
	}
	#
	my $optoffset = ${rawbytes}[$offset];
	if ($optoffset <= 0 || !defined($optional)) {
		# no optional parameters
		return;
	}
	$offset += $optoffset;
	#
	while ((${rawbytes}[$offset] != 0) && ($offset<scalar(@rawbytes))) {
		$self->decodeOptionalParam(\@rawbytes, \$offset, $optional);
	}
	#
	return;
}
#
# encode ISUP msgs
#
sub makemsg {
	my ($self, $protocol, $prawmsg, $msgid, 
		$pfixed, $pvariable, $poptional) = @_;
	#
	# sanity checks
	#
	if (!exists($self->{${protocol}})) {
		$self->{print}->("makemsg: $protocol does not exist.\n");
		return(0);
	}
	if (!exists($self->{${protocol}}{msgs})) {
		$self->{print}->("makemsg: $protocol msgs section does not exist.\n");
		return(0);
	}
	if (!exists($self->{${protocol}}{msgs}{$msgid})) {
		$self->{print}->("makemsg: $protocol msgid ($msgid) does not exist.\n");
		return(0);
	}
	if (!exists($self->{${protocol}}{parameters})) {
		$self->{print}->("makemsg: $protocol parameters section does not exist.\n");
		return(0);
	}
	#
	# get data dictionaries.
	# 
	my $name = $self->{${protocol}}{msgs}{$msgid}{name};
	my $fixed = $self->{${protocol}}{msgs}{$msgid}{fixed};
	my $variable = $self->{${protocol}}{msgs}{$msgid}{variable};
	my $optional = $self->{${protocol}}{msgs}{$msgid}{optional};
	#
	# get sizes.
	# 
	my $expected_fixed_num = scalar(@{$fixed});
	my $expected_variable_num = scalar(@{$variable});
	my $actual_fixed_num = scalar(@{$pfixed});
	my $actual_variable_num = scalar(@{$pvariable});
	my $actual_optional_num = scalar(@{$poptional});
	#
	# verify that number of fixed parameters expected matches the
	# number of fixed values passed in.
	#
	if ($expected_fixed_num != $actual_fixed_num) {
		$self->{print}->("makemsg: expected number of fixed != passed number of fixed.\n");
		return(0);
	}
	if ($expected_variable_num != $actual_variable_num) {
		$self->{print}->("makemsg: expected number of variable != passed number of variable.\n");
		return(0);
	}
	#
	# copy msg id.
	#
	my $offset = 0;
	my @buffer = ();
	#
	$buffer[$offset] = $msgid;
	$offset += 1;
	#
	# pack mandatory fixed data into buffer.
	#
	if ($expected_fixed_num > 0) {
		# we have mandatory fixed parameters
		for (my $fidx=0; $fidx<scalar(@{$fixed}); ${fidx}++) {
			my $actual_fld_len = scalar(@{$pfixed->[$fidx]});
			my $expected_fld_len = $fixed->[$fidx]->{bytes};
			if ($expected_fld_len != $actual_fld_len) {
				my $fldname = $fixed->[$fidx]->{name};
				$self->{print}->("makemsg: fixed fld $fidx ($fldname) actual_len != expected_len\n");
				$self->{print}->("fixed fld $fidx actual_len = $actual_fld_len\n");
				$self->{print}->("fixed fld $fidx expected_len = $expected_fld_len\n");
				return(0);
			}
			for (my $i=0; $i<$actual_fld_len; ${i}++, ${offset}++) {
				$buffer[$offset] = $pfixed->[$fidx][$i];
			}
		}
	}
	#
	# pack mandatory variable data into buffer.
	#
	my $parmoffset = 0;
	if ($expected_variable_num > 0) {
		# we have mandatory variable parameters
		#
		# maintain two offsets, one to where the parameter is
		# copied to and another to where the offset to the
		# current parameter is placed. add optional parameter 
		# offset if optional parameters are possible.
		#
		$parmoffset = $offset;
		$offset += $expected_variable_num;
		if (defined($optional)) {
			# we can have optional parameters, make room
			# for the optional parameter offset.
			$offset += 1;
		}
		#
		# start packing all the variable parameters
		# 
		for (my $fidx=0; $fidx<scalar(@{$variable}); ${fidx}++) {
			# offset to variable parameter
			$buffer[$parmoffset] = $offset - $parmoffset;
			$parmoffset += 1;
			# length of parameter
			my $actual_fld_len = scalar(@{$pvariable->[$fidx]});
			$buffer[$offset] = $actual_fld_len;
			$offset += 1;
			# copy parameter data
			for (my $i=0; $i<$actual_fld_len; ${i}++, ${offset}++) {
				$buffer[$offset] = $pvariable->[$fidx]->[$i];
			}
		}
	} else {
		# no mandatory variable parameters. check if we
		# can have optional parameters.
		if (defined($optional)) {
			# we can have optional parameters, make room
			# for the optional parameter offset.
			$parmoffset = $offset;
			$offset += 1;
		}
	}
	#
	# check if we can have optional parameters and if we do have
	# optional parameters.
	#
	if ( ! defined($optional)) {
		# no optional parameters allowed.
		$$prawmsg = pack "C*", @buffer;
		my $rawmsglen = length($$prawmsg);
		$self->{print}->("makemsg: message length = ${rawmsglen}\n");
		return(1);
	}
	#
	# we can have optional parameters. do we have any optional 
	# parameters to pack?
	#
	if ($actual_optional_num <= 0) {
		# we no optional parameters, set offset to 0.
		$buffer[$parmoffset] = 0;
		$$prawmsg = pack "C*", @buffer;
		my $rawmsglen = length($$prawmsg);
		$self->{print}->("makemsg: message length = ${rawmsglen}\n");
		return(1);
	}
	#
	# we have optional parameters, store offset to 
	# optional parameters.
	#
	$buffer[$parmoffset] = $offset - $parmoffset;
	#
	for (my $fidx=0; $fidx<scalar(@{$poptional}); ${fidx}++) {
		my $parmid = $poptional->[$fidx]{id};
		my $parmfldlen = scalar(@{$poptional->[$fidx]{data}});
		#
		$buffer[$offset] = $parmid;
		$offset += 1;
		$buffer[$offset] = $parmfldlen;
		$offset += 1;
		#
		for (my $i=0; $i<$parmfldlen; ${i}++, ${offset}++) {
			$buffer[$offset] = $poptional->[$fidx]{data}[$i];
		}
	}
	#
	# the end ...
	#
	$buffer[$offset] = 0;
	$$prawmsg = pack "C*", @buffer;
	#
	my $rawmsglen = length($$prawmsg);
	$self->{print}->("makemsg: message length = ${rawmsglen}\n");
	return(1);
}
#
# print msgs structure
#
sub printmsgs {
	my ($self, $protocol) = @_;
	# sanity checks
	confess "printmsgs: $protocol does not exist: $!" 
		unless exists($self->{${protocol}});
	#
	$self->{print}->("PROTOCOL: $protocol\n");
	foreach my $msg (sort keys %{$self->{${protocol}}{msgids}}) {
		my $msgid = $self->{${protocol}}{msgids}{$msg};
		# $self->{print}->("MSG: $msg : $msgid\n");
		$self->{print}->("MSG: $msg\n");
		#
		my $name = $self->{${protocol}}{msgs}{$msgid}{name};
		my $fixed = $self->{${protocol}}{msgs}{$msgid}{fixed};
		my $variable = $self->{${protocol}}{msgs}{$msgid}{variable};
		my $optional = $self->{${protocol}}{msgs}{$msgid}{optional};
		#
		$self->{print}->("\tFIXED: \n");
		for (my $fidx=0; $fidx<scalar(@{$fixed}); ${fidx}++) {
			$self->{print}->("\t\t");
			$self->{print}->(${fixed}->[$fidx]->{name});
			$self->{print}->(":");
			$self->{print}->(${fixed}->[$fidx]->{code});
			$self->{print}->(": \n");
		}
		#
		$self->{print}->("\tVARIABLE: \n");
		for (my $fidx=0; $fidx<scalar(@{$variable}); ${fidx}++) {
			$self->{print}->("\t\t");
			$self->{print}->(${variable}->[$fidx]->{name});
			$self->{print}->(":");
			$self->{print}->(${variable}->[$fidx]->{code});
			$self->{print}->(": \n");
		}
		#
		$self->{print}->("\tOPTIONAL: \n");
		foreach my $fld (sort keys %{$optional}) {
			$self->{print}->("\t\t");
			$self->{print}->(${optional}->{$fld}{name});
			$self->{print}->(":");
			$self->{print}->($fld);
			$self->{print}->(": \n");
		}
		#
		$self->{print}->("END MSG: $msg\n");
	}
	$self->{print}->("END PROTOCOL: $protocol\n");
	return;
}
#
# read in a file containing msg definitions.
#
sub parseinfile {
	my ($self, $isupfile, $prawmsg) = @_;
	# open file for read
	if (! open(ISUPMSGSFILE, $isupfile)) {
		$self->{print}->("parseinfile: can't open file $isupfile.\n");
		return(0);
	}
	# some local variables
	my $crec = undef;
	my $state = undef;
	my $protocol = undef;
	my $msgname = undef;
	my $msgid = undef;
	my $fixed = [];
	my $variable = [];
	my $optional = [];
	my $fldno = 0;
	# read in msg definitions
	while (defined($crec = <ISUPMSGSFILE>)) {
		# remove newline
		chomp($crec);
		# skip comments
		if ($crec =~ /^[\s]*#/) {
			next;
		}
		# parse the record
		if ($crec =~ /^[\s]*PROTOCOL:[\s]+([i\w]+)/i) {
			if (!defined(${1})) {
				$self->{print}->("parseinfile: unable to get protocol from PROTOCOL.\n");
				close(ISUPMSGSFILE);
				return(0);
			}
			$protocol = ${1};
			# check if protocol is known
			if (!exists($self->{${protocol}})) {
				$self->{print}->("parseinfile: unknown protocol: $protocol \n");
				close(ISUPMSGSFILE);
				return(0);
			}
			$self->{print}->("parseinfile: PROTOCOL=${protocol}\n");
			next;
		} elsif ($crec =~ /^[\s]*END\s+PROTOCOL:[\s]+(.*)$/i) {
			# nothing to do.
			$protocol = ${1};
			$self->{print}->("parseinfile: END PROTOCOL=${protocol}\n");
			next;
		} elsif ($crec =~ /^[\s]*MSG:[\s]+(.*)$/i) {
			if (!defined(${1})) {
				$self->{print}->("parseinfile: unable to get msgname from MSG.\n");
				close(ISUPMSGSFILE);
				return(0);
			}
			$msgname = ${1};
			# check if msg name is known
			if (!exists($self->{${protocol}}{msgids}{$msgname})) {
				$self->{print}->("parseinfile: unknown msg: $msgname \n");
				close(ISUPMSGSFILE);
				return(0);
			}
			$msgid = $self->{${protocol}}{msgids}{$msgname};
			# create msgs entry
			$prawmsg->{$protocol}{$msgid} = 0x0;
			$fixed = [];
			$variable = [];
			$optional = [];
			$self->{print}->("parseinfile: MSG=${msgname}\n");
			$self->{print}->("parseinfile: MSGID=${msgid}\n");
			next;
		} elsif ($crec =~ /^[\s]*END\s+MSG:[\s]+(.*)$/i) {
			# done with msg, pack it.
			$self->{print}->("parseinfile: running makemsg MSG=${msgname}\n");
			my $status = $self->makemsg($protocol, 
				\$prawmsg->{$protocol}{$msgid}, 
				$msgid, $fixed, $variable, $optional);
			$self->{print}->("parseinfile: makemsg status=${status}\n");
			if (!$status) {
				close(ISUPMSGSFILE);
				return(0);
			}
			my @Hrawmsg = unpack "H*", $prawmsg->{$protocol}{$msgid};
			$self->{print}->("parseinfile: rawmsg = @{Hrawmsg}\n");
		} elsif ($crec =~ /^[\s]*FIXED:/i) {
			$state = "FIXED";
			$fldno = 0;
			$self->{print}->("parseinfile: STATE=${state}\n");
			next;
		} elsif ($crec =~ /^[\s]*VARIABLE:/i) {
			$state = "VARIABLE";
			$fldno = 0;
			$self->{print}->("parseinfile: STATE=${state}\n");
			next;
		} elsif ($crec =~ /^[\s]*OPTIONAL:/i) {
			$state = "OPTIONAL";
			$fldno = 0;
			$self->{print}->("parseinfile: STATE=${state}\n");
			next;
		} elsif ($state eq "FIXED") {
			my ($fldname, $fldid, $fldval) = split /\s*:\s*/, $crec, 3;
			$fldname =~ s/^\s+//;
			$fldname =~ s/\s+$//;
			if (!defined($fldval) || length($fldval) <= 0) {
				$self->{print}->("parseinfile: skipping FIXED field=${fldname}\n");
				next;
			}
			$fldval =~ s/\s+//g;
			$fldid =~ s/\s+//g;
			$self->{print}->("parseinfile: FIXED field=${fldname}\n");
			$self->{print}->("parseinfile: FIXED value=<${fldval}>\n");
			#
			for (my $i=0; $fldval =~ /(..)/g; ${i}++) {
				$fixed->[$fldno][$i] = hex($1);
			}
			#
			$fldno += 1;
			next;
		} elsif ($state eq "VARIABLE") {
			my ($fldname, $fldid, $fldval) = split /\s*:\s*/, $crec, 3;
			$fldname =~ s/^\s+//;
			$fldname =~ s/\s+$//;
			if (!defined($fldval) || length($fldval) <= 0) {
				$self->{print}->("parseinfile: skipping VARIALBE field=${fldname}\n");
				next;
			}
			$fldval =~ s/\s+//g;
			$fldid =~ s/\s+//g;
			$self->{print}->("parseinfile: VARIABLE field=${fldname}\n");
			$self->{print}->("parseinfile: VARIABLE value=<${fldval}>\n");
			#
			for (my $i=0; $fldval =~ /(..)/g; ${i}++) {
				$variable->[$fldno][$i] = hex($1);
			}
			#
			$fldno += 1;
			next;
		} elsif ($state eq "OPTIONAL") {
			my ($fldname, $fldid, $fldval) = split /\s*:\s*/, $crec, 3;
			$fldname =~ s/^\s+//;
			$fldname =~ s/\s+$//;
			if (!defined($fldval) || length($fldval) <= 0) {
				$self->{print}->("parseinfile: skipping OPTIONAL field=${fldname}\n");
				next;
			}
			$fldval =~ s/\s+//g;
			$fldid =~ s/\s+//g;
			$self->{print}->("parseinfile: OPTIONAL field=${fldname}\n");
			$self->{print}->("parseinfile: OPTIONAL value=<${fldval}>\n");
			#
			$optional->[$fldno]{id} = $fldid;
			for (my $i=0; $fldval =~ /(..)/g; ${i}++) {
				$optional->[$fldno]{data}[$i] = hex($1);
			}
			#
			$fldno += 1;
			next;
		} else {
			$self->{print}->("parseinfile: unknown cmd: $crec \n");
			close(ISUPMSGSFILE);
			return(0);
		}
	}
	# all done
	close(ISUPMSGSFILE);
	return(1);
}
#
# exit with success
#
1;

