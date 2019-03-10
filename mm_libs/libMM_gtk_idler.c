gboolean mmef_gtk_idle_func (gpointer data) {

    

  if(MM_GTKIDLER_REBUILD_RESOURCES) {
    mmef_gtk_rebuild_resources_view();
	MM_GTKIDLER_REBUILD_RESOURCES=0;
  }

  if(MM_GTKIDLER_REBUILD_PAGES) {
    mmef_gtk_rebuild_pages_view();
	MM_GTKIDLER_REBUILD_PAGES=0;
  }

  if(MM_GTKIDLER_REBUILD_ITEMS) {
    mmef_gtk_rebuild_items_view();
	MM_GTKIDLER_REBUILD_ITEMS=0;
  }
  
  if(MM_GTKIDLER_HIDE_WINDOW) {
	gtk_widget_hide_all(GTK_WIDGET(fenster));
	MM_GTKIDLER_HIDE_WINDOW=0;
  }

  if(MM_GTKIDLER_SHOW_WINDOW) {
	gtk_widget_show_all(GTK_WIDGET(fenster));
	MM_GTKIDLER_SHOW_WINDOW=0;
  }

  if(MM_GTKIDLER_FOCUS_WIDGET!=NULL) {
	gtk_widget_grab_focus(MM_GTKIDLER_FOCUS_WIDGET);
	MM_GTKIDLER_FOCUS_WIDGET=NULL;
  }

  return(TRUE);

}
