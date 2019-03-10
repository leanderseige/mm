// helpers


char * mmef_xml_get_property(xmlNode * node, char * name) {
  int id;
  xmlAttr *attr;

  if(node==NULL) return(NULL);
  if(name==NULL) return(NULL);

  for(attr=node->properties;attr;attr=attr->next)
    if(attr->type==XML_ATTRIBUTE_NODE) {
      if(!xmlStrcmp(attr->name,name)) {
        return(attr->children->content);
      }
    }
  return(NULL);
}


char * mmef_xml_getstring(xmlNode * node, char * name) {
  char * retval;

  retval=mmef_xml_get_property(node,name);
  if(retval==NULL)
    return("");
  else
    return(retval);
}


GLuint mmef_xml_geti(xmlNode * node, char * in) {
  char * s;

  s=mmef_xml_get_property(node,in);
  if(s!=NULL)
    return(atoi(s));
  else
    return(0);
}


GLdouble mmef_xml_getf(xmlNode * node, char * in) {
  char * s;

  s=mmef_xml_get_property(node,in);
  if(s!=NULL)
    return(atof(s));
  else
    return(0);
}


void mmef_xml_getcolor(xmlNode * node, char * name, GdkColor * color) {
  char * scolor;

  if(name==NULL) bzero(color,sizeof(GdkColor));
  if(node==NULL) bzero(color,sizeof(GdkColor));
  scolor=mmef_xml_get_property(node,name);
  if(scolor==NULL) bzero(color,sizeof(GdkColor));
  gdk_color_parse(scolor,color);

}


// loaders


int mmef_load_globals(xmlNode * rootmm) {
  char * text;

  MM_ScreenWidthEditor=mmef_xml_geti(rootmm,"screenwidtheditor");
  MM_ScreenHeightEditor=mmef_xml_geti(rootmm,"screenheighteditor");
  MM_ScreenWidthMax=mmef_xml_geti(rootmm,"screenwidthmax");
  MM_ScreenHeightMax=mmef_xml_geti(rootmm,"screenheightmax");
  if(text=mmef_xml_get_property(rootmm,"sbt_device")) {
    MM_SBT_DevName=malloc(sizeof(char)*(strlen(text)+1));
    if(MM_SBT_DevName) {
      strcpy(MM_SBT_DevName,text);
    } else {
      fprintf(stderr,"cannot malloc sbt device name memory\n");
    }
  } else {
    MM_SBT_DevName=&"";
    text="";
  }
  MM_SBT_DevBaud=mmef_xml_geti(rootmm,"sbt_baudrate");

  mmef_xml_getcolor(rootmm,"fgl",&MM_DEF_FGL);
  mmef_xml_getcolor(rootmm,"bgl",&MM_DEF_BGL);
  mmef_xml_getcolor(rootmm,"fgh",&MM_DEF_FGH);
  mmef_xml_getcolor(rootmm,"bgh",&MM_DEF_BGH);
  mmef_xml_getcolor(rootmm,"pbg",&MM_DEF_PBG);
  mmef_xml_getcolor(rootmm,"afg",&MM_DEF_AFG);
  mmef_xml_getcolor(rootmm,"abg",&MM_DEF_ABG);

  MM_DEF_GridX=mmef_xml_getf(rootmm,"gridx");
  MM_DEF_GridY=mmef_xml_getf(rootmm,"gridx");

  MM_DEF_TextLineHeight=mmef_xml_getf(rootmm,"textlineheight");
  MM_DEF_TextItemWidth=mmef_xml_getf(rootmm,"textitemwidth");
  MM_DEF_TextBorderWidth=mmef_xml_getf(rootmm,"textborderwidth");

  printf("SBT: %s %d\n",MM_SBT_DevName,MM_SBT_DevBaud);
}

int mmef_load_page(xmlNode * page) {
  GdkColor color;
  struct mms_page * newpage;
  int id;

  mmef_xml_getcolor(page,"bg_color",&color);
  id=mmef_xml_geti(page,"id");

  mmf_add_page(
    id,
    mmef_xml_get_property(page,"name"),
    color
  );

  if(newpage=mmf_get_page_by_id(id)) {
    newpage->action=mmef_xml_geti(page,"action");
    newpage->action=mmef_xml_geti(page,"avalue");
    return(id);
  }
  return(0);

}

