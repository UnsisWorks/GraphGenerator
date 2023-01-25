#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "entry.c"
#include "grapho.c"

typedef struct structWidgets {
    GtkWidget *mainWindow;
    GtkWidget *nameField;
} widgets;

int numNodes;
GtkWidget *node;
struct Node *grapho;
struct Node *loadGrapho;
GtkWidget *window, *cajitaInterior, *showGrapho, *mainWindow;

static void advertencia (GtkWindow *parent, gchar *message) {
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Message",NULL,flags,("_OK"), GTK_RESPONSE_NONE,NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new (message);

    // Ensure that the dialog box is destroyed when the user responds

    g_signal_connect_swapped (dialog, "response",G_CALLBACK (gtk_widget_destroy),dialog);

    // Add the label, and show everything we’ve added

    gtk_container_add (GTK_CONTAINER (content_area), label);
    gtk_widget_show_all (dialog);

}


// static void advertencia (GtkWindow *parent, gpointer user_data) {

// }

void ExportToDot(gint values[numNodes], char pieGrafo[]) {
    char name[] = "Holaaa";
    char shapeForm[] = "egg";
    char style[] = "filled";
    char fillColor[] = "transparent";
    char bgColor[] = "#07836a00";
    char edgeDir[] = "normal";

    FILE *arbol = fopen("./data.dot", "w");

    fprintf(arbol, "digraph %s {\n\tnode[shape = %s fillcolor = \"%s\" style = filled]\n", name, shapeForm, fillColor);

    fprintf(arbol, "\tlabel = \"%s\"\n", pieGrafo);
    fprintf(arbol, "\tbgcolor = \"%s\"\n\n", bgColor);
    fprintf(arbol, "\tnode [shape=none, image=\"./images/Árboles/Esfera3.png\"];\n\n");

    fprintf(arbol, "\tsubgraph tsubgraph1 {\n");
    // fprintf(arbol, "\t\traiz[label = \"%d\"]\n", values[0]);
    fprintf(arbol, "\t\tedge[dir = \"%s\" color = \"#4613c8\"]\n", edgeDir);

    // Create nodes needed
    for (int i = 0; i < numNodes; i++) {
        fprintf(arbol, "\t\tfila%d[label = \"%d\" group=1]\n", i, values[i]);
    }
    
    // Establece los punteros entre nodos. Basado en id y datos TDA
    struct Node *aux;
    puts("antes de preorder");
    preOrder(grapho);
    puts("despues de preorder");
    postOrder(grapho);
    puts("despues de postorder");

    fprintf(arbol, "\n");
    for (int i = 0; i < numNodes; i++) {
        aux = searchHelper(grapho, i);
        if(aux == NULL){
            puts("Nodo nulo");
        } else {

            // Set node rigth
            if ((aux -> right != NULL)) {
                puts("Entro");
                fprintf(arbol, "\t\tfila%d -> fila%d\n", aux -> id, aux -> right -> id);
            }

            // Set node left
            if (aux -> left != NULL) {
                fprintf(arbol, "\t\tfila%d -> fila%d\n", aux -> id, aux -> left -> id);
            }
        }
    }
    
    fprintf(arbol, "\n\t}");
    fprintf(arbol, "\n}");
    fclose(arbol);

    system("dot -Tpng -o data.png data.dot");
}

