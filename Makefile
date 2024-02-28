all:
	make basic glarea
	
basic:
	gcc -o basic.out cmd/basic.c -lm `pkg-config --cflags gtk4 epoxy --libs gtk4 epoxy`

glarea:
	gcc -o glarea.out cmd/glarea/shaders.c cmd/glarea/draw.c cmd/glarea/glarea.c -lm `pkg-config --cflags gtk4 epoxy --libs gtk4 epoxy` -I./cmd/glarea -I./include

clean:
	rm -f ./glarea
	rm -f ./basic
	
