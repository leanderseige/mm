#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>
#include <SDL_rotozoom.h>
//#include <SDL_Pango.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gtk/gtk.h>
#include <libdv/dv.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <matheval.h>
#include <locale.h>

#include "libMM_defs.c"
#include "libMM_gtk_defs.c"
#include "libMM_tools.c"
#include "libMM_render.c"
#include "libMM_sbt.c"
#include "libMM_dv.c"
#include "libMM_image.c"
#include "libMM_screen.c"
#include "libMM_sound.c"
#include "libMM_text.c"
#include "libMM_removers.c"
#include "libMM_gui.c"
#include "libMM_cloners.c"
#include "libMM_gtk_globals.c"
#include "libMM_gtk_text_request.c"
#include "libMM_gtk_new_text_button.c"
#include "libMM_gtk_new_text.c"
#include "libMM_gtk_animation_editor.c"
#include "libMM_save.c"
#include "libMM_load.c"
#include "libMM_gtk_popups_pages.c"
#include "libMM_gtk_popups_resources.c"
#include "libMM_gtk_popups_items.c"
#include "libMM_gtk_build_trees.c"
#include "libMM_actions.c"
#include "libMM_gtk_tree_rebuilders.c"
#include "libMM_gtk_actions.c"
#include "libMM_gtk_idler.c"
#include "libMM_gtk_new_font.c"
#include "libMM_gtk_new_sound.c"
#include "libMM_gtk_new_video.c"
#include "libMM_gtk_new_image.c"
#include "libMM_gtk_new_image_button.c"
#include "libMM_gtk_new_asiaanim.c"
#include "libMM_gtk_main_buttons.c"
#include "libMM_gtk_page_editor.c"
#include "libMM_gtk_item_editor.c"
#include "libMM_gtk_sbt_editor.c"
#include "libMM_gtk_main.c"

void mmf_reset_all(int set_fs) {
#ifdef PATH_MAX
  MM_PATHMAX = PATH_MAX;
#else
  MM_PATHMAX = pathconf (path, _PC_PATH_MAX);
  if (MM_PATHMAX <= 0)
    MM_PATHMAX = 4096;
#endif


  MM_PathBuf=malloc(sizeof(char)*MM_PATHMAX);
  if(!MM_PathBuf) {
    fprintf(stderr,"Cannot allocate path buffer memory (%d Bytes)\n",sizeof(char)*MM_PATHMAX);
    exit(1);
  }

  if(set_fs)
	MM_SDL_FullScreen=1;
  else
	MM_SDL_FullScreen=0;

  MM_CurrentPage=NULL;
  MM_Pages=NULL;
  MM_Textures=NULL;
  
  MM_RendererLock=SDL_CreateMutex();
  MM_SBTButtonLock=SDL_CreateMutex();
  MM_SoundLock=SDL_CreateMutex();
  MM_VideoLock=SDL_CreateMutex();
  MM_VideoReadyMutex=SDL_CreateMutex();
  MM_VideoReadyCond=SDL_CreateCond();
  
  MM_Run_Mode=MM_MODE_PLAYER;
  MM_GridX=33;
  MM_GridY=33;
  MM_EditorFlash=NULL;
  MM_FrameCounter=0;
  
  MM_SBT_Running=0;
  MM_SBT_Display=NULL;
  MM_SBT_ButtonAction=MM_SBT_ACT_NONE;
  MM_CurrentSBTItem=NULL;
  
  MM_DEF_GridX=33;
  MM_DEF_GridY=33;
  MM_ModuloN=1;
  MM_ModuloM=3;
  MM_DEF_TextLineHeight=.04;
  MM_DEF_TextItemWidth=.0;
  MM_DEF_TextBorderWidth=.025;
  
  gdk_color_parse("yellow",&MM_DEF_FGL);
  gdk_color_parse("darkblue",&MM_DEF_BGL);
  gdk_color_parse("white",&MM_DEF_FGH);
  gdk_color_parse("blue",&MM_DEF_BGH);

  gdk_color_parse("black",&MM_DEF_PBG);

  gdk_color_parse("black",&MM_DEF_AFG);
  gdk_color_parse("white",&MM_DEF_ABG);
  
  MM_GTKIDLER_REBUILD_ITEMS=0;
  MM_GTKIDLER_REBUILD_PAGES=0;
  MM_GTKIDLER_REBUILD_RESOURCES=0;
  MM_GTKIDLER_HIDE_WINDOW=0;
  MM_GTKIDLER_SHOW_WINDOW=0;
  MM_GTKIDLER_FOCUS_WIDGET=NULL;
  srand(SDL_GetTicks());
}


