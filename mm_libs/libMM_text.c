struct word {
  unsigned char *    w;
  GLuint    pxw;
  struct word *next;
};

struct line {
  GLuint    nw;
  GLuint    nc;
  GLuint    pxw;
  struct word *words;
  struct line *next;
};


void mmf_string_dump_structures(struct line * root) {
  struct line *tl;
  struct word *tw;
  
  tl=root;
  while(tl) {
    tw=tl->words;
    while(tw) {
      printf("\"%s\"[%d] ",tw->w,strlen(tw->w));
      tw=tw->next;
    }
    printf("\n");
    tl=tl->next;
  }
}

void mmf_string_free_structures(struct line * root) {
  struct line *tl,*ol;
  struct word *tw,*ow;
  
  tl=root;
  while(tl) {
    tw=tl->words;
    while(tw) {
      free(tw->w);
      ow=tw; tw=tw->next;
      free(ow);
    }
    ol=tl; tl=tl->next;
    free(ol);
  }
}

unsigned char * mmf_string_build_line(struct line * root) {
  struct word * w;
  int len,x;
  unsigned char * ret;
  w=root->words; len=0;
  while(w) {
    len+=strlen(w->w);
    if(w->next) len++;
    w=w->next;
  }

  ret=malloc(sizeof(unsigned char)*(len+1));
  ret[0]='\0';

  w=root->words; x=0;
  while(w) {
    strcat(ret,w->w);
    if(w->next) strcat(ret," ");
    w=w->next;
  }
  return(ret);
}

struct line * mmf_string_build_structures(unsigned char * text) {
  int x,s,xoff;
  struct line *lc,*ll,*root;
  struct word *wc,*wl;
  unsigned char *tc;
  struct line *tl;
  struct word *tw;
  
//  for(x=0;x<strlen(text);x++) printf("%2x ",text[x]);
//  printf("\n");
//  for(x=0;x<strlen(text);x++) printf("%c ",text[x]);
//  printf("\n");
  
  ll=lc=NULL; x=0;
  do {
    wc=wl=NULL; // neue zeile - words resetten
    if(lc) ll=lc; // nicht die erste zeile? dann vorhergehende retten
    lc=malloc(sizeof(struct line)); // neue zeile mallocen
    if(!lc) { fprintf(stderr,"out of memory 1"); return(NULL); } 
    lc->nw=0; lc->next=NULL; lc->words=NULL; // neue zeile - alles nullen
    if(ll) ll->next=lc; else root=lc; // gabs vorhergehende zeile? dann neue dort verlinken, sonst in root verlinken
    while(text[x]!='\n' && text[x]!='\0') { // solange zeile nicht zuende
//      if(text[x]!=' ') { // kommt was druckbares?
//      if(isgraph(text[x])) {
      if(text[x]>0x20) {
//        for(s=0;isgraph(text[x+s]);s++); // laenge des wortes zaehlen
        for(s=0;text[x+s]>0x20;s++); // laenge des wortes zaehlen
        xoff=s; // offset merken
        if(wc) wl=wc; // gabs schon ein wort in dieser zeile? dann altes in wl retten
        wc=malloc(sizeof(struct word)); // neue wort struktur
        if(!wc) { fprintf(stderr,"out of memory 2"); return(NULL); } 
        wc->w=NULL; wc->next=NULL; // im neuen wort alles resetten
        tc=malloc(sizeof(unsigned char *)*(s+1)); // raum fuer string mallocen
        if(!tc) { fprintf(stderr,"out of memory 3"); return(NULL); }
        wc->w=tc; // string verlinken
        if(wl) { wl->next=wc; lc->nw++; } else { lc->words=wc; lc->nw=1; } // gabs wort vorher? dann verlinken und zaehler++, sonst direkt in line einhaengen und zaehler auf 1
        tc[s]='\0'; // neuen string nullterminieren
//        do { s--; tc[s]=text[x+s]; } while(s); // string kopieren
        while(s) { s--; tc[s]=text[x+s]; } // string kopieren
        x+=xoff; // hauptzaehler weitersetzen
      } else {
        x++; // naechstes zeichen
      }
    }
  } while(text[x++]!='\0');

