CC=g++
CFLAGS=-g 
CXXFLAGS=-g
CPPFLAGS= -I../include
LDFLAGS= -L../lib
COMPILECPP= warray.cpp wbitset.cpp wdynamichash.cpp wlist.cpp wstd.cpp wstring.cpp
all:test 
test:test.cpp ${COMPILECPP} 
clean:
	@rm test

