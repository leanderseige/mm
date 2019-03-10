void mmef_animation_editor_makeeval(gchar * string,void ** eval) {

  if(*eval) evaluator_destroy(*eval);
  if(string==NULL) {
//    printf("string null\n");
    (*eval)=NULL;
    return;
  }
  if(strlen(string)<1) {
//    printf("string nully\n");
    (*eval)=NULL;
    return;
  }
  (*eval)=evaluator_create(string);
  if(!(*eval)) {
    printf("funktion fehlerhaft: %s\n",string);
  } else {
//    printf("funktion ok: %s\n",string);
  }
}

void mmef_animation_editor_getstring(GtkWidget * entry, gchar ** string) {
  const gchar *text;
  gchar *temp;

  if((*string)) g_free(*string);

  text=gtk_entry_get_text(GTK_ENTRY(entry));
  if(text==NULL) {
    (*string)=NULL; return;
  }
  temp=g_strdup(text);
  g_strstrip(temp);
  if(strlen(temp)>0) {
    printf("taken: %s\n",temp);
    (*string)=temp;
  } else {
    (*string)=NULL;
    g_free(temp);
  }
}

void mmef_animation_editor(struct mms_evalanim * anim) {
  GtkWidget *box,*box0,*box1,*box2,*box3,*box4,*box5,*box6,*box7,*box8,*box9;
  guint result;
  const gchar * path;
  const gchar * text;
  GLdouble w,h;

  dialog=gtk_dialog_new_with_buttons( "Animation bearbeiten", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box =gtk_vbox_new(FALSE,2);
  box1=gtk_hbox_new(FALSE,2);
  box2=gtk_hbox_new(FALSE,2);
  box3=gtk_hbox_new(FALSE,2);
  box4=gtk_hbox_new(FALSE,2);
  box5=gtk_hbox_new(FALSE,2);
  box6=gtk_hbox_new(FALSE,2);
  box7=gtk_hbox_new(FALSE,2);
  box8=gtk_hbox_new(FALSE,2);
  box9=gtk_hbox_new(FALSE,2);

  uni_label01=gtk_label_new("Rotation X:");
  uni_text_single01=gtk_entry_new();
  if(anim->str_rx) gtk_entry_set_text(GTK_ENTRY(uni_text_single01),anim->str_rx);

  uni_label02=gtk_label_new("Rotation Y:");
  uni_text_single02=gtk_entry_new();
  if(anim->str_ry) gtk_entry_set_text(GTK_ENTRY(uni_text_single02),anim->str_ry);

  uni_label03=gtk_label_new("Rotation Z:");
  uni_text_single03=gtk_entry_new();
  if(anim->str_rz) gtk_entry_set_text(GTK_ENTRY(uni_text_single03),anim->str_rz);

  uni_label04=gtk_label_new("Translation X:");
  uni_text_single04=gtk_entry_new();
  if(anim->str_tx) gtk_entry_set_text(GTK_ENTRY(uni_text_single04),anim->str_tx);

  uni_label05=gtk_label_new("Translation Y:");
  uni_text_single05=gtk_entry_new();
  if(anim->str_ty) gtk_entry_set_text(GTK_ENTRY(uni_text_single05),anim->str_ty);

  uni_label06=gtk_label_new("Translation Z:");
  uni_text_single06=gtk_entry_new();
  if(anim->str_tz) gtk_entry_set_text(GTK_ENTRY(uni_text_single06),anim->str_tz);

  uni_label07=gtk_label_new("Skalierung W:");
  uni_text_single07=gtk_entry_new();
  if(anim->str_sw) gtk_entry_set_text(GTK_ENTRY(uni_text_single07),anim->str_sw);

  uni_label08=gtk_label_new("Skalierung H");
  uni_text_single08=gtk_entry_new();
  if(anim->str_sh) gtk_entry_set_text(GTK_ENTRY(uni_text_single08),anim->str_sh);
  
  uni_label09=gtk_label_new("Dauer (s):");
  uni_spin01=gtk_spin_button_new_with_range(0,1000,0.001);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),anim->playtime);
      
  gtk_box_pack_start (GTK_BOX(box1),uni_label01, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box1),uni_text_single01, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_label02, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box2),uni_text_single02, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box3),uni_label03, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box3),uni_text_single03, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_label04, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box4),uni_text_single04, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box5),uni_label05, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box5),uni_text_single05, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box6),uni_label06, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box6),uni_text_single06, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box7),uni_label07, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box7),uni_text_single07, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box8),uni_label08, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box8),uni_text_single08, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box9),uni_label09, FALSE, FALSE,2);
  gtk_box_pack_start (GTK_BOX(box9),uni_spin01, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box4, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box5, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box6, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box7, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box8, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box9, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      
      mmef_animation_editor_getstring(uni_text_single01,&(anim->str_rx));
      mmef_animation_editor_getstring(uni_text_single02,&(anim->str_ry));
      mmef_animation_editor_getstring(uni_text_single03,&(anim->str_rz));
      mmef_animation_editor_getstring(uni_text_single04,&(anim->str_tx));
      mmef_animation_editor_getstring(uni_text_single05,&(anim->str_ty));
      mmef_animation_editor_getstring(uni_text_single06,&(anim->str_tz));
      mmef_animation_editor_getstring(uni_text_single07,&(anim->str_sw));
      mmef_animation_editor_getstring(uni_text_single08,&(anim->str_sh));
      
      mmef_animation_editor_makeeval(anim->str_rx,&(anim->eval_rx));
      mmef_animation_editor_makeeval(anim->str_ry,&(anim->eval_ry));
      mmef_animation_editor_makeeval(anim->str_rz,&(anim->eval_rz));
      mmef_animation_editor_makeeval(anim->str_tx,&(anim->eval_tx));
      mmef_animation_editor_makeeval(anim->str_ty,&(anim->eval_ty));
      mmef_animation_editor_makeeval(anim->str_tz,&(anim->eval_tz));
      mmef_animation_editor_makeeval(anim->str_sw,&(anim->eval_sw));
      mmef_animation_editor_makeeval(anim->str_sh,&(anim->eval_sh));
      
      anim->playtime=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01));
      
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