int mmef_load_item(int pageid,xmlNode * inode) {
  struct mms_item * retitem;
  char pname[20],*s;
  int x;

  retitem=mmf_insert_item(
    pageid,
    mmef_xml_geti(inode,"id"),
    mmf_get_resource_by_id(mmef_xml_geti(inode,"resource")),
    0,0,
    mmef_xml_geti(inode,"action"),
    mmef_xml_geti(inode,"avalue"),
    mmef_xml_getf(inode,"minx"),
    mmef_xml_getf(inode,"maxx"),
    mmef_xml_getf(inode,"miny"),
    mmef_xml_getf(inode,"maxy"),
    mmef_xml_geti(inode,"is_background"),
    mmef_xml_geti(inode,"drop_shadow"),
    mmef_xml_geti(inode,"is_sbt"),
    mmef_xml_getf(inode,"light")
  );

  for(x=0;x<5;x++) {
    sprintf(pname,"sbtact%d",x);
    retitem->sbtact[x]=mmef_xml_geti(inode,pname);
    sprintf(pname,"sbtava%d",x);
    retitem->sbtava[x]=mmef_xml_geti(inode,pname);
  }

  s=mmef_xml_getstring(inode,"animakt_rx");
  retitem->animakt.str_rx=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_rx));

  s=mmef_xml_getstring(inode,"animakt_ry");
  retitem->animakt.str_ry=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_ry));

  s=mmef_xml_getstring(inode,"animakt_rz");
  retitem->animakt.str_rz=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_rz));

  s=mmef_xml_getstring(inode,"animakt_tx");
  retitem->animakt.str_tx=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_tx));

  s=mmef_xml_getstring(inode,"animakt_ty");
  retitem->animakt.str_ty=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_ty));

  s=mmef_xml_getstring(inode,"animakt_tz");
  retitem->animakt.str_tz=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_tz));

  s=mmef_xml_getstring(inode,"animakt_sw");
  retitem->animakt.str_sw=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_sw));

  s=mmef_xml_getstring(inode,"animakt_sh");
  retitem->animakt.str_sh=strlen(s)>0?g_strdup(s):NULL;
  mmef_animation_editor_makeeval(s,&(retitem->animakt.eval_sh));

  retitem->animakt.playtime=mmef_xml_getf(inode,"animakt_time");
}


int mmef_load_font(xmlNode * rnode, xmlNode * dnode) {

  mmf_add_font(
    mmef_xml_get_property(dnode,"path"),
    mmef_xml_get_property(dnode,"text"),
    mmef_xml_geti(rnode,"id")
  );
}


int mmef_load_sound(xmlNode * rnode, xmlNode * dnode) {

  mmf_add_sound(
    mmef_xml_get_property(dnode,"path"),
    mmef_xml_get_property(dnode,"text"),
    mmef_xml_geti(rnode,"id")
  );
}


int mmef_load_video(xmlNode * rnode, xmlNode * dnode) {

  mmf_add_video(
    mmef_xml_get_property(dnode,"path"),
    mmef_xml_geti(rnode,"id"),
    mmef_xml_getf(dnode,"w"),
    mmef_xml_getf(dnode,"h"),
    mmef_xml_get_property(dnode,"text")
  );
}


int mmef_load_image(xmlNode * rnode, xmlNode * dnode) {

  mmf_add_image(
    mmef_xml_geti(rnode,"id"),
    mmef_xml_get_property(dnode,"text"),
    mmef_xml_get_property(dnode,"path")
  );
}


int mmef_load_button_image(xmlNode * rnode, xmlNode * dnode) {

  mmf_add_button(
    MM_IMAGE_BUTTON,
    mmef_xml_geti(rnode,"id"),
    mmef_xml_getf(dnode,"w"),
    mmef_xml_getf(dnode,"h"),
    mmef_xml_getf(dnode,"b"),
    mmef_xml_get_property(dnode,"text"),
    NULL,
    NULL,
    NULL,
    NULL,
    mmef_xml_get_property(dnode,"path"),
    mmef_xml_geti(dnode,"flags"),
    mmef_xml_getf(dnode,"bow"),
    NULL
  );
  printf("ok\n");
}


