CXX      = g++
CXXFLAGS = -std=c++2a -Weffc++ -Wshadow -Wall -Wextra -g3
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


snake: main.o snake_game.o snake.o termfuncs.o
	$(CXX) -o $@ $^ $(LIBS)

%.o: src/%.cpp $(shell echo src/*.h)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f findpath *.o

# Add some rules for rt website
#include Makefile.publish