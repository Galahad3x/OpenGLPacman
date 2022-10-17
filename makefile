OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: pacman

mac: $(OBJS)
	g++ -std=c++17 $(OBJS) -o pacman -framework GLUT -framework OpenGL

pacman: $(OBJS)
	g++ -std=c++17 $(OBJS) -o $@ -lglut -lGLU -lGL -lm

%.o: %.cpp %.h

clean:
	rm *.o
