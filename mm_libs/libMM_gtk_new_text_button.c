void mmef_newresource_text_button(struct mms_resource * old_res) {
  GtkWidget *box,*box0,*box1,*box2,*box01,*box02,*box001,*box002,*box003,*box004;
  guint result,flags;
  const gchar * text;
  GLdouble w,h,b,bow;
  int type,check;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  GdkColor fgl,bgl,fgh,bgh;
  SDL_Color fgls,bgls,fghs,bghs;
  struct mms_resource * runner;
  struct mms_resource * font;
  struct mms_button * old_but;
  GLuint req_id;
  
  if(old_res!=NULL) old_but=(struct mms_button *) old_res->data;
  else old_but=NULL;

  flags=TTF_STYLE_NORMAL;
  uni_combox01=gtk_combo_box_new_text();
  runner=MM_Resources;
  check=0;
  while(runner) {
    if(runner->type==MM_FONT) {
      gtk_combo_box_append_text(GTK_COMBO_BOX(uni_combox01),runner->text);
      check=1;
    }
    runner=runner->next;
  }
  if(check==0) {
    mmef_error(fenster,"Laden Sie bitte erst mindestens einen Zeichensatz.");
    gtk_widget_destroy(uni_combox01);
    return;
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox01),0);

  dialog=gtk_dialog_new_with_buttons( "Text Button erstellen", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box001 =gtk_hbox_new(FALSE,8);
  box002 =gtk_hbox_new(FALSE,8);
  box003 =gtk_hbox_new(FALSE,8);
  box004 =gtk_hbox_new(FALSE,8);
  box0 =gtk_hbox_new(FALSE,8);
  box1 =gtk_hbox_new(FALSE,8);
  box2 =gtk_hbox_new(FALSE,8);
  box  =gtk_vbox_new(FALSE,8);
  box01=gtk_vbox_new(FALSE,8);
  box02=gtk_vbox_new(FALSE,8);

  uni_text_single01=gtk_entry_new();
  uni_label01=gtk_label_new("Beschriftung:");
  uni_label02=gtk_label_new("Zeichensatz:");

  uni_label07=gtk_label_new("min. Buttonbreite:");
  uni_label08=gtk_label_new("Texthoehe:");
  uni_label09=gtk_label_new("Innenabstand:");
  uni_spin01=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin02=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin03=gtk_spin_button_new_with_range(0,100,.01);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_DEF_TextItemWidth*100.0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_DEF_TextLineHeight*100.0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_DEF_TextBorderWidth*100.0);

  uni_spin04=gtk_spin_button_new_with_range(-1,1,.01);
  uni_label11=gtk_label_new("Woelbung");
  
  memcpy(&fgl,&MM_DEF_FGL,sizeof(GdkColor));
  memcpy(&bgl,&MM_DEF_BGL,sizeof(GdkColor));
  memcpy(&fgh,&MM_DEF_FGH,sizeof(GdkColor));
  memcpy(&bgh,&MM_DEF_BGH,sizeof(GdkColor));
  uni_colbut01=gtk_color_button_new_with_color(&fgl);
  uni_colbut02=gtk_color_button_new_with_color(&bgl);
  uni_colbut03=gtk_color_button_new_with_color(&fgh);
  uni_colbut04=gtk_color_button_new_with_color(&bgh);
  
  uni_toggle01=gtk_toggle_button_new_with_label("  Fett  ");
  uni_toggle02=gtk_toggle_button_new_with_label(" Kursiv ");

  uni_label03=gtk_label_new("Vordergrund Farbe\n(normal)");
  uni_label04=gtk_label_new("Hintergrund Farbe\n(normal)");
  uni_label05=gtk_label_new("Vordergrund Farbe\n(highlighted)");
  uni_label06=gtk_label_new("Hintergrund Farbe\n(highlighted)");
      
  gtk_box_pack_start (GTK_BOX(box0),uni_label01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_text_single01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_label02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_combox01, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle01, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle02, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label07, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label08, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label09, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin03, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label11, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin04, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box001),uni_label03, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box001),uni_colbut01, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box002),uni_label04, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box002),uni_colbut02, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box01),box001, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box01),box002, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box003),uni_label05, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box003),uni_colbut03, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box004),uni_label06, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box004),uni_colbut04, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box02),box003, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box02),box004, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box2),box01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box2),box02, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);
  
  if(old_res!=NULL) {
    gtk_entry_set_text(GTK_ENTRY(uni_text_single01),old_but->text);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut01),&old_but->fgl);    
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut02),&old_but->bgl);    
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut03),&old_but->fgh);    
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut04),&old_but->bgh);    
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),old_but->build_w*100);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),old_but->build_h*100);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),old_but->build_b*100);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),old_but->bow);
    if(old_but->flags&TTF_STYLE_BOLD)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle01),TRUE);
    if(old_but->flags&TTF_STYLE_ITALIC)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle02),TRUE);
    req_id=old_res->id;
  } else {
    req_id=1;
  }

    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX(uni_combox01), &iter)==TRUE) {
        model = gtk_combo_box_get_model (GTK_COMBO_BOX(uni_combox01));
        gtk_tree_model_get (model, &iter, 0, &text, -1);
        runner=MM_Resources; font=NULL;
        while(runner) {
          if(runner->type==MM_FONT && !strcmp(runner->text,text))
            font=runner;
          runner=runner->next;
        }
        if(!font) {
          mmef_error(fenster,"Sie haben keinen Zeichensatz ausgewaehlt!");
          return;
        } 
      } else {
          mmef_error(fenster,"Sie haben keinen Zeichensatz ausgewaehlt!");
          return;
      }
      mmef_lock_player();
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle01)))
          flags|=TTF_STYLE_BOLD;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle02)))
          flags|=TTF_STYLE_ITALIC;
        w=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01))/100.0;
        h=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02))/100.0;
        b=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03))/100.0;
        bow=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&fgl);
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut02),&bgl);
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut03),&fgh);
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut04),&bgh);
        fgls.r=fgl.red>>8; fgls.g=fgl.green>>8; fgls.b=fgl.blue>>8;
        bgls.r=bgl.red>>8; bgls.g=bgl.green>>8; bgls.b=bgl.blue>>8;
        fghs.r=fgh.red>>8; fghs.g=fgh.green>>8; fghs.b=fgh.blue>>8;
        bghs.r=bgh.red>>8; bghs.g=bgh.green>>8; bghs.b=bgh.blue>>8;
        text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
        if(strlen(text)) {
          if(MM_RROW_FocusResource=mmf_add_button(MM_TEXT_BUTTON,req_id,w,h,b,(char *)text,&fgl,&bgl,&fgh,&bgh,font,flags,bow,old_but)) {
            if(old_res!=NULL) mmf_rebuild_item_geometry(old_res);
            mmef_gtk_rebuild_resources_view();
          } else {
            mmef_error(fenster,"Text Button konnte nicht erstellt werden.");
          }
        } else {
          mmef_error(fenster,"Es wurde keine Beschriftung angegeben.");
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
