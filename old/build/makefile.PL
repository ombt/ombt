ifndef ROOT
$(error ROOT is not set)
endif

#ifndef SHELLSCRIPTS
#	$(error SHELLSCRIPTS is not set)
#endif

% : %.PL
	cp $< $@

BINDIR = $(ROOT)/bin

all:	install

install: $(SHELLSCRIPTS)
		-test -d $(BINDIR) || mkdir -p $(BINDIR);
		-$(foreach script, $(SHELLSCRIPTS), cp $(script).PL $(BINDIR)/$(script); )
		-$(foreach script, $(SHELLSCRIPTS), chmod 755 $(BINDIR)/$(script); )
		touch install

clean:
		-rm install
		-$(foreach script, $(SHELLSCRIPTS), rm -f $(script); )
		-$(foreach script, $(SHELLSCRIPTS), rm -f $(BINDIR)/$(script); )

backup:
		bkup

backupd:
		bkup -d
