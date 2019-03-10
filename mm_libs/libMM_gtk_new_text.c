void mmef_newresource_text_test_render_callback() {
  mmf_texture_proc(MM_EditorPrevTexture);
  glEnable(GL_TEXTURE_RECTANGLE_NV);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,MM_EditorPrevTexture->name);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f  (MM_EditorPrevGeometry.x, MM_EditorPrevGeometry.y);        
    glTexCoord2f(MM_EditorPrevTexture->w, 0);
    glVertex2f  (MM_EditorPrevGeometry.x+MM_EditorPrevGeometry.w, MM_EditorPrevGeometry.y);        
    glTexCoord2f(MM_EditorPrevTexture->w, MM_EditorPrevTexture->h);
    glVertex2f  (MM_EditorPrevGeometry.x+MM_EditorPrevGeometry.w, MM_EditorPrevGeometry.y+MM_EditorPrevGeometry.h);       
    glTexCoord2f(0, MM_EditorPrevTexture->h);
    glVertex2f  (MM_EditorPrevGeometry.x, MM_EditorPrevGeometry.y+MM_EditorPrevGeometry.h);       
  glEnd();
}


//---------------------------------------------------------------------------------

struct mms_resource * mmef_newresource_text_guiget_font() {
  char * ftext;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  struct mms_resource *runner,*font;

  if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX(uni_combox01), &iter)==TRUE) {
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(uni_combox01));
    gtk_tree_model_get (model, &iter, 0, &ftext, -1);
    runner=MM_Resources; font=NULL;
    while(runner) {
      if(runner->type==MM_FONT && !strcmp(runner->text,ftext))
        return(runner);
      runner=runner->next;
    }
  }
  return(NULL);

}

//---------------------------------------------------------------------------------

char * mmef_newresource_text_guiget_text() {
  GtkTextIter iters,itere;

  gtk_text_buffer_get_start_iter(ml_buffer, &iters);
  gtk_text_buffer_get_end_iter(ml_buffer, &itere);
  return(gtk_text_buffer_get_text(ml_buffer,&iters,&itere,0));
}

//---------------------------------------------------------------------------------

GLuint mmef_newresource_text_guiget_align() {

  switch(gtk_combo_box_get_active(GTK_COMBO_BOX(uni_combox02))) {
    case 1:
      return(MM_RIGHT);
      break;
    case 2:
      return(MM_CENTER);
      break;
    case 3:
      return(MM_BLOCK);
      break;
    case 0:
    default:
      return(MM_LEFT);
      break;
  }
}

//---------------------------------------------------------------------------------

struct mms_texture * mmef_newresource_text_render() {
  char * text;
  GLuint align,ina,flags,alpha;
  GLdouble line_height,field_width,border_width,bow;
  GdkColor fg,bg;
  SDL_Color fgs,bgs;
  struct mms_texture * temptexture;
  struct mms_resource * font;
  
  text=mmef_newresource_text_guiget_text();
  font=mmef_newresource_text_guiget_font();
  if(!font) {
    mmef_error("Kein Zeichensatz gefunden.\n");
    return(NULL);
  }
  field_width=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01))/100.0;
  line_height=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02))/100.0;
  border_width=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03))/100.0;
  gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&fg);
  gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut02),&bg);
  align=mmef_newresource_text_guiget_align();
  flags=0;
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle01))) flags|=TTF_STYLE_BOLD;
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle02))) flags|=TTF_STYLE_ITALIC;
  alpha=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle03))?1:0;
  bow=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));

  fgs.r=fg.red>>8; fgs.g=fg.green>>8; fgs.b=fg.blue>>8;
  bgs.r=bg.red>>8; bgs.g=bg.green>>8; bgs.b=bg.blue>>8;

  mmef_lock_player();
  temptexture=mmf_build_text_plate(text,(struct mms_font *)(font->data),&fgs,&bgs,line_height*1.1,field_width,border_width,align,flags,alpha,bow);
  mmef_unlock_player();

  return(temptexture);
}

//---------------------------------------------------------------------------------

