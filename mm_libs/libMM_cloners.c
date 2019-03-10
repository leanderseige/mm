int mmf_clone_animation(struct mms_evalanim * src, struct mms_evalanim * dst) {
  
  dst->playtime=src->playtime;

  if(dst->str_rx) free(dst->str_rx);
  if(dst->eval_rx) evaluator_destroy(dst->eval_rx);
  if(src->str_rx) {
    dst->str_rx=g_strdup(src->str_rx);
    dst->eval_rx=evaluator_create(dst->str_rx);
  } else {
    dst->str_rx=NULL;
    dst->eval_rx=NULL;
  }

  if(dst->str_ry) free(dst->str_ry);
  if(dst->eval_ry) evaluator_destroy(dst->eval_ry);
  if(src->str_ry) {
    dst->str_ry=g_strdup(src->str_ry);
    dst->eval_ry=evaluator_create(dst->str_ry);
  } else {
    dst->str_ry=NULL;
    dst->eval_ry=NULL;
  }

  if(dst->str_rz) free(dst->str_rz);
  if(dst->eval_rz) evaluator_destroy(dst->eval_rz);
  if(src->str_rz) {
    dst->str_rz=g_strdup(src->str_rz);
    dst->eval_rz=evaluator_create(dst->str_rz);
  } else {
    dst->str_rz=NULL;
    dst->eval_rz=NULL;
  }

  if(dst->str_tx) free(dst->str_tx);
  if(dst->eval_tx) evaluator_destroy(dst->eval_tx);
  if(src->str_tx) {
    dst->str_tx=g_strdup(src->str_tx);
    dst->eval_tx=evaluator_create(dst->str_tx);
  } else {
    dst->str_tx=NULL;
    dst->eval_tx=NULL;
  }

  if(dst->str_ty) free(dst->str_ty);
  if(dst->eval_ty) evaluator_destroy(dst->eval_ty);
  if(src->str_ty) {
    dst->str_ty=g_strdup(src->str_ty);
    dst->eval_ty=evaluator_create(dst->str_ty);
  } else {
    dst->str_ty=NULL;
    dst->eval_ty=NULL;
  }

  if(dst->str_tz) free(dst->str_tz);
  if(dst->eval_tz) evaluator_destroy(dst->eval_tz);
  if(src->str_tz) {
    dst->str_tz=g_strdup(src->str_tz);
    dst->eval_tz=evaluator_create(dst->str_tz);
  } else {
    dst->str_tz=NULL;
    dst->eval_tz=NULL;
  }

  if(dst->str_sw) free(dst->str_sw);
  if(dst->eval_sw) evaluator_destroy(dst->eval_sw);
  if(src->str_sw) {
    dst->str_sw=g_strdup(src->str_sw);
    dst->eval_sw=evaluator_create(dst->str_sw);
  } else {
    dst->str_sw=NULL;
    dst->eval_sw=NULL;
  }

  if(dst->str_sh) free(dst->str_sh);
  if(dst->eval_sh) evaluator_destroy(dst->eval_sh);
  if(src->str_sh) {
    dst->str_sh=g_strdup(src->str_sh);
    dst->eval_sh=evaluator_create(dst->str_sh);
  } else {
    dst->str_sh=NULL;
    dst->eval_sh=NULL;
  }
                                                                                
}

//-----------------------------------------------------------------------------

int mmf_clone_text(char ** vdst, char ** vsrc) {
  char * dst, * src;
  
  dst=NULL;
  src=(*vsrc);
  if(src) {
    dst=malloc(strlen(src)+1);
    strcpy(dst,src);
  }
  (*vdst)=dst;
}

//-----------------------------------------------------------------------------

int mmf_clone_texture(struct mms_texture ** vdst, struct mms_texture ** vsrc) {
  struct mms_texture * dst, * src;
  GLubyte * tp;

  dst=NULL;
  src=(*vsrc);
  if(src) {
    dst=mmf_new_texture();
    dst->w=src->w;
    dst->h=src->h;
    dst->a=src->a;
    dst->sbt_bw=NULL;
	dst->surf=NULL;
	dst->to_clone=src;
    dst->status=MM_TM_GETTEXIMAGE;                 
  }
  (*vdst)=dst;
}

//-----------------------------------------------------------------------------

struct mms_resource * mmf_clone_resource(struct mms_resource * res) {
  struct mms_resource * new_res;
  struct mms_button * new_button, * old_button;
  struct mms_text * new_text, * old_text;
  char *newname;
  
  newname=malloc(strlen(res->text)+7);
  strcpy(newname,"CLONE ");
  strcat(newname,res->text);
  
