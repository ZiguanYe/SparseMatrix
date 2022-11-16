CXX := g++
CXXFLAGS := -g -Wall --std=c++11

VALGRIND := valgrind --tool=memcheck --leak-check=yes

all: release

debug: CXXFLAGS += -DSPARSE_DEBUG
debug: clean spmat-test-debug

release: CXXFLAGS := -g -Wall --std=c++11
release: clean spmat-test


spmat-test-debug: spmat-test.o spmat.o spmat-debug.o
	$(CXX) $(CXXFLAGS) $^ -o spmat-test

spmat-test: spmat-test.o spmat.o
	$(CXX) $(CXXFLAGS) $^ -o spmat-test

# generic rule for .cpp files
spmat.o : spmat.cpp spmat.h spmat-debug.h
spmat-debug.o : spmat-debug.cpp spmat-debug.h spmat.h
spmat-test.o : spmat-test.cpp spmat.h

clean:
	rm -f *.o spmat-test

.PHONY: clean debug release