void mmef_newresource_text_test_render() {

  MM_EditorPrevTexture=mmef_newresource_text_render();

  MM_EditorPrevGeometry.w=MM_EditorPrevTexture->w/(GLdouble)MM_ScreenWidthDisplay;
  MM_EditorPrevGeometry.h=MM_EditorPrevTexture->h/(GLdouble)MM_ScreenHeightDisplay;
  MM_EditorPrevGeometry.x=0.5-MM_EditorPrevGeometry.w/2;
  MM_EditorPrevGeometry.y=0.5-MM_EditorPrevGeometry.h/2;

  mmef_lock_player();
  MM_EditorFlash=mmef_newresource_text_test_render_callback;
  mmef_unlock_player();
  mmef_info(fenster,"Zum Beenden der Vorschau\nbitte hier klicken.");
  MM_EditorPrevTexture->status=MM_TM_REMOVE;
  mmef_lock_player();
  MM_EditorFlash=NULL;
  mmef_unlock_player();
}

//---------------------------------------------------------------------------------

void mmef_newresource_text(struct mms_resource * old_res) {
  GtkWidget *box,*box0,*box1,*box2,*box3,*box4,*box01,*box02,*box001,*box002,*box003,*box004;
  GtkWidget *sw1;
  guint result,flags;
  const gchar * name;
  const gchar * insert;
  GLdouble w,h,b,bow;
  GtkTextIter iters,itere;
  GLuint align;
  GLdouble line_height,field_width,border_width;
  GdkColor fg,bg;
  struct mms_text * old_text;
  
  int type,check;
  gchar *text,*full_text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  
  GdkColor fgl,bgl,fgh,bgh;
  SDL_Color fgls,bgls,fghs,bghs;
  
  struct mms_resource * runner;
  struct mms_resource * font;
  struct mms_texture * temptexture;


  if(old_res!=NULL) {
    old_text=(struct mms_text *)old_res->data;
  } else {
    old_text=NULL;
  }

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
  
  uni_combox02=gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(uni_combox02),"Linksbuendig");
  gtk_combo_box_append_text(GTK_COMBO_BOX(uni_combox02),"Rechtsbuendig");
  gtk_combo_box_append_text(GTK_COMBO_BOX(uni_combox02),"Zentriert");
  gtk_combo_box_append_text(GTK_COMBO_BOX(uni_combox02),"Blocksatz");
  if(old_text!=NULL) {
	switch (old_text->align) {
	  case MM_RIGHT:
		gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox02),1);
		break;
	  case MM_CENTER:
		gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox02),2);
		break;
	  case MM_BLOCK:
		gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox02),3);
		break;
	  case MM_LEFT:
	  default:
		gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox02),0);
		break;
	}
  } else {
	gtk_combo_box_set_active(GTK_COMBO_BOX(uni_combox02),0);
  }

  dialog=gtk_dialog_new_with_buttons( "Text erstellen", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box001 =gtk_hbox_new(FALSE,8);
  box002 =gtk_hbox_new(FALSE,8);
  box003 =gtk_hbox_new(FALSE,8);
  box004 =gtk_hbox_new(FALSE,8);
  box0 =gtk_hbox_new(FALSE,8);
  box1 =gtk_hbox_new(FALSE,8);
  box2 =gtk_hbox_new(FALSE,8);
  box3 =gtk_hbox_new(FALSE,8);
  box4 =gtk_hbox_new(FALSE,8);
  box  =gtk_vbox_new(FALSE,8);
  box01=gtk_vbox_new(FALSE,8);
  box02=gtk_vbox_new(FALSE,8);

  ml_view=gtk_text_view_new();
  ml_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(ml_view));
  gtk_text_buffer_set_text(ml_buffer,"Geben Sie hier Ihren Text ein...",-1);
  sw1=gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add(GTK_CONTAINER(sw1),ml_view);
  
  uni_button01=gtk_button_new_with_label("Render Test");
  gtk_signal_connect_object(GTK_OBJECT(uni_button01), "clicked",GTK_SIGNAL_FUNC(mmef_newresource_text_test_render), GTK_OBJECT(fenster));

  uni_label02=gtk_label_new("Zeichensatz:");

  uni_label07=gtk_label_new("min. Feldbreite:");
  uni_label08=gtk_label_new("Zeilenhoehe:");
  uni_label09=gtk_label_new("Innenabstand:");
  uni_spin01=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin02=gtk_spin_button_new_with_range(0,100,.01);
  uni_spin03=gtk_spin_button_new_with_range(0,100,.01);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_DEF_TextItemWidth*100);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_DEF_TextLineHeight*100);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_DEF_TextBorderWidth*100);

  uni_spin04=gtk_spin_button_new_with_range(-1,1,.01);
  uni_label11=gtk_label_new("Woelbung");

  uni_text_single01=gtk_entry_new();
  uni_label10=gtk_label_new("Bezeichnung:");
  
  uni_toggle01=gtk_toggle_button_new_with_label("  Fett  ");
  uni_toggle02=gtk_toggle_button_new_with_label(" Kursiv ");

  uni_toggle03=gtk_toggle_button_new_with_label("Hintergrund Transparent");

  uni_label03=gtk_label_new("Vordergrund Farbe\n(normal)");
  uni_label04=gtk_label_new("Hintergrund Farbe\n(normal)");
  uni_label05=gtk_label_new("Vordergrund Farbe\n(highlighted)");
  uni_label06=gtk_label_new("Hintergrund Farbe\n(highlighted)");
  memcpy(&fgl,&MM_DEF_FGL,sizeof(GdkColor));
  memcpy(&bgl,&MM_DEF_BGL,sizeof(GdkColor));
  memcpy(&fgh,&MM_DEF_FGH,sizeof(GdkColor));
  memcpy(&bgh,&MM_DEF_BGH,sizeof(GdkColor));
  uni_colbut01=gtk_color_button_new_with_color(&fgl);
  uni_colbut02=gtk_color_button_new_with_color(&bgl);
  uni_colbut03=gtk_color_button_new_with_color(&fgh);
  uni_colbut04=gtk_color_button_new_with_color(&bgh);
      
  gtk_box_pack_start (GTK_BOX(box3),sw1, TRUE, TRUE, 2);
  
  gtk_box_pack_start (GTK_BOX(box0),uni_button01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_label02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_combox01, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle01, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle02, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_combox02, FALSE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label07, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label08, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label09, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin03, TRUE, TRUE, 2);

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

  gtk_box_pack_start (GTK_BOX(box4),uni_toggle03, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_label10, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_text_single01, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_label11, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_spin04, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box4, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);

  if(old_res!=NULL) {
    gtk_entry_set_text(GTK_ENTRY(uni_text_single01),old_text->text);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),old_text->field_width*100.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),old_text->line_height*100.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),old_text->border_width*100.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),old_text->bow);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut01),&(old_text->fg));
    gtk_color_button_set_color(GTK_COLOR_BUTTON(uni_colbut02),&(old_text->bg));
    gtk_text_buffer_set_text(ml_buffer,old_text->full_text,-1);    
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle03),old_text->alpha?TRUE:FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle01),old_text->flags&TTF_STYLE_BOLD?TRUE:FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle02),old_text->alpha&TTF_STYLE_ITALIC?TRUE:FALSE);
  }
    
  gtk_widget_show_all(dialog);  
  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_lock_player();
      
        full_text=mmef_newresource_text_guiget_text();
        font=mmef_newresource_text_guiget_font();
        field_width=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01))/100.0;
        line_height=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02))/100.0;
        border_width=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03))/100.0;
        bow=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut01),&fg);
        gtk_color_button_get_color(GTK_COLOR_BUTTON(uni_colbut02),&bg);
        align=mmef_newresource_text_guiget_align();
        flags=0;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle01))) flags|=TTF_STYLE_BOLD;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle02))) flags|=TTF_STYLE_ITALIC;

        name=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));

        temptexture= mmef_newresource_text_render();

        if(name&&full_text) {
          if(MM_RROW_FocusResource=mmf_add_text(temptexture,(char *)name,full_text,1,font->id,field_width,line_height,border_width,&fg,&bg,align,flags,bow,old_text)) {
            if(old_res!=NULL) {
              // mmf_rebuild_item_geometry(old_res);
              free(old_res->text);
              old_res->text=malloc(strlen((char *)name)+1);
              strcpy(old_res->text,name);
            }
            mmef_gtk_rebuild_resources_view();
          } else {
            mmef_error(fenster,"Fehler beim Erstellen des Textes.");
          }
          g_free((gpointer) full_text);
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
