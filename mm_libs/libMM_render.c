int mmf_render_sbt_area(struct mms_item * item) {
  GLdouble rw,rh;  
  GLdouble pw,ph;  

  rw=(MM_SBT_Display->bw/MM_SBT_Display->tw)*(item->maxx-item->minx);
  rh=(MM_SBT_Display->bh/MM_SBT_Display->th)*(item->maxy-item->miny);
  pw=(MM_SBT_Display->bxS/MM_SBT_Display->tw)*(item->maxx-item->minx);
  ph=(MM_SBT_Display->byS/MM_SBT_Display->th)*(item->maxy-item->miny);

  glPushMatrix();
//  glDisable(GL_DEPTH_TEST);
  glTranslatef(item->minx,item->miny,-0.02);
  glColor4f(1,1,1,1);
//  glBegin(GL_LINE_LOOP);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
  glBegin(GL_QUADS);
    glVertex3f(pw+ 0,ph+ 0,0);
    glVertex3f(pw+ 0,ph+rh,0);
    glVertex3f(pw+rw,ph+rh,0);
    glVertex3f(pw+rw,ph+ 0,0);
  glEnd();
  glDisable(GL_BLEND);
  glColor4f(1,1,1,1);
//  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
  
}

void mmf_evaluate_animation_shadow(GLdouble minx, GLdouble maxx, GLdouble miny, GLdouble maxy, struct mms_evalanim * animation) {
  GLdouble rx,ry,rz,tx,ty,tz,sw,sh;
  char *names[]={"timescale","rand01","rand02"};
  double values[3];

  if(animation->timescale==0) return;

  values[0]=animation->timescale;
  values[1]=fmod(animation->starttime,1);
  values[2]=animation->random;
  
  rx=(animation->eval_rx)?(evaluator_evaluate(animation->eval_rx,3,names,values)):0;
  ry=(animation->eval_ry)?(evaluator_evaluate(animation->eval_ry,3,names,values)):0;
  rz=(animation->eval_rz)?(evaluator_evaluate(animation->eval_rz,3,names,values)):0;
  tx=(animation->eval_tx)?(evaluator_evaluate(animation->eval_tx,3,names,values)):0;
  ty=(animation->eval_ty)?(evaluator_evaluate(animation->eval_ty,3,names,values)):0;
  tz=(animation->eval_tz)?(evaluator_evaluate(animation->eval_tz,3,names,values)):0;
  sw=(animation->eval_sw)?(evaluator_evaluate(animation->eval_sw,3,names,values)):1;
  sh=(animation->eval_sh)?(evaluator_evaluate(animation->eval_sh,3,names,values)):1;
//  printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",rx,ry,rz,tx,ty,tz,sw,sh);
  tx*=(maxx-minx)/2;
  ty*=(maxy-miny)/2;
  glRotatef(rx*360.0,1,0,0);
  glRotatef(ry*360.0,0,1,0);
  glRotatef(rz*360.0,0,0,1);
  glTranslatef(tx,ty,0);
  glTranslatef(((maxx-minx)/2)*(-tz/10),((maxy-miny)/2)*(-tz/10),0);
  glScalef(sw,sh,1);
}

