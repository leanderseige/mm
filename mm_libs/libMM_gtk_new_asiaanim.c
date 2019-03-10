void mmef_newresource_asiaanim() {
  GtkWidget *box,*box0,*box1,*box2,*box3;
  guint result;
  const gchar * name;
  char * insert,*n2;
  const gchar *n;
  GLdouble w,h,b;

  int type;
  gchar *text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  
  GdkColor fg,bg;

  dialog=gtk_dialog_new_with_buttons( "AsiaAnim laden", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box =gtk_vbox_new(FALSE,8);
  box0=gtk_hbox_new(FALSE,8);
  box1=gtk_hbox_new(FALSE,8);
  box2=gtk_hbox_new(FALSE,8);
  box3=gtk_hbox_new(FALSE,8);

  uni_label01=gtk_label_new("Datei:");
  uni_label02=gtk_label_new("Breite:");
  uni_label03=gtk_label_new("Hoehe:");
  uni_label04=gtk_label_new("Bezeichnung:");
  uni_label07=gtk_label_new("Innenabstand:");
  uni_text_single01=gtk_entry_new();
      
  uni_filebutton=gtk_file_chooser_button_new (("Datei auswaehlen"), GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(uni_filebutton),"/etc");
      
  uni_spin01=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin02=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin03=gtk_spin_button_new_with_range(0,100,.01);

  uni_colbut01=gtk_color_button_new_with_color(&MM_DEF_AFG);
  uni_colbut02=gtk_color_button_new_with_color(&MM_DEF_ABG);
  uni_label05=gtk_label_new("Vordergrund Farbe");
  uni_label06=gtk_label_new("Hintergrund Farbe");
  
  gtk_box_pack_start (GTK_BOX(box3),uni_label05, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box3),uni_colbut01, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box3),uni_label06, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box3),uni_colbut02, FALSE, FALSE,2);

  gtk_box_pack_start (GTK_BOX(box0),uni_label01, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box0),uni_filebutton, TRUE,TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box2),uni_label04, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box2),uni_text_single01, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label02, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01,  FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label03, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02,  FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label07, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin03,  FALSE, FALSE,2);

  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 8);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&fg);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut02),&bg);
      insert=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(uni_filebutton));
      w=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01));
      h=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02));
      b=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03));
      n=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      n2=malloc((strlen(name)+1)*2*sizeof(char));
      strcpy(n2,n);
      if(strlen(insert)) {
        if(MM_RROW_FocusResource=mmf_add_asiaanim(1,w,h,b,n2,&fg,&bg,insert)) {
          mmef_gtk_rebuild_resources_view();
        } else {
          mmef_error(fenster,"Datei konnte nicht gelesen werden.");
        }
      } else {
        mmef_error(fenster,"Es wurde kein Dateiname angegeben.");
      }
      mmef_unlock_player();
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      break;
  }
  gtk_widget_destroy(dialog);
}

