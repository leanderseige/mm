void mmef_save_clicked(GtkObject *object) {
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
  int c;
  FILE *f;
  const gchar * filename;
  char * ctemp;
  char emptystring[]="";
  char cbuffer[1024];
  int x;

  dialog = gtk_file_chooser_dialog_new("Save",GTK_WINDOW(fenster), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    gtk_widget_destroy (dialog);
    if(!filename) {
      mmef_error(fenster,"Kein Dateiname ausgewaehlt.");
      return;
    }
    f=fopen(filename,"w");
    setlocale(LC_NUMERIC, "C");
    if(!f) {
      mmef_error(fenster,"Konnte Datei nicht zum schreiben oeffnen.");
      return;
    }

    mmef_lock_player();

    fprintf(f,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
    fprintf(f,"  <mm\n");
    fprintf(f,"    screenwidtheditor=\"%d\"\n",MM_ScreenWidthEditor);
    fprintf(f,"    screenheighteditor=\"%d\"\n",MM_ScreenHeightEditor);
    fprintf(f,"    screenwidthmax=\"%d\"\n",MM_ScreenWidthMax);
    fprintf(f,"    screenheightmax=\"%d\"\n",MM_ScreenHeightMax);
    // FIXME -- crashs when device(name) is not available
    // fprintf(f,"    sbt_device=\"%s\"\n",MM_SBT_Display->dev_name?MM_SBT_Display->dev_name:"");
    // fprintf(f,"    sbt_baudrate=\"%d\"\n",MM_SBT_Display->dev_baud);
    fprintf(f,"    fgl=\"#%04x%04x%04x\"\n",MM_DEF_FGL.red,MM_DEF_FGL.green,MM_DEF_FGL.blue);
    fprintf(f,"    bgl=\"#%04x%04x%04x\"\n",MM_DEF_BGL.red,MM_DEF_BGL.green,MM_DEF_BGL.blue);
    fprintf(f,"    fgh=\"#%04x%04x%04x\"\n",MM_DEF_FGH.red,MM_DEF_FGH.green,MM_DEF_FGH.blue);
    fprintf(f,"    bgh=\"#%04x%04x%04x\"\n",MM_DEF_BGH.red,MM_DEF_BGH.green,MM_DEF_BGH.blue);
    fprintf(f,"    pbg=\"#%04x%04x%04x\"\n",MM_DEF_PBG.red,MM_DEF_PBG.green,MM_DEF_PBG.blue);
    fprintf(f,"    afg=\"#%04x%04x%04x\"\n",MM_DEF_AFG.red,MM_DEF_AFG.green,MM_DEF_AFG.blue);
    fprintf(f,"    abg=\"#%04x%04x%04x\"\n",MM_DEF_ABG.red,MM_DEF_ABG.green,MM_DEF_ABG.blue);
    fprintf(f,"    gridx=\"%f\"\n",MM_DEF_GridX);
    fprintf(f,"    gridy=\"%f\"\n",MM_DEF_GridY);
    fprintf(f,"    textlineheight=\"%f\"\n",MM_DEF_TextLineHeight);
    fprintf(f,"    textitemwidth=\"%f\"\n",MM_DEF_TextItemWidth);
    fprintf(f,"    textborderwidth=\"%f\"\n",MM_DEF_TextBorderWidth);
    fprintf(f,"  >\n");

    for(r_run=MM_Resources;r_run;r_run=r_run->next) {
      fprintf(f,"    <resource id=\"%d\"",r_run->id);
      fprintf(f," next=\"%d\"",r_run->next?r_run->next->id:-1);
      fprintf(f," prev=\"%d\"",r_run->prev?r_run->prev->id:-1);
      fprintf(f," text=\"%s\"",r_run->text);
      fprintf(f," type=\"%d\"",r_run->type);
      fprintf(f,">\n");
      switch(r_run->type) {
        case MM_IMAGE_BUTTON:
          r_button=(struct mms_button *)r_run->data;
          mmf_realpath(r_button->path);
          fprintf(f,"      <button_image path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_button->text);
          fprintf(f," w=\"%f\"",r_button->w);
          fprintf(f," h=\"%f\"",r_button->h);
          fprintf(f," b=\"%f\"",r_button->build_b);
          fprintf(f," type=\"%d\"",r_button->type);
          fprintf(f," flags=\"%d\"",r_button->flags);
          fprintf(f," fgl=\"#%04x%04x%04x\"",r_button->fgl.red,r_button->fgl.green,r_button->fgl.blue);
          fprintf(f," bgl=\"#%04x%04x%04x\"",r_button->bgl.red,r_button->bgl.green,r_button->bgl.blue);
          fprintf(f," fgh=\"#%04x%04x%04x\"",r_button->fgh.red,r_button->fgh.green,r_button->fgh.blue);
          fprintf(f," bgh=\"#%04x%04x%04x\"",r_button->bgh.red,r_button->bgh.green,r_button->bgh.blue);
          fprintf(f," font_resource=\"%d\"",r_button->font_resource);
          fprintf(f," />\n");
          break;
        case MM_TEXT_BUTTON:
          r_button=(struct mms_button *)r_run->data;
          mmf_realpath(r_button->path);
          fprintf(f,"      <button_text path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_button->text);
          fprintf(f," w=\"%f\"",r_button->build_w);
          fprintf(f," h=\"%f\"",r_button->build_h);
          fprintf(f," b=\"%f\"",r_button->build_b);
          fprintf(f," type=\"%d\"",r_button->type);
          fprintf(f," flags=\"%d\"",r_button->flags);
          fprintf(f," fgl=\"#%04x%04x%04x\"",r_button->fgl.red,r_button->fgl.green,r_button->fgl.blue);
          fprintf(f," bgl=\"#%04x%04x%04x\"",r_button->bgl.red,r_button->bgl.green,r_button->bgl.blue);
          fprintf(f," fgh=\"#%04x%04x%04x\"",r_button->fgh.red,r_button->fgh.green,r_button->fgh.blue);
          fprintf(f," bgh=\"#%04x%04x%04x\"",r_button->bgh.red,r_button->bgh.green,r_button->bgh.blue);
          fprintf(f," font_resource=\"%d\"",r_button->font_resource);
		  fprintf(f," bow=\"%f\"",r_button->bow);
          fprintf(f," />\n");
          break;
        case MM_IMAGE:
          r_image=(struct mms_image *)r_run->data;
          mmf_realpath(r_image->path);
          fprintf(f,"      <image path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_image->text);
          fprintf(f," w=\"%f\"",r_image->w);
          fprintf(f," h=\"%f\"",r_image->h);
          fprintf(f," type=\"%d\"",r_image->type);
          fprintf(f," />\n");
          break;
        case MM_TEXT:
          r_text=(struct mms_text *)r_run->data;
          fprintf(f,"      <text text=\"%s\"",r_text->text);
          fprintf(f," w=\"%f\"",r_text->w);
          fprintf(f," h=\"%f\"",r_text->h);
          fprintf(f," type=\"%d\"",r_text->type);
          fprintf(f," flags=\"%d\"",r_text->flags);
          fprintf(f," border_width=\"%f\"",r_text->border_width);
          fprintf(f," line_height=\"%f\"",r_text->line_height);
          fprintf(f," field_width=\"%f\"",r_text->field_width);
          fprintf(f," fg_color=\"#%04x%04x%04x\"",r_text->fg.red,r_text->fg.green,r_text->fg.blue);
          fprintf(f," bg_color=\"#%04x%04x%04x\"",r_text->bg.red,r_text->bg.green,r_text->bg.blue);
          fprintf(f," font_resource=\"%d\"",r_text->font_resource);
          fprintf(f," align=\"%d\"",r_text->align);
		  fprintf(f," alpha=\"%d\"",r_text->alpha);
		  fprintf(f," bow=\"%f\"",r_text->bow);
          fprintf(f,">");
//          for(c=0;c<strlen(r_text->full_text);c++) {
//            if(!(c%16)) fprintf(f,"\n        ");
//            fprintf(f,"%02x ",r_text->full_text[c]);
//          }
          fprintf(f,"%s",r_text->full_text);
          fprintf(f,"</text>\n");
          break;
        case MM_SOUND:
          r_sound=(struct mms_sound *)r_run->data;
          mmf_realpath(r_sound->path);
          fprintf(f,"      <sound path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_sound->text);
          fprintf(f," type=\"%d\"",r_sound->type);
          fprintf(f," />\n");
          break;
        case MM_ASIAANIM:
          r_aad=(struct mms_asiaanim *)r_run->data;
          mmf_realpath(r_aad->path);
          fprintf(f,"      <aad path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_aad->text);
          fprintf(f," w=\"%f\"",r_aad->build_w);
          fprintf(f," h=\"%f\"",r_aad->build_h);
          fprintf(f," b=\"%f\"",r_aad->build_b);
          fprintf(f," type=\"%d\"",r_aad->type);
          fprintf(f," flags=\"%d\"",r_aad->flags);
          fprintf(f," bg_color=\"#%04x%04x%04x\"",r_aad->bg_color.red,r_aad->bg_color.green,r_aad->bg_color.blue);
          fprintf(f," fg_color=\"#%04x%04x%04x\"",r_aad->fg_color.red,r_aad->fg_color.green,r_aad->fg_color.blue);
          fprintf(f," />\n");
          break;
        case MM_VIDEO:
          r_video=(struct mms_video *)r_run->data;
          mmf_realpath(r_video->path);
          fprintf(f,"      <video path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_video->text);
          fprintf(f," w=\"%f\"",r_video->w);
          fprintf(f," h=\"%f\"",r_video->h);
          fprintf(f," type=\"%d\"",r_video->type);
          fprintf(f," />\n");
          break;
        case MM_FONT:
          r_font=(struct mms_font *)r_run->data;
          mmf_realpath(r_font->path);
          fprintf(f,"      <font path=\"%s\"",MM_PathBuf);
          fprintf(f," text=\"%s\"",r_font->text);
          fprintf(f," type=\"%d\"",r_font->type);
          fprintf(f," />\n");
          break;
        default:
          break;
      }
      fprintf(f,"    </resource>\n");
    }

    for(p_run=MM_Pages;p_run;p_run=p_run->next) {
      fprintf(f,"    <page name=\"%s\"",p_run->name);
      fprintf(f," id=\"%d\"",p_run->id);
      fprintf(f," next=\"%d\"",p_run->next?p_run->next->id:-1);
      fprintf(f," prev=\"%d\"",p_run->prev?p_run->prev->id:-1);
      fprintf(f," bg_color=\"#%04x%04x%04x\"",p_run->bg_color.red,p_run->bg_color.green,p_run->bg_color.blue);
      fprintf(f," action=\"%d\"",p_run->action);
      fprintf(f," avalue=\"%d\"",p_run->avalue);
      fprintf(f," >\n");
      for(i_run=p_run->items;i_run;i_run=i_run->next) {
        fprintf(f,"      <item id=\"%d\"",i_run->id);
        fprintf(f," next=\"%d\"",i_run->next?i_run->next->id:-1);
        fprintf(f," prev=\"%d\"",i_run->prev?i_run->prev->id:-1);
        fprintf(f," type=\"%d\"",i_run->type);
        fprintf(f," resource=\"%d\"",mmf_get_void_resource_id(i_run->data));
        fprintf(f," minx=\"%f\"",i_run->minx);
        fprintf(f," maxx=\"%f\"",i_run->maxx);
        fprintf(f," miny=\"%f\"",i_run->miny);
        fprintf(f," maxy=\"%f\"",i_run->maxy);
        fprintf(f," action=\"%d\"",i_run->action);
        fprintf(f," avalue=\"%d\"",i_run->avalue);
        fprintf(f," is_background=\"%d\"",i_run->is_background);
        fprintf(f," drop_shadow=\"%d\"",i_run->drop_shadow);
        fprintf(f," is_sbt=\"%d\"",i_run->is_sbt);
        fprintf(f," light=\"%d\"",i_run->light);
        fprintf(f," animakt_rx=\"%s\"",i_run->animakt.str_rx?i_run->animakt.str_rx:"");
        fprintf(f," animakt_ry=\"%s\"",i_run->animakt.str_ry?i_run->animakt.str_ry:"");
        fprintf(f," animakt_rz=\"%s\"",i_run->animakt.str_rz?i_run->animakt.str_rz:"");
        fprintf(f," animakt_tx=\"%s\"",i_run->animakt.str_tx?i_run->animakt.str_tx:"");
        fprintf(f," animakt_ty=\"%s\"",i_run->animakt.str_ty?i_run->animakt.str_ty:"");
        fprintf(f," animakt_tz=\"%s\"",i_run->animakt.str_tz?i_run->animakt.str_tz:"");
        fprintf(f," animakt_sw=\"%s\"",i_run->animakt.str_sw?i_run->animakt.str_sw:"");
        fprintf(f," animakt_sh=\"%s\"",i_run->animakt.str_sh?i_run->animakt.str_sh:"");
        fprintf(f," animakt_time=\"%f\"",i_run->animakt.playtime);
        for(x=0;x<5;x++) {
          fprintf(f," sbtact%d=\"%d\"",x,i_run->sbtact[x]);
          fprintf(f," sbtava%d=\"%d\"",x,i_run->sbtava[x]);
        }
        fprintf(f," />\n");

      }
      fprintf(f,"    </page>\n");
    }

    fprintf(f,"  </mm>\n");
    fclose(f);
    mmef_unlock_player();
  } else {
    gtk_widget_destroy (dialog);
  }

}
