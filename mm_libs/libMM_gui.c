int mmf_page_get_background_item(struct mms_page * page) {
  struct mms_item * item;
  
  if(page==NULL) return(-1);
  for(item=page->items;item;item=item->next)
    if(item->is_background) return(item->id);
  return(-1);
}

//-----------------------------------------------------------------------------

int mmf_is_resource_page_item(struct mms_page * page, struct mms_resource * res) {
  struct mms_item * temp;

  if(page->items) {
    temp=page->items;
    while(temp) {
      if(res->id == temp->id)
        return(1);
      temp=temp->next;
    }
    return(0);
  } else {
    return(0);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_get_last_resource() {
  struct mms_resource * temp;

  if(MM_Resources) {
    temp=MM_Resources;
    while(temp->next) temp=temp->next;
    return(temp);
  } else {
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

int mmf_get_void_resource_id(void * pointer) {
  struct mms_resource * temp;

  if(MM_Resources) {
    temp=MM_Resources;
    while(temp) {
      if(temp->data==pointer) return(temp->id);
      temp=temp->next;
    }
    return(-1);
  } else {
    return(-1);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_get_void_resource(void * pointer) {
  struct mms_resource * temp;

  if(MM_Resources) {
    temp=MM_Resources;
    while(temp) {
      if(temp->data==pointer) return(temp);
      temp=temp->next;
    }
    fprintf(stderr,"res not found\n");
    return(NULL);
  } else {
    fprintf(stderr,"res not found\n");
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_item * mmf_get_last_item(struct mms_page * page) {
  struct mms_item * temp;

  if(page->items) {
    temp=page->items;
    while(temp->next) temp=temp->next;
    return(temp);
  } else {
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_page * mmf_get_page_by_id(GLuint id) {
  struct mms_page * temp;

  if(MM_Pages) {
    temp=MM_Pages;
    while(temp) { 
      if(temp->id==id) return(temp);
      temp=temp->next;
    }
    fprintf(stderr,"page not found\n");
    return(NULL);
  } else {
    fprintf(stderr,"page not found\n");
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_get_resource_by_id(GLuint id) {
  struct mms_resource * temp;

  if(MM_Resources) {
    temp=MM_Resources;
    while(temp) { 
      if(temp->id==id) return(temp);
      temp=temp->next;
    }
    return(NULL);
  } else {
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

struct mms_item * mmf_get_item_by_id(struct mms_page * page,GLuint id) {
  struct mms_item * temp;

  if(page->items) {
    temp=page->items;
    while(temp) { 
      if(temp->id==id) return(temp);
      temp=temp->next;
    }
    return(NULL);
  } else {
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

int mmf_delete_item(struct mms_page * page, int item_id) {
  struct mms_item * i_run;
  
  for(i_run=page->items;i_run;i_run=i_run->next) {
    if(i_run->id==item_id) {
      if(i_run->prev) i_run->prev->next=i_run->next;
      if(i_run->next) i_run->next->prev=i_run->prev;
      if(i_run==page->items) page->items=i_run->next;
      free(i_run->text);
      free(i_run);
      return(0);
    }
  }
  return(1);
}

//-----------------------------------------------------------------------------

int mmf_rebuild_item_geometry(struct mms_resource * res) {
  struct mms_page * p_run;
  struct mms_item * i_run;
  GLdouble minx,miny,maxx,maxy;

  for(p_run=MM_Pages;p_run;p_run=p_run->next)
    for(i_run=p_run->items;i_run;i_run=i_run->next) {
      if(i_run->res_id==res->id) {
        switch(res->type) {
          case MM_IMAGE_BUTTON:
          case MM_TEXT_BUTTON:
            i_run->maxx=i_run->minx+((struct mms_button *)res->data)->w;
            i_run->maxy=i_run->miny+((struct mms_button *)res->data)->h;
            break;
          case MM_TEXT:
            i_run->maxx=i_run->minx+((struct mms_text *)res->data)->w;
            i_run->maxy=i_run->miny+((struct mms_text *)res->data)->h;
            break;
          case MM_IMAGE:
            i_run->maxx=i_run->minx+((struct mms_image *)res->data)->w;
            i_run->maxy=i_run->miny+((struct mms_image *)res->data)->h;
            break;
          case MM_VIDEO:
            i_run->maxx=i_run->minx+((struct mms_video *)res->data)->w;
            i_run->maxy=i_run->miny+((struct mms_video *)res->data)->h;
            break;
          case MM_ASIAANIM:
            i_run->maxx=i_run->minx+((struct mms_asiaanim *)res->data)->w;
            i_run->maxy=i_run->miny+((struct mms_asiaanim *)res->data)->h;
            break;
          default:
            break;
        }  
      }
    }
}

//-----------------------------------------------------------------------------

struct mms_item * mmf_insert_item(GLuint page_id, GLuint requested_id, struct mms_resource * res, GLdouble x, GLdouble y, GLuint action, GLuint avalue, GLdouble minx, GLdouble maxx, GLdouble miny, GLdouble maxy, GLuint is_background, GLuint drop_shadow, GLuint is_sbt, GLuint light) {
  struct mms_item * new_item, * last_item;
  struct mms_page * page;

  new_item=malloc(sizeof(struct mms_item));
  if(!new_item) {
    fprintf(stderr,"Cannot alloc item structure.\n");
    return(NULL);
  }
  
  bzero(new_item,sizeof(struct mms_item));

  new_item->text=malloc(sizeof(char)*(1+strlen(res->text)));
  if(!new_item->text) {
    fprintf(stderr,"Cannot alloc resource text memory.\n");
    free(new_item);
    return(NULL);
  }
  strcpy(new_item->text,res->text);
  
  page=mmf_get_page_by_id(page_id);
  last_item=mmf_get_last_item(page);
  
  new_item->next=NULL;
  new_item->prev=last_item;
  if(last_item)
    last_item->next=new_item;
  else
    page->items=new_item;
  
  switch(res->type) {
    case MM_IMAGE_BUTTON:
    case MM_TEXT_BUTTON:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_button *)res->data)->type;
      new_item->data=res->data;
      new_item->action=action;
      new_item->avalue=avalue;
      if(minx||maxx||miny||maxy) {
        new_item->minx=minx;
        new_item->miny=miny;
        new_item->maxx=maxx;
        new_item->maxy=maxy;
      } else {
        new_item->minx=x;
        new_item->miny=y;
        new_item->maxx=x+((struct mms_button *)res->data)->w;
        new_item->maxy=y+((struct mms_button *)res->data)->h;
      }
      break;
    case MM_TEXT:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_text *)res->data)->type;
      new_item->data=res->data;
      new_item->action=action;
      new_item->avalue=avalue;
      if(minx||maxx||miny||maxy) {
        new_item->minx=minx;
        new_item->miny=miny;
        new_item->maxx=maxx;
        new_item->maxy=maxy;
      } else {
        new_item->minx=x;
        new_item->miny=y;
        new_item->maxx=x+((struct mms_text *)res->data)->w;
        new_item->maxy=y+((struct mms_text *)res->data)->h;
      }
      break;
    case MM_IMAGE:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_image *)res->data)->type;
      new_item->data=res->data;
      new_item->action=action;
      new_item->avalue=avalue;
      if(minx||maxx||miny||maxy) {
        new_item->minx=minx;
        new_item->miny=miny;
        new_item->maxx=maxx;
        new_item->maxy=maxy;
      } else {
        new_item->minx=x;
        new_item->miny=y;
        new_item->maxx=x+((struct mms_image *)res->data)->w;
        new_item->maxy=y+((struct mms_image *)res->data)->h;
      }
      break;
    case MM_VIDEO:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_video *)res->data)->type;
      new_item->data=res->data;
      new_item->action=action;
      new_item->avalue=avalue;
      if(minx||maxx||miny||maxy) {
        new_item->minx=minx;
        new_item->miny=miny;
        new_item->maxx=maxx;
        new_item->maxy=maxy;
      } else {
        new_item->minx=x;
        new_item->miny=y;
        new_item->maxx=x+((struct mms_video *)res->data)->w;
        new_item->maxy=y+((struct mms_video *)res->data)->h;
      }
      break;
    case MM_ASIAANIM:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_asiaanim *)res->data)->type;
      new_item->data=res->data;
      new_item->action=action;
      new_item->avalue=avalue;
      if(minx||maxx||miny||maxy) {
        new_item->minx=minx;
        new_item->miny=miny;
        new_item->maxx=maxx;
        new_item->maxy=maxy;
      } else {
        new_item->minx=x;
        new_item->miny=y;
        new_item->maxx=x+((struct mms_asiaanim *)res->data)->w;
        new_item->maxy=y+((struct mms_asiaanim *)res->data)->h;
      }
      break;
    case MM_SOUND:
      new_item->id=mmf_get_item_id(requested_id,page);
      new_item->type=((struct mms_sound *)res->data)->type;
      new_item->data=res->data;
      new_item->minx=0;
      new_item->miny=0;
      new_item->maxx=0;
      new_item->maxy=0;
      break;
    default:
      break;
  }  
  
  new_item->res_id=res->id;
  new_item->is_background=is_background;
  new_item->drop_shadow=drop_shadow;
  new_item->is_sbt=is_sbt;
  new_item->light=light;
  
  return(new_item);
}

//-----------------------------------------------------------------------------

int mmf_delete_resource(int res_id) {
  struct mms_resource * r_run;
  struct mms_page * p_run;
  struct mms_item * i_run;
  
  for(r_run=MM_Resources;r_run;r_run=r_run->next) {
    if(r_run->id==res_id) {

	  switch(r_run->type) {
		case MM_VIDEO:
		  SDL_LockMutex(MM_VideoLock);
		case MM_SOUND:
		  SDL_LockMutex(MM_SoundLock);
		default:
		  break;
	  } 
	
      if(r_run->prev) r_run->prev->next=r_run->next;
      if(r_run->next) r_run->next->prev=r_run->prev;
      if(r_run==MM_Resources) MM_Resources=r_run->next;
	  
	  
      for(p_run=MM_Pages;p_run;p_run=p_run->next) {
        for(i_run=p_run->items;i_run;i_run=i_run->next) {
          if(i_run->res_id==res_id) {
            printf("del item %d on page %d\n",i_run->id,p_run->id);
            mmf_delete_item(p_run,i_run->id);
          }
        }
      }
      switch (r_run->type) {
        case MM_TEXT:
          mmf_delete_res_text((struct mms_text *)r_run->data);
          break;
        case MM_IMAGE:
          mmf_delete_res_image((struct mms_image *)r_run->data);
          break;
        case MM_TEXT_BUTTON:
        case MM_IMAGE_BUTTON:
          mmf_delete_res_button((struct mms_button *)r_run->data);
          break;
        case MM_SOUND:
          mmf_delete_res_sound((struct mms_sound *)r_run->data);
          break;
        case MM_VIDEO:
          mmf_delete_res_video((struct mms_video *)r_run->data);
          break;
        case MM_ASIAANIM:
          mmf_delete_res_aad((struct mms_asiaanim *)r_run->data);
          break;
        case MM_FONT:
          mmf_delete_res_font((struct mms_font *)r_run->data);
          break;
        default:
          break;
      }
      free(r_run->text);

	  switch(r_run->type) {
		case MM_VIDEO:
		  SDL_UnlockMutex(MM_VideoLock);
		case MM_SOUND:
		  SDL_UnlockMutex(MM_SoundLock);
		default:
		  break;
	  } 

      free(r_run);
      return(0);
    }
  }
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_insert_resource(GLuint type, void * data, GLuint requested_id, char * text) {
  struct mms_resource * new_resource, * last_resource;
  
  
  new_resource=malloc(sizeof(struct mms_resource));
  if(!new_resource) {
    fprintf(stderr,"Cannot alloc resource structure.\n");
    return(NULL);
  }

  new_resource->id=mmf_get_resource_id(requested_id);
  printf("insert res: %d|%d [%s]\n",requested_id,new_resource->id,text);

  last_resource=mmf_get_last_resource();

  new_resource->text=malloc(sizeof(char)*(1+strlen(text)));
  if(!new_resource->text) {
    fprintf(stderr,"Cannot alloc resource text memory.\n");
    free(new_resource);
    return(NULL);
  }
  strcpy(new_resource->text,text);
  
  new_resource->next=NULL;
  new_resource->prev=last_resource;
  if(last_resource)
    last_resource->next=new_resource;
  else
    MM_Resources=new_resource;
  
  new_resource->type=type;
  new_resource->data=data;
  
  return(new_resource);
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_button(
                    GLuint type,
                    GLuint requested_id,
                    GLdouble w,
                    GLdouble h,
                    GLdouble b,
                    char * text,
                    GdkColor * fgl,
                    GdkColor * bgl,
                    GdkColor * fgh,
                    GdkColor * bgh,
                    void * data,
                    int flags,
                    GLdouble bow,
                    struct mms_button * old_but
                  ) {

  struct mms_button * tempbutton;
  struct mms_font * font;
  SDL_Color c_fgl,c_bgl,c_fgh,c_bgh;
  
  if(old_but!=NULL) {
    tempbutton=old_but;
    old_but->texture_low->status=MM_TM_REMOVE;
    old_but->texture_high->status=MM_TM_REMOVE;
    free(old_but->text);
    if(type==MM_IMAGE_BUTTON) free(old_but->path);
  }
  else {
    tempbutton=malloc(sizeof(struct mms_button));
    if(!tempbutton) {
      fprintf(stderr,"Cannot alloc button structure.\n");
      return(NULL);
    }
  }

  if(fgl&&bgl&&fgh&&bgh) {
    c_fgl.r=fgl->red>>8; c_fgl.g=fgl->green>>8; c_fgl.b=fgl->blue>>8;
    c_bgl.r=bgl->red>>8; c_bgl.g=bgl->green>>8; c_bgl.b=bgl->blue>>8;
    c_fgh.r=fgh->red>>8; c_fgh.g=fgh->green>>8; c_fgh.b=fgh->blue>>8;
    c_bgh.r=bgh->red>>8; c_bgh.g=bgh->green>>8; c_bgh.b=bgh->blue>>8;
    tempbutton->build_w=w;
    tempbutton->build_h=h;
    tempbutton->build_b=b;
    memcpy(&(tempbutton->fgl),fgl,sizeof(GdkColor));
    memcpy(&(tempbutton->bgl),bgl,sizeof(GdkColor));
    memcpy(&(tempbutton->fgh),fgh,sizeof(GdkColor));
    memcpy(&(tempbutton->bgh),bgh,sizeof(GdkColor));
  }
  
  tempbutton->text=malloc(1+strlen(text)*sizeof(char));
  if(!(tempbutton->text)) {
    free(tempbutton);
    fprintf(stderr,"Cannot alloc button text memory.\n");
    return(NULL);
  }
  
  if(type==MM_TEXT_BUTTON) {
    tempbutton->path=NULL;
    font=((struct mms_resource *)data)->data;
    tempbutton->font_resource=((struct mms_resource *)data)->id;
    tempbutton->texture_low=mmf_build_text_button(font,text,&c_fgl,&c_bgl,w,h,b,flags,bow);
    tempbutton->texture_high=mmf_build_text_button(font,text,&c_fgh,&c_bgh,w,h,b,flags,bow);
        
  } else if(type==MM_IMAGE_BUTTON) {
    tempbutton->path=malloc(1+strlen((char *)data)*sizeof(char));
    if(!tempbutton->path) {
      free(tempbutton->text);
      free(tempbutton);
      fprintf(stderr,"Cannot alloc button path memory.\n");
      return(NULL);
    }
    strcpy(tempbutton->path,(char *)data);
    tempbutton->texture_low=mmf_load_image(data,1);
    tempbutton->texture_high=mmf_load_image(data,1);
  } else {
    fprintf(stderr,"Unknown button type.\n");
    free(tempbutton->text);
    free(tempbutton);
    return(NULL);
  }

  tempbutton->flags=flags;
  tempbutton->type=type;
  tempbutton->bow=bow;
  strcpy(tempbutton->text,text);

  if(type==MM_TEXT_BUTTON) {
    tempbutton->h=h+2*b;
    tempbutton->w=((GLdouble)tempbutton->texture_low->w)/MM_ScreenWidthMax;
  } else {
    tempbutton->h=((GLdouble)tempbutton->texture_low->h)/MM_ScreenHeightMax;
    tempbutton->w=((GLdouble)tempbutton->texture_low->w)/MM_ScreenWidthMax;
  }

  if(old_but==NULL)
    return(mmf_insert_resource(type,(void *)tempbutton,requested_id,tempbutton->text));
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_text(
                                    struct mms_texture * texture,
                                    char * text,
                                    char * full_text,
                                    GLuint requested_id,
                                    GLuint font_resource,
                                    GLdouble field_width,
                                    GLdouble line_height,
                                    GLdouble border_width,
                                    GdkColor *fg,
                                    GdkColor *bg,
                                    GLuint align,
                                    int flags,
                                    GLdouble bow,
                                    struct mms_text * old_text
                                  ) {
  struct mms_text * temptext;

  if(old_text!=NULL) {
    temptext=old_text;
    free(temptext->text);
    free(temptext->full_text);
    old_text->texture->status=MM_TM_REMOVE;
  } else {
    temptext=malloc(sizeof(struct mms_text));
    if(!temptext) {
      fprintf(stderr,"cannot allocate text structure\n");
      return(NULL);
    }
  }
  temptext->font_resource=font_resource;
  temptext->field_width=field_width;
  temptext->border_width=border_width;
  temptext->line_height=line_height;
  memcpy(&(temptext->fg),fg,sizeof(GdkColor));
  memcpy(&(temptext->bg),bg,sizeof(GdkColor));
  temptext->align=align;

  temptext->text=malloc(sizeof(char)*(strlen(text)+1));
  if(!(temptext->text)) {
    fprintf(stderr,"cannot allocate text text memory\n");
    free(temptext);
    return(NULL);
  }

  temptext->full_text=malloc(sizeof(char)*(strlen(full_text)+1));
  if(!(temptext->full_text)) {
    fprintf(stderr,"cannot allocate text text memory\n");
    free(temptext->text);
    free(temptext);
    return(NULL);
  }

  strcpy(temptext->text,text);
  strcpy(temptext->full_text,full_text);
  
  temptext->texture=texture;
  if(texture) temptext->alpha=texture->a;
  temptext->type=MM_TEXT;
  temptext->flags=flags; // FIXME flags fuer fett, kursiv usw
  
  temptext->w=texture->w/(GLdouble)MM_ScreenWidthMax;
  temptext->h=texture->h/(GLdouble)MM_ScreenHeightMax;
  
  temptext->bow=bow;

  if(old_text==NULL) {
    return(mmf_insert_resource(MM_TEXT,temptext,requested_id,temptext->text));
  }
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_asiaanim(
                    GLuint requested_id,
                    GLdouble w,
                    GLdouble h,
                    GLdouble b,
                    char * text,
                    GdkColor * fg,
                    GdkColor * bg,
                    char * fn
                  ) {

  struct mms_asiaanim * tempasiaanim;
  FILE *f;
  char buffer[1024];
  struct aad_frame * cf;
  int ct,cs,cv,in0,in1,in2,in3;
  GLuint nf;
  double minx,maxx,miny,maxy,origw,origh,offx,offy;
  GLuint c,totalticks;
  GLuint fbo,renderbuffer;
  
  f=fopen(fn,"r");
  if(!f) {
    return(NULL);
  }
  fscanf(f,"%7c ",buffer);
  buffer[7]=0x00;
  if(strncmp(buffer,"AADV001",7)) {
    fclose(f);
    return(NULL);
  }

  tempasiaanim=malloc(sizeof(struct mms_asiaanim));
  if(!tempasiaanim) {
    fprintf(stderr,"Cannot alloc asiaanim structure.\n");
    fclose(f);
    return(NULL);
  }

  tempasiaanim->build_w=w;
  tempasiaanim->build_h=h;  
  tempasiaanim->build_b=b;

  cf=NULL; nf=0;
  while(fgets(buffer,1023,f)!=NULL) {
    if(buffer[0]=='#') {
      switch(buffer[1]) {
        case 'F':
          nf++;
          if(cf) {
            cf->next=malloc(sizeof(struct aad_frame));
            if(!cf->next) {
              fprintf(stderr,"out of memory while loading aad\n");
              fclose(f);
              free(tempasiaanim);
              return(NULL);
            }
            cf->next->prev=cf;
            cf=cf->next;
          }
          else {
            cf=malloc(sizeof(struct aad_frame));
            if(!cf) {
              fprintf(stderr,"out of memory while loading aad\n");
              fclose(f);
              free(tempasiaanim);
              return(NULL);
            }
            cf->prev=NULL;
            tempasiaanim->root=cf;
            tempasiaanim->current_frame=cf;
          }
          cf->next=NULL;
          cf->name[0]=0x00;
          sscanf(&buffer[2],"%d %63[^\n\r]",&(cf->ns),&(cf->name));
//          printf("loading new frame: %d %s\n",(cf->ns),(cf->name));
          cf->name[63]=0x00;
          cs=-1; cv=-1;
          break;
        case 'S':
          cf->s[++cs]=malloc(sizeof(struct aad_stroke));
          if(!cf->s[cs]) {
            fprintf(stderr,"out of memory (aad stroke malloc).");
            fclose(f);
            free(tempasiaanim);
            return(NULL);
          }
          sscanf(&buffer[2],"%d %63[^\n\r]",&(cf->s[cs]->nv),&(cf->s[cs]->name));
//          printf("new stroke: %d %s\n",(cf->s[cs]->nv),(cf->s[cs]->name));
          cv=0;
          break;
        default:
          break;
      }
    } else {
      if(cf->ns) {
        if(cs==-1||cv==-1) {
          fprintf(stderr,"file format error (aad vector).");
          fclose(f);
          free(tempasiaanim);
          return(NULL);
        }
        sscanf(buffer,"%d %d %d %d",&in0,&in1,&in2,&in3);
        cf->s[cs]->v[cv*4+0]=(double)in0;
        cf->s[cs]->v[cv*4+1]=(double)in1;
        cf->s[cs]->v[cv*4+2]=(double)in2;
        cf->s[cs]->v[cv*4+3]=(double)in3;
//        printf("read v: %4.4f %4.4f %4.4f %4.4f\n",cf->s[cs]->v[cv*4+0],cf->s[cs]->v[cv*4+1],cf->s[cs]->v[cv*4+2],cf->s[cs]->v[cv*4+3]);
        cv++;
      }
    }
  }
  
  tempasiaanim->all_frames=malloc(sizeof(struct aad_frame *)*nf);
  if(!tempasiaanim->all_frames) {
    fprintf(stderr,"out of memory (aad - all_frames)\n");
    fclose(f);
    free(tempasiaanim);
    return(NULL);
  }
  minx=999999; maxx=0;
  miny=999999; maxy=0;
  for(c=0,cf=tempasiaanim->root;cf;cf->next) {
    for(ct=0,cs=0;cs<cf->ns;cs++) {
      for(cv=0;cv<cf->s[cs]->nv;cv++) {
        if(cf->s[cs]->v[cv*4+0]<minx) minx=cf->s[cs]->v[cv*4+0];
        if(cf->s[cs]->v[cv*4+1]<miny) miny=cf->s[cs]->v[cv*4+1];
        if(cf->s[cs]->v[cv*4+0]>maxx) maxx=cf->s[cs]->v[cv*4+0];
        if(cf->s[cs]->v[cv*4+1]>maxy) maxy=cf->s[cs]->v[cv*4+1];
        ct+=cf->s[cs]->v[cv*4+2];
      }
    }
    tempasiaanim->all_frames[c  ]=cf;
    tempasiaanim->all_frames[c++]->ticks=ct;
//    printf("count %d [%d]\n",c,cf->ns);
    cf=cf->next;
  }

  if(maxx-minx>maxy-miny) {
    origw=origh=maxx-minx;
    offx=0; offy=(origh-(maxy-miny))/2;
    h=w*MM_ScreenAspect;
    
  } else {
    origw=origh=maxy-miny;
    offy=0; offx=(origw-(maxx-minx))/2;
    w=h/MM_ScreenAspect;
  }

  tempasiaanim->origw=origw;
  tempasiaanim->origh=origh;
  

  for(cf=tempasiaanim->root;cf;cf->next) {
    for(cs=0;cs<cf->ns;cs++) {
      for(cv=0;cv<cf->s[cs]->nv;cv++) {
        cf->s[cs]->v[cv*4+0]-=minx-offx;
        cf->s[cs]->v[cv*4+1]-=miny-offy;
      }
    }
    cf=cf->next;
  }

  

  fclose(f);
  
  if(!strlen(text)) text=fn;
  tempasiaanim->text=malloc(1+strlen(text)*sizeof(char));
  if(!(tempasiaanim->text)) {
    free(tempasiaanim);
    fprintf(stderr,"Cannot alloc asiaanim text memory.\n");
    return(NULL);
  }
  tempasiaanim->path=malloc(1+strlen(fn)*sizeof(char));
  if(!(tempasiaanim->path)) {
    free(tempasiaanim);
    fprintf(stderr,"Cannot alloc asiaanim text memory.\n");
    return(NULL);
  }

  tempasiaanim->current_frame=tempasiaanim->all_frames[nf-1];
  tempasiaanim->flags=MM_AD_PLAYFRAME; // PLAYFRAME;
  tempasiaanim->w=w;
  tempasiaanim->h=h;
  tempasiaanim->b=b;
  tempasiaanim->aa_totalticks=3000*(nf-1);
  tempasiaanim->aa_currenttick=0;
  tempasiaanim->aa_oldtick=0;
  tempasiaanim->nf=nf;
  tempasiaanim->type=MM_ASIAANIM;
  strcpy(tempasiaanim->text,text);
  strcpy(tempasiaanim->path  ,fn);
  
  memcpy(&tempasiaanim->fg_color,fg,sizeof(GdkColor));
  memcpy(&tempasiaanim->bg_color,bg,sizeof(GdkColor));
  
  tempasiaanim->texture=mmf_new_texture();
  tempasiaanim->texture->status=MM_TM_RENDERAAD;
  tempasiaanim->texture->w=MM_SBT_TextureSize;
  tempasiaanim->texture->h=MM_SBT_TextureSize;
  tempasiaanim->texture->a=0;
  tempasiaanim->texture->sbt_bw=NULL;
  
  return(mmf_insert_resource(MM_ASIAANIM,(void *)tempasiaanim,requested_id,tempasiaanim->text));
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_image(
                    GLuint requested_id,
                    char * text,
                    char * fn
                  ) {

  struct mms_image * tempimage;
  
  tempimage=malloc(sizeof(struct mms_image));
  if(!tempimage) {
    fprintf(stderr,"Cannot alloc image structure.\n");
    return(NULL);
  }
  
  if(!strlen(text)) text=fn;
  tempimage->text=malloc(1+strlen(text)*sizeof(char));
  if(!(tempimage->text)) {
    free(tempimage);
    fprintf(stderr,"Cannot alloc button text memory.\n");
    return(NULL);
  }

  tempimage->path=malloc(1+strlen(fn)*sizeof(char));
  if(!(tempimage->path)) {
    free(tempimage);
    free(tempimage->text);
    fprintf(stderr,"Cannot alloc button text memory.\n");
    return(NULL);
  }
  
  tempimage->texture=mmf_load_image(fn,1);

  strcpy(tempimage->text,text);
  strcpy(tempimage->path,fn);

  tempimage->type=MM_IMAGE;

  tempimage->h=((GLdouble)tempimage->texture->h)/MM_ScreenHeightMax;
  tempimage->w=((GLdouble)tempimage->texture->w)/MM_ScreenWidthMax;

  return(mmf_insert_resource(MM_IMAGE,(void *)tempimage,requested_id,tempimage->text));
}

//-----------------------------------------------------------------------------

struct mms_page * mmf_get_last_page() {
  struct mms_page * temp;

  if(MM_Pages) {
    temp=MM_Pages;
    while(temp->next) temp=temp->next;
    return(temp);
  } else {
    return(NULL);
  }
  
}

//-----------------------------------------------------------------------------

int mmf_delete_page(int page_id) {
  struct mms_page * p_run;
  
  for(p_run=MM_Pages;p_run;p_run=p_run->next) {
    if(p_run->id==page_id) {
      if(p_run->prev) p_run->prev->next=p_run->next;
      if(p_run->next) p_run->next->prev=p_run->prev;
      if(p_run==MM_Pages) MM_Pages=p_run->next;
      if(p_run==MM_CurrentPage) MM_CurrentPage=MM_Pages;
      while(p_run->items) mmf_delete_item(p_run,p_run->items->id);
      free(p_run->name);
      free(p_run);
      return(0);
    }
  }
}

//-----------------------------------------------------------------------------

struct mms_page * mmf_add_page(GLuint requested_id, char * name, GdkColor bg_col) {
  struct mms_page * p, * s;
  
  p=malloc(sizeof(struct mms_page));
  if(!p) {
    fprintf(stderr,"Cannot alloc page structure.\n");
    return(NULL);
  }

  p->name=malloc(sizeof(char)*(strlen(name)+1));
  if(!(p->name)) {
    fprintf(stderr,"cannot allocate page name memory\n");
    free(p);
    return(NULL);
  }
  strcpy(p->name,name);
  
  p->id=mmf_get_page_id(requested_id);
  p->items=NULL;
  s=mmf_get_last_page();
  if(!s) {
    p->next=NULL;
    p->prev=NULL;
    MM_Pages=p;
    MM_CurrentPage=p;
  } else {
    s->next=p;
    p->prev=s;
    p->next=NULL;
  }

  
  p->bg_color.red=bg_col.red;
  p->bg_color.green=bg_col.green;
  p->bg_color.blue=bg_col.blue;
  
  p->action=MM_NO_ACTION;
  p->avalue=0;
  
  return(p);
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_font(char * fn, char * text, GLuint requested_id) {
  struct mms_font * tempfont;
  
  tempfont=malloc(sizeof(struct mms_font));
  if(!tempfont) {
    fprintf(stderr,"cannot allocate font structure\n");
    return(NULL);
  }

  tempfont->text=malloc(sizeof(char)*(strlen(text)+1));
  if(!(tempfont->text)) {
    fprintf(stderr,"cannot allocate font text memory\n");
    free(tempfont);
    return(NULL);
  }

  tempfont->path=malloc(sizeof(char)*(strlen(fn)+1));
  if(!(tempfont->path)) {
    fprintf(stderr,"cannot allocate font path memory\n");
    free(tempfont);
    free(tempfont->text);
    return(NULL);
  }

  strcpy(tempfont->path,fn);
  strcpy(tempfont->text,text);
  
  return(mmf_insert_resource(MM_FONT,tempfont,requested_id,tempfont->text));
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_sound(char * fn, char * text, GLuint requested_id) {
  struct mms_sound * tempsound;
  
  tempsound=malloc(sizeof(struct mms_sound));
  if(!tempsound) {
    fprintf(stderr,"cannot allocate sound structure\n");
    return(NULL);
  }
  
  if( SDL_LoadWAV(fn, &tempsound->spec, &tempsound->buffer, &tempsound->len) == NULL ){
    fprintf(stderr, "Could not open:%s: %s\n",fn, SDL_GetError());
    free(tempsound);
    return(NULL);
  }
  
  if(   tempsound->spec.freq!=48000
      ||tempsound->spec.channels!=2
      ||tempsound->spec.format!=AUDIO_S16
  ) {
    SDL_FreeWAV(tempsound->buffer);
    free(tempsound);
    fprintf(stderr,"Wrong audio format (48kHz/16bit/Stereo required)\n");
    return(NULL);
  };

  tempsound->path=malloc(sizeof(char)*(strlen(fn)+1));
  if(!(tempsound->path)) {
    SDL_FreeWAV(tempsound->buffer);
    fprintf(stderr,"cannot allocate sound text memory\n");
    free(tempsound);
    return(NULL);
  }

  if(!strlen(text)) text=fn;
  tempsound->text=malloc(sizeof(char)*(strlen(text)+1));
  if(!(tempsound->text)) {
    SDL_FreeWAV(tempsound->buffer);
    fprintf(stderr,"cannot allocate sound text memory\n");
    free(tempsound->path);
    free(tempsound);
    return(NULL);
  }
  
  SDL_LockMutex(MM_SoundLock);
  tempsound->play_pos=tempsound->buffer;
  tempsound->play_remain=tempsound->len;
  tempsound->is_playing=0;
  tempsound->type=MM_SOUND;
  SDL_UnlockMutex(MM_SoundLock);
  
  strcpy(tempsound->text,text);
  strcpy(tempsound->path,fn);
  
  return(mmf_insert_resource(MM_SOUND,tempsound,requested_id,tempsound->text));
  
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_add_video( char *fn,
                                  GLuint requested_id,
                                  GLdouble w,
                                  GLdouble h,
                                  char * text
                                ) {
  struct mms_video * tempvideo;
  int i;
  
  tempvideo=malloc(sizeof(struct mms_video));
  if(!tempvideo) {
    fprintf(stderr,"cannot allocate video structure\n");
    return(NULL);
  }

  tempvideo->text=malloc(sizeof(char)*(strlen(text)+1));
  if(!(tempvideo->text)) {
    fprintf(stderr,"cannot allocate video text memory\n");
    free(tempvideo);
    return(NULL);
  }
  strcpy(tempvideo->text,text);

  tempvideo->path=malloc(sizeof(char)*(strlen(fn)+1));
  if(!(tempvideo->path)) {
    fprintf(stderr,"cannot allocate video text memory\n");
    free(tempvideo->text);
    free(tempvideo);
    return(NULL);
  }
  strcpy(tempvideo->path,fn);

  for(i=0; i < 4; i++) {
    if(!(tempvideo->audio_buffers[i] = malloc(DV_AUDIO_MAX_SAMPLES*sizeof(gint16)))) {
      fprintf(stderr,"error mallocing audio buffers for dv video\n");
      free(tempvideo->text);
      free(tempvideo);
      return(NULL);
    }
  }
  
//  printf("max_samples: %d\n",DV_AUDIO_MAX_SAMPLES);
  
  if(mmf_dv_open_file(tempvideo,fn)) {
    fprintf(stderr,"cannot open video file\n");
    free(tempvideo->text);
    free(tempvideo);
    return(NULL);
  }
  
  tempvideo->pixels=malloc(tempvideo->framesize);
  if(!tempvideo->pixels) {
    fprintf(stderr,"cannot malloc video pixel buffer\n");
    close(tempvideo->filedesc);
    free(tempvideo->text);
    free(tempvideo);
    return(NULL);
  }
  tempvideo->texture->rawpixels=tempvideo->pixels; // same space
  
  tempvideo->type=MM_VIDEO;
  tempvideo->is_playing=0;
  tempvideo->w=w;
  tempvideo->h=h;

  mmf_dv_read_frame(tempvideo);
  
//  mmf_dv_get_audio_specs(tempvideo);
  
  return(mmf_insert_resource(MM_VIDEO,tempvideo,requested_id,tempvideo->text));
}