void createGrafo(GtkWidget *button, widgets *ws) {
    // Get input the user
    GString *input = g_string_new(gtk_entry_get_text(GTK_ENTRY(node)));  

    // Compare string null
    if(strcmp("", input -> str) != 0) {

        numNodes = 1;
        
        // Count number the "-" and get number the nodes
        for (int i = 0; i < input -> len; i++) {
            if (input ->str[i] == '-') {
                numNodes++;
            }   
        }
        // printf("El usuario ingreso %d datos\n", numNodes);

        // Create array for values the nodes
        gint values[numNodes];

        // Save values for each nodes
        int preInput = 0;
        int count = 0;
        GString *aux = g_string_new("");
        for (int i = 0; i < input -> len; i++) {
            if (input -> str[i] == '-') {
                // g_string_assign(aux, "");
                // Chars between the flags "-"
                for (int j = preInput; j < i; j++) {
                    g_string_append_c(aux, input->str[j]);
                    values[count] = atoi(aux->str);
                    preInput = i + 1;
                }
                // printf("Valor: %d : i: %d\n", values[count], i);
                count++;
            }
            g_string_assign(aux, "");
            // Save the ultimate value
            if (count == (numNodes - 1)) {
                for (int j = i + 1; j < input -> len; j++) {
                    g_string_append_c(aux, input->str[j]);
                    values[count] = atoi(aux->str);
                    preInput = i + 1;
                }
                // printf("Valor: %d : i: %d\n", values[count], i);
                break;
            }
        }
        // GString name = g_string_new(gtk_entry_get_text(GTK_ENTRY(name)));
        for (int i = 0; i < numNodes; i++) {
            grapho = insert(grapho, values[i], i);
            // puts("add node");
        }
        inOrder(grapho);
        puts("Order");
        ExportToDot(values, "Pie de grapho");

        loadGrapho = grapho;
        freeAVLTree(grapho);

        // Sets grapho as the main. Change window 
        puts("aqui actual");
        // GtkWidget *mainWindow = GTK_WIDGET(ws -> mainWindow);
        gtk_widget_set_visible(GTK_WIDGET(mainWindow), FALSE);
        gtk_widget_set_name(GTK_WIDGET(showGrapho), "show-grapho");
        gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
    } else {
        puts("Vacia");
    }
}

static void create (GtkWidget *widget, gpointer user_data) {
    free(grapho);
    gtk_widget_set_visible(GTK_WIDGET(window), FALSE);

    GtkWidget *fixed, *entryBox, *background, *title, *labelEntrys, *comboBoxCreate;
    GtkWidget *nameField;
    GtkWidget *buttonCreate, *buttonBoxCreate;
    // GtkCssProvider *cssProvider;
    // text = text_field_new();
    //     gtk_widget_set_size_request(GTK_WIDGET(text), 20, -1);

    // Create containers for windows
    background = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    entryBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    fixed = gtk_fixed_new();
    gtk_widget_set_size_request(GTK_WIDGET(entryBox), 800, -1);
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
    node = text_field_new();
    gtk_widget_set_opacity(GTK_WIDGET(node), 0.5);
    gtk_entry_set_alignment(GTK_ENTRY(node), 0.5);
    gtk_widget_set_focus_on_click(GTK_WIDGET(node), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(node), 30, 5);
    gtk_box_pack_start(GTK_BOX(entryBox), node, TRUE, TRUE, 0);
    gtk_entry_set_placeholder_text(GTK_ENTRY(node), "1-2-3-4-5-6-7-8-9-...-");
    gtk_style_context_add_class(gtk_widget_get_style_context(node), "entry-node");

    nameField = text_field_new();
    gtk_widget_set_opacity(GTK_WIDGET(nameField), 0.5);
    gtk_entry_set_alignment(GTK_ENTRY(nameField), 0.5);
    gtk_fixed_put(GTK_FIXED(fixed), nameField, 340, 150);
    gtk_widget_set_size_request(GTK_WIDGET(nameField), 230, -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(nameField), "Nombre del grafo");
    gtk_style_context_add_class(gtk_widget_get_style_context(nameField), "entry-node");


    // Create button for create three with diferents recorridos
    buttonCreate = gtk_button_new();
    gtk_widget_set_focus_on_click(GTK_WIDGET(buttonCreate), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(buttonCreate), 144, -1);
    buttonBoxCreate = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(buttonBoxCreate), buttonCreate);

    gtk_style_context_add_class(gtk_widget_get_style_context(buttonCreate), "button-create");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonBoxCreate), "button-box-create");

    gtk_fixed_put(GTK_FIXED(fixed), buttonBoxCreate, 348, 240);

    // Create window for the capture values the nodes and selection the type order
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW (mainWindow), "Generar nuevo arbol");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 820, 500);
    gtk_window_set_resizable(GTK_WINDOW(mainWindow), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(mainWindow), 0, -1);

    // Set values for param g_signal
    widgets parameter = {mainWindow, nameField};

    g_signal_connect(buttonCreate, "clicked", G_CALLBACK(createGrafo), &parameter);

    gtk_widget_set_name(GTK_WIDGET(background), "background");
    gtk_widget_set_name(GTK_WIDGET(entryBox), "entry-box");
    gtk_widget_set_name(GTK_WIDGET(fixed), "fixed");

    // Load CSS file
    // cssProvider = gtk_css_provider_new();
    // gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);
    // gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    //                                         GTK_STYLE_PROVIDER(cssProvider),
    //                                         GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_fixed_put(GTK_FIXED(fixed), entryBox, 0, 35);
    gtk_container_add(GTK_CONTAINER(background), fixed);
    gtk_container_add(GTK_CONTAINER(mainWindow), background);
    gtk_widget_show_all (mainWindow);
}

