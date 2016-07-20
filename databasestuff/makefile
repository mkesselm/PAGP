OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := /lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := /lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := lib/libLeap.dylib
endif

main: main.o driver.o database.o
	g++ -Wall -g main.cpp driver.o database.o -o Main $(LEAP_LIBRARY) -lsqlite3


database.o: database.cpp database.h
	g++ -c database.cpp

driver.o: driver.cpp driver.h
		g++ -c driver.cpp


clean:
	rm -rf Main *.o
