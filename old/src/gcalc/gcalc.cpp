// gtk+ scientific calculator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "gtk/gtk.h"

#define VOIDFN(X) ((void(*)())(X))

#define GCALCTRACE() if (verbose) g_print("%s[%s]'%d\n", __FILE__, __FUNCTION__, __LINE__)

enum States {
    Start,
    Binary,
    Error
};

static States current_state = Start;

// global data
static bool verbose = false;
static GtkWidget *text_entry = NULL;
static double memory = 0.0;
static double accumulator = 0.0;
static const char *binary_op = NULL;
static bool reset_input = true;
static double angle_scale = 1.0;
static gboolean editable = FALSE;

static void initdata(States new_state = Start, 
                     double new_accumulator = 0)
{
    binary_op = NULL;
    accumulator = new_accumulator;
    current_state = new_state;
    reset_input = true;
}

// unary fucntions and wrappers
static double
reciprocal(double x)
{
    return 1.0/x;
}

static double
squared(double x)
{
    return x*x;
}

static double
cubed(double x)
{
    return x*x*x;
}

static double
factorial(double x)
{
    unsigned long n = (unsigned long)(x+0.5);
    if (n == 0 || n == 1)
        return(1L);
    else if (n == 2)
        return(2L);
    else if (n < 13)
    {
        unsigned long total = 1;
        for ( ; n > 1; n--)
        {
            total *= n;
        }
        return(total);
    } else {
        double logtotal = 0;
        for ( ; n > 1; n--)
        {
            logtotal += log10((double)(n));;
        }
        return(pow(10.0, logtotal));
    }
}

static double
squareroot(double x)
{
    return sqrt(x);
}

static double
logarithmbase10(double x)
{
    return log10(x);
}

static double
sine(double x)
{
    return sin(x*angle_scale);
}

static double
cosine(double x)
{
    return cos(x*angle_scale);
}

static double
tangent(double x)
{
    return tan(x*angle_scale);
}

static double
naturallog(double x)
{
    return log(x);
}

static double
hyperbolicsine(double x)
{
    return sinh(x);
}

static double
hyperboliccosine(double x)
{
    return cosh(x);
}

static double
hyperbolictangent(double x)
{
    return tanh(x);
}

static double
exponential(double x)
{
    return exp(x);
}

static double
arcsine(double x)
{
    return asin(x)/angle_scale;
}

static double
arccosine(double x)
{
    return acos(x)/angle_scale;
}

static double
arctangent(double x)
{
    return atan(x)/angle_scale;
}

static double
logarithmbase2(double x)
{
    return log10(x)/log10(2.0);
}

static double
archyperbolicsine(double x)
{
    return asinh(x);
}

static double
archyperboliccosine(double x)
{
    return acosh(x);
}

static double
archyperbolictangent(double x)
{
    return atanh(x);
}

static double
twotothepowerof(double x)
{
    return pow(2.0, x);
}

static double
percentage(double x)
{
    return x/100.0;
}

static double
togglesign(double x)
{
    return -1.0*x;
}

// array of unary operations
struct UnaryFunctions {
    const char *name;
    double (*handler)(const double);
} unaryops[] = {
    { "1/x", reciprocal },
    { "x^2", squared },
    { "x^3", cubed },
    { "x!", factorial },
    { "sqrt", squareroot },
    { "log10", logarithmbase10 },
    { "sin", sine },
    { "cos", cosine },
    { "tan", tangent },
    { "ln", naturallog },
    { "sinh", hyperbolicsine },
    { "cosh", hyperboliccosine },
    { "tanh", hyperbolictangent },
    { "e^x", exponential },
    { "asin", arcsine },
    { "acos", arccosine },
    { "atan", arctangent },
    { "log2", logarithmbase2 },
    { "asinh", archyperbolicsine },
    { "acosh", archyperboliccosine },
    { "atanh", archyperbolictangent },
    { "2^x", twotothepowerof },
    { "%", percentage },
    { "+/-", togglesign },
    { NULL, NULL }
};

// call backs
static gboolean 
delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GCALCTRACE();
    gtk_main_quit();
    return FALSE;
}