  tl=root;
  while(tl) {
    tw=tl->words; x=0;
    while(tw) {
      x+=strlen(tw->w); 
      if(tw->next) x++;
//      printf("\"%s\"[%d] ",tw->w,strlen(tw->w));
      tw=tw->next;
    }
//    printf(" (%d:%d)\n",tl->nw,tl->nc=x);
    tl=tl->next;
  }


  return(root);
}



struct mms_texture * mmf_build_text_plate(unsigned char * text, struct mms_font * font, SDL_Color * fg, SDL_Color * bg, GLdouble fs, GLdouble rfw, GLdouble bs, int align, int flags, int alpha, GLdouble bow) {
  SDL_Surface *surf,*surf2;
  SDL_Rect r;
  struct line * root;
  TTF_Font * ttf_font;
  struct line *tl;
  struct word *tw;
  unsigned char * ts;
  GLdouble maxw,maxh,yoff,xoff,xspace,lskip,font_size,border_width,requested_field_width,surfw,surfh;
  GLint tempw,blankw;
  struct mms_texture * temptexture;
  Uint32 bgcol,tcol;  
  GLdouble tr,tg,tb,cf,x;

//  fs*=2;

//  printf("alpha is %s\n",alpha?"set":"not set");

  bgcol=0x00000000|(bg->r<<16)|(bg->g<<8)|(bg->b<<0);
  font_size=fs*MM_ScreenHeightMax;
  border_width=bs*MM_ScreenHeightMax;
  requested_field_width=rfw*(GLdouble)MM_ScreenWidthMax;
  
  if(!text) {
    fprintf(stderr,"NULL char pointer passed\n");
    return(NULL);
  } else if (text[0]=='\0') {
    fprintf(stderr,"empty string passed\n");
    return(NULL);
  }
  
  ttf_font=TTF_OpenFont(font->path,font_size);
  if(!ttf_font) {
    fprintf(stderr,"cannot open font\n");
    return(NULL);
  }

  TTF_SetFontStyle(ttf_font, flags);

  root=mmf_string_build_structures(text);

  tl=root;
  while(tl) {
    tw=tl->words;
    while(tw) {
      TTF_SizeUTF8(ttf_font,tw->w,&tw->pxw,NULL);
      tw=tw->next;
    }
    tl=tl->next;
  }
  
  tl=root; maxw=0; maxh=0;
  while(tl) {
    ts=mmf_string_build_line(tl);
    TTF_SizeUTF8(ttf_font,ts,&tempw,NULL);
//    printf("\"%s\" (%d)\n",ts,tempw);
    if(tempw>maxw) maxw=tempw;
    maxh++;
    free(ts);
    tl=tl->next;
  }
  maxh*=(lskip=TTF_FontLineSkip(ttf_font));
  maxh+=2*border_width;
  maxw+=2*border_width;
//  printf("F %d %d\n",maxw,requested_field_width);
  if(requested_field_width>maxw) maxw=requested_field_width;
  
//  printf("maxw: %dx%d\n",maxw,maxh);