void mmf_quit() {
  printf("-q1\n");
  if(MM_SBT_Display) {
  printf("-q2\n");
    if(MM_SBT_Display->th_vibe) {
      MM_SBT_Display->th_vibe_exit=1;
      while(MM_SBT_Display->th_vibe_exit!=2) SDL_Delay(100);
    }
  printf("-q3\n");
    if(MM_SBT_Display->th_proc) {
      MM_SBT_Display->th_proc_exit=1;
      while(MM_SBT_Display->th_proc_exit!=2) SDL_Delay(100);
    }
  }
  printf("-q4\n");
}


void mmf_close_all() {
  TTF_Quit();
  SDL_Quit();
}

int mmf_get_page_id(GLuint requested) {
  struct mms_page * runner;
  int find;
  
  find=1;
//  if(requested==0) requested=1;
  
  if(runner=MM_Pages) {
      while(runner) {
        if(runner->id==requested) {
          while(mmf_get_page_by_id(find)) find++;
          return(find);
        }
        runner=runner->next;
      }
      return(requested);
  } else {
    return(requested);
  }
}


int mmf_get_resource_id(GLuint requested) {
  struct mms_resource * runner;
  int find;
  
  find=1;
  
  if(runner=MM_Resources) {
      while(runner) {
        if(runner->id==requested) {
	  printf(" WARNING: duplicate id(%d) found: %s\n",requested,runner->text); 
          while(mmf_get_resource_by_id(find)) find++;
          return(find);
        }
        runner=runner->next;
      }
      return(requested);
  } else {
    return(requested);
  }
}


int mmf_get_item_id(GLuint requested,struct mms_page * page) {
  struct mms_item * runner;
  int find;
  
  find=1;
  
  if(runner=page->items) {
      while(runner) {
        if(runner->id==requested) {
          while(mmf_get_item_by_id(page,find)) find++;
          return(find);
        }
        runner=runner->next;
      }
      return(requested);
  } else {
    return(requested);
  }
}


int mmf_main_events_editor() {
  SDL_Event event;
  struct mms_item * runner;
  GLdouble dx,dy;

  dy=(((GLdouble)MM_Edit_Item->maxy)-((GLdouble)MM_Edit_Item->miny))/2;
  dx=(((GLdouble)MM_Edit_Item->maxx)-((GLdouble)MM_Edit_Item->minx))/2;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        exit(1);
        break;
      case SDL_MOUSEMOTION:
        if(event.motion.state&SDL_BUTTON(1)) {
          if(MM_Grid_Snap) {
            MM_Edit_Item->minx=(floor(event.motion.x/floor(MM_ScreenWidthDisplay *MM_GridSizeX))*(MM_ScreenWidthDisplay *MM_GridSizeX))/MM_ScreenWidthDisplay;
            MM_Edit_Item->miny=(floor(event.motion.y/floor(MM_ScreenHeightDisplay*MM_GridSizeY))*(MM_ScreenHeightDisplay*MM_GridSizeY))/MM_ScreenHeightDisplay;
            MM_Edit_Item->maxx=MM_Edit_Item->minx+2*dx;
            MM_Edit_Item->maxy=MM_Edit_Item->miny+2*dy;
            MM_Editor_Manual_Update=1;
            mmef_item_editor_get();
            MM_Editor_Manual_Update=0;
          } else {
            MM_Edit_Item->minx+=((GLdouble)event.motion.xrel)/MM_ScreenWidthDisplay;
            MM_Edit_Item->maxx+=((GLdouble)event.motion.xrel)/MM_ScreenWidthDisplay;
            MM_Edit_Item->miny+=((GLdouble)event.motion.yrel)/MM_ScreenHeightDisplay;
            MM_Edit_Item->maxy+=((GLdouble)event.motion.yrel)/MM_ScreenHeightDisplay;
            MM_Editor_Manual_Update=1;
            mmef_item_editor_get();
            MM_Editor_Manual_Update=0;
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        MM_Edit_Item->minx=((GLdouble)event.motion.x)/MM_ScreenWidthDisplay-dx;
        MM_Edit_Item->maxx=((GLdouble)event.motion.x)/MM_ScreenWidthDisplay+dx;
        MM_Edit_Item->miny=((GLdouble)event.motion.y)/MM_ScreenHeightDisplay-dy;
        MM_Edit_Item->maxy=((GLdouble)event.motion.y)/MM_ScreenHeightDisplay+dy;
        MM_Editor_Manual_Update=1;
        mmef_item_editor_get();
        MM_Editor_Manual_Update=0;
        break;
      case SDL_MOUSEBUTTONUP:
        break;
      default:
        break;
    }
  }
}


