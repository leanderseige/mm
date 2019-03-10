int mmf_set_stdtexparms() {
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

int mmf_set_simpletexparms() {
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void mmf_build_shadow_textures() {
  SDL_Surface *shadow;
  SDL_Rect r;

  mm_shadow_center.w=4;
  mm_shadow_center.h=4;
  mm_shadow_corner.w=4;
  mm_shadow_corner.h=4;
  mm_shadow_edge.w=4;
  mm_shadow_edge.h=4;
            
  glGenTextures(1,&(mm_shadow_center.name));
  glGenTextures(1,&(mm_shadow_corner.name));
  glGenTextures(1,&(mm_shadow_edge.name));
          
  shadow=SDL_CreateRGBSurface(0,4,4,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
                        
  SDL_FillRect(shadow,NULL,0x80000000);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_center.name);
  mmf_set_stdtexparms();
  glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,4,4,0,GL_BGRA,GL_UNSIGNED_BYTE,shadow->pixels);    
                            
  SDL_FillRect(shadow,NULL,0x00000000);
  r.x=0; r.y=0; r.w=2; r.h=2;
  SDL_FillRect(shadow,&r,0x80000000);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_corner.name);
  mmf_set_stdtexparms();
  glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,4,4,0,GL_BGRA,GL_UNSIGNED_BYTE,shadow->pixels);    
                                          
  SDL_FillRect(shadow,NULL,0x00000000);
  r.x=0; r.y=0; r.w=4; r.h=2;
  SDL_FillRect(shadow,&r,0x80000000);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV,mm_shadow_edge.name);
  mmf_set_stdtexparms();
  glTexImage2D(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,4,4,0,GL_BGRA,GL_UNSIGNED_BYTE,shadow->pixels);    
                                              
  SDL_FreeSurface(shadow);
}
                                                          

int mmf_screen_init() {
  unsigned char empty[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  int bpp=0,rgb_size[3],sx,sy,sf;
  const SDL_VideoInfo* info = NULL;
  SDL_Surface *window;

  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0) {
    fprintf(stderr,"SDL ERROR Video initialization failed: %s\n", SDL_GetError() );
    return(1); 
  }

  info = SDL_GetVideoInfo();

  if( !info ) {
    fprintf(stderr,"SDL ERROR Video query failed: %s\n", SDL_GetError() );
    return(1);
  }

  bpp = info->vfmt->BitsPerPixel;
  switch (bpp) {
    case 8:
      rgb_size[0]=2;rgb_size[1]=3;rgb_size[2]=3;break;
    case 15:
    case 16:
      rgb_size[0]=5;rgb_size[1]=5;rgb_size[2]=5;break;
    default:
      rgb_size[0]=8;rgb_size[1]=8;rgb_size[2]=8;break;
  }
    
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE,  rgb_size[0] );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE,rgb_size[1] );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, rgb_size[2] );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,1 );    

  if(MM_SDL_FullScreen) {
	sx=MM_ScreenWidthMax;
	sy=MM_ScreenHeightMax;
	MM_ScreenWidthDisplay=MM_ScreenWidthMax;
	MM_ScreenHeightDisplay=MM_ScreenHeightMax;
	sf=SDL_OPENGL|SDL_FULLSCREEN;
	MM_GTKIDLER_HIDE_WINDOW=1;
  } else {
	sx=MM_ScreenWidthEditor;
	sy=MM_ScreenHeightEditor;
	MM_ScreenWidthDisplay=MM_ScreenWidthEditor;
	MM_ScreenHeightDisplay=MM_ScreenHeightEditor;
	sf=SDL_OPENGL;
	MM_GTKIDLER_SHOW_WINDOW=1;
  }
  if( (window=SDL_SetVideoMode( sx, sy, bpp, sf)) == 0 ) {
    fprintf(stderr,"SDL ERROR Video mode set failed: %s\n", SDL_GetError() );
    return(1);
  }

  MM_ScreenFlags=sf;
  MM_ScreenBPP=bpp;
  MM_ScreenAspect=((GLdouble)sx)/((GLdouble)sy);
  
    printf("screen a\n");
    MM_CursorEmpty=SDL_CreateCursor(empty,empty,8,8,0,0);
    printf("screen b\n");
    MM_CursorDefault=SDL_GetCursor();
    printf("screen c\n");
  
  
  if(MM_SDL_FullScreen) {
    SDL_SetCursor(MM_CursorEmpty);
  } else {
    SDL_SetCursor(MM_CursorDefault);
  }
  
  glClearColor(0.0,0.0,0.0,0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
    
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST); 
  glEnable(GL_POINT_SMOOTH);   
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); 

  SDL_EnableUNICODE(1);

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);

  mmf_build_shadow_textures();
  
  MM_SDL_Window=window;

  return(0);
}

int mmf_screen_set_ortho2d() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();       
  gluOrtho2D(0,1.0,1.0,0);
}

int mmf_screen_set_perspective3d() {
  MM_CameraDistance=10;
  MM_CameraAngle=(180.0*(2.0*atan2(.5,MM_CameraDistance))/PI);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();       
  gluPerspective(MM_CameraAngle,1,1,100);
  gluLookAt(0.5,0.5,-MM_CameraDistance, 0.5,0.5,0, 0,-1,0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST); 
  glEnable(GL_POINT_SMOOTH);   
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); 
}

int mmf_light_on() {
  glLightfv(GL_LIGHT0, GL_AMBIENT,  MM_LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  MM_LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, MM_LightSpecular);
  glLightfv(GL_LIGHT0, GL_POSITION, MM_LightPosition);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, MM_LightDirection);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
}

int mmf_light_off() {
  glDisable(GL_LIGHTING);
}

int mmf_screen_set_viewport() {
  glViewport(0,0,MM_ScreenWidthDisplay,MM_ScreenHeightDisplay);             
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

//  glEnable (GL_MULTISAMPLE_ARB);
//  glHint (GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}


