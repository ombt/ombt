SUBDIRS = lib \
	suid \
	auto_mkindex \
	pkg_mkindex \
	ama \
	getspips \
	automount \
	lcsstat \
	reload \
	saveload \
	set-bts-server \
	setup_lcs \
	tl1exec \
	lcsksh \
	load2images \
	showvars \
	rtrv-eqpt \
	dbquery \
	copyscripts  \
	tl1sh \
	mybc \
	ip2bip \
	hardware \
	sipsim \
	lockadm \
	nsipsim \
	tarlogs \
	misc \
	postlcsreload \
	hug \
	func \
	cronjobs \
	gench2ioms \
	webtools \
	syncfs \
	ptalog \
	$(ENDLIST)

all:
	for SUBDIR in $(SUBDIRS); do \
		echo "make $$SUBDIR"; \
		(cd $$SUBDIR; make; ) ; \
	done

mymk:
	for SUBDIR in $(SUBDIRS); do \
		echo "mymk $$SUBDIR"; \
		(cd $$SUBDIR; make mymk; ) ; \
	done

backup:
	for SUBDIR in $(SUBDIRS); do \
		echo "backup $$SUBDIR"; \
		(cd $$SUBDIR; make backup; ) ; \
	done

backupd:
	for SUBDIR in $(SUBDIRS); do \
		echo "backup -d $$SUBDIR"; \
		(cd $$SUBDIR; make backupd; ) ; \
	done

clean:
	for SUBDIR in $(SUBDIRS); do \
		echo "clean $$SUBDIR"; \
		(cd $$SUBDIR; make clean; ) ; \
	done