int mmf_render_shadow_rect(GLdouble minx, GLdouble maxx, GLdouble miny, GLdouble maxy, struct mms_evalanim * animation) {
  GLdouble ya,yb,xa,xb,dx,dy;

  glPushMatrix();
  glTranslatef(minx+(maxx-minx)/2,miny+(maxy-miny)/2,.05);

//  glDisable(GL_DEPTH_TEST);
//  dx= (.5-minx+(maxx-minx)/2) * .025;
//  dy= (.5-miny+(maxy-miny)/2) * .025;
  dx=.015;
  dy=.015;

  glTranslatef(dx,dy,.05);

  if(animation!=NULL) {
    mmf_evaluate_animation_shadow(minx,maxx,miny,maxy,animation);
  }

  yb=(maxy-miny)/2-.025;
  xb=(maxx-minx)/2-.025;
  ya=(maxy-miny)/2+.025;
  xa=(maxx-minx)/2+.025;
  xb=(xb<0)?0:xb;
  xa=(xa<0)?0:xa;
  yb=(yb<0)?0:yb;
  ya=(ya<0)?0:ya;
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_center.name);
  glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f  (-xb,-yb,0);        
    glTexCoord2f(4,0); glVertex3f  ( xb,-yb,0);        
    glTexCoord2f(4,4); glVertex3f  ( xb, yb,0);       
    glTexCoord2f(0,4); glVertex3f  (-xb, yb,0);       
  glEnd();
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_edge.name);
  glBegin(GL_QUADS);
    glTexCoord2f(0,4); glVertex3f  (-xb, ya,0);        
    glTexCoord2f(4,4); glVertex3f  ( xb, ya,0);        
    glTexCoord2f(4,0); glVertex3f  ( xb, yb,0);       
    glTexCoord2f(0,0); glVertex3f  (-xb, yb,0);
           
    glTexCoord2f(0,0); glVertex3f  (-xb,-yb,0);        
    glTexCoord2f(4,0); glVertex3f  ( xb,-yb,0);        
    glTexCoord2f(4,4); glVertex3f  ( xb,-ya,0);       
    glTexCoord2f(0,4); glVertex3f  (-xb,-ya,0);       
    
    glTexCoord2f(4,0); glVertex3f  (-xb,-yb,0);        
    glTexCoord2f(4,4); glVertex3f  (-xa,-yb,0);        
    glTexCoord2f(0,4); glVertex3f  (-xa, yb,0);       
    glTexCoord2f(0,0); glVertex3f  (-xb, yb,0);       
    
    glTexCoord2f(0,4); glVertex3f  ( xa,-yb,0);        
    glTexCoord2f(0,0); glVertex3f  ( xb,-yb,0);        
    glTexCoord2f(4,0); glVertex3f  ( xb, yb,0);       
    glTexCoord2f(4,4); glVertex3f  ( xa, yb,0);       
  glEnd();
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_corner.name);
  glBegin(GL_QUADS);
    glTexCoord2f(0,4); glVertex3f  (-xb, ya,0);        
    glTexCoord2f(4,4); glVertex3f  (-xa, ya,0);        
    glTexCoord2f(4,0); glVertex3f  (-xa, yb,0);       
    glTexCoord2f(0,0); glVertex3f  (-xb, yb,0);
           
    glTexCoord2f(0,0); glVertex3f  (-xb,-yb,0);        
    glTexCoord2f(4,0); glVertex3f  (-xa,-yb,0);        
    glTexCoord2f(4,4); glVertex3f  (-xa,-ya,0);       
    glTexCoord2f(0,4); glVertex3f  (-xb,-ya,0);       
    
    glTexCoord2f(0,0); glVertex3f  ( xb,-yb,0);        
    glTexCoord2f(4,0); glVertex3f  ( xa,-yb,0);        
    glTexCoord2f(4,4); glVertex3f  ( xa,-ya,0);       
    glTexCoord2f(0,4); glVertex3f  ( xb,-ya,0);       
    
    glTexCoord2f(4,4); glVertex3f  ( xa, ya,0);        
    glTexCoord2f(4,0); glVertex3f  ( xb, ya,0);        
    glTexCoord2f(0,0); glVertex3f  ( xb, yb,0);       
    glTexCoord2f(0,4); glVertex3f  ( xa, yb,0);       
  glEnd();

  glPopMatrix();
}

int mmf_texture_proc(struct mms_texture * texture) {
  GLubyte * tp;

  switch(texture->status) {
	case MM_TM_SURFACE:
  	  if(texture->surf!=NULL) {
		glEnable(GL_TEXTURE_RECTANGLE_NV);  
	    glGenTextures(1,&(texture->name));
	    glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->name);
	    mmf_set_stdtexparms();
	    glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,texture->w,texture->h,0,GL_BGRA,GL_UNSIGNED_BYTE,texture->surf->pixels);
	    SDL_FreeSurface(texture->surf);
	    texture->surf=NULL;
	    texture->status=MM_TM_UPLOADED;
	  }
	  break;
	case MM_TM_GETTEXIMAGE:
	  if(texture->to_clone!=NULL) {
    	tp=malloc(texture->to_clone->w*texture->to_clone->h*4);
  		glEnable(GL_TEXTURE_RECTANGLE_NV);
    	glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->to_clone->name);
    	glGetTexImage(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,tp);
    	glGenTextures(1,&texture->name);
  		glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->name);
    	glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,texture->to_clone->w,texture->to_clone->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,tp);
    	free(tp);
		texture->status=MM_TM_UPLOADED;
	  }							  
	  break;
    case MM_TM_VIDEOGENUPD:
  	  glEnable(GL_TEXTURE_RECTANGLE_NV);
      glGenTextures(1,&(texture->name));
    case MM_TM_VIDEOUPDATE:
      if(texture->rawpixels!=NULL) {
        glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->name);

        SDL_LockMutex(MM_VideoLock);
        glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGB,texture->w,texture->h,0,GL_RGB,GL_UNSIGNED_BYTE,texture->rawpixels);
