void mmef_newresource_font() {
  GtkWidget *box,*box0,*box1;
  guint result;
  const gchar * name;
  const gchar * text;

  dialog=gtk_dialog_new_with_buttons( "Zeichensatz laden", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box0=gtk_hbox_new(FALSE,8);
  box1=gtk_hbox_new(FALSE,8);

  uni_label01=gtk_label_new("Datei:");
  uni_label02=gtk_label_new("Bezeichnung:");

  uni_text_single01=gtk_entry_new();
      
  uni_filebutton=gtk_file_chooser_button_new (("Select a file"), GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(uni_filebutton),"/etc");
      
  gtk_box_pack_start (GTK_BOX(box0),uni_label02, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box0),uni_text_single01, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(box1),uni_label01, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box1),uni_filebutton, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box1, FALSE, FALSE, 8);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box0, FALSE, FALSE, 8);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      name=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(uni_filebutton));
      text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      if(name) {
        if(MM_RROW_FocusResource=mmf_add_font((char *)name,(char *)text,1)) {
          mmef_gtk_rebuild_resources_view();
        } else {
          mmef_error(fenster,"Fehler beim Laden der Datei.");
        }
        g_free((gpointer) name);
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
