int mmef_get_selected_resource(GtkTreeView * treeview) {
  GtkTreeModel * model;
  GtkTreeSelection * selection;
  GtkTreeIter iter;
  guint id;

  model=gtk_tree_view_get_model(treeview);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get (model, &iter, COL_RES_ID, &id, -1);
  } else {
    id=0;
  }
  return(id);
}


void mmef_resources_delete (GtkWidget *menuitem, gpointer userdata) {
  int res_id;

  if(mmef_yesno(fenster,"Diese Resource wirklich entfernen?\n\nEs werden alle abhaengigen Items ebenfalls geloescht!.")) {
    g_print ("delete %d\n",res_id=mmef_get_selected_resource(GTK_TREE_VIEW(userdata)));
    mmef_lock_player();
    if(mmf_delete_resource(res_id)) {
      mmef_error(fenster,"Resource konnte nicht geloescht werden.\n");
    } else {
      gtk_tree_store_clear(treestore_resources);
      mmef_fill_resources_tree();
      gtk_tree_store_clear(treestore_items);
      mmef_fill_items_tree();
    }
    mmef_unlock_player();
  }
}


void mmef_resources_rename (GtkWidget *menuitem, gpointer userdata) {
  struct mms_resource * res;
  GLuint res_id;
  char * in;
  GtkTreePath *path;
  
  g_print ("__rename %d\n",mmef_get_selected_resource(GTK_TREE_VIEW(userdata)));

  res_id=mmef_get_selected_resource(GTK_TREE_VIEW(userdata));
  res=mmf_get_resource_by_id(res_id);
  in=mmef_gtk_text_request("Umbenennen",res->text);
  if(in) {
    free(res->text);
    res->text=in;
  }
  MM_RROW_FocusResource=res;
  mmef_gtk_rebuild_resources_view();  
}



void mmef_resources_clone (GtkWidget *menuitem, gpointer userdata) {
  struct mms_resource * res, *new_res;
  GLuint res_id;
  GtkTreePath *path;
  
  g_print ("__clone %d\n",mmef_get_selected_resource(GTK_TREE_VIEW(userdata)));

  res_id=mmef_get_selected_resource(GTK_TREE_VIEW(userdata));
  res=mmf_get_resource_by_id(res_id);
  new_res=mmf_clone_resource(res);
  if(new_res) MM_RROW_FocusResource=new_res;
  mmef_gtk_rebuild_resources_view();
}


void mmef_resources_edit (GtkWidget *menuitem, gpointer userdata) {
  struct mms_resource * res;
  GLuint res_id;
  GtkTreePath *path;
  
//  g_print ("edit__ %d\n",mmef_get_selected_resource(GTK_TREE_VIEW(userdata)));
  
  res_id=mmef_get_selected_resource(GTK_TREE_VIEW(userdata));
  res=mmf_get_resource_by_id(res_id);
  if(res->type==MM_TEXT_BUTTON) {
    mmef_newresource_text_button(res);
  } else if(res->type==MM_TEXT) {
    mmef_newresource_text(res);
  } else {
    mmef_error(fenster,"Dieser Resourcentyp kann noch nicht bearbeitet werden.\nBitte loeschen und neu anlegen.\n");
  }
}


void mmef_resources_add2page (GtkWidget *menuitem, gpointer userdata) {
  GLuint res_id;
  struct mms_item * ni;
  GtkTreePath * path;
  
  if(MM_CurrentPage==NULL) {
    mmef_error(fenster,"Es existiert keine aktuelle Seite.\n");
    return;
  }

  res_id=mmef_get_selected_resource(GTK_TREE_VIEW(userdata));
  mmef_lock_player();
  ni=mmf_insert_item(MM_CurrentPage->id,1,mmf_get_resource_by_id(res_id),0,0,MM_NO_ACTION,0,0,0,0,0,0,0,0,0);
  MM_IROW_FocusItem=ni;
  mmef_gtk_rebuild_items_view();
  mmef_unlock_player();  
}


void mmef_view_popup_resources (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkWidget *menu, *mi_del, *mi_edit, *mi_add2page, *mi_clone, *mi_rename;
  
  menu = gtk_menu_new();
  mi_del = gtk_menu_item_new_with_label("Entfernen");
  mi_edit= gtk_menu_item_new_with_label("Bearbeiten");
  mi_clone= gtk_menu_item_new_with_label("Clonen");
  mi_rename= gtk_menu_item_new_with_label("Umbenennen");
  mi_add2page= gtk_menu_item_new_with_label("Zu aktueller Seite hinzufuegen");

  g_signal_connect(mi_edit, "activate", (GCallback) mmef_resources_edit, treeview);
  g_signal_connect(mi_del, "activate", (GCallback) mmef_resources_delete, treeview);
  g_signal_connect(mi_clone, "activate", (GCallback) mmef_resources_clone, treeview);
  g_signal_connect(mi_rename, "activate", (GCallback) mmef_resources_rename, treeview);
  g_signal_connect(mi_add2page, "activate", (GCallback) mmef_resources_add2page, treeview);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_edit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_clone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_rename);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_del);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_add2page);

  gtk_widget_show_all(menu);

  /* Note: event can be NULL here when called from view_onPopupMenu;
   *  gdk_event_get_time() accepts a NULL argument */
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}


gboolean mmef_resources_button_event (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
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

      mmef_view_popup_resources(treeview, event, userdata);

      return TRUE; /* we handled this */
    }

    return FALSE; /* we did not handle this */
}

gboolean mmef_resources_popup_event (GtkWidget *treeview, gpointer userdata) {
    mmef_view_popup_resources(treeview, NULL, userdata);
    return TRUE; /* we handled this */
}