//		texture->status=MM_TM_UPLOADED;
        SDL_UnlockMutex(MM_VideoLock);

        SDL_LockMutex(MM_VideoReadyMutex);
		texture->status=MM_TM_UPLOADED;
        SDL_CondBroadcast(MM_VideoReadyCond);
        SDL_UnlockMutex(MM_VideoReadyMutex);

      }
      break;
	default:
	  break;
  }
}

void mmf_evaluate_animation(struct mms_evalanim * animation) {
  GLdouble rx,ry,rz,tx,ty,tz,sw,sh;
  char *names[]={"timescale","rand01","rand02"};
  double values[3];

  if(animation->timescale==0) return;

  values[0]=animation->timescale;
  values[1]=fmod(animation->starttime,1);
  values[2]=animation->random;
  
  rx=(animation->eval_rx)?(evaluator_evaluate(animation->eval_rx,3,names,values)):0;
  ry=(animation->eval_ry)?(evaluator_evaluate(animation->eval_ry,3,names,values)):0;
  rz=(animation->eval_rz)?(evaluator_evaluate(animation->eval_rz,3,names,values)):0;
  tx=(animation->eval_tx)?(evaluator_evaluate(animation->eval_tx,3,names,values)):0;
  ty=(animation->eval_ty)?(evaluator_evaluate(animation->eval_ty,3,names,values)):0;
  tz=(animation->eval_tz)?(evaluator_evaluate(animation->eval_tz,3,names,values)):0;
  sw=(animation->eval_sw)?(evaluator_evaluate(animation->eval_sw,3,names,values)):1;
  sh=(animation->eval_sh)?(evaluator_evaluate(animation->eval_sh,3,names,values)):1;
//  printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",rx,ry,rz,tx,ty,tz,sw,sh);
  glRotatef(rx*360.0,1,0,0);
  glRotatef(ry*360.0,0,1,0);
  glRotatef(rz*360.0,0,0,1);
  glTranslatef(tx,ty,0);
  glTranslatef(tz/10,tz/10,0);
  glScalef(sw,sh,1);
}

int mmf_render_texture_quad(
  GLdouble minx, GLdouble maxx,
  GLdouble miny, GLdouble maxy,
  struct mms_texture * texture, GLdouble zoom,
  int vflip,
  struct mms_evalanim * animation)
{

  if(texture->status!=MM_TM_UPLOADED)
    mmf_texture_proc(texture);

  glPushMatrix();
  glTranslatef(minx+(maxx-minx)/2,miny+(maxy-miny)/2,0);
  glScalef((maxx-minx)/2,(maxy-miny)/2,1);
  if(animation!=NULL) {
    mmf_evaluate_animation(animation);
  }
  if(texture->a) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,1);
  }
  glEnable(GL_TEXTURE_RECTANGLE_NV);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->name);
  if(texture->a)
	mmf_set_stdtexparms();
  else
	mmf_set_stdtexparms();
  glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glTexCoord2f(texture->w*(0.0+zoom),texture->h*(0.0+zoom));
    glVertex3f  (-1, -1+(2*vflip),0);        
    glNormal3f(0,0,1);
    glTexCoord2f(texture->w*(1.0-zoom),texture->h*(0.0+zoom));
    glVertex3f  ( 1, -1+(2*vflip),0);        
    glNormal3f(0,0,1);
    glTexCoord2f(texture->w*(1.0-zoom),texture->h*(1.0-zoom));
    glVertex3f  ( 1,  1-(2*vflip),0);       
    glNormal3f(0,0,1);
    glTexCoord2f(texture->w*(0.0+zoom),texture->h*(1.0-zoom));
    glVertex3f  (-1,  1-(2*vflip),0);       
  glEnd();
  if(texture->a) {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
  }
  glPopMatrix();
}


