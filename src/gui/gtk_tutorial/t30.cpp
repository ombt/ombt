#include <gtk/gtk.h>

/* Obligatory basic callback */
static void print_hello( GtkWidget *w,
                         gpointer   data )
{
  g_message ("Hello, World!\n");
}

/* For the check button */
static void print_toggle( gpointer   callback_data,
                          guint      callback_action,
                          GtkWidget *menu_item )
{
   g_message ("Check button state - %d\n",
              GTK_CHECK_MENU_ITEM (menu_item)->active);
}

/* For the radio buttons */
static void print_selected( gpointer   callback_data,
                            guint      callback_action,
                            GtkWidget *menu_item )
{
   if(GTK_CHECK_MENU_ITEM(menu_item)->active)
     g_message ("Radio button %d selected\n", callback_action);
}

#define C2V(XXX) ((void (*)())XXX)
/* Our menu, an array of GtkItemFactoryEntry structures that defines each menu item */
static GtkItemFactoryEntry menu_items[] = {
  { (gchar*)"/_File",         NULL,         NULL,           0, (gchar*)"<Branch>" },
  { (gchar*)"/File/_New",     (gchar*)"<control>N", C2V(print_hello),    0, (gchar*)"<StockItem>", GTK_STOCK_NEW },
  { (gchar*)"/File/_Open",    (gchar*)"<control>O", C2V(print_hello),    0, (gchar*)"<StockItem>", GTK_STOCK_OPEN },
  { (gchar*)"/File/_Save",    (gchar*)"<control>S", C2V(print_hello),    0, (gchar*)"<StockItem>", GTK_STOCK_SAVE },
  { (gchar*)"/File/Save _As", NULL,         NULL,           0, (gchar*)"<Item>" },
  { (gchar*)"/File/sep1",     NULL,         NULL,           0, (gchar*)"<Separator>" },
  { (gchar*)"/File/_Quit",    (gchar*)"<CTRL>Q", gtk_main_quit, 0, (gchar*)"<StockItem>", GTK_STOCK_QUIT },
  { (gchar*)"/_Options",      NULL,         NULL,           0, (gchar*)"<Branch>" },
  { (gchar*)"/Options/tear",  NULL,         NULL,           0, (gchar*)"<Tearoff>" },
  { (gchar*)"/Options/Check", NULL,         C2V(print_toggle),   1, (gchar*)"<CheckItem>" },
  { (gchar*)"/Options/sep",   NULL,         NULL,           0, (gchar*)"<Separator>" },
  { (gchar*)"/Options/Rad1",  NULL,         C2V(print_selected), 1, (gchar*)"<RadioItem>" },
  { (gchar*)"/Options/Rad2",  NULL,         C2V(print_selected), 2, (gchar*)"/Options/Rad1" },
  { (gchar*)"/Options/Rad3",  NULL,         C2V(print_selected), 3, (gchar*)"/Options/Rad1" },
  { (gchar*)"/_Help",         NULL,         NULL,           0, (gchar*)"<LastBranch>" },
  { (gchar*)"/_Help/About",   NULL,         NULL,           0, (gchar*)"<Item>" },
};

static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

/* Returns a menubar widget made from the above menu */
static GtkWidget *get_menubar_menu( GtkWidget  *window )
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;

  /* Make an accelerator group (shortcut keys) */
  accel_group = gtk_accel_group_new ();

  /* Make an ItemFactory (that makes a menubar) */
  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, (gchar*)"<main>",
                                       accel_group);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

  /* Finally, return the actual menu bar created by the item factory. */
  return gtk_item_factory_get_widget (item_factory, (gchar*)"<main>");
}

/* Popup the menu when the popup button is pressed */
static gboolean popup_cb( GtkWidget *widget,
                          GdkEvent *event,
                          GtkWidget *menu )
{
   GdkEventButton *bevent = (GdkEventButton *)event;
  
   /* Only take button presses */
   if (event->type != GDK_BUTTON_PRESS)
     return FALSE;
  
   /* Show the menu */
   gtk_menu_popup (GTK_MENU(menu), NULL, NULL,
                   NULL, NULL, bevent->button, bevent->time);
  
   return TRUE;
}

/* Same as with get_menubar_menu() but just return a button with a signal to
   call a popup menu */
GtkWidget *get_popup_menu( void )
{
   GtkItemFactory *item_factory;
   GtkWidget *button, *menu;
  
   /* Same as before but don't bother with the accelerators */
   item_factory = gtk_item_factory_new (GTK_TYPE_MENU, (gchar*)"<main>",
                                        NULL);
   gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);
   menu = gtk_item_factory_get_widget (item_factory, (gchar*)"<main>");
  
   /* Make a button to activate the popup menu */
   button = gtk_button_new_with_label ("Popup");
   /* Make the menu popup when clicked */
   g_signal_connect (button,
                     "event",
                     G_CALLBACK(popup_cb),
                     (gpointer) menu);

   return button;
}

/* Same again but return an option menu */
GtkWidget *get_option_menu( void )
{
   GtkItemFactory *item_factory;
   GtkWidget *option_menu;
  
   /* Same again, not bothering with the accelerators */
   item_factory = gtk_item_factory_new (GTK_TYPE_OPTION_MENU, "<main>",
                                        NULL);
   gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);
   option_menu = gtk_item_factory_get_widget (item_factory, "<main>");

   return option_menu;
}

/* You have to start somewhere */
int main( int argc,
          char *argv[] )
{
  GtkWidget *window;
  GtkWidget *main_vbox;
  GtkWidget *menubar, *option_menu, *popup_button;
 
  /* Initialize GTK */
  gtk_init (&argc, &argv);
 
  /* Make a window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  gtk_window_set_title (GTK_WINDOW(window), "Item Factory");
  gtk_widget_set_size_request (GTK_WIDGET(window), 300, 200);
 
  /* Make a vbox to put the three menus in */
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
 
  /* Get the three types of menu */
  /* Note: all three menus are separately created, so they are not the
     same menu */
  menubar = get_menubar_menu (window);
  popup_button = get_popup_menu ();
  option_menu = get_option_menu ();
  
  /* Pack it all together */
  gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
  gtk_box_pack_end (GTK_BOX (main_vbox), popup_button, FALSE, TRUE, 0);
  gtk_box_pack_end (GTK_BOX (main_vbox), option_menu, FALSE, TRUE, 0);

  /* Show the widgets */
  gtk_widget_show_all (window);
  
  /* Finished! */
  gtk_main ();
 
  return 0;
}
