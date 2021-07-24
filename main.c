/**
*@file main.c
*@version 1.0
*@date 22/7/2021
*@brief Function for the GUI to set values and ask for the KPI.
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlData.h"

/**
 * @brief Struct with the pointers for every widget that is going to be used or modified in the GUI.
 */
typedef struct
{
    // Setpoint widgets
    GtkWidget *w_sbtn_setpoint;
    GtkWidget *w_lbl_current_setpoint;

    // VT pointers
    GtkWidget *w_sbtn_vt;
    GtkWidget *w_lbl_current_vt;

    // From pointers
    GtkWidget *w_from_hour;
    GtkWidget *w_from_minute;
    GtkWidget *w_from_second;
    GtkWidget *w_from_year;
    GtkWidget *w_from_month;
    GtkWidget *w_from_day;

    // To pointers
    GtkWidget *w_to_hour;
    GtkWidget *w_to_minute;
    GtkWidget *w_to_second;
    GtkWidget *w_to_year;
    GtkWidget *w_to_month;
    GtkWidget *w_to_day;

    // KPI pointer
    GtkWidget *w_lbl_kpi;

}app_widgets;

/**
 * @brief main Initializes the GUI and assings the pointers to it's corresponding widget. It also defines the initial values of the GtkLabels and GtkSpinButtons of the first part of the GUI 
 * @param argc Int that stores the number of comand-line arguments.
 * @param argv[] Array of character pointers listing all arguments.
 * @return Returns a 0 for success.
 */

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets); // Allocate memory

    /**
     * @brief gtk_init Sets up default setting and calls gdk_init. Calls all the libraries to use. 
     * @param argc Int that stores the number of comand-line arguments.
     * @param argv[] Array of character pointers listing all arguments.
     */
    gtk_init(&argc, &argv);

    /**
     * @brief gtk_builder_new_from_file Builds prokect from glade file.
     * @param filename Receives the address of the .glade file.
     * @return GtkBuilder with the given interface.
     */
    builder = gtk_builder_new_from_file ("glade/window_main.glade");

    /**
     * @brief gtk_builder_get_object Gets the object named "window_main".
     * @param builder Recieves a GtkBuilder, created before.
     * @param name Receives object name.
     * @return The created object.
     */
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    // Get pointers to widgets
    widgets->w_sbtn_setpoint = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_setpoint"));
    widgets->w_lbl_current_setpoint = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_current_setpoint"));

    widgets->w_sbtn_vt = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_vt"));
    widgets->w_lbl_current_vt = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_current_vt"));

    widgets->w_from_year = GTK_WIDGET(gtk_builder_get_object(builder, "from_year"));
    widgets->w_from_month = GTK_WIDGET(gtk_builder_get_object(builder, "from_month"));
    widgets->w_from_day = GTK_WIDGET(gtk_builder_get_object(builder, "from_day"));
    widgets->w_from_hour = GTK_WIDGET(gtk_builder_get_object(builder, "from_hour"));
    widgets->w_from_minute = GTK_WIDGET(gtk_builder_get_object(builder, "from_minute"));
    widgets->w_from_second = GTK_WIDGET(gtk_builder_get_object(builder, "from_second"));

    widgets->w_to_year = GTK_WIDGET(gtk_builder_get_object(builder, "to_year"));
    widgets->w_to_month = GTK_WIDGET(gtk_builder_get_object(builder, "to_month"));
    widgets->w_to_day = GTK_WIDGET(gtk_builder_get_object(builder, "to_day"));
    widgets->w_to_hour = GTK_WIDGET(gtk_builder_get_object(builder, "to_hour"));
    widgets->w_to_minute = GTK_WIDGET(gtk_builder_get_object(builder, "to_minute"));
    widgets->w_to_second = GTK_WIDGET(gtk_builder_get_object(builder, "to_second"));

    widgets->w_lbl_kpi = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_kpi"));

    // Parameteres for SQL program
    int scanClass;
	int theoreticalUPM;
	int deviceID = 1;
    char scanClass_str[100] = {0};
    char theoreticalUPM_str[100] = {0};

    /**
     * @brief getDeviceData SQL program that define the scanClass and theoretical UPM for the database and counting.
     * @param deviceID Is the number of the device, 1 defines the Arduino.
     * @param scanClass Defines how much time we save a value from the Arduino's counting.
     * @param theoreticalUPM Defines the theoretical count in a defined time
     * @return The initial value of both parameters
     */
    getDeviceData(deviceID,  &scanClass, &theoreticalUPM);
    
    /**
     * @brief gtk_spin_button_set_value Sets the initial value of the spin button.
     * @param spin_button Recieves the spin button where it will write
     * @param value Receives the initial value, calculated before.
     */
    gtk_spin_button_set_value((GtkSpinButton*)gtk_builder_get_object(builder, "sbtn_setpoint"), scanClass);
    gtk_spin_button_set_value((GtkSpinButton*)gtk_builder_get_object(builder, "sbtn_vt"), theoreticalUPM);

    // Save int value as string
    g_snprintf(scanClass_str, sizeof(scanClass_str), "%d", scanClass);
    /**
     * @brief gtk_label_set_text Sets the initial text of the text label.
     * @param label Receives the text label where it will write.
     * @param str Receives the initial text.
     */
    gtk_label_set_text(GTK_LABEL(widgets->w_lbl_current_setpoint), scanClass_str);
    g_snprintf(theoreticalUPM_str, sizeof(theoreticalUPM_str), "%d", theoreticalUPM);
    gtk_label_set_text(GTK_LABEL(widgets->w_lbl_current_vt), theoreticalUPM_str);

    /**
     * @brief gtk_builder_connect_signals Connects the signals defined in the builder with it's corresponding widget and call the corresponding function.
     * @param builder Receives the builder done with the glade final.
     * @param user_data Receives the pointer with the struct with all the pointers to the widgets.
     */
    gtk_builder_connect_signals(builder, widgets);
    /**
     * @brief g_object_unref Decreases the object's count, when it drops to 0 it is finalized.
     * @param object Receives the builder, this contains the window created.
     */
    g_object_unref(builder);
    /**
     * @brief gtk_widget_show Displays the created window in the screen.
     * @param widget Receives a GtkWidget the contains the window with every widget.
     */
    gtk_widget_show(window);
    /**
     * @brief gtk_main The GTK application will stay asleeped waiting for an event i.e. a button is pressed.
     */
    gtk_main();
    /**
     * @brief g_slice_free Here we free the memory allocated for the program.
     * @param type Receives the struct to free.
     * @param mem Pointer to allocated memory.
     */
    g_slice_free(app_widgets, widgets);

    return 0;
}