static void
clear_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("clear_button_callback: %s was pressed\n", data);
    if (current_state == Error) initdata();
    gtk_entry_set_text(GTK_ENTRY(text_entry), "");
}

static void
allclear_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("allclear_button_callback: %s was pressed\n", data);
    initdata();
    gtk_entry_set_text(GTK_ENTRY(text_entry), "");
}

static void
unaryop_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("unaryop_button_callback: %s was pressed\n", data);

    if (current_state == Error) return;
    GCALCTRACE();

    reset_input = true;

    const char *string = gtk_entry_get_text(GTK_ENTRY(text_entry));
    errno = 0;
    double x = strtod(string, NULL);
    if (isnan(x))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
    }
    else if (isinf(x))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
    }
    else if (errno != 0)
    {
        GCALCTRACE();
        initdata(Error);
        char buf[BUFSIZ];
        sprintf(buf, "errno=%d", errno);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
    }
    else
    {
        GCALCTRACE();
        for (int op = 0; unaryops[op].name != NULL; ++op)
        {
            if (strcmp(data, unaryops[op].name) != 0) continue;
            GCALCTRACE();
            double y = unaryops[op].handler(x);
            if (isnan(y))
            {
                GCALCTRACE();
                initdata(Error);
                gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
            }
            else if (isinf(y))
            {
                GCALCTRACE();
                gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
            }
            else
            {
                GCALCTRACE();
                char buf[BUFSIZ];
                sprintf(buf, "%.14g", y);
                gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
            }
            GCALCTRACE();
            return;
        }

        // should never get here
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "");
    }

    return;
}

static void
memory_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("button_callback: %s was pressed\n", data);

    if (current_state == Error) return;
    GCALCTRACE();

    reset_input = true;

    const char *string = gtk_entry_get_text(GTK_ENTRY(text_entry));
    errno = 0;
    double x = strtod(string, NULL);

    if (isnan(x))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
    }
    else if (isinf(x))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
    }
    else if (errno != 0)
    {
        GCALCTRACE();
        initdata(Error);
        char buf[BUFSIZ];
        sprintf(buf, "errno=%d", errno);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
    }
    else if (strcmp(data, "MC") == 0)
    {
        GCALCTRACE();
        memory = 0.0;
    }
    else if (strcmp(data, "M+") == 0)
    {
        GCALCTRACE();
        memory += x;
    }
    else if (strcmp(data, "M-") == 0)
    {
        GCALCTRACE();
        memory -= x;
    }
    else if (strcmp(data, "MR") == 0)
    {
        GCALCTRACE();
        if (isnan(memory))
        {
            GCALCTRACE();
            initdata(Error);
            gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        }
        else if (isinf(memory))
        {
            GCALCTRACE();
            gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
        }
        else
        {
            GCALCTRACE();
            char buf[BUFSIZ];
            sprintf(buf, "%.14g", memory);
            gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
        }
    }
}