int mmef_load_button_text(xmlNode * rnode, xmlNode * dnode) {
  GdkColor fgl,bgl,fgh,bgh;

  mmef_xml_getcolor(dnode,"fgl",&fgl);
  mmef_xml_getcolor(dnode,"bgl",&bgl);
  mmef_xml_getcolor(dnode,"fgh",&fgh);
  mmef_xml_getcolor(dnode,"bgh",&bgh);

  mmf_add_button(
    MM_TEXT_BUTTON,
    mmef_xml_geti(rnode,"id"),
    mmef_xml_getf(dnode,"w"),
    mmef_xml_getf(dnode,"h"),
    mmef_xml_getf(dnode,"b"),
    mmef_xml_get_property(dnode,"text"),
    &fgl,&bgl,&fgh,&bgh,
    mmf_get_resource_by_id(mmef_xml_geti(dnode,"font_resource")),
    mmef_xml_geti(dnode,"flags"),
    mmef_xml_getf(dnode,"bow"),
    NULL
  );

}

int mmef_load_aad(xmlNode * rnode, xmlNode * dnode) {
  GdkColor bg,fg;

  mmef_xml_getcolor(dnode,"fg_color",&fg);
  mmef_xml_getcolor(dnode,"bg_color",&bg);

  mmf_add_asiaanim(
    mmef_xml_geti(rnode,"id"),
    mmef_xml_getf(dnode,"w"),
    mmef_xml_getf(dnode,"h"),
    mmef_xml_getf(dnode,"b"),
    mmef_xml_get_property(dnode,"text"),
    &fg,&bg,
    mmef_xml_get_property(dnode,"path")
  );
}

int mmef_load_text(xmlNode * rnode, xmlNode * dnode) {
  GdkColor fg,bg;
  SDL_Color fgs,bgs;
  struct mms_resource * font_resource;
  int font_id,alpha;
  char * text;
  xmlChar * content;
  GLdouble line_height,field_width,border_width,bow;
  GLuint align,flags;

  mmef_xml_getcolor(dnode,"fg_color",&fg);
  mmef_xml_getcolor(dnode,"bg_color",&bg);
  fgs.r=fg.red>>8; fgs.g=fg.green>>8; fgs.b=fg.blue>>8;
  bgs.r=bg.red>>8; bgs.g=bg.green>>8; bgs.b=bg.blue>>8;

  font_id=mmef_xml_geti(dnode,"font_resource");
  font_resource=mmf_get_resource_by_id(font_id);
  text=mmef_xml_get_property(dnode,"text");
  field_width=mmef_xml_getf(dnode,"field_width");
  line_height=mmef_xml_getf(dnode,"line_height");
  border_width=mmef_xml_getf(dnode,"border_width");
  align=mmef_xml_geti(dnode,"align");
  alpha=mmef_xml_geti(dnode,"alpha");
  bow=mmef_xml_getf(dnode,"bow");
	flags=mmef_xml_geti(dnode,"flags"); // why? flags=0;

  content=xmlNodeGetContent(dnode);

  mmf_add_text(
    mmf_build_text_plate(
          content,
          (struct mms_font *)(font_resource->data),
          &fgs,&bgs,
          line_height,
          field_width,
          border_width,
          align,
          flags,
		  alpha,
          bow),
    text,
    content,
    mmef_xml_geti(rnode,"id"),
    font_id,
    field_width,
    line_height,
    border_width,
    &fg,&bg,
    align,
    flags,
    bow,
    NULL
  );

  xmlFree(content);

}


