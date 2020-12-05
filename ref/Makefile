PRG=gnu.exe
OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

GCC=g++
GCCFLAGS=-O3 -std=c++11 -pedantic
DEFINE=

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=pl.cpp
DRIVER0=driver.cpp

gcc0:
	g++ $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -o $(PRG)
0 1 2 3 4 5 6 7 8 9:
	@echo "should run in less than 1000 ms"
	./$(PRG) $@ >studentout$@
	@echo "lines after the next are mismatches with master output -- see out$@"
	diff out$@ studentout$@ $(DIFF_OPTIONS)
clean:
	rm -f *.exe *.o *.obj
