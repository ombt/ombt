


################################################################
#
# Subdirectories for building
#
################################################################

error:
	@echo "+-----------------------------------------------------------------+"
	@echo "|                                                                 |"
	@echo "|              MV++ Matrix/Vector C++ Class Library               |"	
	@echo "|                                                                 |"
	@echo "| Usage: make all              install and test MV++              |"
	@echo "|                                                                 |"
	@echo "|        make mvlib            install non-templated MV++ library |"
	@echo "|        make mvtest           run non-templated test suite       |"
	@echo "|        make clean            clean *.o and test executables     |"
	@echo "|                                                                 |"
	@echo "|  Make sure the system-specific makefile.def has been edited     |"
	@echo "|  to reflect your system configuration.                          |"
	@echo "|                                                                 |"
	@echo "+-----------------------------------------------------------------+"
	
all: mvlib mvtest done


mvlib:
	cd ./src; make;
	
	

mvtest:
	cd ./testing; make;
	cd ./testing; mvtest > mvtest.out;
	@echo "  "
	@echo " +---------------------------------------------------------------+"
	@echo " |                                                               |"
	@echo " |  MV++ test complete.  Trace is in <mv++>/testing/mvtest.out   |"
	@echo " |                                                               |"
	@echo " +---------------------------------------------------------------+"
	@echo "  "

clean:
	cd ./src; make clean;
	cd ./testing; make clean;

wipe:
	cd ./src; make wipe;
	cd ./testing; make wipe;

done:
	@echo "  "
	@echo " +---------------------------------------------------------------+"
	@echo " |                                                               |"
	@echo " |                     MV++ installed.                           |"
	@echo " |                                                               |"
	@echo " +---------------------------------------------------------------+"
	@echo "  "
	@echo "  "