int mmf_render_page_background() {
  struct mms_item * runner;
  struct mms_texture * temptexture;

  glDisable(GL_LIGHTING);
  mmf_screen_set_ortho2d();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  runner=MM_CurrentPage->items;
  while(runner) {
    if(runner->is_background) {
      switch(runner->type) {
        case MM_TEXT_BUTTON:
        case MM_IMAGE_BUTTON:
          temptexture=((struct mms_button *)(runner->data))->texture_low;
          break;
        case MM_IMAGE:
          temptexture=((struct mms_image *)(runner->data))->texture;
          break;
        case MM_VIDEO:
          temptexture=((struct mms_video *)(runner->data))->texture;
          break;
        case MM_TEXT:
          temptexture=((struct mms_text *)(runner->data))->texture;
          break;
        default:
          return(0);
          break;
      }
      mmf_render_texture_quad( 0, 1, 0, 1, temptexture, runner->type==MM_VIDEO?.05:0,0,NULL );
      return(0);
    }
    runner=runner->next;
  }
  glEnable(GL_DEPTH_TEST);
}

int mmf_render_asiaanim_playanim (struct mms_asiaanim * aa, double minx, double maxx, double miny, double maxy) {
  GLuint temp,c,cs,cv;
  GLdouble dx,dy,stepx,stepy,startx,starty,endx,endy,runx,runy,swap;
  struct aad_frame *sf,*ef;
  GLdouble isx,iex,isy,iey;
  GLdouble factor;

  glPushMatrix();
  glTranslatef(minx,miny,-0.01);
  glScalef((maxx-minx),(maxy-miny),1);
  glTranslatef(aa->b,aa->b,0);
  glScalef(1-(2*aa->b),1-(2*aa->b),1);
  
  temp=SDL_GetTicks();
  if(aa->aa_oldtick==0) aa->aa_oldtick=temp;
  aa->aa_currenttick=temp-(aa->aa_oldtick);
  if((aa->aa_currenttick)>=(aa->aa_totalticks)+1000) {
    aa->aa_oldtick=temp;
    aa->aa_currenttick=0;
  }
  
  if(aa->aa_currenttick<1000) {
    sf=ef=aa->all_frames[0];
    factor=1;
  } else {
    sf=aa->all_frames[((aa->aa_currenttick-1000)/3000)];
    ef=aa->all_frames[(((aa->aa_currenttick-1000)/3000)+1)%(aa->nf)];
    factor=((GLdouble)((aa->aa_currenttick-1000)%3000))/3000;
  }
  
  glBegin(GL_POINTS);
  for(cs=0;cs<sf->ns;cs++) {
    for(cv=1;cv<sf->s[cs]->nv;cv++) {
      startx=(sf->s[cs]->v[cv*4+0]*(1-factor)+ef->s[cs]->v[cv*4+0]*factor)/aa->origw;
      starty=(sf->s[cs]->v[cv*4+1]*(1-factor)+ef->s[cs]->v[cv*4+1]*factor)/aa->origh;
      endx  =(sf->s[cs]->v[(cv-1)*4+0]*(1-factor)+ef->s[cs]->v[(cv-1)*4+0]*factor)/aa->origw;
      endy  =(sf->s[cs]->v[(cv-1)*4+1]*(1-factor)+ef->s[cs]->v[(cv-1)*4+1]*factor)/aa->origh;
      dx=endx-startx;
      dy=endy-starty;
      stepx=(1/(maxx-minx))*(1/(GLdouble)MM_ScreenWidthDisplay);
      stepy=(1/(maxy-miny))*(1/(GLdouble)MM_ScreenWidthDisplay);
      if(fabs(dx)>fabs(dy)) {
        if(startx<endx) 
          for(runx=startx;runx<=endx;runx+=stepx) {
            glVertex3f(runx, starty+(dy*(runx-startx))/fabs(dx), 0);
          }
        else
          for(runx=endx;runx<=startx;runx+=stepx) {
            glVertex3f(runx, endy-(dy*  (runx-endx))/fabs(dx), 0);
          }
      } else {
        if(starty<endy)
          for(runy=starty;runy<=endy;runy+=stepy) {
            glVertex3f(startx+(dx*(runy-starty))/fabs(dy), runy, 0);
          }
        else
          for(runy=endy;runy<=starty;runy+=stepy) {
            glVertex3f(endx-(dx*  (runy-endy))/fabs(dy), runy, 0);
          }
      }
    }
  }
  glEnd();
  glPopMatrix();

}

