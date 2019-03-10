int mmef_get_selected_item(GtkTreeView * treeview) {
  GtkTreeModel * model;
  GtkTreeSelection * selection;
  GtkTreeIter iter;
  guint id;

  model=gtk_tree_view_get_model(treeview);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(viewi));
  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get (model, &iter, COL_ITM_ID, &id, -1);
  } else {
    id=0;
  }
  return(id);
}


void mmef_items_delete (GtkWidget *menuitem, gpointer userdata) {
  int del_item;
  if(mmef_yesno(fenster,"Dieses Item wirklich entfernen?")) {
    g_print ("delete %d\n",del_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi)));
    mmef_lock_player();
    if(mmf_delete_item(MM_CurrentPage,del_item)) {
      mmef_error(fenster,"Item konnte nicht gefunden werden.\n");
    } else {
      gtk_tree_store_clear(treestore_items);
      mmef_fill_items_tree();
    }
    mmef_unlock_player();
  }
}


void mmef_items_edit (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  mmef_item_editor(this_item);
}

void mmef_items_edanim (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_animation_editor(&(item->animakt));
}

void mmef_items_spreadanim (GtkWidget *menuitem, gpointer userdata) {
  struct mms_item *runi,*item;
  struct mms_page *runp;

  if(!mmef_yesno(fenster,"Diese Item-Animation wirklich auf alle Instanzen\ndieser Resource auf allen Seiten kopieren?")) return;

  item=mmf_get_item_by_id(MM_CurrentPage,mmef_get_selected_item(GTK_TREE_VIEW(viewi)));
  mmef_lock_player();
  for(runp=MM_Pages;runp;runp=runp->next) {
	if(runp!=MM_CurrentPage)
      for(runi=runp->items;runi;runi=runi->next) {
        if(runi->res_id==item->res_id) {
          mmf_clone_animation(&(item->animakt),&(runi->animakt));
        }
      }
  }
  mmef_unlock_player();
  MM_IROW_FocusItem=item;
  mmef_gtk_rebuild_items_view();
}

void mmef_items_cgeo (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_lock_player();
  MM_CP_MINX=item->minx;
  MM_CP_MAXX=item->maxx;
  MM_CP_MINY=item->miny;
  MM_CP_MAXY=item->maxy;
  mmef_unlock_player();
}

void mmef_items_pgeo (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_lock_player();
  item->minx=MM_CP_MINX;
  item->maxx=MM_CP_MAXX;
  item->miny=MM_CP_MINY;
  item->maxy=MM_CP_MAXY;
  mmef_unlock_player();
}

void mmef_items_canim (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_lock_player();
  MM_AnimationClipboard=&(item->animakt);
  mmef_unlock_player();
}

void mmef_items_panim (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_lock_player();
  mmf_clone_animation(MM_AnimationClipboard,&(item->animakt));
  mmef_unlock_player();
}

void mmef_items_delanim (GtkWidget *menuitem, gpointer userdata) {
  GLuint id;
  struct mms_item * item;

  id=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  item=mmf_get_item_by_id(MM_CurrentPage,id);
  mmef_lock_player();
  if(item->animakt.str_rx) free(item->animakt.str_rx); item->animakt.str_rx=NULL;
  if(item->animakt.str_ry) free(item->animakt.str_ry); item->animakt.str_ry=NULL;
  if(item->animakt.str_rz) free(item->animakt.str_rz); item->animakt.str_rz=NULL;
  if(item->animakt.str_tx) free(item->animakt.str_tx); item->animakt.str_tx=NULL;
  if(item->animakt.str_ty) free(item->animakt.str_ty); item->animakt.str_ty=NULL;
  if(item->animakt.str_tz) free(item->animakt.str_tz); item->animakt.str_tz=NULL;
  if(item->animakt.str_sw) free(item->animakt.str_sw); item->animakt.str_sw=NULL;
  if(item->animakt.str_sh) free(item->animakt.str_sh); item->animakt.str_sh=NULL;
  if(item->animakt.eval_rx) evaluator_destroy(item->animakt.eval_rx); item->animakt.eval_rx=NULL;
  if(item->animakt.eval_ry) evaluator_destroy(item->animakt.eval_ry); item->animakt.eval_ry=NULL;
  if(item->animakt.eval_rz) evaluator_destroy(item->animakt.eval_rz); item->animakt.eval_rz=NULL;
  if(item->animakt.eval_tx) evaluator_destroy(item->animakt.eval_tx); item->animakt.eval_tx=NULL;
  if(item->animakt.eval_ty) evaluator_destroy(item->animakt.eval_ty); item->animakt.eval_ty=NULL;
  if(item->animakt.eval_tz) evaluator_destroy(item->animakt.eval_tz); item->animakt.eval_tz=NULL;
  if(item->animakt.eval_sw) evaluator_destroy(item->animakt.eval_sw); item->animakt.eval_sw=NULL;
  if(item->animakt.eval_sh) evaluator_destroy(item->animakt.eval_sh); item->animakt.eval_sh=NULL;
  item->animakt.timescale=0;
  item->animakt.random=0;
  item->animakt.playtime=0;
  mmef_unlock_player();
}

