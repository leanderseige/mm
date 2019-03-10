int mmef_get_selected_page(GtkTreeView * treeview) {
  GtkTreeModel * model;
  GtkTreeSelection * selection;
  GtkTreeIter iter;
  guint id;

  model=gtk_tree_view_get_model(treeview);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get (model, &iter, COL_PGS_ID, &id, -1);
  } else {
    id=0;
  }
  return(id);
}


void mmef_pages_delete (GtkWidget *menuitem, gpointer userdata) {
  int   id;
  if(mmef_yesno(fenster,"Diese Seite wirklich entfernen?")) {
    g_print ("delete %d\n",id=mmef_get_selected_page(GTK_TREE_VIEW(userdata)));
    mmef_lock_player();
    if(mmf_delete_page(id)) {
      mmef_error(fenster,"Seite konnte nicht geloescht werden.\n");
    } else {
      gtk_tree_store_clear(treestore_pages);
      mmef_fill_pages_tree();
      gtk_tree_store_clear(treestore_items);
      mmef_fill_items_tree();
    }
    mmef_unlock_player();
  }
}


void mmef_pages_edit (GtkWidget *menuitem, gpointer userdata) {

  mmef_page_editor();
  g_print ("edit %d\n",mmef_get_selected_page(GTK_TREE_VIEW(userdata)));
}


void mmef_pages_bg2ap (GtkWidget *menuitem, gpointer userdata) { // background to all pages
  int this_item;
  int page_id;
  struct mms_item * newi, * runi;
  struct mms_page * run;
      
  if(!mmef_yesno(fenster,"Diesen Hintergrund wirklich auf alle Seiten kopieren\n und alle alten Hintergruende entfernen?")) return;
  
  page_id=mmef_get_selected_page(GTK_TREE_VIEW(userdata));
  this_item=mmf_page_get_background_item(mmf_get_page_by_id(page_id));
  if(this_item==-1) {
    mmef_error(fenster,"Auf dieser Seite ist kein Hintergrund gesetzt!");
    return;
  }
  mmef_lock_player();
  for(run=MM_Pages;run;run=run->next) {
    if(run!=MM_CurrentPage) {
      for(runi=run->items;runi;runi=runi->next)
        if(runi->is_background) mmf_delete_item(run,runi->id);
          newi=mmf_clone_item(this_item,MM_CurrentPage,run);
    }
  }
  mmef_unlock_player();
  MM_IROW_FocusItem=mmf_get_item_by_id(MM_CurrentPage,this_item);
  mmef_gtk_rebuild_items_view();
}


void mmef_pages_clone (GtkWidget *menuitem, gpointer userdata) {
  GLuint page_id;
  GtkTreePath *path;
  
  page_id=mmef_get_selected_page(GTK_TREE_VIEW(userdata));
  MM_PROW_FocusPage=mmf_clone_page(page_id);
  gtk_tree_store_clear(treestore_pages);
  mmef_fill_pages_tree();
  path=gtk_tree_path_new_from_indices(MM_PROW_0,-1);
  gtk_tree_view_expand_to_path(GTK_TREE_VIEW(viewp),path);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(viewp),path,NULL,0);
}


void mmef_view_popup_pages (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkWidget *menu, *mi_del, *mi_edit, *mi_clone, *mi_bg2ap;
  
  menu = gtk_menu_new();
  mi_del = gtk_menu_item_new_with_label("Entfernen");
  mi_clone = gtk_menu_item_new_with_label("Clonen");
  mi_edit= gtk_menu_item_new_with_label("Bearbeiten");
  mi_bg2ap= gtk_menu_item_new_with_label("Hintergrund Verteilen");

  g_signal_connect(mi_edit, "activate", (GCallback) mmef_pages_edit, treeview);
  g_signal_connect(mi_bg2ap, "activate", (GCallback) mmef_pages_bg2ap, treeview);
  g_signal_connect(mi_del, "activate", (GCallback) mmef_pages_delete, treeview);
  g_signal_connect(mi_clone, "activate", (GCallback) mmef_pages_clone, treeview);
  
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_edit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_bg2ap);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_del);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_clone);

  gtk_widget_show_all(menu);

  /* Note: event can be NULL here when called from view_onPopupMenu;
   *  gdk_event_get_time() accepts a NULL argument */
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}


gboolean mmef_pages_button_event (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
    /* single click with the right mouse button? */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {
      /* optional: select row if no row is selected or only
       *  one other row is selected (will only do something
       *  if you set a tree selection mode as described later
       *  in the tutorial) */
      if (1)
      {
        GtkTreeSelection *selection;

        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

        /* Note: gtk_tree_selection_count_selected_rows() does not
         *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
        if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
        {
           GtkTreePath *path;

           /* Get tree path for row that was clicked */
           if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                             (gint) event->x, 
                                             (gint) event->y,
                                             &path, NULL, NULL, NULL))
           {
             gtk_tree_selection_unselect_all(selection);
             gtk_tree_selection_select_path(selection, path);
             gtk_tree_path_free(path);
           }
        }
      } /* end of optional bit */

      mmef_view_popup_pages(treeview, event, userdata);

      return TRUE; /* we handled this */
    }

    return FALSE; /* we did not handle this */
}

gboolean mmef_pages_popup_event (GtkWidget *treeview, gpointer userdata) {
    mmef_view_popup_pages(treeview, NULL, userdata);
    return TRUE; /* we handled this */
}