int mmf_render_asiaanim_still (struct mms_asiaanim * aa, double minx, double maxx, double miny, double maxy) {
  GLuint temp,c,cs,cv;
  GLdouble dx,dy,stepx,stepy,startx,starty,endx,endy,runx,runy,swap;

  glPushMatrix();
  glTranslatef(minx,miny,-0.01);
  glScalef((maxx-minx),(maxy-miny),1);
  glTranslatef(aa->b,aa->b,0);
  glScalef(1-(2*aa->b),1-(2*aa->b),1);

  
  glBegin(GL_POINTS);
  for(cs=0;cs<aa->current_frame->ns;cs++) {
    for(cv=1;cv<aa->current_frame->s[cs]->nv;cv++) {
      startx=(aa->current_frame->s[cs]->v[cv*4+0])/aa->origw;
      starty=(aa->current_frame->s[cs]->v[cv*4+1])/aa->origh;
      endx  =(aa->current_frame->s[cs]->v[(cv-1)*4+0])/aa->origw;
      endy  =(aa->current_frame->s[cs]->v[(cv-1)*4+1])/aa->origh;
      dx=endx-startx;
      dy=endy-starty;
      stepx=(1/(maxx-minx))*(1/(GLdouble)MM_ScreenWidthMax);
      stepy=(1/(maxy-miny))*(1/(GLdouble)MM_ScreenWidthMax);
      if(fabs(dx)>fabs(dy)) {
        if(startx<endx) 
          for(runx=startx;runx<=endx;runx+=stepx) {
            glVertex3f(runx, starty+(dy*(runx-startx))/fabs(dx), 0);
          }
        else
          for(runx=endx;runx<=startx;runx+=stepx) {
            glVertex3f(runx, endy-(dy*  (runx-endx))/fabs(dx), 0);
          }
      } else {
        if(starty<endy)
          for(runy=starty;runy<=endy;runy+=stepy) {
            glVertex3f(startx+(dx*(runy-starty))/fabs(dy), runy, 0);
          }
        else
          for(runy=endy;runy<=starty;runy+=stepy) {
            glVertex3f(endx-(dx*  (runy-endy))/fabs(dy), runy, 0);
          }
      }
    }
  }
  glEnd();
  glPopMatrix();

}

int mmf_render_asiaanim_playframe (struct mms_asiaanim * aa, double minx, double maxx, double miny, double maxy) {
  GLuint temp,c,cs,cv,ct,retval;
  GLdouble dx,dy,stepx,stepy,startx,starty,endx,endy,runx,runy,swap;

  glPushMatrix();
  glTranslatef(minx,miny,-0.01);
  glScalef((maxx-minx),(maxy-miny),1);
  glTranslatef(aa->b,aa->b,0);
  glScalef(1-(2*aa->b),1-(2*aa->b),1);
  
  temp=SDL_GetTicks();    
  aa->aa_currenttick=temp-aa->aa_oldtick;
  if(aa->aa_currenttick>aa->current_frame->ticks) {
    aa->aa_oldtick=temp;
    aa->aa_currenttick=0;
  } else {
  }
  
  glBegin(GL_POINTS);
  for(ct=0,cs=0;(cs<aa->current_frame->ns)&&(ct<aa->aa_currenttick);cs++) {
    for(cv=1;(cv<aa->current_frame->s[cs]->nv)&&(ct<aa->aa_currenttick);cv++) {
      ct+=aa->current_frame->s[cs]->v[cv*4+2];
      startx=(aa->current_frame->s[cs]->v[cv*4+0])/aa->origw;
      starty=(aa->current_frame->s[cs]->v[cv*4+1])/aa->origh;
      endx  =(aa->current_frame->s[cs]->v[(cv-1)*4+0])/aa->origw;
      endy  =(aa->current_frame->s[cs]->v[(cv-1)*4+1])/aa->origh;
      dx=endx-startx;
      dy=endy-starty;
      stepx=(1/(maxx-minx))*(1/(GLdouble)MM_ScreenWidthDisplay);
      stepy=(1/(maxy-miny))*(1/(GLdouble)MM_ScreenWidthDisplay);
      if(fabs(dx)>fabs(dy)) {
        if(startx<endx) 
          for(runx=startx;runx<=endx;runx+=stepx) {
            glVertex3f(runx, starty+(dy*(runx-startx))/fabs(dx), 0);
          }
        else
          for(runx=endx;runx<=startx;runx+=stepx) {
            glVertex3f(runx, endy-(dy*  (runx-endx))/fabs(dx), 0);
          }
      } else {
        if(starty<endy)
          for(runy=starty;runy<=endy;runy+=stepy) {
            glVertex3f(startx+(dx*(runy-starty))/fabs(dy), runy, 0);
          }
        else
          for(runy=endy;runy<=starty;runy+=stepy) {
            glVertex3f(endx-(dx*  (runy-endy))/fabs(dy), runy, 0);
          }
      }
    }
  }
  glEnd();
  glPopMatrix();
}


