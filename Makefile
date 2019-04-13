#
# GENESIS makefile
#

.PHONY: clean install beta

# libraries
#
LIB= -lm -lstdc++ -lmpi_cxx -lfftw3
#
INCLUDE=-I./include
#
# cpp-macros - to enable the FFTW library
#
DMACRO = -DFFTW
#
#  compilers
#
VPATH = src/Core src/IO src/Lattice src/Util src/Main src/Loading
CCOMPILER = h5pcc
#CCOMPILER = vtcxx -vt:cxx h5pcc -vt:inst manual -DVTRACE
#
#  executable name 
#
EXECUTABLE = gencore
#
# targets
#
OBJECTS = Sorting.o BesselJ.o Inverfc.o Hammerslay.o RandomU.o GaussHermite.o StringProcessing.o Track.o Setup.o AlterSetup.o Time.o Wake.o Parser.o Dump.o SponRad.o EField.o LoadBeam.o ImportBeam.o LoadField.o ImportField.o Profile.o ShotNoise.o QuietLoading.o Optics.o Lattice.o LatticeElements.o LatticeParser.o AlterLattice.o Gencore.o TrackBeam.o Control.o Field.o FieldSolver.o EFieldSolver.o Incoherent.o Collective.o Beam.o BeamSolver.o Undulator.o HDF5base.o readBeamHDF5.o writeBeamHDF5.o readFieldHDF5.o writeFieldHDF5.o SDDSBeam.o Output.o  GenMain.o build_info.o

genesis:	$(OBJECTS)
	ar -cvq libgenesis13.a $(OBJECTS)
	mv libgenesis13.a ./lib
	$(CCOMPILER) src/Main/mainwrap.cpp -o $(EXECUTABLE) $(INCLUDE) $(LIB) -lgenesis13 -Llib

genesisexecutable:	$(OBJECTS)
	$(CCOMPILER)  -o $(EXECUTABLE) $(OBJECTS) $(LIB)



.cpp.o:
	$(CCOMPILER) -O2 -c $(DMACRO) $(INCLUDE) $<

# ALWAYS update the file informing them from which git commit the binary was made
build_info.o: FORCE
	rm -f build_info.cpp build_info.o
	./build_info.sh	
	$(CCOMPILER) -O2 -c $(DMACRO) $(INCLUDE) build_info.cpp
# so-called "force target", see GNU make manual "rules without recipes or prerequisites"
FORCE:
	

clean:
	rm -f src/Core/*~
	rm -f src/IO/*~
	rm -f src/Lattice/*~
	rm -f src/Loading/*~
	rm -f src/Main/*~
	rm -f src/Util/*~
	rm -f include/*~
	rm -f *.o
	rm -f lib/*.a
	rm -f build_info.cpp
	rm -f $(EXECUTABLE)

install:
	cp ./$(EXECUTABLE) ~/bin/genesis4

beta:
	cp ./$(EXECUTABLE) ~/bin/$(EXECUTABLE).beta











