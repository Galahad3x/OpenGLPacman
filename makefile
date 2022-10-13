OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: pacman

pacman: $(OBJS)
	g++ -std=c++17 $(OBJS) -o $@

%.o: %.cpp %.h

clean:
	rm *.o