void mmf_build_aad_texture(struct mms_asiaanim * aad) {
  GLuint fbo,renderbuffer;
  struct mms_texture * ret;
  
  ret=aad->texture;
  
  glGenFramebuffersEXT(1,&fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
  glGenRenderbuffersEXT(1,&renderbuffer);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,renderbuffer);
  //  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,MM_SBT_TextureSize,MM_SBT_TextureSize);
  //  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,renderbuffer);
	  
  glEnable(GL_TEXTURE_RECTANGLE_NV);  
  glGenTextures(1,&(ret->name));
		  
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,ret->name);
  glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,MM_SBT_TextureSize,MM_SBT_TextureSize,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_NV,ret->name,0);
  if(GL_FRAMEBUFFER_COMPLETE_EXT!=glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
      fprintf(stderr,"framebuffer problem\n");
						  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
				
  glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0,MM_SBT_TextureSize,MM_SBT_TextureSize);  
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_RECTANGLE_NV);  
  mmf_screen_set_perspective3d();
  glPointSize(MM_SBT_PointFactor);
  glColor4f(.75,.75,.75,1);
    glDisable(GL_LIGHTING);
  mmf_render_asiaanim_still(aad,0,1,0,1);
    mmf_light_on();
  glPopAttrib();
												
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
												  
  glEnable(GL_TEXTURE_RECTANGLE_NV);  
  mmf_set_stdtexparms();    
  glColor4f(1,1,1,1);

  ret->status=MM_TM_UPLOADED;
}


int mmf_render_asiaanim(struct mms_asiaanim * aa,GLdouble minx,GLdouble maxx,GLdouble miny,GLdouble maxy) {
  GLdouble ps;

  if(aa->texture->status==MM_TM_RENDERAAD) mmf_build_aad_texture(aa);

  if(aa->flags==MM_AD_PLAYFRAME) {
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    mmf_render_texture_quad(minx,maxx,miny,maxy,aa->texture,0,1,NULL);
    glDisable(GL_TEXTURE_RECTANGLE_NV);
  } else {
    glDisable(GL_TEXTURE_RECTANGLE_NV);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
      glVertex3f(minx,miny,0);
      glVertex3f(minx,maxy,0);
      glVertex3f(maxx,maxy,0);
      glVertex3f(maxx,miny,0);
    glEnd();
    glDisable(GL_TEXTURE_RECTANGLE_NV);
  }

  ps=(MM_ScreenWidthDisplay+MM_ScreenHeightDisplay)/2;
  ps*=(((maxx-minx)+(maxy-miny))/2);
  ps*=((GLdouble)MM_SBT_PointFactor/(GLdouble)MM_SBT_TextureSize);
  glPointSize(ps);

  glPushMatrix();
  
  switch(aa->flags) {
    case MM_AD_CLEAR:
      break;
    case MM_AD_STILL:
      glColor4f(0,0,0,1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
      mmf_render_asiaanim_still(aa,minx,maxx,miny,maxy);
        mmf_light_on();
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      break;
    case MM_AD_PLAYFRAME:
      glColor4f(aa->fg_color.red/65535.0,aa->fg_color.green/65535.0,aa->fg_color.blue/65535.0,1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
      mmf_render_asiaanim_playframe(aa,minx,maxx,miny,maxy);
        mmf_light_on();
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      if(aa->aa_currenttick+40>=aa->current_frame->ticks) {
        aa->flags=MM_AD_STILL;
      }
      break;
    case MM_AD_PLAYANIM:
      glColor4f(aa->fg_color.red/65535.0,aa->fg_color.green/65535.0,aa->fg_color.blue/65535.0,1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
      mmf_render_asiaanim_playanim(aa,minx,maxx,miny,maxy);
        mmf_light_on();
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      if(aa->aa_currenttick+40>=aa->aa_totalticks+1000) {
        aa->flags=MM_AD_STILL;
        aa->current_frame=aa->all_frames[aa->nf-1];
      }
      break;
    default:
      break;
  }

  glEnable(GL_TEXTURE_RECTANGLE_NV);
  glColor4f(1,1,1,1);

  glPopMatrix();

}

