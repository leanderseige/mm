
//-----------------------------------------------------------------------------

int mmef_page_editor() {
  GtkWidget *box,*box0,*box1,*box2,*box3,*box4,*box5;
  guint result;
  const gchar * name,*new_text,*for_free;
  char * insert;
  char * pe_labels[]={"Aktion:"};

  int type,check,id;
  gchar *text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  struct mms_page undo;

  if(MM_CurrentPage==NULL) return(0);

  memcpy(&undo,MM_CurrentPage,sizeof(struct mms_page));

  dialog=gtk_dialog_new_with_buttons( "Seite bearbeiten", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box0=gtk_hbox_new(FALSE,8);
  box1=gtk_hbox_new(FALSE,8);
  box2=gtk_hbox_new(FALSE,8);
  MM_EditorActionMode=MM_PAGE;
  box3=(GtkWidget *)mmef_built_action_selector(1,pe_labels);
  box4=gtk_hbox_new(FALSE,8);
  box5=gtk_hbox_new(FALSE,8);
  box =gtk_vbox_new(FALSE,8);

  uni_text_single01=gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(uni_text_single01),MM_CurrentPage->name);
  uni_label01=gtk_label_new("Name:");
  uni_label02=gtk_label_new("Hintergrundfarbe");
  uni_colbut01=gtk_color_button_new_with_color(&MM_CurrentPage->bg_color);
  gtk_box_pack_start (GTK_BOX(box0),uni_label01, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box0),uni_text_single01, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(box1),uni_label02, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box1),uni_colbut01, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 8);
  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 8);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 8);
  
  gtk_widget_show_all(dialog);  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:

      mmef_lock_player();
printf("a\n");
      new_text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
      for_free=malloc(sizeof(char)*(strlen(new_text)+1));
      if(!for_free) {
        mmef_error("Kein Speicher fuer neuen Namen.");
      } else {
      printf("b1\n");
        free(MM_CurrentPage->name);
      printf("b2\n");
        MM_CurrentPage->name=(char *)for_free;
      printf("b3\n");
        strcpy(MM_CurrentPage->name,new_text);
      printf("b4\n");
      }
printf("a1\n");

      if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(action_combox[0]),&action_iter[0])) {
        gtk_tree_model_get(GTK_TREE_MODEL(action_store[0]), &action_iter[0], 1, &id, -1);
        MM_CurrentPage->action=id;
      } else {
        MM_CurrentPage->action=0;
      }
printf("a2\n");

      if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(avalue_combox[0]),&action_iter[0])) {
        gtk_tree_model_get(GTK_TREE_MODEL(avalue_store[0]), &action_iter[0], 1, &id, -1);
        MM_CurrentPage->avalue=id;
      } else {
        MM_CurrentPage->avalue=0;
      }
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&MM_CurrentPage->bg_color);
printf("a3\n");
      gtk_tree_store_clear(treestore_pages);
      mmef_fill_pages_tree();
      mmef_unlock_player();

      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      mmef_lock_player();
      undo.name=MM_CurrentPage->name;
      memcpy(MM_CurrentPage,&undo,sizeof(struct mms_page));
      mmef_unlock_player();
      break;
  }  

  gtk_widget_destroy(dialog);
}
