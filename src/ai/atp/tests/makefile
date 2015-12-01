SOPT = -S all -VE
IOPT = -I ..
MOREOPTS = 

.SUFFIXES:	.t

.t:
	atp $(MOREOPTS) $(IOPT) $(SOPT) $<

all:
	-(rm -rf atp_temp)
	#
	for var in *.t ; \
	do \
		atp $(MOREOPTS) $(IOPT) $(SOPT) $$var ; \
	done
	#

clean:
	-(rm -rf atp_temp)
	
