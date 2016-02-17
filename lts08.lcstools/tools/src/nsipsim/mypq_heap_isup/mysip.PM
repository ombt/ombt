# SIP msg encoding/decoding class
#
# Header field          where   proxy ACK BYE CAN INV OPT REG
# ___________________________________________________________
# Accept                  R            -   o   -   o   m*  o
# Accept                 2xx           -   -   -   o   m*  o
# Accept                 415           -   c   -   c   c   c
# Accept-Encoding         R            -   o   -   o   o   o
# Accept-Encoding        2xx           -   -   -   o   m*  o
# Accept-Encoding        415           -   c   -   c   c   c
# Accept-Language         R            -   o   -   o   o   o
# Accept-Language        2xx           -   -   -   o   m*  o
# Accept-Language        415           -   c   -   c   c   c
# Alert-Info              R      ar    -   -   -   o   -   -
# Alert-Info             180     ar    -   -   -   o   -   -
# Allow                   R            -   o   -   o   o   o
# Allow                  2xx           -   o   -   m*  m*  o
# Allow                   r            -   o   -   o   o   o
# Allow                  405           -   m   -   m   m   m
# Authentication-Info    2xx           -   o   -   o   o   o
# Authorization           R            o   o   o   o   o   o
# Call-ID                 c       r    m   m   m   m   m   m
# Call-Info                      ar    -   -   -   o   o   o
# Contact                 R            o   -   -   m   o   o
# Contact                1xx           -   -   -   o   -   -
# Contact                2xx           -   -   -   m   o   o
# Contact                3xx      d    -   o   -   o   o   o
# Contact                485           -   o   -   o   o   o
# Content-Disposition                  o   o   -   o   o   o
# Content-Encoding                     o   o   -   o   o   o
# Content-Language                     o   o   -   o   o   o
# Content-Length                 ar    t   t   t   t   t   t
# Content-Type                         *   *   -   *   *   *
# CSeq                    c       r    m   m   m   m   m   m
# Date                            a    o   o   o   o   o   o
# Error-Info           300-699    a    -   o   o   o   o   o
# Expires                              -   -   -   o   -   o
# From                    c       r    m   m   m   m   m   m
# In-Reply-To             R            -   -   -   o   -   -
# Max-Forwards            R      amr   m   m   m   m   m   m
# Min-Expires            423           -   -   -   -   -   m
# MIME-Version                         o   o   -   o   o   o
# Organization                   ar    -   -   -   o   o   o
# Priority            R          ar    -   -   -   o   -   -
# Proxy-Authenticate 407         ar    -   m   -   m   m   m
# Proxy-Authenticate 401         ar    -   o   o   o   o   o
# Proxy-Authorization R          dr    o   o   -   o   o   o
# Proxy-Require       R          ar    -   o   -   o   o   o
# Record-Route        R          ar    o   o   o   o   o   -
# Record-Route     2xx,18x       mr    -   o   o   o   o   -
# Reply-To                             -   -   -   o   -   -
# Require                        ar    -   c   -   c   c   c
# Retry-After  404,413,480,486         -   o   o   o   o   o
#                  500,503             -   o   o   o   o   o
#                  600,603             -   o   o   o   o   o
# Route               R          adr   c   c   c   c   c   c
# Server              r                -   o   o   o   o   o
# Subject             R                -   -   -   o   -   -
# Supported           R                -   o   o   m*  o   o
# Supported          2xx               -   o   o   m*  m*  o
# Timestamp                            o   o   o   o   o   o
# To                c(1)          r    m   m   m   m   m   m
# Unsupported         0                -   m   -   m   m   m
# User-Agent                           o   o   o   o   o   o
# Via                 R          amr   m   m   m   m   m   m
# Via                rc          dr    m   m   m   m   m   m
# Warning             r                -   o   o   o   o   o
# WWW-Authenticate   401         ar    -   m   -   m   m   m
# WWW-Authenticate   407         ar    -   o   -   o   o   o
#
package mysip;
#
use Carp;
#
# msg/parameter encoding/decoding functions
#
sub all {
	confess "barf: $!";
}
sub ack {
	confess "barf: $!";
}
sub bye {
	confess "barf: $!";
}
sub cancel {
	confess "barf: $!";
}
sub invite {
	confess "barf: $!";
}
sub option {
	confess "barf: $!";
}
sub register {
	confess "barf: $!";
}
sub refer {
	confess "barf: $!";
}
sub notify {
	confess "barf: $!";
}
sub info {
	confess "barf: $!";
}
sub subscribe {
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
		sip => {
			msgs => {
			ALL => {
				name => "ALL",
				codec => \&all,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Authorization" => {
						name => "Authorization",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"Date" => {
						name => "Date",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Route" => {
						name => "Route",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
				},
			},
			ACK => {
				name => "ACK",
				codec => \&ack,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Authorization" => {
						name => "Authorization",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"Date" => {
						name => "Date",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Route" => {
						name => "Route",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
				},
			},
			BYE => {
				name => "BYE",
				codec => \&bye,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			CANCEL => {
				name => "CANCEL",
				codec => \&cancel,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			INVITE => {
				name => "INVITE",
				codec => \&invite,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			OPTION => {
				name => "OPTION",
				codec => \&option,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			REGISTER => {
				name => "REGISTER",
				codec => \&register,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			REFER => {
				name => "REFER",
				codec => \&refer,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			NOTIFY => {
				name => "NOTIFY",
				codec => \&notify,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			INFO => {
				name => "INFO",
				codec => \&info,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
			},
			SUBSCRIBE => {
				name => "SUBSCRIBE",
				codec => \&subscribe,
				mandatory => {
					"Call-ID" => {
						name => "Call-ID",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"From" => {
						name => "From",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"To" => {
						name => "To",
					},
					"Via" => {
						name => "Via",
					},
				},
				optional => {
					"Accept" => {
						name => "Accept",
					},
					"Accept-Encoding" => {
						name => "Accept-Encoding",
					},
					"Accept-Language" => {
						name => "Accept-Language",
					},
					"Alert-Info" => {
						name => "Alert-Info",
					},
					"Allow" => {
						name => "Allow",
					},
					"Authentication-Info" => {
						name => "Authentication-Info",
					},
					"Authorization" => {
						name => "Authorization",
					},
					"Call-ID" => {
						name => "Call-ID",
					},
					"Call-Info" => {
						name => "Call-Info",
					},
					"Contact" => {
						name => "Contact",
					},
					"Content-Disposition" => {
						name => "Content-Disposition",
					},
					"Content-Encoding" => {
						name => "Content-Encoding",
					},
					"Content-Language" => {
						name => "Content-Language",
					},
					"Content-Length" => {
						name => "Content-Length",
					},
					"Content-Type" => {
						name => "Content-Type",
					},
					"CSeq" => {
						name => "CSeq",
					},
					"Date" => {
						name => "Date",
					},
					"Error-Info" => {
						name => "Error-Info",
					},
					"Expires" => {
						name => "Expires",
					},
					"From" => {
						name => "From",
					},
					"In-Reply-To" => {
						name => "In-Reply-To",
					},
					"Max-Forwards" => {
						name => "Max-Forwards",
					},
					"MIME-Version" => {
						name => "MIME-Version",
					},
					"Min-Expires" => {
						name => "Min-Expires",
					},
					"Organization" => {
						name => "Organization",
					},
					"Priority" => {
						name => "Priority",
					},
					"Proxy-Authenticate" => {
						name => "Proxy-Authenticate",
					},
					"Proxy-Authorization" => {
						name => "Proxy-Authorization",
					},
					"Proxy-Require" => {
						name => "Proxy-Require",
					},
					"Record-Route" => {
						name => "Record-Route",
					},
					"Reply-To" => {
						name => "Reply-To",
					},
					"Require" => {
						name => "Require",
					},
					"Retry-After" => {
						name => "Retry-After",
					},
					"Route" => {
						name => "Route",
					},
					"Server" => {
						name => "Server",
					},
					"Subject" => {
						name => "Subject",
					},
					"Supported" => {
						name => "Supported",
					},
					"Timestamp" => {
						name => "Timestamp",
					},
					"To" => {
						name => "To",
					},
					"Unsupported" => {
						name => "Unsupported",
					},
					"User-Agent" => {
						name => "User-Agent",
					},
					"Via" => {
						name => "Via",
					},
					"Warning" => {
						name => "Warning",
					},
					"WWW-Authenticate" => {
						name => "WWW-Authenticate",
					},
				},
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

