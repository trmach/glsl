CC = clang++
CFLAGS = `pkg-config --cflags glfw3` -Ofast -Wall
LIBS = `pkg-config --static --libs glfw3` 

main: main.cpp shader.h
	$(CC) -o main main.cpp glad.cpp $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm -r main
