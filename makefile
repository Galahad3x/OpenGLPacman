OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: pacman

pacman: $(OBJS)
	g++ -std=c++17 $(OBJS) -o $@ -lglut -lGLU -lGL -lm

%.o: %.cpp %.h

clean:
	rm *.o
