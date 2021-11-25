UNAME_S := $(shell uname)
OS := Darwin
main:
ifeq ($(UNAME_S), $(OS))
	@echo "macOS system"
	gcc -framework OpenGL -framework GLUT -ljpeg main.cpp -o main -std=c++11  -pthread
else
	echo "Other system"
	g++ main.cpp -o main -lglut -lGLU -lGL -lm -ljpeg
#gcc maze.cpp -o maze -lglut -lGLU -lGL -lm 
endif
clean:
	rm -f main
	
test:
	./main
all: 
	make clean 
	make
	make test