void mmef_items_sbtedit (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  mmef_sbt_editor(this_item);
}

void mmef_items_clone (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_item * newi;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  mmef_lock_player();
  newi=mmf_clone_item(this_item,MM_CurrentPage,MM_CurrentPage);
  mmef_unlock_player();
  if(newi) MM_IROW_FocusItem=newi;
  mmef_gtk_rebuild_items_view();
}

void mmef_items_c2ap (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_item * newi;
  struct mms_page * run;

  if(!mmef_yesno(fenster,"Dieses Item wirklich auf alle Seiten kopieren?")) return;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  mmef_lock_player();
  for(run=MM_Pages;run;run=run->next) {
	if(run!=MM_CurrentPage)
	  newi=mmf_clone_item(this_item,MM_CurrentPage,run);
  }
  mmef_unlock_player();
  MM_IROW_FocusItem=mmf_get_item_by_id(MM_CurrentPage,this_item);
  mmef_gtk_rebuild_items_view();
}

void mmef_items_geo2ap (GtkWidget *menuitem, gpointer userdata) { // geometry to all pages
  GLuint this_item;
  struct mms_item * seli,*runi;
  struct mms_page * runp;

  if(!mmef_yesno(fenster,"Geometrie dieses Items wirklich auf alle Seiten kopieren?")) return;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  seli=mmf_get_item_by_id(MM_CurrentPage,this_item);
  if(seli==NULL) {
    mmef_error(fenster,"Fehler beim suchend des Items.");
    return;
  }
  mmef_lock_player();
  for(runp=MM_Pages;runp;runp=runp->next) {
	if(runp!=MM_CurrentPage) {
      for(runi=runp->items;runi;runi=runi->next) {
        if(runi->data==seli->data) {
          runi->minx=seli->minx;
          runi->maxx=seli->maxx;
          runi->miny=seli->miny;
          runi->maxy=seli->maxy;
        }
      }
    }
  }
  mmef_unlock_player();
  MM_IROW_FocusItem=mmf_get_item_by_id(MM_CurrentPage,this_item);
  mmef_gtk_rebuild_items_view();
}


void mmef_items_findres (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_resource * s_res;
  struct mms_item * s_item;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  s_item=mmf_get_item_by_id(MM_CurrentPage,this_item);
  if(s_item) if(NULL!=(s_res=mmf_get_void_resource(s_item->data))) {
    MM_RROW_FocusResource=s_res;
    mmef_gtk_rebuild_resources_view();
  }
  
}

void mmef_items_georeset (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_resource * s_res;
  struct mms_item * s_item;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  s_item=mmf_get_item_by_id(MM_CurrentPage,this_item);
  if(s_item) if(NULL!=(s_res=mmf_get_void_resource(s_item->data))) {
    mmef_lock_player();
    mmf_rebuild_item_geometry(s_res);
    MM_RROW_FocusResource=s_res;
    mmef_gtk_rebuild_resources_view();
    mmef_unlock_player();
  }
  
}

