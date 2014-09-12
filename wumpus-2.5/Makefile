# Makefile
global_flags= -g -W -Wall -Wextra -pedantic -Werror
CFLAGS:= $(CFLAGS) $(global_flags) -std=c99
CXXFLAGS:= $(CXXFLAGS) $(global_flags) -std=c++0x
OBJECTS = Action.o Agent.o Location.o Orientation.o Percept.o WumpusWorld.o
# all: wumpsim
wumpsim.app: wumpsim.h wumpsim.cc $(OBJECTS)
	$(CXX) -o $@ wumpsim.cc $(OBJECTS) $(CXXFLAGS)
Action.o: Action.h Action.cc
Agent.o: Agent.h Agent.cc
Location.o: Location.h Location.cc
Orientation.o: Orientation.h Orientation.cc
Percept.o: Percept.h Percept.cc
WumpusWorld.o: WumpusWorld.h WumpusWorld.cc
clean:
	rm -f *.o *.app
