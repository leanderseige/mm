
int mmef_fill_resources_tree () {
  GtkTreeIter toplevel, child;
  struct mms_resource * runner;
  int x;
  int rowc0,rowc1;

  rowc0=0;
  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_TEXT_BUTTON) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Text Buttons", COL_RES_ID, x, -1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_TEXT_BUTTON) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text , COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_IMAGE_BUTTON) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Image Buttons", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_IMAGE_BUTTON) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text , COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_IMAGE) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Bilder/Grafiken", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_IMAGE) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_VIDEO) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Videos", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_VIDEO) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_SOUND) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Sounds", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_SOUND) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_ASIAANIM) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "AsiaAnimatedDraws", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_ASIAANIM) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_TEXT) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Texte", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_TEXT) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }

  rowc1=0;

  for(x=0,runner=MM_Resources;runner;runner=runner->next) if(runner->type==MM_FONT) x++;
  if(x) {
  gtk_tree_store_append(treestore_resources, &toplevel, NULL);
  gtk_tree_store_set(treestore_resources, &toplevel, COL_RES_NAME, "Zeichensaetze", COL_RES_ID, x,-1 );
  rowc0++;
  runner=MM_Resources; while(runner) if(runner->type==MM_FONT) {
    gtk_tree_store_append(treestore_resources, &child, &toplevel);
    gtk_tree_store_set(treestore_resources, &child, COL_RES_NAME, runner->text ,  COL_RES_ID, runner->id,-1 );
    rowc1++; if(runner==MM_RROW_FocusResource) { MM_RROW_0=rowc0-1; MM_RROW_1=rowc1-1; }
    runner=runner->next;
  } else { runner=runner->next; }
  }
  
}

static GtkTreeModel * mmef_create_resources_tree () {

  treestore_resources = gtk_tree_store_new(NUM_RES_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT);
  mmef_fill_resources_tree();
  return GTK_TREE_MODEL(treestore_resources);
}

//-----------------------------------------------------------------------------

void mmef_resources_cell_data_func (GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data) {
  guint  id;
/*                                                                                      
  gtk_tree_model_get(model, iter, COL_RES_ID, &id, -1);
  if (!id) {
    g_object_set(renderer, "foreground", "darkblue", "foreground-set", TRUE, NULL);
  } else {
    g_object_set(renderer, "foreground-set", FALSE, NULL);
  }
*/                                                                                                                            
//    g_object_set(renderer, "text", "", NULL);
}

//-----------------------------------------------------------------------------

int mmef_fill_pages_tree() {
  GtkTreeIter toplevel, child;
  struct mms_page * runner;
  int x;
  char * t;
  int rowc0;

  rowc0=0;

  runner=MM_Pages;
  while(runner) {
    gtk_tree_store_append(treestore_pages, &toplevel, NULL);
    gtk_tree_store_set(treestore_pages, &toplevel, COL_PGS_NAME, runner->name, COL_PGS_ID, runner->id, -1 );
    if(runner==MM_PROW_FocusPage) { MM_PROW_0=rowc0; }
    rowc0++;
    runner=runner->next;
  }
}

static GtkTreeModel * mmef_create_pages_tree() {

  treestore_pages = gtk_tree_store_new(NUM_PGS_COLS, G_TYPE_STRING, G_TYPE_UINT);
  mmef_fill_pages_tree();
  return GTK_TREE_MODEL(treestore_pages);
}

//-----------------------------------------------------------------------------

int mmef_fill_items_tree (void) {
  GtkTreeIter    toplevel, child;
  struct mms_item * runner;
  int x;
  char *t, *n;
  int rowc0;

  rowc0=0;

  if(MM_CurrentPage==NULL)
    return(0);

  runner=MM_CurrentPage->items;
    while(runner) {
    gtk_tree_store_append(treestore_items, &toplevel, NULL);
    switch(runner->type) {
      case MM_TEXT_BUTTON:
      case MM_IMAGE_BUTTON:
        switch (((struct mms_button *)runner->data)->type) {
          case MM_TEXT_BUTTON:
            t="Text-Button";
            break;
          case MM_IMAGE_BUTTON:
            t="Grafik-Button";
            break;
          default:
            t="Unbekannt";
            break;
        }
        n=runner->text;
        break;
      case MM_FONT:
        t="Zeichensatz";
        n=runner->text;
        break;
      case MM_VIDEO:
        t="Video";
        n=runner->text;
        break;
      case MM_TEXT:
        t="Text";
        n=runner->text;
        break;
      case MM_IMAGE:
        t="Bild/Grafik";
        n=runner->text;
        break;
      case MM_SOUND:
        t="Sound";
        n=runner->text;
        break;
      case MM_ASIAANIM:
        t="AsiaAnim";
        n=runner->text;
        break;
      default:
        t="Unbekannt";
        n="Unbekannt";
        break;
    }
    gtk_tree_store_set(treestore_items, &toplevel, COL_ITM_NAME, n, COL_ITM_TYPE, t, COL_ITM_ID, runner->id, -1 );
    if(runner==MM_IROW_FocusItem) { MM_IROW_0=rowc0; }
    rowc0++;
    runner=runner->next;

  }
  return (0);
}

static GtkTreeModel * mmef_create_items_tree (void) {

  treestore_items=gtk_tree_store_new(NUM_ITM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT);
  mmef_fill_items_tree();
  return GTK_TREE_MODEL(treestore_items);
}

//-----------------------------------------------------------------------------

gboolean mmef_treestore_find_page_by_id(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data) {
  GLuint id;
  guint  year_of_birth;
  GtkTreeSelection selection;

  if(MM_CurrentPage==NULL)
    return(FALSE);
//  gtk_tree_model_get(model,iter,COL_PGS_ID,&id,-1);
  if(MM_CurrentPage->id==id) {
//    gtk_tree_selection_select_iter(&selection,iter);
    //alt gtk_combo_box_set_active_iter(GTK_COMBO_BOX(uni_combox01),iter);
    return(TRUE);
  } else {
    return(FALSE);
  }
}


