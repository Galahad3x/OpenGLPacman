pacman: pacman.cpp
	gcc pacman.cpp -o pacman -lglut -lGLU -lGL -lm
clean:
	rm chess
