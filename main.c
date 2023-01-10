#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

void main() {

    int numNodes = 3;
    char name[] = "Holaaa";
    char shapeForm[] = "egg";
    char style[] = "filled";
    char fillColor[] = "#c81313";

    char bgColor[] = "#00ff0d";
    char pieGrafo[] = "Matriz disperza";
    char edgeDir[] = "both";

    FILE *arbol = fopen("./data.dot", "w");

    fprintf(arbol, "digraph %s {\n\tnode[shape = %s fillcolor = \"%s\" style = filled]\n", name, shapeForm, fillColor);

    fprintf(arbol, "\tlabel = \"%s\"\n", pieGrafo);
    fprintf(arbol, "\tbgcolor = \"%s\"\n\n", bgColor);

    fprintf(arbol, "\tsubgraph subGraph {\n");
    fprintf(arbol, "\t\traiz[label = \"%s\"]\n", "Raiz");
    fprintf(arbol, "\t\tedge[dir = \"%s\"]\n", edgeDir);

    // Create nodes needed
    for (int i = 0; i < numNodes; i++) {
        fprintf(arbol, "\t\tfila%d[label = \"%s\" group=1]\n", i, "Vector");
    }
    
    fprintf(arbol, "\n\t\traiz -> fila0\n");  
    
    for (int i = 0; i < numNodes - 1; i++) {
        fprintf(arbol, "\t\tfila%d -> fila%d\n", i, i + 1); 
        
    }
    
    // fprintf(arbol, "");  

    fprintf(arbol, "\n\t}");
    fprintf(arbol, "\n}");
    fclose(arbol);
}

void createGrafo(char name[], char style[], int numNodes, char bgColor[], char pieGrafo[]) {

}


// Compiler gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` 
