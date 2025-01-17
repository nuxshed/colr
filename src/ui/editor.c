#include "../app.h"

// DO NOT TRY TO MAKE A EditorCallbackData struct
// It simply doesnt work for some reason
// and I end up in segfault hell

static GtkWidget *label_scale(GtkWidget *scale, const char* label) {
	GtkWidget * container = BOX;
	GtkWidget *lw = gtk_label_new(label);
	gtk_label_set_yalign(GTK_LABEL(lw), 0.7);
	gtk_container_add(GTK_CONTAINER(container), lw);
	gtk_box_pack_start(GTK_BOX(container), scale, 1, 1, 0);
	return container;
}

static void update_r(GtkWidget *self, struct CallbackData *ui) {
	ui->color_data.r = gtk_range_get_value(GTK_RANGE(self));

	// This will get recreated and dumped many times...
	// Figure out a better way
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_GREEN, ui->buffer);
	apply_style(ui->scale_g, ui->buffer);
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_BLUE, ui->buffer);
	apply_style(ui->scale_b, ui->buffer);

	show_color(NULL, ui);
	/* update_editor(ui); */
};

static void update_g(GtkWidget *self, struct CallbackData *ui) {

	ui->color_data.g = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_RED, ui->buffer);
	apply_style(ui->scale_r, ui->buffer);
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_BLUE, ui->buffer);
	apply_style(ui->scale_b, ui->buffer);

	show_color(NULL, ui);
	/* update_editor(ui); */
};

static void update_b(GtkWidget *self, struct CallbackData *ui) {
	ui->color_data.b = gtk_range_get_value(GTK_RANGE(self));
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_RED, ui->buffer);
	apply_style(ui->scale_r, ui->buffer);
	create_color_range_gradient(ui->color_data, COLOR_CHANNEL_GREEN, ui->buffer);
	apply_style(ui->scale_g, ui->buffer);

	show_color(NULL, ui);
	/* update_editor(ui); */
};

void editor_on_selection(GtkWidget *self, UNUSED int resp, struct CallbackData *ui) {
	add_new_color(ui);
	free(ui->buffer); // dont need you anymore
	gtk_widget_destroy(self);
}

void color_edit_menu(GtkWidget *self, struct CallbackData *ui) {
	ui->color_data = color_get_bg(ui->color); // Get starting color
	ui->buffer = malloc(100); // This is super ugly yes but it saves a lottt of allocations
	GtkWidget *dialog = gtk_dialog_new_with_buttons(
			"Edit Color", 
			NULL, 
			GTK_DIALOG_MODAL, 
			"Select",
			GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 180);
	GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	// TODO: Look into using the gtk color scale widget
	ui->scale_r = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_r), ui->color_data.r);
	ui->scale_g = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_g), ui->color_data.g);
	ui->scale_b = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 255, 5);
	gtk_range_set_value(GTK_RANGE(ui->scale_b), ui->color_data.b);

	update_r(ui->scale_r, ui); update_g(ui->scale_g, ui); update_b(ui->scale_b, ui);
	g_signal_connect(G_OBJECT(ui->scale_r), "value-changed", G_CALLBACK(update_r), ui);
	g_signal_connect(G_OBJECT(ui->scale_g), "value-changed", G_CALLBACK(update_g), ui);
	g_signal_connect(G_OBJECT(ui->scale_b), "value-changed", G_CALLBACK(update_b), ui);
	g_signal_connect(G_OBJECT(dialog),      "response",      G_CALLBACK(editor_on_selection), ui);

	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_r, "Red"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_g, "Green"));
	gtk_container_add(GTK_CONTAINER(content), label_scale(ui->scale_b, "Blue"));
	gtk_widget_show_all(dialog);
}
