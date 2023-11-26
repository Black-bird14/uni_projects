# The following just specifies some variables for "flexibility".

# Specify the C++ compiler
CXX     = g++

# Specify options to pass to the compiler. Here it sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main FlexArrayTesterMain

# These are the two executables to be produced
main: main.cpp FlexArray.o
	$(CXX) $(CXXFLAGS) main.cpp FlexArray.o -o main

FlexArrayTesterMain: FlexArrayTesterMain.cpp FlexArray.o FlexArrayTester.o
	$(CXX) $(CXXFLAGS) FlexArrayTesterMain.cpp FlexArray.o FlexArrayTester.o -o FlexArrayTesterMain

# These are the "intermediate" object files
# The -c command produces them
FlexArray.o: FlexArray.cpp FlexArray.h
	$(CXX) $(CXXFLAGS) -c FlexArray.cpp -o FlexArray.o

FlexArrayTester.o: FlexArrayTester.cpp FlexArrayTester.h
	$(CXX) $(CXXFLAGS) -c FlexArrayTester.cpp -o FlexArrayTester.o

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
	rm -f *~ *.o FlexArrayTesterMain main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
