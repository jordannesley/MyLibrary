IDIR=Headers
CC=g++

IDIR=Headers/
SDIR=Main/ DebugLogger/ GeneticAlgorithm/ GuassianMixtureModel/ HiddenMarkovModel/ Utilities/
ODIR=obj

SRCFILES=$(addsuffix *.cpp,$(SDIR))

CFLAGS= $(SRCFILES) -I$(IDIR)

default: $(SRCFILES)
	$(CC) -o test_executable $(CFLAGS) 
	
#make: $(OBJ)
#	$(CC) -o $@ $^ $(CFLAGS) 
