#include <gtk/gtk.h>

typedef struct _TextField TextField;
typedef struct _TextFieldClass TextFieldClass;

struct _TextField {
    GtkEntry parent;
};

struct _TextFieldClass {
    GtkEntryClass parent_class;
};

GType text_field_get_type(void);
GtkWidget *text_field_new(void);

G_DEFINE_TYPE(TextField, text_field, GTK_TYPE_ENTRY)

static void text_field_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
    gtk_widget_set_size_request(widget, 0, -1);
    GTK_WIDGET_CLASS(text_field_parent_class)->size_allocate(widget, allocation);
}


static void text_field_class_init(TextFieldClass *class) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
    widget_class->size_allocate = text_field_size_allocate;
}

static void text_field_init(TextField *text_field) {
}

GtkWidget *text_field_new(void) {
    return GTK_WIDGET(g_object_new(text_field_get_type(), NULL));
}
