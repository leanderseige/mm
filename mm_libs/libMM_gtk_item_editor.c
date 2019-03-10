int mmef_item_editor_set() {
  GLdouble oldw,oldh,neww,newh,buttonw,buttonh;
  int temp_is_sbt;
  
  if(!MM_Editor_Manual_Update) {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle02))) {
      oldh=MM_Edit_Item->maxy-MM_Edit_Item->miny;
      oldw=MM_Edit_Item->maxx-MM_Edit_Item->minx;
      buttonh=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));
      buttonw=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03));
      if(oldw!=buttonw) {
        neww=buttonw;
        newh=buttonw*oldh/oldw;
//        MM_Editor_Manual_Update=1;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),newh);
//        MM_Editor_Manual_Update=0;
      } else if(oldh!=buttonh) {
        neww=buttonh*oldw/oldh;
        newh=buttonh;
//        MM_Editor_Manual_Update=1;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),neww);
//        MM_Editor_Manual_Update=0;
      } else {
        newh=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));
        neww=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03));
      }
    } else {
      newh=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin04));
      neww=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin03));
    }
    MM_Edit_Item->minx=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin01));
    MM_Edit_Item->miny=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin02));
    MM_Edit_Item->maxx=neww+MM_Edit_Item->minx;
    MM_Edit_Item->maxy=newh+MM_Edit_Item->miny;
    MM_Edit_Item->is_background=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle01))==TRUE)?1:0;
    MM_Edit_Item->drop_shadow=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle04))==TRUE)?1:0;
    MM_Edit_Item->is_sbt=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle05))==TRUE)?1:0;
    MM_Edit_Item->light=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle07))==TRUE)?1:0;
  }
}


int mmef_item_editor_center_h() {
  GLdouble minx,maxx;
  minx=0.5-(MM_Edit_Item->maxx-MM_Edit_Item->minx)/2;
  maxx=minx+(MM_Edit_Item->maxx-MM_Edit_Item->minx);
  MM_Edit_Item->minx=minx;
  MM_Edit_Item->maxx=maxx;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}

int mmef_item_editor_13_h() {
  GLdouble minx,maxx;
  minx=(1.0/3.0)-(MM_Edit_Item->maxx-MM_Edit_Item->minx)/2;
  maxx=minx+(MM_Edit_Item->maxx-MM_Edit_Item->minx);
  MM_Edit_Item->minx=minx;
  MM_Edit_Item->maxx=maxx;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}

int mmef_item_editor_23_h() {
  GLdouble minx,maxx;
  minx=(2.0/3.0)-(MM_Edit_Item->maxx-MM_Edit_Item->minx)/2;
  maxx=minx+(MM_Edit_Item->maxx-MM_Edit_Item->minx);
  MM_Edit_Item->minx=minx;
  MM_Edit_Item->maxx=maxx;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}

int mmef_item_editor_modulo_h() {
  GLdouble w,maxx,minx;
  GLdouble n,m,space;
  w=(MM_Edit_Item->maxx-MM_Edit_Item->minx);
  n=gtk_spin_button_get_value(GTK_SPIN_BUTTON(uni_spin07));
  m=gtk_spin_button_get_value(GTK_SPIN_BUTTON(uni_spin08));
  if((w<=0)||(n<=0)||(m<=0)) return(0);
  space=(1-(w*m))/(m+1);
  MM_Edit_Item->minx=(space*n)+(w*(n-1));
  MM_Edit_Item->maxx=(space*n)+(w*(n));
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}

int mmef_item_editor_center_v() {
  GLdouble miny,maxy;
  miny=0.5-(MM_Edit_Item->maxy-MM_Edit_Item->miny)/2;
  maxy=miny+(MM_Edit_Item->maxy-MM_Edit_Item->miny);
  MM_Edit_Item->miny=miny;
  MM_Edit_Item->maxy=maxy;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}


int mmef_item_editor_get() {

  gtk_entry_set_text(GTK_ENTRY(uni_text_single01),MM_Edit_Item->text);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle01),MM_Edit_Item->is_background?TRUE:FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle04),MM_Edit_Item->drop_shadow?TRUE:FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle05),MM_Edit_Item->is_sbt?TRUE:FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle07),MM_Edit_Item->light?TRUE:FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin01),MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin02),MM_Edit_Item->miny);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin03),MM_Edit_Item->maxx-MM_Edit_Item->minx);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin04),MM_Edit_Item->maxy-MM_Edit_Item->miny);
}

//-----------------------------------------------------------------------------

