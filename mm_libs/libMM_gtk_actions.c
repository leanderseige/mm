
int mmef_actions_fill_avalue(int x) {
struct mms_page * p_run;
struct mms_item * i_run;
int idx,act,action,avalue;

  if(MM_EditorActionMode==MM_ITEM) {
    action=MM_Edit_Item->action;
    avalue=MM_Edit_Item->avalue;
  }
  else if(MM_EditorActionMode==MM_PAGE) {
    action=MM_CurrentPage->action;
    avalue=MM_CurrentPage->avalue;
  }
  else if(MM_EditorActionMode==MM_SBT) {
    action=MM_Edit_Item->sbtact[x];
    avalue=MM_Edit_Item->sbtava[x];
  }
    
  switch(action) {
    case MM_JUMP_TO_PAGE:
      p_run=MM_Pages;
      idx=0;
      while(p_run) {
        gtk_list_store_append(avalue_store[x],&action_iter[x]);
        if(p_run->id==avalue)
          gtk_combo_box_set_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x]);
        gtk_list_store_set(avalue_store[x],&action_iter[x],0,p_run->name,1,p_run->id,-1);
        p_run=p_run->next;
      }
      break;
    case MM_SET_SBT:
      i_run=MM_CurrentPage->items;
      idx=0;
      while(i_run) {
      printf("c: %s\n",i_run->text);
        if(i_run->type==MM_ASIAANIM||i_run->type==MM_IMAGE_BUTTON||i_run->type==MM_TEXT_BUTTON||i_run->type==MM_IMAGE) {
          gtk_list_store_append(avalue_store[x],&action_iter[x]);
          if(i_run->id==avalue) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x]);
          }
          gtk_list_store_set(avalue_store[x],&action_iter[x],0,i_run->text,1,i_run->id,-1);
        }
      i_run=i_run->next;      
      }
      break;
      break;
    case MM_AAD_PLAY_FRAME:
    case MM_AAD_PLAY_ANIMATION:
    case MM_AAD_SHOW_STILL:
    case MM_AAD_CLEAR:
      i_run=MM_CurrentPage->items;
      idx=0;
      while(i_run) {
      printf("c: %s\n",i_run->text);
        if(i_run->type==MM_ASIAANIM) {
          gtk_list_store_append(avalue_store[x],&action_iter[x]);
          if(i_run->id==avalue) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x]);
          }
          gtk_list_store_set(avalue_store[x],&action_iter[x],0,i_run->text,1,i_run->id,-1);
        }
      i_run=i_run->next;      
      }
      break;
    case MM_SOUND_PLAY:
    case MM_SOUND_STOP:
    case MM_SOUND_PAUSE:
      i_run=MM_CurrentPage->items;
      idx=0;
      while(i_run) {
      printf("s: %s\n",i_run->text);
        if(i_run->type==MM_SOUND) {
          gtk_list_store_append(avalue_store[x],&action_iter[x]);
          if(i_run->id==avalue) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x]);
          }
          gtk_list_store_set(avalue_store[x],&action_iter[x],0,i_run->text,1,i_run->id,-1);
        }
      i_run=i_run->next;      
      }
      break;
    case MM_VIDEO_PLAY:
    case MM_VIDEO_STOP:
    case MM_VIDEO_PAUSE:
      i_run=MM_CurrentPage->items;
      idx=0;
      while(i_run) {
      printf("s: %s\n",i_run->text);
        if(i_run->type==MM_VIDEO) {
          gtk_list_store_append(avalue_store[x],&action_iter[x]);
          if(i_run->id==avalue) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(avalue_combox[x]),&action_iter[x]);
          }
          gtk_list_store_set(avalue_store[x],&action_iter[x],0,i_run->text,1,i_run->id,-1);
        }
      i_run=i_run->next;      
      }
      break;
    
    default:
      break;
  }
}

