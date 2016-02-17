all:	
	hw2chassis -D >Dhwchassis
	hw2chassis >hwchassis
	hw2ioms -D >Dhwioms
	hw2ioms >hwioms
	hw2enet -D >Dhwenet
	hw2enet >hwenet
	# hw2tdm -D >Dhwtdm
	# hw2tdm >hwtdm
	#
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req Main >chassis2ioms
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req BRANCH-DEV-6-2-1 >>chassis2ioms
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req BRANCH-6-2-0 >>chassis2ioms
	uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms >chassis2ioms
	cat NONTL1chassis2ioms >>chassis2ioms
	sort -u -o chassis2ioms chassis2ioms
	uprint chassis2ioms where type req UNKNOWN

hw2chassis:
	hw2chassis -D >Dhwchassis
	hw2chassis >hwchassis

hw2ioms:
	hw2ioms -D >Dhwioms
	hw2ioms >hwioms
	#
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req Main >chassis2ioms
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req BRANCH-DEV-6-2-1 >>chassis2ioms
	# uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms where branch req BRANCH-6-2-0 >>chassis2ioms
	uprintf -f"%s\t%s\t%s\t%s\n" labid branch iom type in hwioms >chassis2ioms
	cat NONTL1chassis2ioms >>chassis2ioms
	sort -u -o chassis2ioms chassis2ioms
	uprint chassis2ioms where type req UNKNOWN

hw2enet:
	hw2enet -D >Dhwenet
	hw2enet >hwenet

# hw2tdm:
	# hw2tdm -D >Dhwtdm
	# hw2tdm >hwtdm
