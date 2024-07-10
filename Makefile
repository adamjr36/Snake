CXX      = g++
CXXFLAGS = -std=c++2a -Weffc++ -Wshadow -Wall -Wextra -g3


snake: main.o snake_game.o snake.o termfuncs.o
	$(CXX) -o $@ $^

%.o: %.cpp $(shell echo *.h)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f findpath *.o

# Add some rules for rt website
#include Makefile.publish
