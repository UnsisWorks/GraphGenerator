#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int numNodes = 5;

void createGrafo() {

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

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *mainWindow, *fixed, *entryBox, *background;
    GtkCssProvider *cssProvider;

    background = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    entryBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    fixed = gtk_fixed_new();
    
    GtkWidget *node;
    for (int i = 0; i < numNodes; i++) {
        node = gtk_entry_new();
        gtk_widget_set_size_request(GTK_WIDGET(node), 10, 25);
        gtk_box_pack_start(GTK_BOX(entryBox), node, TRUE, TRUE, 0);
        gtk_style_context_add_class(gtk_widget_get_style_context(node), "entry-node");

    }
    
    mainWindow = gtk_application_window_new(app);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW (mainWindow), "Generar nuevo arbol");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 820, 500);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), TRUE);
    

    // gtk_style_context_add_class(gtk_widget_get_style_context(buttonAcer), "button");

    // Load CSS file
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(cssProvider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_container_add(GTK_CONTAINER(background), entryBox);
    gtk_container_add(GTK_CONTAINER(mainWindow), background);
    gtk_widget_show_all (mainWindow);
}

// funtion main
void main (int argc, char **argv) {
    system("clear");
    GtkApplication *app;
    int status;
    app = gtk_application_new ("org.gtk.music", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

}
// Compiler gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` 