int mmef_actions_changed_action(gpointer unused,gpointer user_data) {
  int id,x;
  char * s;

  x=GPOINTER_TO_INT(user_data);
  gtk_combo_box_get_active_iter(GTK_COMBO_BOX(action_combox[x]),&action_iter[x]);
  gtk_tree_model_get(GTK_TREE_MODEL(action_store[x]), &action_iter[x], 1, &id, -1);
  if(MM_EditorActionMode==MM_ITEM) {
    MM_Edit_Item->action=id;
  } else if(MM_EditorActionMode==MM_PAGE) {
    MM_CurrentPage->action=id;
  } else if(MM_EditorActionMode==MM_SBT) {
    MM_Edit_Item->sbtact[x]=id;
  }
  gtk_list_store_clear(avalue_store[x]);
  mmef_actions_fill_avalue(x);
}

gboolean mmef_actions_set_active(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data) {
  GLuint id,x;

  x=GPOINTER_TO_INT(user_data);
  gtk_tree_model_get(model,iter,1,&id,-1);
  if(MM_EditorActionMode==MM_ITEM) {
    if(MM_Edit_Item->action==id) {
      gtk_combo_box_set_active_iter(GTK_COMBO_BOX(action_combox[x]),iter);
      return(TRUE);
    } else {
      return(FALSE);
    }
  } else if(MM_EditorActionMode==MM_SBT) {
    if(MM_Edit_Item->sbtact[x]==id) {
      gtk_combo_box_set_active_iter(GTK_COMBO_BOX(action_combox[x]),iter);
      return(TRUE);
    } else {
      return(FALSE);
    }
  } else if(MM_EditorActionMode==MM_PAGE) {
    if(MM_CurrentPage->action==id) {
      gtk_combo_box_set_active_iter(GTK_COMBO_BOX(action_combox[x]),iter);
      return(TRUE);
    } else {
      return(FALSE);
    }
  }
}

GtkWidget * mmef_built_action_selector(int num,char ** labels) {
  GtkWidget *retbox,*tbox;
  int x;

  retbox=gtk_vbox_new(FALSE,2);
  
  for(x=0;x<num;x++) {
    tbox=gtk_hbox_new(FALSE,2);
    action_label[x]=gtk_label_new(labels[x]);
    action_store[x]=gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    action_combox[x]=gtk_combo_box_new_with_model(GTK_TREE_MODEL(action_store[x]));
    action_textrender[x]= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(action_combox[x]), action_textrender[x], TRUE);
    gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(action_combox[x]),action_textrender[x], "text", 0);  
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Springe zu Seite",1,MM_JUMP_TO_PAGE,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Video Play",1,MM_VIDEO_PLAY,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Video Stop",1,MM_VIDEO_STOP,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Video Pause",1,MM_VIDEO_PAUSE,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Audio Play",1,MM_SOUND_PLAY,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Audio Stop",1,MM_SOUND_STOP,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Audio Pause",1,MM_SOUND_PAUSE,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"AAD Frame Play",1,MM_AAD_PLAY_FRAME,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"AAD Animation Play",1,MM_AAD_PLAY_ANIMATION,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"AAD Frame Still",1,MM_AAD_SHOW_STILL,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"AAD Leer",1,MM_AAD_CLEAR,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"SBT Setzen",1,MM_SET_SBT,-1);
    gtk_list_store_append(action_store[x],&action_iter[x]); gtk_list_store_set(action_store[x],&action_iter[x],0,"Keine",1,MM_NO_ACTION,-1);
    gtk_tree_model_foreach(GTK_TREE_MODEL(action_store[x]),mmef_actions_set_active,GINT_TO_POINTER(x));

    avalue_store[x]=gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    avalue_combox[x]=gtk_combo_box_new_with_model(GTK_TREE_MODEL(avalue_store[x]));
    avalue_textrender[x]= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(avalue_combox[x]), avalue_textrender[x], TRUE);
    gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(avalue_combox[x]),avalue_textrender[x], "text", 0);  
    mmef_actions_fill_avalue(x);

    gtk_box_pack_start (GTK_BOX(tbox),action_label[x], TRUE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX(tbox),action_combox[x], TRUE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX(tbox),avalue_combox[x], TRUE, TRUE, 2);

    gtk_box_pack_start (GTK_BOX(retbox),tbox,TRUE, TRUE, 2);

    g_signal_connect(GTK_OBJECT(action_combox[x]),"changed",G_CALLBACK(mmef_actions_changed_action),GINT_TO_POINTER(x));      
  }
  
  return(retbox);
}