void mmef_items_cloneres (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_resource * s_res;
  struct mms_item * s_item;
  GLdouble minx,miny,maxx,maxy,light;
  GLuint drop_shadow;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  s_item=mmf_get_item_by_id(MM_CurrentPage,this_item);
  if(s_item) if(NULL!=(s_res=mmf_get_void_resource(s_item->data))) {
    mmef_lock_player();
    minx=s_item->minx;
    maxx=s_item->maxx;
    miny=s_item->miny;
    maxy=s_item->maxy;
    light=s_item->light;
    drop_shadow=s_item->drop_shadow;
    if(s_res=mmf_clone_resource(s_res)) {
  	  mmf_delete_item(MM_CurrentPage,this_item);
  	  s_item=mmf_insert_item(MM_CurrentPage->id,1,s_res,0,0,MM_NO_ACTION,0,minx,maxx,miny,maxy,0,drop_shadow,0,light);
  	  MM_RROW_FocusResource=s_res;
  	  MM_IROW_FocusItem=s_item;
  	  mmef_gtk_rebuild_resources_view();
  	  mmef_gtk_rebuild_items_view();
	}
    mmef_unlock_player();
  }
  
}


void mmef_items_editres (GtkWidget *menuitem, gpointer userdata) {
  GLuint this_item;
  struct mms_resource * s_res;
  struct mms_item * s_item;
  GLdouble minx,miny,maxx,maxy;
  GLuint drop_shadow;

  this_item=mmef_get_selected_item(GTK_TREE_VIEW(viewi));
  s_item=mmf_get_item_by_id(MM_CurrentPage,this_item);
  if(s_item) if(NULL!=(s_res=mmf_get_void_resource(s_item->data))) {
    MM_RROW_FocusResource=s_res;
    MM_IROW_FocusItem=s_item;
    mmef_gtk_rebuild_resources_view();
    mmef_gtk_rebuild_items_view();
    if(s_res->type==MM_TEXT_BUTTON) {
      mmef_newresource_text_button(s_res);
    } else if(s_res->type==MM_TEXT) {
      mmef_newresource_text(s_res);
    } else {
      mmef_error(fenster,"Dieser Resourcentyp kann noch nicht bearbeitet werden.\nBitte loeschen und neu anlegen.\n");
    }
  }
  
}