struct mms_item * mmf_get_clicked_item(GLuint x, GLuint y) {
  struct mms_item * runner;
  
  runner=MM_CurrentPage->items;
  while(runner) {
    if(   (runner->minx*MM_ScreenWidthDisplay)<x
        &&(runner->maxx*MM_ScreenWidthDisplay)>x
        &&(runner->miny*MM_ScreenHeightDisplay)<y
        &&(runner->maxy*MM_ScreenHeightDisplay)>y
        &&!(runner->is_background)
    ) return(runner);
    runner=runner->next;
  }
  return(NULL);
}

int mmf_call_action(GLuint action, GLuint avalue) {
  struct mms_item * temp_item;

  switch(action) {
    case MM_JUMP_TO_PAGE:
      mmf_action_jump_to_page(avalue);
      return(1);
      break;
    case MM_AAD_PLAY_FRAME:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        ((struct mms_asiaanim *)temp_item->data)->flags=MM_AD_PLAYFRAME;
        ((struct mms_asiaanim *)temp_item->data)->aa_currenttick=0;
        ((struct mms_asiaanim *)temp_item->data)->aa_oldtick=SDL_GetTicks();
        ((struct mms_asiaanim *)temp_item->data)->current_frame=((struct mms_asiaanim *)temp_item->data)->all_frames[((struct mms_asiaanim *)temp_item->data)->nf-1];   
      }
      return(1);
      break;
    case MM_AAD_PLAY_ANIMATION:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        ((struct mms_asiaanim *)temp_item->data)->flags=MM_AD_PLAYANIM;
        ((struct mms_asiaanim *)temp_item->data)->aa_currenttick=0;
        ((struct mms_asiaanim *)temp_item->data)->aa_oldtick=SDL_GetTicks();
        ((struct mms_asiaanim *)temp_item->data)->current_frame=((struct mms_asiaanim *)temp_item->data)->root;
      }
      return(1);
      break;
    case MM_AAD_SHOW_STILL:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        ((struct mms_asiaanim *)temp_item->data)->flags=MM_AD_STILL;
      }
      return(1);
      break;
    case MM_AAD_CLEAR:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        ((struct mms_asiaanim *)temp_item->data)->flags=MM_AD_CLEAR;
      }
      return(1);
      break;
    case MM_SOUND_PLAY:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        SDL_LockMutex(MM_SoundLock);
        ((struct mms_sound *)temp_item->data)->is_playing=1;
        ((struct mms_sound *)temp_item->data)->play_pos=((struct mms_sound *)temp_item->data)->buffer;
        ((struct mms_sound *)temp_item->data)->play_remain=((struct mms_sound *)temp_item->data)->len;
        SDL_UnlockMutex(MM_SoundLock);
      }
      break;
    case MM_SOUND_STOP:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        SDL_LockMutex(MM_SoundLock);
        ((struct mms_sound *)temp_item->data)->is_playing=0;
        // ((struct mms_sound *)temp_item->data)->play_pos=((struct mms_sound *)temp_item->data)->buffer;
        // ((struct mms_sound *)temp_item->data)->play_remain=((struct mms_sound *)temp_item->data)->len;
        SDL_UnlockMutex(MM_SoundLock);
      }
      break;
    case MM_VIDEO_STOP:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        SDL_LockMutex(MM_VideoLock);
        ((struct mms_video *)temp_item->data)->is_playing=MM_VIDEO_PLEASE_STOP;
        ((struct mms_video *)temp_item->data)->fidx=0;
        SDL_UnlockMutex(MM_VideoLock);
      }
      break;
    case MM_VIDEO_PLAY:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        SDL_LockMutex(MM_VideoLock);
		if(((struct mms_video *)temp_item->data)->is_playing!=MM_VIDEO_PLAYING) {
	  	  ((struct mms_video *)temp_item->data)->is_playing=MM_VIDEO_PLAYING;
		}
        SDL_UnlockMutex(MM_VideoLock);
      }
      break;
    case MM_VIDEO_PAUSE:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        SDL_LockMutex(MM_VideoLock);
      	((struct mms_video *)temp_item->data)->is_playing=MM_VIDEO_STOPPED;
        SDL_UnlockMutex(MM_VideoLock);
      }
     break;            
    case MM_SET_SBT:
      if(temp_item=mmf_get_item_by_id(MM_CurrentPage,avalue)) {
        temp_item->is_sbt=1;
        mmf_sbt_set_current(temp_item);
      }      
      break;
    default:
      return(0);
      break;
  }
}