/**
 * @brief on_btn_update_clicked Function called when "Update" button is pressed. This updates the setpoint and theoreticalUPM in the database.
 * @param button Receives the pointer to the button.
 * @param app_wdgts Contains the pointers to all the widgets used in the window.
 */
void on_btn_update_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    // Stores integer read from spin button widget (Set point)
    guint32 quantity_sp = 0;
    // Buffer for string
    gchar out_str_sp[100] = {0};

    // Stores integer read from spin button widget (VT)
    guint32 quantity_vt = 0;
    gchar out_str_vt[100] = {0};

    /**
     * @brief gtk_spin_button_get_value_as_int Gets the input value of the spin button as type int.
     * @param spin_button Receives the pointer to the spin button.
     * @return Value of the spin button.
     */
    quantity_sp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_sbtn_setpoint));
    g_snprintf(out_str_sp, sizeof(out_str_sp), "%d", quantity_sp);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_current_setpoint), out_str_sp);

    quantity_vt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_sbtn_vt));
    g_snprintf(out_str_vt, sizeof(out_str_vt), "%d", quantity_vt);
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_current_vt), out_str_vt);

    /**
     * @brief deviceManagement Updates the setpoint and theoretical UPM in the database.
     * @param deviceID Is the number of the device, 1 defines the Arduino.
     * @param scanClass Setpoint value from spin button.
     * @param theoreticalUPM Theoretical UPM from spin button.
     * @param name Receives name of new device, NULL if we want to update the existing one.
     * @param isInsert 0 if we want to update the values.
     */
    deviceManagement(1, quantity_sp, quantity_vt, NULL, 0);
}