int mmef_item_editor_finalize() {
  const gchar * new_text;
  void * for_free;
  int id;
  struct mms_page * p_run;

  MM_ModuloN=gtk_spin_button_get_value(GTK_SPIN_BUTTON(uni_spin07));
  MM_ModuloM=gtk_spin_button_get_value(GTK_SPIN_BUTTON(uni_spin08));

  mmef_lock_player();
  new_text=gtk_entry_get_text(GTK_ENTRY(uni_text_single01));
  for_free=malloc(sizeof(char)*(strlen(new_text)+1));
  if(!MM_Edit_Item->text) {
    mmef_error("Kein Speicher fuer neuen Namen.");
  } else {
    free(MM_Edit_Item->text);
    MM_Edit_Item->text=for_free;
    strcpy(MM_Edit_Item->text,new_text);
  }

  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(action_combox[0]),&action_iter[0])) {
    gtk_tree_model_get(GTK_TREE_MODEL(action_store[0]), &action_iter[0], 1, &id, -1);
    MM_Edit_Item->action=id;
  } else {
    MM_Edit_Item->action=0;
  }

  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(avalue_combox[0]),&action_iter[0])) {
    gtk_tree_model_get(GTK_TREE_MODEL(avalue_store[0]), &action_iter[0], 1, &id, -1);
    MM_Edit_Item->avalue=id;
  } else {
    MM_Edit_Item->avalue=0;
  }

  mmef_item_editor_set();

  if(MM_Edit_Item->is_sbt)
    mmf_sbt_set_current(MM_Edit_Item);

  gtk_tree_store_clear(treestore_items);
  mmef_fill_items_tree();
  mmef_unlock_player();
}

//-----------------------------------------------------------------------------

int mmef_item_editor_check_gridsnap() {
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle03))) {
    MM_Grid_Snap=1;
    MM_GridSizeX=1/(MM_GridX=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin05))+1);
    MM_GridSizeY=1/(MM_GridY=gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(uni_spin06))+1);
  } else {
    MM_Grid_Snap=0;
  }
}

//-----------------------------------------------------------------------------

int mmef_item_editor_check_itemsnap() {
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(uni_toggle06))) {
    MM_Item_Snap=1;
  } else {
    MM_Item_Snap=0;
  }
}

//-----------------------------------------------------------------------------

