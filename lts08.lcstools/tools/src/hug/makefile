SUBDIRS = libsrc \
	src \
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

