CPPFLAGS=-Wall -std=c++11 -O3

pins: pins.cc

debug:
	$(CPP) $(CPPFLAGS) -D DEBUG pins.cc -o pins-debug
