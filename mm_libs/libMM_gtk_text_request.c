char * mmef_gtk_text_request(char * label, char * old) {
  GtkWidget *box,*box0,*box1,*box2;
  guint result;
  const gchar * path;
  const gchar * text;
  char * out;
  GLdouble w,h;

  dialog=gtk_dialog_new_with_buttons( label, GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box =gtk_vbox_new(FALSE,8);
  box2=gtk_hbox_new(FALSE,8);

  uni_label04=gtk_label_new("Bezeichnung:");

  uni_text_single01=gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(uni_text_single01),old);      

  gtk_box_pack_start (GTK_BOX(box2),uni_label04, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box2),uni_text_single01, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 8);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      out=malloc(strlen(text)+1);
      strcpy(out,text);
      gtk_widget_destroy(dialog);      
      return(out);
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      gtk_widget_destroy(dialog);      
      return(NULL);
      break;
  }  

}