static void
equal_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("equal_button_callback: %s was pressed\n", data);

    if (current_state == Error || binary_op == NULL) return;
    GCALCTRACE();

    reset_input = true;

    const char *string = gtk_entry_get_text(GTK_ENTRY(text_entry));
    errno = 0;
    double x = strtod(string, NULL);
    if (isnan(x))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        return;
    }
    else if (isinf(x))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
    }
    else if (errno != 0)
    {
        GCALCTRACE();
        initdata(Error);
        char buf[BUFSIZ];
        sprintf(buf, "errno=%d", errno);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
        return;
    }

    GCALCTRACE();
    if (strcmp("y^x", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator = pow(accumulator, x);
    }
    else if (strcmp("/", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator /= x;
    }
    else if (strcmp("x", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator *= x;
    }
    else if (strcmp("y^1/x", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator = pow(accumulator, 1.0/x);
    }
    else if (strcmp("-", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator -= x;
    }
    else if (strcmp("+", binary_op) == 0)
    {
        GCALCTRACE();
        accumulator += x;
    }
    else
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        return;
    }

    GCALCTRACE();
    if (isnan(accumulator))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
    }
    else if (isinf(accumulator))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
        initdata(Start, accumulator);
    }
    else
    {
        GCALCTRACE();
        char buf[BUFSIZ];
        sprintf(buf, "%.14g", accumulator);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
        initdata(Start, accumulator);
    }
}

static void
binaryop_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("binaryop_button_callback: %s was pressed\n", data);

    reset_input = true;

    switch (current_state)
    {
    case Start:
    {
        GCALCTRACE();
        const char *string = gtk_entry_get_text(GTK_ENTRY(text_entry));
        errno = 0;
        double x = strtod(string, NULL);
        if (isnan(x))
        {
            GCALCTRACE();
            initdata(Error);
            gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        }
        else if (errno != 0)
        {
            GCALCTRACE();
            initdata(Error);
            char buf[BUFSIZ];
            sprintf(buf, "errno=%d", errno);
            gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
        }
        else
        {
            GCALCTRACE();
            accumulator = x;
            current_state = Binary;
            binary_op = data;
        }
        return;
    }
    case Binary:
    {
        GCALCTRACE();
        if (binary_op != NULL)
        {
            GCALCTRACE();
            // two binary ops in a row. complete one, then use
            // result for second op.
            equal_button_callback(widget, data);
            if (current_state != Error)
            {
                GCALCTRACE();
                binary_op = data;
                current_state = Binary;
            }
        }
        else
        {
            GCALCTRACE();
            binary_op = data;
        }
        return;
    }
    case Error:
    {
        GCALCTRACE();
        return;
    }
    default:
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        return;
    }
    }
}

static void
button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("button_callback: %s was pressed\n", data);

    if (strcmp(data, "EE") != 0 )
        gtk_entry_append_text(GTK_ENTRY(text_entry), data);
    else
        gtk_entry_append_text(GTK_ENTRY(text_entry), "E");
}

static void
debug_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("debug_button_callback: %s was pressed\n", data);

    verbose = !verbose;
}

static void
checked_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("checked_callback: %s was pressed\n", data);

    // the active button
    if (strcmp("dbg", data) == 0)
    {
        GCALCTRACE();
        verbose = !verbose;
    }
    else if (strcmp("edt", data) == 0)
    {
        GCALCTRACE();
        editable = !editable;
        gtk_entry_set_editable(GTK_ENTRY(text_entry), editable);
    }
}

static void
angle_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("angle_button_callback: %s was pressed\n", data);

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) != TRUE)
    {
        // not the active button
        GCALCTRACE();
        return;
    }

    // the active button
    GCALCTRACE();
    if (strcmp("rad", data) == 0)
    {
        // radians
        GCALCTRACE();
        angle_scale = 1.0;
    }
    else if (strcmp("deg", data) == 0)
    {
        // degrees
        GCALCTRACE();
        angle_scale = (2.0*M_PI)/360.0;
    }
}

static void
swap_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("swap_button_callback: %s was pressed\n", data);

    if (current_state == Error) return;
    GCALCTRACE();

    reset_input = true;

    const char *string = gtk_entry_get_text(GTK_ENTRY(text_entry));
    errno = 0;
    double x = strtod(string, NULL);

    if (isnan(x))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        return;
    }
    else if (isinf(x))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
    }
    else if (errno != 0)
    {
        GCALCTRACE();
        initdata(Error);
        char buf[BUFSIZ];
        sprintf(buf, "errno=%d", errno);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
        return;
    }

    GCALCTRACE();

    if (isnan(accumulator))
    {
        GCALCTRACE();
        initdata(Error);
        gtk_entry_set_text(GTK_ENTRY(text_entry), "NaN");
        return;
    }
    else if (isinf(accumulator))
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "INF");
    }
    else
    {
        GCALCTRACE();
        char buf[BUFSIZ];
        sprintf(buf, "%.14g", accumulator);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
    }

    accumulator = x;
}

static void
constant_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("constant_button_callback: %s was pressed\n", data);

    reset_input = true;

    if (strcmp("pi", data) == 0)
    {
        char buf[BUFSIZ];
        sprintf(buf, "%.14g", M_PI);
        gtk_entry_set_text(GTK_ENTRY(text_entry), buf);
    }
}

