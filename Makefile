CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -O3

all: InvenTrack.exe

clean:
	rm -f InvenTrack.exe

run: InvenTrack.exe
	./$<

%.exe: %.cpp
	c++-12 $(CXXFLAGS) -o $@ $<