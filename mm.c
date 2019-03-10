#include "mm_libs/libMM_main.c"

int main(int argc, char ** argv) {
  GLuint now, next;
  GLuint set_fs,x;
  char * filename;
  
  set_fs=0;
  filename=NULL;
  
  for(x=1;x<argc;x++) {
	if(argv[x][0]=='-') {
	  if(strlen(argv[x])==3) {
		if(strncmp(argv[x],"-fs",3)==0) {
		  set_fs=1;
		}
	  }
	} else {
	  filename=argv[x];
	}
	
  }
  
  if(filename==NULL) {
	printf("Kein Dateiname angegeben!\n");
	exit(1);
  }
  
  mmf_reset_all(set_fs);  

  mmf_init_sound();
  mmf_font_init();

  mmef_load(filename);
//  printf("%d %d %d %d\n",MM_ScreenWidthMax,MM_ScreenHeightMax,MM_ScreenWidthEditor,MM_ScreenHeightEditor);

  mmef_launch_editor();
  mmf_screen_init();

  next=SDL_GetTicks()+40; 
  while(1) {
    mmf_main();
    now=SDL_GetTicks();
    if(now<next) SDL_Delay(next-now);
    next+=40;
  }
  
}