int mmef_item_editor(GLuint item_id) {
  GtkWidget *box,*box0,*box1,*box2,*box3,*box4,*box5;
  guint result;
  const gchar * name;
  char * insert;

  int type,check;
  gchar *text;
  GtkTreeIter  iter;
  GtkTreeModel *model;
  struct mms_item undo;
  
  char * pe_labels[]={"Aktion:"};
  

  MM_Edit_Item=mmf_get_item_by_id(MM_CurrentPage,item_id);
  MM_Editor_Manual_Update=0;
  memcpy(&undo,MM_Edit_Item,sizeof(struct mms_item));

  dialog=gtk_dialog_new_with_buttons( "Item bearbeiten", GTK_WINDOW(fenster), GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,NULL);
  //  gtk_signal_connect_object(GTK_OBJECT(dialog), "response", gtk_widget_destroy, GTK_OBJECT(dialog));
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

  box0=gtk_hbox_new(FALSE,8);
  box1=gtk_hbox_new(FALSE,8);
  box2=gtk_hbox_new(FALSE,8);
  MM_EditorActionMode=MM_ITEM;
  box3=(GtkWidget *)mmef_built_action_selector(1,pe_labels);
  box4=gtk_hbox_new(FALSE,8);
  box5=gtk_hbox_new(FALSE,8);
  box =gtk_vbox_new(FALSE,8);

  uni_text_single01=gtk_entry_new();
  uni_spin01=gtk_spin_button_new_with_range(0,1,0.0001);
  uni_spin02=gtk_spin_button_new_with_range(0,1,0.0001);
  uni_spin03=gtk_spin_button_new_with_range(0,1,0.0001);
  uni_spin04=gtk_spin_button_new_with_range(0,1,0.0001);

  uni_spin07=gtk_spin_button_new_with_range(0,100,1);
  uni_spin08=gtk_spin_button_new_with_range(0,100,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin07),MM_ModuloN);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin08),MM_ModuloM);

  uni_toggle01=gtk_toggle_button_new_with_label ("Hintergrund");
  uni_toggle02=gtk_toggle_button_new_with_label ("Verhaeltnis erhalten");
  uni_toggle04=gtk_toggle_button_new_with_label ("Schlagschatten");
  uni_toggle05=gtk_toggle_button_new_with_label ("SeeByTouch");
  uni_toggle07=gtk_toggle_button_new_with_label ("Licht");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle02),TRUE);
  
  uni_button01=gtk_button_new_with_label("Zentrieren Horizontal");
  uni_button02=gtk_button_new_with_label("Zentrieren Vertikal");
  uni_button03=gtk_button_new_with_label("1/3 Horizontal");
  uni_button04=gtk_button_new_with_label("2/3 Horizontal");
  uni_button05=gtk_button_new_with_label("Set Modulo H");
  gtk_signal_connect_object(GTK_OBJECT(uni_button01),"clicked",GTK_SIGNAL_FUNC(mmef_item_editor_center_h),GTK_OBJECT(fenster));
  gtk_signal_connect_object(GTK_OBJECT(uni_button02),"clicked",GTK_SIGNAL_FUNC(mmef_item_editor_center_v),GTK_OBJECT(fenster));
  gtk_signal_connect_object(GTK_OBJECT(uni_button03),"clicked",GTK_SIGNAL_FUNC(mmef_item_editor_13_h),GTK_OBJECT(fenster));
  gtk_signal_connect_object(GTK_OBJECT(uni_button04),"clicked",GTK_SIGNAL_FUNC(mmef_item_editor_23_h),GTK_OBJECT(fenster));
  gtk_signal_connect_object(GTK_OBJECT(uni_button05),"clicked",GTK_SIGNAL_FUNC(mmef_item_editor_modulo_h),GTK_OBJECT(fenster));
  
  mmef_item_editor_get();

  uni_toggle03=gtk_toggle_button_new_with_label ("Snap Grid");
  uni_toggle06=gtk_toggle_button_new_with_label ("Snap Items");
  uni_spin05=gtk_spin_button_new_with_range(0,2000,1);
  uni_spin06=gtk_spin_button_new_with_range(0,2000,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin05),MM_DEF_GridX-1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(uni_spin06),MM_DEF_GridY-1);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle03),MM_Grid_Snap?TRUE:FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uni_toggle06),MM_Item_Snap?TRUE:FALSE);

  g_signal_connect(GTK_OBJECT(uni_toggle03),"toggled",G_CALLBACK(mmef_item_editor_check_gridsnap),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_toggle06),"toggled",G_CALLBACK(mmef_item_editor_check_itemsnap),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin05),"value-changed",G_CALLBACK(mmef_item_editor_check_gridsnap),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin06),"value-changed",G_CALLBACK(mmef_item_editor_check_gridsnap),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin01),"value-changed",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin02),"value-changed",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin03),"value-changed",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_spin04),"value-changed",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_toggle01),"toggled",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_toggle04),"toggled",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_toggle05),"toggled",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);
  g_signal_connect(GTK_OBJECT(uni_toggle07),"toggled",G_CALLBACK(mmef_item_editor_set),MM_Edit_Item);

  uni_label01=gtk_label_new("Name:");
  uni_label02=gtk_label_new("Position horiz.:");
  uni_label03=gtk_label_new("Position vert.:");
  uni_label04=gtk_label_new("Breite");
  uni_label05=gtk_label_new("Hoehe:");

  gtk_box_pack_start (GTK_BOX(box0),uni_label01, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_text_single01, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle01, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle04, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle05, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box0),uni_toggle07, TRUE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box4),uni_toggle03, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_spin05, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_spin06, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box4),uni_toggle06, TRUE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box1),uni_label02, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin01, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_label03, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_spin02, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_button01, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_button02, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_button03, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box1),uni_button04, TRUE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box2),uni_label04, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_spin03, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_label05, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_spin04, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_toggle02, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_spin07, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_spin08, TRUE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX(box2),uni_button05, TRUE, FALSE, 2);

  gtk_box_pack_start (GTK_BOX(box),box0, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box1, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box2, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box4, TRUE, TRUE, 2);
  gtk_box_pack_start (GTK_BOX(box),box3, TRUE, TRUE, 2);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),box, FALSE, FALSE, 2);
    
  mmef_lock_player();
  MM_Run_Mode=MM_MODE_EDITOR;
  mmef_unlock_player();
  
  gtk_widget_show_all(dialog);  
  result = gtk_dialog_run(GTK_DIALOG(dialog));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
      mmef_item_editor_finalize();
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_CANCEL:
    default:
      mmef_lock_player();
      undo.text=MM_Edit_Item->text;
      memcpy(MM_Edit_Item,&undo,sizeof(struct mms_item));
      mmef_unlock_player();
      break;
  }  

  mmef_lock_player();
  MM_Run_Mode=MM_MODE_PLAYER;
  mmef_unlock_player();

  gtk_widget_destroy(dialog);
}
