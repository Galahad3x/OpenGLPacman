OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: pacman

pacman: $(OBJS)
	echo $(OBJS)
	g++ $(OBJS) -o $@

%.o: %.cpp %.h

clean:
	rm *.o
