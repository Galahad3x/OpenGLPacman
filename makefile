OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: pacman

pacman: $(OBJS)
ifeq ($(shell uname -s), Darwin)
	g++ -std=c++17 $(OBJS) -o pacman -framework GLUT -framework OpenGL -ljpeg

else
	g++ -std=c++17 $(OBJS) -o $@ -lglut -lGLU -lGL -lm -ljpeg
endif


%.o: %.cpp %.h

clean:
	rm *.o
