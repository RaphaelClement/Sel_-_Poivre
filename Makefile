all: main

main: main.o personne.o ressource.o menu.o
	gcc -o main main.c personne.c ressource.c menu.c -ljson-c -lcrypt

main.o: main.c personne.h ressource.h menu.h
	gcc -c main.c

personne.o: personne.c personne.h
	gcc -c personne.c

ressource.o: ressource.c ressource.h
	gcc -c ressource.c

menu.o: menu.c menu.h personne.h ressource.h
	gcc -c menu.c

clean:
	rm -f *.o main