  switch (res->type) {
    case MM_TEXT_BUTTON:
    case MM_IMAGE_BUTTON:
      new_res=mmf_insert_resource(0,NULL,0,newname);
      old_button=(struct mms_button *)res->data;
      new_button=malloc(sizeof(struct mms_button));
      new_res->data=new_button;
      new_res->type=res->type;
      new_button->type=old_button->type;
      new_button->flags=old_button->flags;
      new_button->h=old_button->h;
      new_button->w=old_button->w;
      new_button->bow=old_button->bow;
      new_button->build_w=old_button->build_w;
      new_button->build_h=old_button->build_h;
      new_button->build_b=old_button->build_b;
      new_button->font_resource=old_button->font_resource;
      memcpy(&(new_button->fgl),&(old_button->fgl),sizeof(GdkColor));
      memcpy(&(new_button->bgl),&(old_button->bgl),sizeof(GdkColor));
      memcpy(&(new_button->fgh),&(old_button->fgh),sizeof(GdkColor));
      memcpy(&(new_button->bgh),&(old_button->bgh),sizeof(GdkColor));
      mmf_clone_text(&(new_button->text),&(old_button->text));
      mmf_clone_text(&(new_button->path),&(old_button->path));
      mmf_clone_texture(&(new_button->texture_low),&(old_button->texture_low));
      mmf_clone_texture(&(new_button->texture_high),&(old_button->texture_high));
      break;
    case MM_TEXT:
      new_res=mmf_insert_resource(0,NULL,0,newname);
      old_text=(struct mms_text *)res->data;
      new_text=malloc(sizeof(struct mms_text));
      new_res->data=new_text;
      new_res->type=res->type;
      new_text->type=old_text->type;
      new_text->w=old_text->w;
      new_text->h=old_text->h;
      new_text->bow=old_text->bow;
      mmf_clone_texture(&(new_text->texture),&(old_text->texture));
      mmf_clone_text(&(new_text->text),&(old_text->text));
      mmf_clone_text(&(new_text->full_text),&(old_text->full_text));
      new_text->font_resource=old_text->font_resource;
      new_text->field_width=old_text->field_width;
      new_text->border_width=old_text->border_width;
      new_text->line_height=old_text->line_height;
      new_text->align=old_text->align;
      memcpy(&(new_text->fg),&(old_text->fg),sizeof(GdkColor));
      memcpy(&(new_text->bg),&(old_text->bg),sizeof(GdkColor));
      break;
    case MM_IMAGE:
    case MM_SOUND:
    case MM_VIDEO:
    case MM_ASIAANIM:
    case MM_FONT:
      mmef_error(fenster,"Dieser Resourcentyp kann noch nicht geclont werden.");
      return(NULL);
    default:
      break;
  }
  free(newname);
  return(new_res);
}

//-----------------------------------------------------------------------------

struct mms_page * mmf_clone_page(GLuint old_page_id) {
  struct mms_page * old_page, * new_page;
  struct mms_item * old_item, * new_item;
  char * tt;

  old_page=mmf_get_page_by_id(old_page_id);
  
  tt=malloc(6+strlen(old_page->name)+1);
  strcpy(tt,"CLONE ");
  strcat(tt,old_page->name);
  new_page=mmf_add_page(0,tt,old_page->bg_color);
  free(tt);
    
  for(old_item=old_page->items;old_item;old_item=old_item->next) {
    mmf_insert_item(
      new_page->id,
      old_item->id,
      mmf_get_void_resource(old_item->data),
      0,0,
      MM_NO_ACTION,0,
      old_item->minx,old_item->maxx,
      old_item->miny,old_item->maxy,
      old_item->is_background,
      old_item->drop_shadow,
      old_item->is_sbt,
      old_item->light
    );
  }
  return(new_page);
}

//-----------------------------------------------------------------------------

struct mms_item * mmf_clone_item(GLuint old_item_id, struct mms_page * src_page, struct mms_page * dst_page) {
  struct mms_item * old_item;
  struct mms_resource * old_resource;
  
  old_item=mmf_get_item_by_id(src_page,old_item_id);
  if(!old_item) return(NULL);
  
  old_resource=mmf_get_void_resource(old_item->data);
  if(!old_resource) return(NULL);
  
  return mmf_insert_item(
    dst_page->id,
    old_item_id, // 0,
    old_resource,
    old_item->minx,old_item->miny,
    old_item->action,old_item->avalue,
    old_item->minx,old_item->maxx,
    old_item->miny,old_item->maxy,
    old_item->is_background,
    old_item->drop_shadow,
    0,
    old_item->light
  );
    
}
/*
struct mms_item * mmf_clone_item(GLuint old_item_id) {
  struct mms_item * old_item;
  struct mms_resource * old_resource;
  
  old_item=mmf_get_item_by_id(MM_CurrentPage,old_item_id);
  if(!old_item) return(NULL);
  
  old_resource=mmf_get_void_resource(old_item->data);
  if(!old_resource) return(NULL);
  
  return mmf_insert_item(
    MM_CurrentPage->id,
    0,
    old_resource,
    old_item->minx,old_item->miny,
    old_item->action,old_item->avalue,
    old_item->minx,old_item->maxx,
    old_item->miny,old_item->maxy,
    0,
    old_item->drop_shadow,
    0
  );
    
}
*/
//-----------------------------------------------------------------------------

