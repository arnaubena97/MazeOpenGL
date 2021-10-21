UNAME_S := $(shell uname)
OS := Darwin
main:
ifeq ($(UNAME_S), $(OS))
	@echo "macOS system"
	gcc -framework OpenGL -framework GLUT -ljpeg main.cpp -o main -lstdc++ 
else
	echo "Other system"
	g++ main.cpp -o main -lglut -lGLU -lGL -lm
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