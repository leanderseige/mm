void mmef_cleansweep() {
  struct mms_page * p_run;
  struct mms_resource * r_run;

  printf("cleansweep do\n");

  for(p_run=MM_Pages;p_run;p_run=p_run->next)
    mmf_delete_page(p_run->id);
  for(r_run=MM_Resources;r_run;r_run=r_run->next)
    mmf_delete_resource(r_run->id);
}

void mmef_cleansweep_clicked(GtkObject *object) {

  printf("cleansweep click\n");

  mmef_lock_player();

  mmef_cleansweep();

  mmef_gtk_rebuild_items_view();
  mmef_gtk_rebuild_resources_view();
  mmef_gtk_rebuild_pages_view();

  mmef_unlock_player();
}


//---------------------------------------------------------------------------------

void mmef_globals_clicked() {
  GtkWidget *box,*box0,*box1,*box2,*box3,*box4,*box5,*box6,*box01,*box02,*box001,*box002,*box003,*box004;
  guint result,flags;
  const gchar * name;
  const gchar * insert;
  GLdouble w,h,b;
  GtkTextIter iters,itere;
  
  int type,check;
  gchar *text,*full_text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  
  GdkColor fgl,bgl,fgh,bgh;
  SDL_Color fgls,bgls,fghs,bghs;
  
  struct mms_resource * runner;
  struct mms_resource * font;
  struct mms_texture * temptexture;

  dialog=gtk_dialog_new_with_buttons( "Globale Parameter", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  memcpy(&fgl,&MM_DEF_FGL,sizeof(GdkColor));
  memcpy(&bgl,&MM_DEF_BGL,sizeof(GdkColor));
  memcpy(&fgh,&MM_DEF_FGH,sizeof(GdkColor));
  memcpy(&bgh,&MM_DEF_BGH,sizeof(GdkColor));

  box001 =gtk_hbox_new(FALSE,8);
  box002 =gtk_hbox_new(FALSE,8);
  box003 =gtk_hbox_new(FALSE,8);
  box004 =gtk_hbox_new(FALSE,8);
  box01=gtk_vbox_new(FALSE,8);
  box02=gtk_vbox_new(FALSE,8);
  box0 =gtk_hbox_new(FALSE,8);
  box1 =gtk_hbox_new(FALSE,8);
  box2 =gtk_hbox_new(FALSE,8);
  box3 =gtk_hbox_new(FALSE,8);
  box4 =gtk_hbox_new(FALSE,8);
  box5 =gtk_hbox_new(FALSE,8);
  box6 =gtk_hbox_new(FALSE,8);
  box  =gtk_vbox_new(FALSE,8);

  uni_label05=gtk_label_new("Gitter horiz.");
  uni_label06=gtk_label_new("Gitter vert.");
  uni_spin04=gtk_spin_button_new_with_range(0,200,1);
  uni_spin05=gtk_spin_button_new_with_range(0,200,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_DEF_GridX-1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin05),MM_DEF_GridY-1);

  uni_label07=gtk_label_new("min. Feldbreite:");
  uni_label08=gtk_label_new("Zeilenhoehe:");
  uni_label09=gtk_label_new("Innenabstand:");
  uni_spin01=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin02=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin03=gtk_spin_button_new_with_range(0,100,.01);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_DEF_TextItemWidth);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_DEF_TextLineHeight);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_DEF_TextBorderWidth);

  uni_label01=gtk_label_new("Vordergrund Farbe\n(normal)");
  uni_label02=gtk_label_new("Hintergrund Farbe\n(normal)");
  uni_label03=gtk_label_new("Vordergrund Farbe\n(highlighted)");
  uni_label04=gtk_label_new("Hintergrund Farbe\n(highlighted)");
  uni_colbut01=gtk_color_button_new_with_color(&MM_DEF_FGL);
  uni_colbut02=gtk_color_button_new_with_color(&MM_DEF_BGL);
  uni_colbut03=gtk_color_button_new_with_color(&MM_DEF_FGH);
  uni_colbut04=gtk_color_button_new_with_color(&MM_DEF_BGH);

  uni_label10=gtk_label_new("Seitenhintergrundfarbe:");
  uni_colbut05=gtk_color_button_new_with_color(&MM_DEF_PBG);

  uni_label11=gtk_label_new("Asiadraw Vordergrund");
  uni_colbut06=gtk_color_button_new_with_color(&MM_DEF_AFG);
  uni_label12=gtk_label_new("Asiadraw Hintergrund");
  uni_colbut07=gtk_color_button_new_with_color(&MM_DEF_ABG);

  gtk_box_pack_start (GTK_BOX(box6),uni_label11, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box6),uni_colbut06, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box6),uni_label12, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box6),uni_colbut07, TRUE, TRUE, 2);
      
  gtk_box_pack_start (GTK_BOX(box0),uni_label05, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_spin04, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_label06, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_spin05, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label07, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label08, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label09, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin03, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box001),uni_label01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box001),uni_colbut01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box002),uni_label02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box002),uni_colbut02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box01),box001, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box01),box002, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box003),uni_label03, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box003),uni_colbut03, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box004),uni_label04, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box004),uni_colbut04, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box02),box003, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box02),box004, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box5),uni_label10, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box5),uni_colbut05, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box4),box01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box4),box02, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box4, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box5, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box6, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  printf("returned\n");
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&MM_DEF_FGL);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut02),&MM_DEF_BGL);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut03),&MM_DEF_FGH);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut04),&MM_DEF_BGH);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut05),&MM_DEF_PBG);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut06),&MM_DEF_AFG);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut07),&MM_DEF_ABG);
      MM_DEF_GridX=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04))+1;
      MM_DEF_GridY=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin05))+1;
      MM_DEF_TextItemWidth=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01));
      MM_DEF_TextLineHeight=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02));
      MM_DEF_TextBorderWidth=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03));
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      break;
  }  
  gtk_widget_destroy(dialog);
}