  surf2=SDL_CreateRGBSurface(SDL_SWSURFACE,maxw,maxh,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
  if(surf2==NULL) {
    fprintf(stderr,"Cannot create RGB surface.\n");
    return(NULL);
  }
  if(bow==0&&alpha!=0) {
    if(-1==SDL_FillRect(surf2,NULL,bgcol)) {
      fprintf(stderr,"Cannot fill rect.\n");
      return(NULL);
    }
  } else if(alpha==0) {
    for(x=0;x<maxh;x+=1) {
      cf=x/(GLdouble)maxh;
//      tr=((GLdouble)bg->r)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tr=tr>255?255:tr; tr=tr<0?0:tr;
//      tg=((GLdouble)bg->g)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tg=tg>255?255:tg; tg=tg<0?0:tg;
//      tb=((GLdouble)bg->b)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tb=tb>255?255:tb; tb=tb<0?0:tb;
      tr=((GLdouble)bg->r)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tr=tr>255?255:tr; tr=tr<0?0:tr;
      tg=((GLdouble)bg->g)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tg=tg>255?255:tg; tg=tg<0?0:tg;
      tb=((GLdouble)bg->b)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tb=tb>255?255:tb; tb=tb<0?0:tb;
//      printf("%3.3f %3.3f %3.3f %3.3f\n",cf,tr,tg,tb);
      r.x=0; r.w=maxw; r.y=x; r.h=1;
      tcol=(0xFF<<24)|(((GLubyte)tr)<<16)|(((GLubyte)tg)<<8)|(((GLubyte)tb)<<0);
      SDL_FillRect(surf2,&r,tcol);
    }
  }

  TTF_SizeUTF8(ttf_font," ",&blankw,NULL);    

  if(align==MM_BLOCK) {
    tl=root; yoff=border_width;
    TTF_SizeUTF8(ttf_font," ",&blankw,NULL);    
    while(tl) {
      xoff=border_width;
      if(tl->next) {
        if(tl->next->nw) {
          tw=tl->words; xspace=0;
          while(tw) { xspace+=tw->pxw; tw=tw->next; }
          xspace=(maxw-2*border_width-xspace)/(tl->nw-1);
        } else {
          xspace=blankw;
        }
      } else {
        xspace=blankw;
      }
      tw=tl->words;
      while(tw) {
        surf=TTF_RenderUTF8_Blended(ttf_font, tw->w, *fg);
        if(surf==NULL) { fprintf(stderr,"Cannot build texture: %s\n",ts); return(NULL); }
		if(alpha) {
		  SDL_SetAlpha(surf2,SDL_SRCALPHA,0);
		  SDL_SetAlpha(surf,0,0);
		}
        r.x=xoff;  r.y=yoff; r.w=surf->w; r.h=surf->h;
        if(-1==SDL_BlitSurface(surf,NULL,surf2,&r)) { fprintf(stderr,"Cannot blit surface\n"); return(NULL); }
        xoff+=surf->w; if(tw->next) xoff+=xspace;
        SDL_FreeSurface(surf);
        tw=tw->next;
      }
      yoff+=lskip;
      tl=tl->next;
    }  
  } else {
    tl=root; yoff=border_width;
    while(tl) {
      ts=mmf_string_build_line(tl);
      if(strlen(ts)) {
        surf=TTF_RenderUTF8_Blended(ttf_font, ts, *fg);
		if(alpha) {
		  SDL_SetAlpha(surf2,SDL_SRCALPHA,0);
		  SDL_SetAlpha(surf,0,0);
		}
        if(surf==NULL) { 
          fprintf(stderr,"Cannot build texture: %s\n",ts);
          return(NULL);
        }
        switch(align) {
          case MM_RIGHT: r.x=maxw-surf->w-border_width; break;
          case MM_CENTER: r.x=(maxw-surf->w)/2; break;
          case MM_LEFT:
          default: r.x=border_width; break;
        }
        r.y=yoff; r.w=surf->w; r.h=surf->h;
        if(-1==SDL_BlitSurface(surf,NULL,surf2,&r)) {
          fprintf(stderr,"Cannot blit surface\n");
          return(NULL);
        }
        SDL_FreeSurface(surf);
      }
      yoff+=lskip;
      tl=tl->next;
    }
  }

  temptexture=mmf_new_texture();
  if(!temptexture) {
    fprintf(stderr,"Cannot alloc texture structure.\n");
    return(NULL);
  }

  temptexture->name=0;
  temptexture->w=surf2->w;
  temptexture->h=surf2->h;
  temptexture->sbt_bw=NULL;
  temptexture->a=alpha?1:0;
  temptexture->surf=surf2;
  temptexture->status=MM_TM_SURFACE;

  mmf_string_free_structures(root);
  
  return(temptexture);
}

//------------------------------------------------------------------------------------------------


struct mms_texture *  mmf_build_text_button(struct mms_font * font, unsigned char * s, SDL_Color * fg, SDL_Color * bg, GLdouble f_button_w, GLdouble f_line_h, GLdouble f_border,int flags, GLdouble bow) {
  SDL_Surface *surf,*surf2,*surf3;
  SDL_Rect r;
  Uint32 bgcol,tcol;
  GLdouble x,cf,tr,tg,tb;
  struct mms_texture * temptexture;
  TTF_Font *this_font;
  GLdouble px_button_w,px_button_h;
  GLdouble px_text_w,px_text_h;
  GLdouble px_border;
  GLdouble temp;