int mmf_main_events_player() {
  SDL_Event event;
  struct mms_item * temp_item;
  int action_done,item_caught;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        mmf_quit();
        exit(1);
        break;
      case SDL_MOUSEMOTION:
        break;
      case SDL_MOUSEBUTTONDOWN:
        temp_item=mmf_get_clicked_item(event.button.x,event.button.y);
        if(event.button.button==SDL_BUTTON_LEFT) {
          if(temp_item) {
            temp_item->animakt.timescale=1;
            temp_item->animakt.starttime=((GLdouble)SDL_GetTicks())/1000.0;
            temp_item->animakt.random=((GLdouble)rand())/((GLdouble)RAND_MAX);
            mmf_call_action(temp_item->action,temp_item->avalue);
          } else {
            mmf_call_action(MM_CurrentPage->action,MM_CurrentPage->avalue);
          }
        } else if(event.button.button==SDL_BUTTON_RIGHT) {
          if(temp_item) {
            MM_RROW_FocusResource=mmf_get_void_resource(temp_item->data);
            MM_IROW_FocusItem=temp_item;
            MM_GTKIDLER_REBUILD_ITEMS=1;
            MM_GTKIDLER_REBUILD_RESOURCES=1;
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        break;
	  case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		  case SDLK_f:
			if(MM_SDL_FullScreen) {
			  MM_SDL_Window = SDL_SetVideoMode(MM_ScreenWidthEditor, MM_ScreenHeightEditor, MM_ScreenBPP, SDL_OPENGL);
              SDL_SetCursor(MM_CursorDefault);
			  MM_ScreenWidthDisplay=MM_ScreenWidthEditor;
			  MM_ScreenHeightDisplay=MM_ScreenHeightEditor;
			  MM_SDL_FullScreen=0;
			  MM_GTKIDLER_SHOW_WINDOW=1;
			} else {
			  MM_SDL_Window = SDL_SetVideoMode(MM_ScreenWidthMax, MM_ScreenHeightMax, MM_ScreenBPP, SDL_FULLSCREEN|SDL_OPENGL);
              SDL_SetCursor(MM_CursorEmpty);
			  MM_ScreenWidthDisplay=MM_ScreenWidthMax;
			  MM_ScreenHeightDisplay=MM_ScreenHeightMax;
			  MM_SDL_FullScreen=1;
			  MM_GTKIDLER_HIDE_WINDOW=1;
			}
			break;
		  case SDLK_c:
			if(MM_SDL_FullScreen) {
			  if(MM_CursorDefault==SDL_GetCursor()) {
                SDL_SetCursor(MM_CursorEmpty);
			  }
			  else {
                SDL_SetCursor(MM_CursorDefault);
			  }
			} 
			break;
		  case SDLK_ESCAPE:
			mmf_quit();
      		exit(1);
			break;
		  default:
			break;
		}
		break;
      default:
        break;
    }
  }
}

void mmf_quick_texture_check() {
  struct mms_texture * trun;

  for(trun=MM_Textures;trun;trun=trun->next) {
	if(trun->status==MM_TM_GETTEXIMAGE||trun->status==MM_TM_VIDEOUPDATE) {
      mmf_texture_proc(trun);
    } 
  }
}