static void
number_button_callback(GtkWidget *widget, const char *data)
{
    GCALCTRACE();
    if (verbose) g_print("number_button_callback: %s was pressed\n", data);

    if (reset_input)
    {
        GCALCTRACE();
        gtk_entry_set_text(GTK_ENTRY(text_entry), "");
    }
    reset_input = false;

    GCALCTRACE();
    gtk_entry_append_text(GTK_ENTRY(text_entry), data);
}

static GSList *
add_radio_button_at(const char *label, GtkWidget *table, GSList *group, 
                    gboolean active, void (*callback)(),
                    int leftcol, int rightcol, int toprow, int bottomrow,
                    GdkColor *bgcolor = NULL, GdkColor *fgcolor = NULL)
{
    GCALCTRACE();
    GtkWidget *button = gtk_radio_button_new_with_label(group, label);

    GtkWidget *box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(box), button);

    if (bgcolor != NULL)
    {
        GCALCTRACE();
        gtk_widget_modify_bg(box, GTK_STATE_NORMAL, bgcolor);
    }
    if (fgcolor != NULL)
    {
        GCALCTRACE();
        gtk_widget_modify_fg(box, GTK_STATE_NORMAL, fgcolor);
    }

    g_signal_connect(G_OBJECT(button), "toggled",
                     G_CALLBACK(callback), (gpointer)label);

    gtk_table_attach_defaults(GTK_TABLE(table), box, 
                              leftcol, rightcol, toprow, bottomrow);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), active);

    return gtk_radio_button_get_group(GTK_RADIO_BUTTON(button));
}

static void
add_button_at(const char *label, GtkWidget *table, void (*callback)(),
              int leftcol, int rightcol, int toprow, int bottomrow,
              GdkColor *bgcolor = NULL, GdkColor *fgcolor = NULL)
{
    GCALCTRACE();
    GtkWidget *button = gtk_button_new_with_label(label);

    if (bgcolor != NULL)
    {
        GCALCTRACE();
        gtk_widget_modify_bg(button, GTK_STATE_NORMAL, bgcolor);
    }
    if (fgcolor != NULL)
    {
        GCALCTRACE();
        gtk_widget_modify_fg(button, GTK_STATE_NORMAL, fgcolor);
    }

    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(callback), (gpointer)label);

    gtk_table_attach_defaults(GTK_TABLE(table), button, 
                              leftcol, rightcol, toprow, bottomrow);
}

int
main(int argc, char **argv)
{
    // support standard gtk options
    gtk_init(&argc, &argv);

    // check other options
    if (argc == 2 && strcmp(argv[1], "-v") == 0) verbose = true;

    // get get some color in the world
    GdkColor yellow;
    gdk_color_parse("yellow", &yellow);
    GdkColor red;
    gdk_color_parse("red", &red);
    GdkColor darkgray;
    gdk_color_parse("dark gray", &darkgray);
    GdkColor blue;
    gdk_color_parse("blue", &blue);
    GdkColor lightblue;
    gdk_color_parse("light blue", &lightblue);
    GdkColor lightgreen;
    gdk_color_parse("light green", &lightgreen);
    GdkColor magenta;
    gdk_color_parse("magenta", &magenta);
    GdkColor orange;
    gdk_color_parse("orange", &orange);
    GdkColor violet;
    gdk_color_parse("violet", &violet);

    // create main window and add trimmings
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &blue);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_window_set_title(GTK_WINDOW(window), "GCALC");
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    // always handle exits
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);

    // use a table to organize
    GtkWidget *table = gtk_table_new(10, 9, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_table_set_row_spacings(GTK_TABLE(table), 2);
    gtk_table_set_col_spacings(GTK_TABLE(table), 2);

    // add text box at row 0
    text_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text_entry), 400);
    gtk_entry_set_editable(GTK_ENTRY(text_entry), editable);
    gtk_entry_set_alignment(GTK_ENTRY(text_entry), 0);
    gtk_table_attach_defaults(GTK_TABLE(table), text_entry, 0, 10, 0, 1);

    // add lines of separation
    GtkWidget *horizontal_line = gtk_hseparator_new();
    gtk_table_attach_defaults(GTK_TABLE(table), horizontal_line, 0, 10, 1, 2);
    GtkWidget *vertical_line = gtk_vseparator_new();
    gtk_table_attach_defaults(GTK_TABLE(table), vertical_line, 5, 6, 1, 10);

    // add row 1 buttons
    // add_button_at("quit", table, VOIDFN(gtk_main_quit), 1, 2, 2, 3, &darkgray);
    add_button_at("quit", table, VOIDFN(gtk_main_quit), 1, 2, 2, 3, &red);