gchar* select_folder_path() {
    GtkWidget *dialog;
    gint res;
    gchar *filename;

    dialog = gtk_file_chooser_dialog_new ("Guardar archivo",
                                        GTK_WINDOW(window),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "_Cancelar", GTK_RESPONSE_CANCEL,
                                        "_Guardar", GTK_RESPONSE_ACCEPT,
                                        NULL);
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "nuevo_grafo.bin");
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        //guardar archivo con la ruta filename
        // g_free (filename);
    }
    gtk_widget_destroy (dialog);

    return filename;
}

void saveTree(struct Node *node, FILE *archivo) {

    fwrite(&node, sizeof(struct Node*), 1, archivo);
    puts("3");
    if (node -> left != NULL) {
        puts("izqui");
        saveTree(node->left, archivo);
    }
    if (node -> right != NULL) {
        puts("derec");
        saveTree(node->right, archivo);
    }
}




static void saveGrapho (GtkApplication* app, gpointer user_data) {
    gchar* path = select_folder_path();

    if((strcmp(path, "") != 0) && (loadGrapho != NULL)){

        // strcat(path, "/grapho.bin");
        printf("path: %s\n", path);

        // Abre el archivo en modo escritura binaria
        FILE *file = fopen(path, "wb");
        if (file == NULL) {
            printf("Error al abrir el archivo");
            // return void;
        } else {

            // Escribe el struct en el archivo
            saveTree(loadGrapho, file);

            // Cierra el archivo
            fclose(file);
            puts("Guardado");
        }
    } else {
            puts("No gardado");
    }

}

gchar* getFileName () {
    GtkWidget *dialog;
    gchar *filename;
    // Crea un diálogo de selección de archivos
    dialog = gtk_file_chooser_dialog_new("Seleccionar archivo",
                                                    GTK_WINDOW(window),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancelar", GTK_RESPONSE_CANCEL,
                                                    "_Abrir", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    // Muestra el diálogo y espera a que el usuario seleccione un archivo
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        // Obtiene la ruta del archivo seleccionado
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }

    // Destruye el diálogo
    gtk_widget_destroy(dialog);
        printf("Archivo seleccionado: %s\n", filename);
    return filename;
}

static void openFile (GtkApplication* app, gpointer user_data) {
    puts("1");
    gchar *path = getFileName();
    puts("2");
    printf("Archivo: %s\n", path);
    // Valida que se seleccione un archivo
    if(strcmp(path, "") != 0){
        puts("3");
        FILE *file = fopen(path, "rb");

        if (file != NULL) {

            freeAVLTree(loadGrapho);

            puts("4");
            fread(&loadGrapho, sizeof(struct Node), 10, file);
            puts("5");
            if (loadGrapho == NULL)
            {
                puts("Nulo");
            }
            
            puts("6");
            printf("key: %d id: %d\n", height(loadGrapho), 1);
            puts("7");
            // int *vector = (int*) malloc(sizeof(int)); // Crea un vector con espacio para 1 entero
            // int size = 0;
            // inOrder(loadGrapho);
            // ExportToDot(vector, "Sin nombre");

        }
        fclose(file);
    }    
}