void mmef_view_popup_items (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkWidget *menu, *mi_del, *mi_edit, *mi_findres, *mi_clone, *mi_editres, *mi_cloneres, *mi_sbted, *mi_c2ap, *mi_cgeo, *mi_pgeo, *mi_edanim;
  
  static GtkItemFactoryEntry entries[] = {
    { "/Bearbeiten",            NULL,   mmef_items_edit,    0, "<Item>" },
    { "/_Geometrie",            NULL,   NULL,               0, "<Branch>" },
    { "/Geometrie/Kopieren",    NULL,   mmef_items_cgeo,    0, "<Item>" },
    { "/Geometrie/Einfuegen",   NULL,   mmef_items_pgeo,    0, "<Item>" },
    { "/Geometrie/Verteilen",   NULL,   mmef_items_geo2ap,    0, "<Item>" },
    { "/Geometrie/Zuruecksetzen",  NULL,   mmef_items_georeset,   0, "<Item>" },
    { "/_Klonen",               NULL,   NULL,               0, "<Branch>" },
    { "/Klonen/Item",           NULL,   mmef_items_clone,   0, "<Item>" },
    { "/Klonen/Verteilen", NULL,   mmef_items_c2ap,   0, "<Item>" },
    { "/Klonen/Resource",       NULL,   mmef_items_cloneres,0, "<Item>" },
    { "/_Resource",               NULL,   NULL,               0, "<Branch>" },
    { "/Resource/Bearbeiten",  NULL,   mmef_items_editres,   0, "<Item>" },
    { "/Resource/Anzeigen",  NULL,   mmef_items_findres,   0, "<Item>" },
    { "/SeeByTouch Bearbeiten",  NULL,   mmef_items_sbtedit,   0, "<Item>" },
    { "/_Animation",  NULL,   NULL,   0, "<Branch>" },
    { "/Animation/Bearbeiten",  NULL,   mmef_items_edanim,   0, "<Item>" },
    { "/Animation/Verteilen",  NULL,   mmef_items_spreadanim,   0, "<Item>" },
    { "/Animation/Kopieren",  NULL,   mmef_items_canim,   0, "<Item>" },
    { "/Animation/Einfuegen",  NULL,   mmef_items_panim,   0, "<Item>" },
    { "/Animation/Zuruecksetzen",  NULL,   mmef_items_delanim,   0, "<Item>" },
    { "/Loeschen",            NULL,   mmef_items_delete,    0, "<Item>" },
  };
  static gint nmenu_items = sizeof (entries) / sizeof (entries[0]);
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  
  accel_group = gtk_accel_group_new ();
  item_factory = gtk_item_factory_new (GTK_TYPE_MENU, "<main>", accel_group);

  gtk_item_factory_create_items (item_factory, nmenu_items, entries, NULL);
  gtk_window_add_accel_group (GTK_WINDOW (fenster), accel_group);
  
  menu=gtk_item_factory_get_widget (item_factory, "<main>");
/*  
  
  menu = gtk_menu_new();
  mi_cgeo = gtk_menu_item_new_with_label("Copy Geometry");
  mi_pgeo = gtk_menu_item_new_with_label("Paste Geometry");
  mi_del = gtk_menu_item_new_with_label("Entfernen");
  mi_edanim = gtk_menu_item_new_with_label("Animation");
  mi_edit= gtk_menu_item_new_with_label("Bearbeiten");
  mi_clone= gtk_menu_item_new_with_label("Clonen");
  mi_c2ap= gtk_menu_item_new_with_label("Clonen (alle Seiten)");
  mi_cloneres= gtk_menu_item_new_with_label("Res. Clonen+Ersetzen");
  mi_editres= gtk_menu_item_new_with_label("Res. Bearbeiten");
  mi_findres= gtk_menu_item_new_with_label("Res. Finden");
  mi_sbted= gtk_menu_item_new_with_label("SBT Parameter");

  g_signal_connect(mi_edit, "activate", (GCallback) mmef_items_edit, treeview);
  g_signal_connect(mi_cgeo, "activate", (GCallback) mmef_items_cgeo, treeview);
  g_signal_connect(mi_pgeo, "activate", (GCallback) mmef_items_pgeo, treeview);
  g_signal_connect(mi_del, "activate", (GCallback) mmef_items_delete, treeview);
  g_signal_connect(mi_clone, "activate", (GCallback) mmef_items_clone, treeview);
  g_signal_connect(mi_c2ap, "activate", (GCallback) mmef_items_c2ap, treeview);
  g_signal_connect(mi_cloneres, "activate", (GCallback) mmef_items_cloneres, treeview);
  g_signal_connect(mi_findres, "activate", (GCallback) mmef_items_findres, treeview);
  g_signal_connect(mi_editres, "activate", (GCallback) mmef_items_editres, treeview);
  g_signal_connect(mi_sbted, "activate", (GCallback) mmef_items_sbtedit, treeview);
  g_signal_connect(mi_edanim, "activate", (GCallback) mmef_items_edanim, treeview);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_edit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_edanim);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_clone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_c2ap);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_findres);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_editres);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_cloneres);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_sbted);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_cgeo);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_pgeo);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi_del);

*/
  gtk_widget_show_all(menu);
   
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}


gboolean mmef_items_button_event (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
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

        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(viewi));

        /* Note: gtk_tree_selection_count_selected_rows() does not
         *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
        if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
        {
           GtkTreePath *path;

           /* Get tree path for row that was clicked */
           if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(viewi),
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

      mmef_view_popup_items(treeview, event, userdata);

      return TRUE; /* we handled this */
    }

    return FALSE; /* we did not handle this */
}

gboolean mmef_items_popup_event (GtkWidget *treeview, gpointer userdata) {
    mmef_view_popup_items(treeview, NULL, userdata);
    return TRUE; /* we handled this */
}