//-----------------------------------------------------------------------------

gboolean view_selection_func (GtkTreeSelection *selection, GtkTreeModel *model, GtkTreePath *path, gboolean path_currently_selected, gpointer userdata) {
  GtkTreeIter iter;
  guint id;

  if (gtk_tree_model_get_iter(model, &iter, path)) {
    gtk_tree_model_get(model, &iter, COL_PGS_ID, &id, -1);
    if (!path_currently_selected) {
      mmef_lock_player();
//        MM_CurrentPage=mmf_get_page_by_id(id);
		MM_GTKIDLER_FOCUS_WIDGET=viewp;
        mmf_action_jump_to_page(id);
//        gtk_tree_store_clear(treestore_items);
//        mmef_fill_items_tree();
      mmef_unlock_player();
    }
  }

  return TRUE;
}

//-----------------------------------------------------------------------------

void mmef_drag_rec_items_table( GtkWidget *wgt, GdkDragContext *context, int x , int y, GtkSelectionData *seldata, guint info, guint time, gpointer userdata) {
  GtkTreeModel *model;
  GtkTreeIter   iter;
  
//  model=GTK_TREE_MODEL(userdata);
  
  printf("catch %d %d\n",x,y);
  
//  if(seldata->data==treestore_resources) printf("sel ok\n");
  if(userdata==treestore_items) printf("sel ok\n");
//  if(seldata->data==viewr) printf("sel ok\n");
//  if(userdata==viewr) printf("sel ok\n");
  
  switch(info) {
    case TARGET_STRING:
      printf("string\n");
      break;
    case TARGET_URL:
      printf("url\n");
      break;
    default:
      printf("unknown\n");
      break;
    }
  
}

//-----------------------------------------------------------------------------

GtkWidget * mmef_build_resources_table () {
  GtkWidget *view;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkTreeModel  *model;

  
  view=gtk_tree_view_new();
  
  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"ID");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_RES_ID);

  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_RES_NAME);
  
  g_object_set(renderer,"weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

  gtk_tree_view_column_set_cell_data_func(col, renderer, mmef_resources_cell_data_func, NULL, NULL);

  model=mmef_create_resources_tree();
  
//  gtk_tree_view_enable_model_drag_dest( GTK_TREE_VIEW(view), NULL,0,0);
  gtk_tree_view_enable_model_drag_source( GTK_TREE_VIEW(view), GDK_BUTTON1_MASK, targetentries,3, GDK_ACTION_COPY|GDK_ACTION_MOVE|GDK_ACTION_LINK);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(view),model);

g_signal_connect(view, "button-press-event", (GCallback) mmef_resources_button_event, NULL);
g_signal_connect(view, "popup-menu", (GCallback) mmef_resources_popup_event, NULL);
  
  g_object_unref(model);
  
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_SINGLE);
  
  return(view);
  
}

//-----------------------------------------------------------------------------


GtkWidget * mmef_build_pages_table() {
  GtkWidget *view;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkTreeModel  *model;
  GtkTreeSelection  *selection;
  

  view=gtk_tree_view_new();

  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"ID");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_PGS_ID);

  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_PGS_NAME);
  
  g_object_set(renderer,"weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
  
  model=mmef_create_pages_tree();
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(view),model);
  
  g_object_unref(model);
  
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_SINGLE);

g_signal_connect(view, "button-press-event", (GCallback) mmef_pages_button_event, NULL);
g_signal_connect(view, "popup-menu", (GCallback) mmef_pages_popup_event, NULL);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_select_function(selection, view_selection_func, NULL, NULL);
  
  return(view);
  
}

//-----------------------------------------------------------------------------


GtkWidget * mmef_build_items_table() {
  GtkWidget *view;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkTreeModel  *model;
  GtkTreeSelection  *selection;
  
  view=gtk_tree_view_new();

  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"ID");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_ITM_ID);

  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"Typ");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_ITM_TYPE);


  col=gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col,"Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer=gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col,renderer,TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_ITM_NAME);
  
  g_object_set(renderer,"weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
  
  model=mmef_create_items_tree();

  gtk_tree_view_enable_model_drag_dest( GTK_TREE_VIEW(view), targetentries, 3, GDK_ACTION_COPY|GDK_ACTION_MOVE|GDK_ACTION_LINK);
//  gtk_tree_view_enable_model_drag_source( GTK_TREE_VIEW(view), GDK_BUTTON1_MASK, NULL,0,0);
//  gtk_drag_dest_set(view, GTK_DEST_DEFAULT_ALL, targetentries, 3, GDK_ACTION_COPY|GDK_ACTION_MOVE|GDK_ACTION_LINK);
//  gtk_signal_connect(view,"drag_data_received", G_CALLBACK(mmef_drag_rec_items_table), treestore_items);

//  gtk_signal_connect(GTK_OBJECT(view),"drag_drop", G_CALLBACK(mmef_drag_rec_items_table), treestore_items);
  gtk_signal_connect(GTK_OBJECT(view),"drag-data-received", G_CALLBACK(mmef_drag_rec_items_table), treestore_items);
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(view),model);
  
  g_object_unref(model);
  
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_SINGLE);

g_signal_connect(view, "button-press-event", (GCallback) mmef_items_button_event, NULL);
g_signal_connect(view, "popup-menu", (GCallback) mmef_items_popup_event, NULL);

//  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
//  gtk_tree_selection_set_select_function(selection, view_item_selection_func, NULL, NULL);
  
  return(view);
  
}

