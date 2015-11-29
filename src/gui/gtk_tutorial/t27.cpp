#include <gtk/gtk.h>

/* This function is connected to the Close button or
 * closing the window from the WM */
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent *event,
                              gpointer data )
{
  gtk_main_quit ();
  return FALSE;
}

GtkWidget* close_button; /* This button will emit signal to close
                          * application */
GtkWidget* tooltips_button; /* to enable/disable tooltips */
GtkWidget* text_button,
         * icon_button,
         * both_button; /* radio buttons for toolbar style */
GtkWidget* entry; /* a text entry to show packing any widget into
                   * toolbar */

/* that's easy... when one of the buttons is toggled, we just
 * check which one is active and set the style of the toolbar
 * accordingly
 * ATTENTION: our toolbar is passed as data to callback ! */
static void radio_event( GtkWidget *widget,
                         gpointer data )
{
  if (GTK_TOGGLE_BUTTON (text_button)->active) 
    gtk_toolbar_set_style (GTK_TOOLBAR (data), GTK_TOOLBAR_TEXT);
  else if (GTK_TOGGLE_BUTTON (icon_button)->active)
    gtk_toolbar_set_style (GTK_TOOLBAR (data), GTK_TOOLBAR_ICONS);
  else if (GTK_TOGGLE_BUTTON (both_button)->active)
    gtk_toolbar_set_style (GTK_TOOLBAR (data), GTK_TOOLBAR_BOTH);
}

/* even easier, just check given toggle button and enable/disable 
 * tooltips */
static void toggle_event( GtkWidget *widget,
                          gpointer   data )
{
  gtk_toolbar_set_tooltips (GTK_TOOLBAR (data),
                            GTK_TOGGLE_BUTTON (widget)->active );
}

int main (int argc, char *argv[])
{
  /* Here is our main window (a dialog) and a handle for the handlebox */
  GtkWidget* dialog;
  GtkWidget* handlebox;

  /* Ok, we need a toolbar, an icon with a mask (one for all of 
     the buttons) and an icon widget to put this icon in (but 
     we'll create a separate widget for each button) */
  GtkWidget * toolbar;
  GtkWidget * iconw;

  /* this is called in all GTK application. */
  gtk_init (&argc, &argv);
  
  /* create a new window with a given title, and nice size */
  dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog), "GTKToolbar Tutorial");
  gtk_widget_set_size_request (GTK_WIDGET (dialog), 600, 300);

  /* typically we quit if someone tries to close us */
  g_signal_connect (dialog, "delete-event",
                    G_CALLBACK (delete_event), NULL);

  /* we need to realize the window because we use pixmaps for 
   * items on the toolbar in the context of it */
  gtk_widget_realize (dialog);

  /* to make it nice we'll put the toolbar into the handle box, 
   * so that it can be detached from the main window */
  handlebox = gtk_handle_box_new ();
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox),
                      handlebox, FALSE, FALSE, 5);

  /* toolbar will be horizontal, with both icons and text, and
   * with 5pxl spaces between items and finally, 
   * we'll also put it into our handlebox */
  toolbar = gtk_toolbar_new ();
  gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);
  gtk_container_set_border_width (GTK_CONTAINER (toolbar), 5);
  // gtk_toolbar_set_space_size (GTK_TOOLBAR (toolbar), 5);
  gtk_container_add (GTK_CONTAINER (handlebox), toolbar);

  /* our first item is <close> button */
  iconw = gtk_image_new_from_file ("gtk.xpm"); /* icon widget */
  close_button = 
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), /* our toolbar */
                             "Close",               /* button label */
                             "Closes this app",     /* this button's tooltip */
                             "Private",             /* tooltip private info */
                             iconw,                 /* icon widget */
                             G_CALLBACK (delete_event), /* a signal */
                             NULL);
  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); /* space after item */

  /* now, let's make our radio buttons group... */
  iconw = gtk_image_new_from_file ("gtk.xpm");
  icon_button = gtk_toolbar_append_element (
                    GTK_TOOLBAR (toolbar),
                    GTK_TOOLBAR_CHILD_RADIOBUTTON, /* a type of element */
                    NULL,                          /* pointer to widget */
                    "Icon",                        /* label */
                    "Only icons in toolbar",       /* tooltip */
                    "Private",                     /* tooltip private string */
                    iconw,                         /* icon */
                    G_CALLBACK (radio_event), /* signal */
                    toolbar);                      /* data for signal */
  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

  /* following radio buttons refer to previous ones */
  iconw = gtk_image_new_from_file ("gtk.xpm");
  text_button = 
    gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                                GTK_TOOLBAR_CHILD_RADIOBUTTON,
                                icon_button,
                                "Text",
                                "Only texts in toolbar",
                                "Private",
                                iconw,
                                G_CALLBACK (radio_event),
                                toolbar);
  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
                                          
  iconw = gtk_image_new_from_file ("gtk.xpm");
  both_button = 
    gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                                GTK_TOOLBAR_CHILD_RADIOBUTTON,
                                text_button,
                                "Both",
                                "Icons and text in toolbar",
                                "Private",
                                iconw,
                                G_CALLBACK (radio_event),
                                toolbar);
  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (both_button), TRUE);

  /* here we have just a simple toggle button */
  iconw = gtk_image_new_from_file ("gtk.xpm");
  tooltips_button = 
    gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "Tooltips",
                                "Toolbar with or without tips",
                                "Private",
                                iconw,
                                G_CALLBACK (toggle_event),
                                toolbar);
  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tooltips_button), TRUE);

  /* to pack a widget into toolbar, we only have to 
   * create it and append it with an appropriate tooltip */
  entry = gtk_entry_new ();
  gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar), 
                             entry, 
                             "This is just an entry", 
                             "Private");

  /* well, it isn't created within the toolbar, so we must still show it */
  gtk_widget_show (entry);

  /* that's it ! let's show everything. */
  gtk_widget_show (toolbar);
  gtk_widget_show (handlebox);
  gtk_widget_show (dialog);

  /* rest in gtk_main and wait for the fun to begin! */
  gtk_main ();
  
  return 0;
}