static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget *tex;
    GtkWidget *buttonAbrir;
    GtkWidget *buttonBuscar;
    GtkWidget *buttonEliminar;
    GtkWidget *buttonDetalles;
    GtkWidget *buttonCrear;
    GtkWidget *b;
    GtkWidget *buttonRecorrido;
    GtkWidget *buttonInsertar;
    GtkWidget *cajita;
    GtkWidget *buttonSave;
    GtkCssProvider *cssProvider;
    cssProvider = gtk_css_provider_new();
    //declaraciones

    //tipo de dato
    buttonAbrir = gtk_button_new();
    buttonCrear = gtk_button_new();
    buttonBuscar =  gtk_button_new();
    buttonEliminar =  gtk_combo_box_new();
    buttonDetalles = gtk_button_new();
    buttonSave = gtk_button_new();
    tex = gtk_fixed_new ();
    g_signal_connect(buttonCrear, "clicked", G_CALLBACK(create), NULL);
    g_signal_connect(buttonAbrir, "clicked", G_CALLBACK(openFile), NULL);
    g_signal_connect(buttonSave, "clicked", G_CALLBACK(saveGrapho), NULL);
    buttonRecorrido = gtk_combo_box_new();
    buttonInsertar = gtk_button_new();
    
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonCrear), "buttonCrear");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonBuscar), "buttonBuscar");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonEliminar), "buttonEliminar");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonDetalles), "buttonDetalles");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonAbrir), "buttonAbrir");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonRecorrido), "buttonRecorrido");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonInsertar), "buttonInsertar");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonSave), "buttonSave");
    
    cajita = gtk_box_new (GTK_ORIENTATION_VERTICAL,0);
    cajitaInterior = gtk_box_new (GTK_ORIENTATION_VERTICAL,0);
    showGrapho = gtk_box_new (GTK_ORIENTATION_VERTICAL,0);
    // gtk_widget_set_margin_start(GTK_WIDGET(showGrapho), 160);


    b = gtk_label_new ("GENERADOR DE ARBOLES");
    gtk_style_context_add_class(gtk_widget_get_style_context(buttonInsertar), "b");


    buttonEliminar = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonEliminar), "\tEliminar");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonEliminar), "nodo");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonEliminar), "elemento");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonEliminar), "dato");

    // buttonRecorrido = gtk_combo_box_text_new();
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonRecorrido), "\tRecorrido");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonRecorrido), "Pre-Orden");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonRecorrido), "In-Orden");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(buttonRecorrido), "Pos-Orden");

    gtk_combo_box_set_active(GTK_COMBO_BOX(buttonEliminar), 0);
    // gtk_combo_box_set_active(GTK_COMBO_BOX(buttonRecorrido), 0);

    //inicializacion de la ventana
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "JARVIS CALCULATOR");//nombre de la ventana
    gtk_window_set_default_size (GTK_WINDOW (window), 1100, 700);//tamaño de la ventana
    //gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    gtk_window_maximize(GTK_WINDOW(window));


    gtk_container_add(GTK_CONTAINER(window), cajita);
    gtk_container_add(GTK_CONTAINER(cajita), tex);
    // gtk_container_add(GTK_CONTAINER(cajitaInterior), showGrapho);
    gtk_box_set_center_widget(GTK_BOX(cajitaInterior), showGrapho);
    gtk_widget_set_name(GTK_WIDGET(cajita), "cajita");
    gtk_widget_set_name(GTK_WIDGET(cajitaInterior), "cajitaInt");

    //gtk_container_add(GTK_CONTAINER(buttonAbrir), entrada);
    gtk_fixed_put (GTK_FIXED(tex),b, 550, 25);
    gtk_fixed_put (GTK_FIXED(tex), buttonAbrir, 480, 50);
    gtk_fixed_put (GTK_FIXED(tex), buttonCrear, 280, 50);
    gtk_fixed_put (GTK_FIXED(tex), buttonInsertar, 680, 50);
    gtk_fixed_put (GTK_FIXED(tex), buttonBuscar, 880, 50);

    gtk_fixed_put (GTK_FIXED(tex),buttonEliminar, 480, 150);
    gtk_fixed_put (GTK_FIXED(tex),buttonDetalles, 280, 150);
    gtk_fixed_put (GTK_FIXED(tex), buttonSave, 680, 150);
    // gtk_fixed_put (GTK_FIXED(tex), buttonSave, 880, 150);
    gtk_fixed_put (GTK_FIXED(tex), cajitaInterior, 310, 220);

    // gtk_button_clicked (GTK_BUTTON (buttonCrear));

    gtk_widget_set_size_request (GTK_WIDGET(buttonCrear),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonAbrir),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonBuscar),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonEliminar),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonDetalles),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonRecorrido),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonInsertar),150,35);
    gtk_widget_set_size_request (GTK_WIDGET(buttonSave),150, 35);
    gtk_widget_set_size_request (GTK_WIDGET(cajitaInterior),402,400);
    gtk_widget_set_size_request (GTK_WIDGET(showGrapho),500,400);
    // Load CSS file

    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(cssProvider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show_all (window);//muestra todos los elementos de la ventana
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
