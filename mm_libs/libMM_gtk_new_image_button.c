void mmef_newresource_image_button() {
  GtkWidget *box,*box0,*box1,*box2;
  guint result;
  const gchar * path;
  const gchar * text;
  GLdouble w,h;

  dialog=gtk_dialog_new_with_buttons( "Grafik Button erstellen", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box =gtk_vbox_new(FALSE,8);
  box0=gtk_hbox_new(FALSE,8);
  box1=gtk_hbox_new(FALSE,8);
  box2=gtk_hbox_new(FALSE,8);

  uni_label01=gtk_label_new("Datei:");
  uni_label02=gtk_label_new("Breite:");
  uni_label03=gtk_label_new("Hoehe:");
  uni_label04=gtk_label_new("Bezeichnung:");

  uni_text_single01=gtk_entry_new();
      
  uni_filebutton=gtk_file_chooser_button_new (("Datei auswaehlen"), GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(uni_filebutton),"/etc");
      
  uni_spin01=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin02=gtk_spin_button_new_with_range(0,100,.01);
  
  gtk_box_pack_start (GTK_BOX(box0),uni_label01, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box0),uni_filebutton, TRUE,TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box2),uni_label04, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box2),uni_text_single01, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label02, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01,  FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label03, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02,  FALSE, FALSE,2);

  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 8);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      path=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(uni_filebutton));
      text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      w=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01));
      h=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02));
      if(path) {
        if(MM_RROW_FocusResource=mmf_add_button(MM_IMAGE_BUTTON,1,w,h,0,(char *)text,NULL,NULL,NULL,NULL,(char *)path,0,0,NULL)) {
          mmef_gtk_rebuild_resources_view();
        } else {
          mmef_error(fenster,"Datei konnte nicht gelesen werden.");
        }
        g_free((gpointer) path);
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

