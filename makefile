OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

pacman: $(OBJS)
	g++ $(OBJS) -o $@

%.o: %.cpp %.h

clean:
	rm *.c