int mmf_main () {
  struct mms_item * runner;
  struct mms_texture * temptexture;
  struct mms_texture * trun;
  GLuint t;
  GLenum error;
  GLdouble cgrid,ftmp;
  GLdouble minx,maxx,miny,maxy,t1;
  GdkScreen * defscreen;
  GtkSettings * defsets;
  
//  defscreen=gdk_screen_get_default();
//  defsets=gtk_settings_get_for_screen(defscreen);
//  gtk_settings_set_string_property(defsets,"gtk-font-name","arial unicode 10","XProperty");
//  gtk_settings_set_string_property(defsets,"gtk-font-name","arial unicode 8","XProperty");
  
  SDL_LockMutex(MM_RendererLock);

  if(MM_CurrentPage!=NULL) {
    mmf_screen_set_viewport();
    glClearColor(((GLdouble)MM_CurrentPage->bg_color.red)/65535.0,((GLdouble)MM_CurrentPage->bg_color.green)/65535.0,((GLdouble)MM_CurrentPage->bg_color.blue)/65535.0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_RECTANGLE_NV);
    mmf_render_page_background();
    mmf_screen_set_perspective3d();
//    glScalef(200,150,1);
    mmf_light_on();
    glClear(GL_DEPTH_BUFFER_BIT);
    if(MM_EditorFlash) MM_EditorFlash();

    glDisable(GL_DEPTH_TEST);
    runner=MM_CurrentPage->items;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    while(runner) {
      if(runner->drop_shadow) {
        mmf_render_shadow_rect(runner->minx,runner->maxx,runner->miny,runner->maxy,&(runner->animakt));
      }
      runner=runner->next;
    }
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    runner=MM_CurrentPage->items;
    while(runner) {
      if(runner->light) mmf_light_on(); else mmf_light_off();
      if(runner->animakt.timescale>0) {
        ftmp=(((GLdouble)SDL_GetTicks())/1000)-runner->animakt.starttime;
        if(ftmp>=runner->animakt.playtime) {
          runner->animakt.timescale=0;
        } else {
          runner->animakt.timescale=1.0-(ftmp/runner->animakt.playtime);
        }
      }
      if(!runner->is_background)
      switch(runner->type) {
        case MM_TEXT_BUTTON:
        case MM_IMAGE_BUTTON:
          mmf_render_texture_quad(
            runner->minx, runner->maxx,
            runner->miny, runner->maxy,
            ((struct mms_button *)(runner->data))->texture_low,
            0,0,&(runner->animakt)
          );
          break;
        case MM_TEXT:
          mmf_render_texture_quad(
            runner->minx, runner->maxx,
            runner->miny, runner->maxy,
            ((struct mms_text *)(runner->data))->texture,
            0,0,&(runner->animakt)
          );
          break;
        case MM_IMAGE:
          mmf_render_texture_quad(
            runner->minx, runner->maxx,
            runner->miny, runner->maxy,
            ((struct mms_image *)(runner->data))->texture,
            0,0,&(runner->animakt)
          );
          break;
        case MM_VIDEO:
          mmf_render_texture_quad(
            runner->minx, runner->maxx,
            runner->miny, runner->maxy,
            ((struct mms_video *)(runner->data))->texture,
            .05,0,&(runner->animakt)
          );
          break;
        case MM_ASIAANIM:
          mmf_render_asiaanim((struct mms_asiaanim *)runner->data,runner->minx,runner->maxx,runner->miny,runner->maxy);
          break;
        default:
          break;
      }
      if(runner->is_sbt) {
		if(MM_SBT_Display->taxel==NULL) {
		  if(MM_SBT_Display->texture!=NULL) {
			printf("TM: %d\n",MM_SBT_Display->texture->status);
			if(MM_SBT_Display->texture->status==MM_TM_UPLOADED) {
			  mmf_sbt_texture2taxel(MM_SBT_Display,MM_SBT_Display->texture);
			}
		  }
		}
		if(MM_SBT_Running) {
      	  mmf_render_sbt_area(runner);
		}
      }
      runner=runner->next;
    }

    if(MM_Run_Mode==MM_MODE_EDITOR) {

      glDisable(GL_TEXTURE_RECTANGLE_NV);
      t1=(sin((PI*2*.001*(double)(SDL_GetTicks()%1000))))/2+.5;
      glColor4f(t1,t1,t1,1);
      glLineWidth(1);
      glPushMatrix();
      minx=MM_Edit_Item->minx;
      maxx=MM_Edit_Item->maxx;
      miny=MM_Edit_Item->miny;
      maxy=MM_Edit_Item->maxy;
      glBegin(GL_LINE_LOOP);
        glVertex3f(minx+.01,miny+.01,-.025);
        glVertex3f(maxx-.01,miny+.01,-.025);
        glVertex3f(maxx-.01,maxy-.01,-.025);
        glVertex3f(minx+.01,maxy-.01,-.025);
      glEnd();
      glPopMatrix();
      glColor4f(1,1,1,1);

      if(MM_Grid_Snap) {
        glPushMatrix();
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glDisable(GL_LINE_SMOOTH);
        glEnable(GL_LINE_STIPPLE);
        glColor4f(1,1,1,1);
        glLineWidth(.5);
        glLineStipple(1,0xAAAA);
        glBegin(GL_LINES);
          if(MM_GridSizeX) for(cgrid=MM_GridSizeX;cgrid<1;cgrid+=MM_GridSizeX) {
            glVertex2f(cgrid,1);
            glVertex2f(cgrid,0);
          }
          if(MM_GridSizeX) for(cgrid=MM_GridSizeY;cgrid<1;cgrid+=MM_GridSizeY) {
            glVertex2f(1,cgrid);
            glVertex2f(0,cgrid);
          }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glEnable(GL_LINE_SMOOTH);
        glPopMatrix();
        glColor4f(1,1,1,1);
      }    
      if(MM_Item_Snap) {
        glPushMatrix();
        glDisable(GL_TEXTURE_RECTANGLE_NV);
        glDisable(GL_LINE_SMOOTH);
        glEnable(GL_LINE_STIPPLE);
        glColor4f(1,1,1,1);
        glLineWidth(.5);
        glLineStipple(1,0xAAAA);
        glBegin(GL_LINES);
        runner=MM_CurrentPage->items;
        while(runner) {
          if(runner->type!=MM_SOUND) {
            glVertex2f(runner->minx,1); glVertex2f(runner->minx,0);
            glVertex2f(runner->maxx,1); glVertex2f(runner->maxx,0);
            glVertex2f(1,runner->miny); glVertex2f(0,runner->miny);
            glVertex2f(1,runner->maxy); glVertex2f(0,runner->maxy);
          }
          runner=runner->next;        
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glEnable(GL_LINE_SMOOTH);
        glPopMatrix();
        glColor4f(1,1,1,1);
      }
    }
    SDL_GL_SwapBuffers();
    MM_FrameCounter++;

    switch(MM_Run_Mode) {
      case MM_MODE_EDITOR:
        mmf_main_events_editor();
        break;
      case MM_MODE_PLAYER:
        mmf_main_events_player();
        break;
      default:
        break;
    }
    
	SDL_LockMutex(MM_SBTButtonLock);
    if(MM_SBT_ButtonAction!=MM_SBT_ACT_NONE) {
  	  if(MM_SBT_Running && MM_CurrentSBTItem!=NULL) {
    	switch(MM_SBT_ButtonAction) {
      	  case MM_SBT_ACT_L:
      	  case MM_SBT_ACT_M:
      	  case MM_SBT_ACT_R:
      	  case MM_SBT_ACT_E:
      	  case MM_SBT_ACT_D:
        	mmf_call_action(
          	  MM_CurrentSBTItem->sbtact[MM_SBT_ButtonAction],
          	  MM_CurrentSBTItem->sbtava[MM_SBT_ButtonAction]
        	);
        	MM_SBT_ButtonAction=MM_SBT_ACT_NONE;
        	break;
      	  default:
        	break;
    	}
	  } else {
		mmf_action_jump_to_page(1); // FIXME jump to defined/loaded/saved startpage
	  }
    }
	SDL_UnlockMutex(MM_SBTButtonLock);

    mmf_quick_texture_check();

	for(trun=MM_Textures;trun;trun=trun->next) {
	  if(trun->status==MM_TM_REMOVE) {
		mmf_delete_texture(trun);
	  }
	}

    mmf_gl_errors();
  } 
  
  SDL_UnlockMutex(MM_RendererLock);
    
  return(0);
}
