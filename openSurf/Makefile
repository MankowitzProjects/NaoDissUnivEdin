# Indicates compilator to use
CC      = g++

# Specifies compilator options
CFLAGS  = -O0 -g -Wall `pkg-config --cflags opencv` -D LINUX 
LDFLAGS =
LDLIBS  = `pkg-config --libs opencv`

# Files extensions .cpp, .o
SUFFIXES = .cpp .o
.SUFFIXES: $(SUFFIXES) .

# Name of the main program
#PROG  = surf
PROG = surf
PROG1 = thresholds
PROG2 = matches

# Name of the library
LIB = libsurf.so

# Object files .o necessary to build the main program
OBJS  = fasthessian.o integral.o robotImplementationOpenSURF.o surf.o utils.o ipoint.o OSFeatureExtraction.o
OBJS1  = fasthessian.o integral.o openSURFThresholds.o surf.o utils.o ipoint.o OSFeatureExtraction.o DataAnalysis.o
OBJS2  = fasthessian.o integral.o openSURFCompareMatches.o surf.o utils.o ipoint.o OSFeatureExtraction.o DataAnalysis.o

# Object files .o necessary to build the library
LIBOBJS  = fasthessian.o integral.o surf.o utils.o ipoint.o OSFeatureExtraction.o DataAnalysis.o

all: $(PROG) $(LIB)
all: $(PROG1) $(LIB)
all: $(PROG2) $(LIB)


# Compilation and link
$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

$(PROG1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $(PROG1) $(OBJS1) $(LDLIBS)

$(PROG2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $(PROG2) $(OBJS2) $(LDLIBS)

$(LIB): $(LIBOBJS)
	$(CC) $(LIBOBJS) -o $(LIB) -ldl -shared -fPIC

%.o: $(addprefix ./, %.cpp)
	$(CC)  $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(PROG)
	-rm -f $(PROG1)
	-rm -f $(PROG2)
	-rm -f $(LIB)
	-rm -f *.o