  px_text_h=f_line_h*(GLdouble)MM_ScreenHeightMax;
  px_border=f_border*(GLdouble)MM_ScreenHeightMax;

  px_button_w=f_button_w*(GLdouble)MM_ScreenWidthMax;

  px_button_h=px_text_h+2*px_border;

  this_font=TTF_OpenFont(font->path,px_text_h);
  if(!this_font) {
    return(0);
  }
  
  TTF_SetFontStyle(this_font, flags);
  
  surf=TTF_RenderUTF8_Blended(this_font, s, *fg);
  if(surf==NULL) { 
    fprintf(stderr,"Cannot build texture: %s\n",s);
    return(NULL);
  }
  
  px_text_w=surf->w;
  px_text_h=surf->h;
  temp=surf->w+2*px_border;
  if(temp>px_button_w) px_button_w=temp;

  temptexture=mmf_new_texture();
  if(!temptexture) {
    fprintf(stderr,"Cannot alloc texture structure.\n");
    return(NULL);
  }

  bgcol=(0xFF<<24)|(bg->r<<16)|(bg->g<<8)|(bg->b<<0);

  surf2=SDL_CreateRGBSurface(0,px_button_w,px_button_h,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);

  if(surf2==NULL) {
    fprintf(stderr,"Cannot create RGB surface.\n");
    return(NULL);
  }
  if(bow==0) {
    if(-1==SDL_FillRect(surf2,NULL,bgcol)) {
      fprintf(stderr,"Cannot fill rect.\n");
      return(NULL);
    }
  } else {
    for(x=0;x<px_button_h;x+=1) {
      cf=x/(GLdouble)px_button_h;
//      tr=((GLdouble)bg->r)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tr=tr>255?255:tr; tr=tr<0?0:tr;
//      tg=((GLdouble)bg->g)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tg=tg>255?255:tg; tg=tg<0?0:tg;
//      tb=((GLdouble)bg->b)+(cf>.5?-1:1)*fabs(cf-.5)*100*bow; tb=tb>255?255:tb; tb=tb<0?0:tb;
      tr=((GLdouble)bg->r)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tr=tr>255?255:tr; tr=tr<0?0:tr;
      tg=((GLdouble)bg->g)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tg=tg>255?255:tg; tg=tg<0?0:tg;
      tb=((GLdouble)bg->b)*(1.0+(cf>.5?-1:1)*fabs(cf-.5)*.4*bow); tb=tb>255?255:tb; tb=tb<0?0:tb;
//      printf("%3.3f %3.3f %3.3f %3.3f\n",cf,tr,tg,tb);
      r.x=0; r.w=px_button_w; r.y=x; r.h=1;
      tcol=(0xFF<<24)|(((GLubyte)tr)<<16)|(((GLubyte)tg)<<8)|(((GLubyte)tb)<<0);
      SDL_FillRect(surf2,&r,tcol);
    }
  }
  r.x=(px_button_w-px_text_w)/2; r.y=(px_button_h-px_text_h)/2; r.w=px_text_w; r.h=px_text_h;
  if(-1==SDL_BlitSurface(surf,NULL,surf2,&r)) {
    fprintf(stderr,"Cannot blit surface\n");
    return(NULL);
  }

  temptexture->name=0;
  temptexture->w=surf2->w;
  temptexture->h=surf2->h;
  temptexture->sbt_bw=NULL;
  temptexture->a=0;
  temptexture->surf=surf2;
  temptexture->status=MM_TM_SURFACE;

  SDL_FreeSurface(surf);

  return(temptexture);
}

//------------------------------------------------------------------------------------------------

int mmf_font_init() {
    TTF_Init();
}



