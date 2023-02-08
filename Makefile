# La siguiente no es necesariamente requerida, se agrega para
# mostrar como funciona.

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
CFLAGSEND = `pkg-config --libs gtk+-3.0`
SRC = main.c 
OBJ = arbol.o 


# Reglas explicitas

all: $(OBJ)
	$(CC) $(CFLAGS) -o main main.c $(CFLAGSEND)
clean:
	$(RM) $(OBJ) main

# Reglas implicitas

arbol.o: main.c 