int mmef_load(char * filename) {
  struct mms_page * p_run;
  struct mms_item * i_run;
  struct mms_resource * r_run;
  struct mms_button * r_button;
  struct mms_image * r_image;
  struct mms_text * r_text;
  struct mms_sound * r_sound;
  struct mms_asiaanim * r_aad;
  struct mms_video * r_video;
  struct mms_font * r_font;
  int c,id;
  FILE *f;
  char * ctemp;
  char emptystring[]="";
  char cbuffer[1024];

  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;
  xmlNode *cur_p,*cur_r,*cur_i,*cur_d;

    setlocale(LC_NUMERIC, "C");

    doc=xmlReadFile(filename,NULL,0);
    if(doc==NULL) {
      mmef_error(fenster,"Datei konnte nicht geladen werden.");
      return;
    }

    root_element = xmlDocGetRootElement(doc);

    if(root_element->type==XML_ELEMENT_NODE)
      if(!xmlStrcmp(root_element->name,"mm")) {
//        printf("mm file signature found.\n");
        mmef_cleansweep();
        mmef_load_globals(root_element);

        for(cur_r=root_element->children;cur_r;cur_r=cur_r->next)
          if(cur_r->type==XML_ELEMENT_NODE)
            if(!xmlStrcmp(cur_r->name, "resource")){
//              printf("r_found %s\n",cur_r->name);
              for(cur_d=cur_r->children;cur_d;cur_d=cur_d->next)
                if(cur_d->type==XML_ELEMENT_NODE) {
                  if(!xmlStrcmp(cur_d->name, "font")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_font(cur_r,cur_d);
                  }
                }
            }

        for(cur_r=root_element->children;cur_r;cur_r=cur_r->next)
          if(cur_r->type==XML_ELEMENT_NODE)
            if(!xmlStrcmp(cur_r->name, "resource")){
//              printf("r_found %s\n",cur_r->name);
              for(cur_d=cur_r->children;cur_d;cur_d=cur_d->next)
                if(cur_d->type==XML_ELEMENT_NODE) {
                  if(!xmlStrcmp(cur_d->name, "button_image")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_button_image(cur_r,cur_d);
                  } else
                  if(!xmlStrcmp(cur_d->name, "image")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_image(cur_r,cur_d);
                  } else
                  if(!xmlStrcmp(cur_d->name, "sound")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_sound(cur_r,cur_d);
                  } else
                  if(!xmlStrcmp(cur_d->name, "video")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_video(cur_r,cur_d);
                  } else
                  if(!xmlStrcmp(cur_d->name, "aad")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_aad(cur_r,cur_d);
                  }
                }
            }

        for(cur_r=root_element->children;cur_r;cur_r=cur_r->next)
          if(cur_r->type==XML_ELEMENT_NODE)
            if(!xmlStrcmp(cur_r->name, "resource")){
//              printf("r_found %s\n",cur_r->name);
              for(cur_d=cur_r->children;cur_d;cur_d=cur_d->next)
                if(cur_d->type==XML_ELEMENT_NODE) {
                  if(!xmlStrcmp(cur_d->name, "button_text")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_button_text(cur_r,cur_d);
                  } else
                  if(!xmlStrcmp(cur_d->name, "text")) {
//                    printf("  d_found %s\n",cur_d->name);
                    mmef_load_text(cur_r,cur_d);
                  }
                }
            }

        for(cur_p=root_element->children;cur_p;cur_p=cur_p->next)
          if(cur_p->type==XML_ELEMENT_NODE)
            if(!xmlStrcmp(cur_p->name, "page")) {
//              printf("p_found %s\n",cur_p->name);
              id=mmef_load_page(cur_p);
              for(cur_i=cur_p->children;cur_i;cur_i=cur_i->next)
                if(cur_i->type==XML_ELEMENT_NODE)
                  if(!xmlStrcmp(cur_i->name, "item")) {
                    printf("  i_found %s\n",cur_i->name);
                    mmef_load_item(id,cur_i);
                  }
            }
    printf("a\n");
        mmf_sbt_open();
				mmf_action_jump_to_page(1); // FIXME startseite markieren/laden/speichern
    printf("load b\n");


    } else {
      mmef_error(fenster,"Falscher Dateityp.");

    }

    xmlFreeDoc(doc);
    printf("load b-2\n");
    xmlCleanupParser();
    printf("load b-3\n");

}



void mmef_load_clicked(GtkObject *object) {
  const gchar * filename;

  if(!mmef_yesno(fenster,"Aktuelles Setup wird verworfen.")) {
    return;
  }

  dialog = gtk_file_chooser_dialog_new ("Laden",GTK_WINDOW(fenster), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    gtk_widget_destroy (dialog);
    if(!filename) {
      mmef_error(fenster,"Kein Dateiname ausgewaehlt.");
      return;
    }
    mmef_lock_player();
	mmef_load((char *)filename);
    mmef_unlock_player();

    gtk_tree_store_clear(treestore_resources);
    mmef_fill_resources_tree();
    gtk_tree_store_clear(treestore_items);
    mmef_fill_items_tree();
    gtk_tree_store_clear(treestore_pages);
    mmef_fill_pages_tree();

  } else {
    gtk_widget_destroy (dialog);
  }

}
