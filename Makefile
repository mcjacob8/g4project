# This Makefile builds a stand alone program that has access to 
# the ROOT libraries. 

# here we access the root configuration, include files, and libraries
ROOTCFLAGS=$(shell root-config --cflags)
ROOTINC=$(shell root-config --incdir)
ROOTLIBDIR=$(shell root-config --libdir)
ROOTLIBS=$(shell root-config --libs) -lMinuit
ROOTLDFLAGS=$(shell root-config --ldflags)

ROOTC=$(ROOTCFLAGS) 
#-I$(ROOTINC)
ROOTLINK=-L$(ROOTLIBDIR) $(ROOTLIBS) $(ROOTLDFLAGS)

CPP=g++

default: Histograms

Histograms: Histograms.cpp
	$(CPP) -O -Wall $(ROOTC) -o Histograms Histograms.cpp $(ROOTLINK) 

# note: just replace the -O flag with -g to build a debug version

clean: 
	rm -f Histogramss *~ *.d *.so