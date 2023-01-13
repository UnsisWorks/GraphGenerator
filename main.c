#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int numNodes = 4;

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
    GtkWidget *mainWindow, *fixed, *entryBox, *background, *title, *labelEntrys, *comboBoxCreate;
    GtkCssProvider *cssProvider;

    // Create containers for windows
    background = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    entryBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    fixed = gtk_fixed_new();
    gtk_widget_set_margin_start(GTK_WIDGET(entryBox), 25);
    gtk_widget_set_margin_end(GTK_WIDGET(entryBox), 25);

    // Create labels for windows
    title = gtk_label_new("CREAR NUEVO ARBOL");
    gtk_box_pack_start(GTK_BOX(background), title, FALSE, FALSE, 5);
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "label-title");

    labelEntrys = gtk_label_new("Elementos para el arbòl");
    gtk_style_context_add_class(gtk_widget_get_style_context(labelEntrys), "label-entrys");
    gtk_fixed_put(GTK_FIXED(fixed), labelEntrys, 40, 0);
    
    // Create entrys for the nodes values
    GtkWidget *node;
    for (int i = 0; i < numNodes; i++) {
        node = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(node), 10);
        gtk_widget_set_vexpand_set(GTK_WIDGET(node), TRUE);
        gtk_widget_set_size_request(GTK_WIDGET(node), 20, -1);
        gtk_widget_set_opacity(GTK_WIDGET(node), 0.5);
        gtk_box_pack_start(GTK_BOX(entryBox), node, TRUE, TRUE, 0);
        gtk_style_context_add_class(gtk_widget_get_style_context(node), "entry-node");

    }

    // Create button for create three with diferents recorridos
    comboBoxCreate = gtk_combo_box_new();
    gtk_combo_box_append(GTK_COMBO_BOX(comboBoxCreate), 0, "\t   Crear");
    gtk_combo_box_append(GTK_COMBO_BOX(comboBoxCreate), 0, "\t  Preorden");
    gtk_combo_box_append(GTK_COMBO_BOX(comboBoxCreate), 0, "\t  Orden");
    gtk_combo_box_append(GTK_COMBO_BOX(comboBoxCreate), 0, "\tPost-orden");
    gtk_combo_box_set_
    gtk_combo_box_set_active(GTK_COMBO_BOX(comboBoxCreate) 0);

    gtk_fixed_put(GTK_FIXED(fixed), comboBoxCreate, 350, 150);

    // gtk_box_pack_end(GTK_BOX(background), comboBoxCreate, FALSE, FALSE, 0);

    // Create window for the capture values the nodes and selection the type order
    mainWindow = gtk_application_window_new(app);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW (mainWindow), "Generar nuevo arbol");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 820, 500);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), TRUE);


    // gtk_style_context_add_class(gtk_widget_get_style_context(buttonAcer), "button");
    gtk_widget_set_name(GTK_WIDGET(background), "background");
    // Load CSS file
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(cssProvider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_fixed_put(GTK_FIXED(fixed), entryBox, 0, 35);
    gtk_container_add(GTK_CONTAINER(background), fixed);
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