#if 0
    add_button_at("(", table, VOIDFN(button_callback), 2, 3, 2, 3);
    add_button_at(")", table, VOIDFN(button_callback), 3, 4, 2, 3);
#endif
    add_button_at("swap", table, VOIDFN(swap_button_callback), 2, 3, 2, 3, &darkgray);
    add_button_at("C", table, VOIDFN(clear_button_callback), 3, 4, 2, 3, &darkgray);
    add_button_at("AC", table, VOIDFN(allclear_button_callback), 4, 5, 2, 3, &darkgray);
    add_button_at("MC", table, VOIDFN(memory_button_callback), 6, 7, 2, 3, &orange);
    add_button_at("M+", table, VOIDFN(memory_button_callback), 7, 8, 2, 3, &orange);
    add_button_at("M-", table, VOIDFN(memory_button_callback), 8, 9, 2, 3, &orange);
    add_button_at("MR", table, VOIDFN(memory_button_callback), 9, 10, 2, 3, &orange);


    // add row 2 buttons
    add_button_at("1/x", table, VOIDFN(unaryop_button_callback), 1, 2, 3, 4, &magenta);
    add_button_at("x^2", table, VOIDFN(unaryop_button_callback), 2, 3, 3, 4, &magenta);
    add_button_at("x^3", table, VOIDFN(unaryop_button_callback), 3, 4, 3, 4, &magenta);
    add_button_at("y^x", table, VOIDFN(binaryop_button_callback), 4, 5, 3, 4, &lightgreen);
    add_button_at("%", table, VOIDFN(unaryop_button_callback), 6, 7, 3, 4, &magenta);
    add_button_at("+/-", table, VOIDFN(unaryop_button_callback), 7, 8, 3, 4, &magenta);
    add_button_at("/", table, VOIDFN(binaryop_button_callback), 8, 9, 3, 4, &lightgreen);
    add_button_at("x", table, VOIDFN(binaryop_button_callback), 9, 10, 3, 4, &lightgreen);

    // add row 3 buttons
    add_button_at("x!", table, VOIDFN(unaryop_button_callback), 1, 2, 4, 5, &magenta);
    add_button_at("sqrt", table, VOIDFN(unaryop_button_callback), 2, 3, 4, 5, &magenta);
    add_button_at("y^1/x", table, VOIDFN(binaryop_button_callback), 3, 4, 4, 5, &lightgreen);
    add_button_at("log10", table, VOIDFN(unaryop_button_callback), 4, 5, 4, 5, &magenta);
    add_button_at("7", table, VOIDFN(number_button_callback), 6, 7, 4, 5, &yellow);
    add_button_at("8", table, VOIDFN(number_button_callback), 7, 8, 4, 5, &yellow);
    add_button_at("9", table, VOIDFN(number_button_callback), 8, 9, 4, 5, &yellow);
    add_button_at("-", table, VOIDFN(binaryop_button_callback), 9, 10, 4, 5, &lightgreen);

    // add row 4 buttons
    add_button_at("sin", table, VOIDFN(unaryop_button_callback), 1, 2, 5, 6, &magenta);
    add_button_at("cos", table, VOIDFN(unaryop_button_callback), 2, 3, 5, 6, &magenta);
    add_button_at("tan", table, VOIDFN(unaryop_button_callback), 3, 4, 5, 6, &magenta);
    add_button_at("ln", table, VOIDFN(unaryop_button_callback), 4, 5, 5, 6, &magenta);
    add_button_at("4", table, VOIDFN(number_button_callback), 6, 7, 5, 6, &yellow);
    add_button_at("5", table, VOIDFN(number_button_callback), 7, 8, 5, 6, &yellow);
    add_button_at("6", table, VOIDFN(number_button_callback), 8, 9, 5, 6, &yellow);
    add_button_at("+", table, VOIDFN(binaryop_button_callback), 9, 10, 5, 6, &lightgreen);

    // add row 5 buttons
    add_button_at("sinh", table, VOIDFN(unaryop_button_callback), 1, 2, 6, 7, &magenta);
    add_button_at("cosh", table, VOIDFN(unaryop_button_callback), 2, 3, 6, 7, &magenta);
    add_button_at("tanh", table, VOIDFN(unaryop_button_callback), 3, 4, 6, 7, &magenta);
    add_button_at("e^x", table, VOIDFN(unaryop_button_callback), 4, 5, 6, 7, &magenta);
    add_button_at("1", table, VOIDFN(number_button_callback), 6, 7, 6, 7, &yellow);
    add_button_at("2", table, VOIDFN(number_button_callback), 7, 8, 6, 7, &yellow);
    add_button_at("3", table, VOIDFN(number_button_callback), 8, 9, 6, 7, &yellow);
    add_button_at("=", table, VOIDFN(equal_button_callback), 9, 10, 6, 8, &lightblue);

    // add row 6 buttons
    add_button_at("asin", table, VOIDFN(unaryop_button_callback), 1, 2, 7, 8, &magenta);
    add_button_at("acos", table, VOIDFN(unaryop_button_callback), 2, 3, 7, 8, &magenta);
    add_button_at("atan", table, VOIDFN(unaryop_button_callback), 3, 4, 7, 8, &magenta);
    add_button_at("log2", table, VOIDFN(unaryop_button_callback), 4, 5, 7, 8, &magenta);
    add_button_at("0", table, VOIDFN(number_button_callback), 6, 8, 7, 8, &yellow);
    add_button_at(".", table, VOIDFN(number_button_callback), 8, 9, 7, 8, &lightblue);

    // add row 7 buttons
    add_button_at("asinh", table, VOIDFN(unaryop_button_callback), 1, 2, 8, 9, &magenta);
    add_button_at("acosh", table, VOIDFN(unaryop_button_callback), 2, 3, 8, 9, &magenta);
    add_button_at("atanh", table, VOIDFN(unaryop_button_callback), 3, 4, 8, 9, &magenta);
    add_button_at("2^x", table, VOIDFN(unaryop_button_callback), 4, 5, 8, 9, &magenta);

    // add row 8 buttons
    GSList *group = add_radio_button_at("rad", table, NULL, TRUE, VOIDFN(angle_button_callback), 1, 2, 9, 10, &violet);
    add_radio_button_at("deg", table, group, FALSE, VOIDFN(angle_button_callback), 2, 3, 9, 10, &violet);
    add_button_at("pi", table, VOIDFN(constant_button_callback), 3, 4, 9, 10, &violet);
    add_button_at("EE", table, VOIDFN(button_callback), 4, 5, 9, 10, &violet);

#if 0
    // add debug button
    add_button_at("dbg", table, VOIDFN(debug_button_callback), 9, 10, 9, 10, &violet);
#else
    // add menu item with choices to activate
    GtkWidget *menubar = gtk_menu_bar_new();

    GtkWidget *options = gtk_menu_item_new_with_label("options");
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), options);

    GtkWidget *submenu= gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options), submenu);

    GtkWidget *dbgitem = gtk_check_menu_item_new_with_label("dbg");
    gtk_menu_append(GTK_MENU(submenu), dbgitem);
    g_signal_connect(G_OBJECT(dbgitem), "activate",
                     G_CALLBACK(checked_callback), (gpointer)"dbg");

    GtkWidget *edtitem = gtk_check_menu_item_new_with_label("edt");
    gtk_menu_append(GTK_MENU(submenu), edtitem);
    g_signal_connect(G_OBJECT(edtitem), "activate",
                     G_CALLBACK(checked_callback), (gpointer)"edt");

    gtk_table_attach_defaults(GTK_TABLE(table), menubar, 8, 10, 9, 10);
#endif

    // show main window and anything it contains
    gtk_widget_show_all(window);

    // start up main event loop
    gtk_main();

    // all done
    return 0;
}
