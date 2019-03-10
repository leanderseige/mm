void mmef_newpage_clicked(GtkObject *object) {
  GtkWidget *box,*box0;
  guint result;
  const gchar * name;
  char * insert;
  GdkColor color;
  
  dialog=gtk_dialog_new_with_buttons( "Neue Seite", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);

//  gtk_signal_connect_object(GTK_OBJECT(dialog), "response", gtk_widget_destroy, GTK_OBJECT(dialog));
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
  box=gtk_hbox_new(FALSE,0);
  box0=gtk_hbox_new(FALSE,0);

  uni_text_single01=gtk_entry_new();
  uni_label01=gtk_label_new("Name");
  uni_label02=gtk_label_new("Hintergrundfarbe:");
  uni_colbut01=gtk_color_button_new_with_color(&MM_DEF_PBG);
  
  gtk_box_pack_start (GTK_BOX(box),uni_label01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),uni_text_single01, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box0),uni_label02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_colbut01, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box0, TRUE, TRUE, 2);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      name=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      insert=malloc((strlen(name)+1)*2*sizeof(char));
      if(!insert) {
        fprintf(stderr,"cannot alloc page name memory\n");
        mmef_unlock_player();
        return;
      }
      strcpy(insert,name);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&color);
      mmf_add_page(1,insert,color);
      gtk_tree_store_clear(treestore_pages);
      mmef_fill_pages_tree();
      gtk_widget_destroy(dialog);
      mmef_unlock_player();
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      gtk_widget_destroy(dialog);
      break;
  }  
//  g_free(name);
}

//-----------------------------------------------------------------------------

void mmef_newresource_clicked(GtkObject *object) {
  GtkWidget *box,*box0,*box1;
  guint result;
  const gchar * name;
  char * insert;

  int type;
  gchar *text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  
  if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX(c_resources), &iter)) {
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(c_resources));
    gtk_tree_model_get (model, &iter, 0, &text, -1);
    if(!strcmp("Zeichensatz",text)) {
      mmef_newresource_font();
    } else if(!strcmp("Text Button",text)) {
      mmef_newresource_text_button(NULL);
    } else if(!strcmp("Grafik Button",text)) {
      mmef_newresource_image_button();
    } else if(!strcmp("Video",text)) {
      mmef_newresource_video();
    } else if(!strcmp("Bild/Grafik",text)) {
      mmef_newresource_image();
    } else if(!strcmp("Text",text)) {
      mmef_newresource_text(NULL);
    } else if(!strcmp("Sound",text)) {
      mmef_newresource_sound();
    } else if(!strcmp("AnimatedAsiaDraw",text)) {
      mmef_newresource_asiaanim();
    } else {
      printf ("Auswahl: %s\n", text);
    }
//    gtk_widget_destroy(dialog);
//    g_free (text);
  }
}