/**
 * @brief on_btn_date_clicked Function called when "Update date" button is pressed. Here we get the date to look up in the database and obtain the KPI.
 * @param button Receives the pointer to the button.
 * @param app_wdgts Contains the pointers to all the widgets used in the window.
 */
void on_btn_date_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    // FROM DATE
    // Get year
    int from_year_int = 0;
    char from_year_str[100] = {0};
    from_year_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_year));
    g_snprintf(from_year_str, sizeof(from_year_str), "%d", from_year_int);

    // Get month
    int from_month_int = 0;
    char from_month_str[100] = {0};
    from_month_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_month));

    // Check if number is of one digit and if it is, add a zero.
    if(from_month_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(from_month_str, sizeof(from_month_str), "%d", from_month_int);
        strcat(right_size, zero);
        strcat(right_size, from_month_str);
        strcpy(from_month_str, right_size);
    }
    else
    {
        g_snprintf(from_month_str, sizeof(from_month_str), "%d", from_month_int);
    }

    // Get day
    int from_day_int = 0;
    char from_day_str[100] = {0};
    from_day_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_day));

    if(from_day_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(from_day_str, sizeof(from_day_str), "%d", from_day_int);
        strcat(right_size, zero);
        strcat(right_size, from_day_str);
        strcpy(from_day_str, right_size);
    }
    else
    {
        g_snprintf(from_day_str, sizeof(from_day_str), "%d", from_day_int);
    }

    // Get hour
    int from_hour_int = 0;
    char from_hour_str[100] = {0};
    from_hour_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_hour));

    if(from_hour_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(from_hour_str, sizeof(from_hour_str), "%d", from_hour_int);
        strcat(right_size, zero);
        strcat(right_size, from_hour_str);
        strcpy(from_hour_str, right_size);
    }
    else
    {
        g_snprintf(from_hour_str, sizeof(from_hour_str), "%d", from_hour_int);
    }

    // Get minute
    int from_min_int = 0;
    char from_min_str[100] = {0};
    from_min_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_minute));

    if(from_min_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(from_min_str, sizeof(from_min_str), "%d", from_min_int);
        strcat(right_size, zero);
        strcat(right_size, from_min_str);
        strcpy(from_min_str, right_size);
    }
    else
    {
        g_snprintf(from_min_str, sizeof(from_min_str), "%d", from_min_int);
    }

    // Get second
    int from_sec_int = 0;
    char from_sec_str[100] = {0};
    from_sec_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_from_second));

    if(from_sec_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(from_sec_str, sizeof(from_sec_str), "%d", from_sec_int);
        strcat(right_size, zero);
        strcat(right_size, from_sec_str);
        strcpy(from_sec_str, right_size);
    }
    else
    {
        g_snprintf(from_sec_str, sizeof(from_sec_str), "%d", from_sec_int);
    }

    // TO DATE
    // Get year
    int to_year_int = 0;
    char to_year_str[100] = {0};
    to_year_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_year));
    g_snprintf(to_year_str, sizeof(to_year_str), "%d", to_year_int);

    // Get month
    int to_month_int = 0;
    char to_month_str[100] = {0};
    to_month_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_month));

    if(to_month_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(to_month_str, sizeof(to_month_str), "%d", to_month_int);
        strcat(right_size, zero);
        strcat(right_size, to_month_str);
        strcpy(to_month_str, right_size);
    }
    else
    {
        g_snprintf(to_month_str, sizeof(to_month_str), "%d", to_month_int);
    }

    // Get day
    int to_day_int = 0;
    char to_day_str[100] = {0};
    to_day_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_day));

    if(to_day_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(to_day_str, sizeof(to_day_str), "%d", to_day_int);
        strcat(right_size, zero);
        strcat(right_size, to_day_str);
        strcpy(to_day_str, right_size);
    }
    else
    {
        g_snprintf(to_day_str, sizeof(to_day_str), "%d", to_day_int);
    }

    // Get hour
    int to_hour_int = 0;
    char to_hour_str[100] = {0};
    to_hour_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_hour));

    if(to_hour_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(to_hour_str, sizeof(to_hour_str), "%d", to_hour_int);
        strcat(right_size, zero);
        strcat(right_size, to_hour_str);
        strcpy(to_hour_str, right_size);
    }
    else
    {
        g_snprintf(to_hour_str, sizeof(to_hour_str), "%d", to_hour_int);
    }

    // Get minute
    int to_min_int = 0;
    char to_min_str[100] = {0};
    to_min_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_minute));

    if(to_min_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(to_min_str, sizeof(to_min_str), "%d", to_min_int);
        strcat(right_size, zero);
        strcat(right_size, to_min_str);
        strcpy(to_min_str, right_size);
    }
    else
    {
        g_snprintf(to_min_str, sizeof(to_min_str), "%d", to_min_int);
    }

    // Get second
    int to_sec_int = 0;
    char to_sec_str[100] = {0};
    to_sec_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->w_to_second));

    if(to_sec_int < 10)
    {
        char right_size[100] = {0};
        char zero[] = "0";
        g_snprintf(to_sec_str, sizeof(to_sec_str), "%d", to_sec_int);
        strcat(right_size, zero);
        strcat(right_size, to_sec_str);
        strcpy(to_sec_str, right_size);
    }
    else
    {
        g_snprintf(to_sec_str, sizeof(to_sec_str), "%d", to_sec_int);
    }

    // Whole date FROM -> Format: YYYY-MM-DD hh:mm:ss
    char whole_date_from[100] = {0};
    char dash[] = "-";
    char separator[] = ":";
    char space[] = " ";

    strcat(whole_date_from, from_year_str);
    strcat(whole_date_from, dash);
    strcat(whole_date_from, from_month_str);
    strcat(whole_date_from, dash);
    strcat(whole_date_from, from_day_str);
    strcat(whole_date_from, space);
    strcat(whole_date_from, from_hour_str);
    strcat(whole_date_from, separator);
    strcat(whole_date_from, from_min_str);
    strcat(whole_date_from, separator);
    strcat(whole_date_from, from_sec_str);

    // Whole date TO
    char whole_date_to[100] = {0};

    strcat(whole_date_to, to_year_str);
    strcat(whole_date_to, dash);
    strcat(whole_date_to, to_month_str);
    strcat(whole_date_to, dash);
    strcat(whole_date_to, to_day_str);
    strcat(whole_date_to, space);
    strcat(whole_date_to, to_hour_str);
    strcat(whole_date_to, separator);
    strcat(whole_date_to, to_min_str);
    strcat(whole_date_to, separator);
    strcat(whole_date_to, to_sec_str);

    // Parameters for KPI
    float indicador;
    char indicador_str[100] = {0};
    /**
     * @brief calculateIndicator It calculates the KPI by the defines dates and the theoretical UPM.
     * @param startTime String with the start date in "YYYY-MM-DD hh:mm:ss" format.
     * @param endTime String with the end date in "YYYY-MM-DD hh:mm:ss" format.
     * @param deviceID Is the number of the device, 1 defines the Arduino.
     */
    indicador = calculateIndicator(whole_date_from, whole_date_to, 1);
    g_snprintf(indicador_str, sizeof(indicador_str), "%.3f", indicador); // Save int value as string
    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_kpi), indicador_str); // Print value in label
}

/**
 * @brief on_window_main_destroy Function called when 'X' button pressed to close the window.
 */
void on_window_main_destroy()
{
    gtk_main_quit();
}