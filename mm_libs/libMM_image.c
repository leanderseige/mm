struct mms_texture * mmf_load_image( unsigned char * fn, int alpha) {
  struct mms_texture * temptexture;
  SDL_Surface *surf,*surf2;
  double fw,fh,f;

  surf=IMG_Load(fn);
  if(surf==NULL) { 
    fprintf(stderr,"cannot load texture: %s\n",fn); 
    exit(1); 
  }
  
    fw=fh=f=1; // if we scale factor will be lower than 1
    if(surf->w>2048) {
	fw=2040.0/((double)surf->w);
    }
    if(surf->h>2048) {
	fh=2040.0/((double)surf->h);
    }  
    if(fw<fh) f=fw;
    if(fw>fh) f=fh;
    if(f<1) {
    printf("scale:%f\n",f);
	surf2=zoomSurface(surf,f,f,SMOOTHING_ON);
	surf=surf2; // schwapp schwapp
    }
  
//  surf2=SDL_CreateRGBSurface(0,surf->w,surf->h,32,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
  surf2=SDL_CreateRGBSurface(0,surf->w,surf->h,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
  if(surf2==NULL) {
    fprintf(stderr,"Cannot create RGB surface.\n");
    SDL_FreeSurface(surf);
    return(NULL);
  }
  if(-1==SDL_BlitSurface(surf,NULL,surf2,NULL)) {
    fprintf(stderr,"Cannot blit surface\n");
    return(NULL);
  }
  temptexture=mmf_new_texture();
  if(!temptexture) {
    fprintf(stderr,"Cannot alloc texture structure.\n");
    return(NULL);
  }

  temptexture->w=surf2->w;
  temptexture->h=surf2->h;
  temptexture->a=0;
  temptexture->sbt_bw=NULL;
  temptexture->surf=surf2;
  temptexture->status=MM_TM_SURFACE;
/*
  if(alpha)
    glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,surf2->w,surf2->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surf2->pixels);
  else
    glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGB,surf2->w,surf2->h,0,GL_BGR,GL_UNSIGNED_BYTE,surf2->pixels);
*/
  SDL_FreeSurface(surf);

  return(temptexture);
}
