int mmef_sbt_editor(GLuint item_id) {
  GtkWidget *box,*box0,*box1,*box2,*box3;
  guint result;
  const gchar * name;
  char * insert;

  int x,id;
  gchar *text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  struct mms_item undo;
  
  char * pe_labels[]={"Aktion L:","Aktion M:","Aktion R:","Aktion E:","Aktion D:"};
  

  MM_Edit_Item=mmf_get_item_by_id(MM_CurrentPage,item_id);
  MM_EditorActionMode=MM_SBT;
  MM_Editor_Manual_Update=0;
  memcpy(&undo,MM_Edit_Item,sizeof(struct mms_item));

  dialog=gtk_dialog_new_with_buttons( "Item SBT bearbeiten", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box0=(GtkWidget *)mmef_built_action_selector(5,pe_labels);
  box =gtk_vbox_new(FALSE,10);

  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);
    
  gtk_widget_show_all(dialog);  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      for(x=0;x<5;x++) {
        if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x])) {
          gtk_tree_model_get(GTK_TREE_MODEL(avalue_store[x]), &action_iter[x], 1, &id, -1);
          MM_Edit_Item->sbtava[x]=id;
        } else {
          MM_Edit_Item->sbtava[x]=0;
        }
      }
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      break;
  }  

  gtk_widget_destroy(dialog);
}
