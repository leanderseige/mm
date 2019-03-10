/************************************************************************************
    libsbt - a library to control SeeByTouch braille displays using SDL and OpenGL
    (c)2007 by Leander Seige, www.seige.net
    
    see http://see-by-touch.sf.net for informations about SeeByTouch
    SeeByTouch is a project of Klaus Rieger
    
 ************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_image.h>



#define SBT_TEXT_LOCK 0x01



// taxel coordinates in mm
const double SBT_TAXELCOOX[] = {
    2.45*1,
    2.45*0,
    2.45*1,
    2.45*1,
    2.45*1,
    2.45*0,
    2.45*0,
    2.45*0
};

const double SBT_TAXELCOOY[] = {
    2.45*3,
    2.45*3,
    2.45*2,
    2.45*1,
    2.45*0,
    2.45*2,
    2.45*1,
    2.45*0
};




// helper function for error messages
int mmf_sbt_error_msg (char * string) {
    fprintf(stderr,"libsbt error: %s\n",string);
    return 0;
}



//function loading ascii tables of brltty
int mmf_sbt_load_chars(struct mms_sbt_display * sbt_display, char * filename) {
    FILE * f;
    char c1,c2,s1[9],in[512],out;
    unsigned int x1;
    
    s1[8]=0x00;
    
    f=fopen(filename,"r");
    if(f==NULL) {
        mmf_sbt_error_msg("Cannot load character table file.");
        return(1);
    }
    
    while(fgets(in,512,f)==in) {
        sscanf(in,"%1c%1c%x (%8c)",&c1,&c2,&x1,s1);
        if(c1=='\\'&&c2=='X') {
            if(x1<0||x1>255) {
                fclose(f);
                mmf_sbt_error_msg("Error loading character table.\n");
                sbt_display->chars[0]=0x00;
                return(1);
            }
            out=0x00;
            if(s1[0]=='1') out|=0x80;
            if(s1[1]=='2') out|=0x40;
            if(s1[2]=='3') out|=0x20;
            if(s1[3]=='4') out|=0x10;
            if(s1[4]=='5') out|=0x08;
            if(s1[5]=='6') out|=0x04;
            if(s1[6]=='7') out|=0x02;
            if(s1[7]=='8') out|=0x01;
            sbt_display->chars[x1]=out;
//            printf("read: %3d -> \"%s\" -> %x\n",x1,s1,out);
        }
    }
    
    fclose(f);
    return(0);
}



// write data to the braille display
int mmf_sbt_write_data(struct mms_sbt_display * sbt_display) {
    int retval;
    retval=write(sbt_display->dev_file,sbt_display->bd,sizeof(unsigned char)*sbt_display->bn);
    if(retval!=sbt_display->bn) return 1;
    return(0);
}



// set the text to be viewed when the middle mouse button is pressed
int mmf_sbt_set_text(struct mms_sbt_display * sbt_display, unsigned char * text) {
    sbt_display->text=text;
    return(0);
}



// thread for vibrating border taxels
int mmf_sbt_vibration(void * data) {
    struct mms_sbt_display * sbt_display;
    int x;

    sbt_display=(struct mms_sbt_display *)data;

    while(sbt_display->th_vibe_exit==0) if(MM_SBT_Running&&sbt_display->taxel) {

        if(sbt_display->lock&SBT_TEXT_LOCK&&sbt_display->text) {
            for(x=0;x<sbt_display->bn;x++) {    
//                sbt_display->bd[x]=sbt_display->chars[(unsigned int)sbt_display->text[sbt_display->bn-1-x]];
            }
            mmf_sbt_write_data(sbt_display);
            SDL_Delay(50);
        } else {
            for(x=0;x<sbt_display->bn;x++) {
                sbt_display->bd[x]|=sbt_display->bv[x];
            }
            mmf_sbt_write_data(sbt_display);
            SDL_Delay(100);
            for(x=0;x<sbt_display->bn;x++) {
                sbt_display->bd[x]&=~sbt_display->bv[x];
            }
            mmf_sbt_write_data(sbt_display);
            SDL_Delay(100);
        }
    } else { 
	  SDL_Delay(100);
	}
    
    sbt_display->th_vibe_exit=2;
    return(0);
}



// main sbt processing thread
int mmf_sbt_processor(void * data) {
    unsigned char ind[3][3],in;
    unsigned int cp,cd,data_ready;
    struct mms_sbt_display * sbt_display;
    int x,y,tx,ty,set,retval;
    struct timeval timeout;
    fd_set rfds;
    
    bzero(ind,9);
    cp=cd=0; data_ready=0;
    sbt_display=(struct mms_sbt_display *)data;
    
    while(sbt_display->th_proc_exit==0) {
	  if(data_ready) {
		data_ready=0;
		if(ind[0][0]==ind[0][1]==ind[1][0]==ind[1][1]==ind[2][0]==ind[2][1]==0xFF) {
			printf("reseted\n");
            sbt_display->my=0;
            sbt_display->mx=0;
            sbt_display->bxS=sbt_display->mx*sbt_display->ppm;
            sbt_display->byS=sbt_display->my*sbt_display->ppm;
            sbt_display->bxE=sbt_display->bxS+sbt_display->bw;
            sbt_display->byE=sbt_display->byS+sbt_display->bh;
        
            set=(sbt_display->inv)?0x00:0x01;
            if(sbt_display->taxel) for(x=0;x<sbt_display->bn;x++) {
                    sbt_display->bd[sbt_display->bn-1-x]=0;
                    sbt_display->bv[sbt_display->bn-1-x]=0;                    
                for(y=0;y<8;y++) {
                    tx=((sbt_display->mx + (3.97+2.45)*x + SBT_TAXELCOOX[y]) * sbt_display->ppm);
                    ty=((sbt_display->my                 + SBT_TAXELCOOY[y]) * sbt_display->ppm);
                    if(tx>=0&&tx<sbt_display->tw && ty>=0&&ty<sbt_display->th)
                        sbt_display->bd[sbt_display->bn-1-x]|=(sbt_display->taxel[(tx+ty*sbt_display->tw)]?(set<<y):((1-set)<<y));
                    else 
                        sbt_display->bv[sbt_display->bn-1-x]|=0x01<<y;
                }
            }
		} else {
			  printf("accepted\n");
          	  if(ind[0][1]&0x04) {
                sbt_display->lock|= SBT_TEXT_LOCK;
          	  } else {
                sbt_display->lock&=~SBT_TEXT_LOCK;
          	  }
			  if(ind[0][1]&0x04) {
				SDL_LockMutex(MM_SBTButtonLock);
				printf("MB Middle\n"); MM_SBT_ButtonAction=MM_SBT_ACT_M;
				SDL_UnlockMutex(MM_SBTButtonLock);
			  }
          	  else if((ind[0][1]&0x03)==0x03)
			  {
				SDL_LockMutex(MM_SBTButtonLock);
				printf("MB Double\n"); MM_SBT_ButtonAction=MM_SBT_ACT_D;
				SDL_UnlockMutex(MM_SBTButtonLock);
			  }
          	  else if(ind[0][1]&0x02) {
				SDL_LockMutex(MM_SBTButtonLock);
				printf("MB Right\n"); MM_SBT_ButtonAction=MM_SBT_ACT_R;
				SDL_UnlockMutex(MM_SBTButtonLock);
			  }
          	  else if(ind[0][1]&0x01) {
				SDL_LockMutex(MM_SBTButtonLock);
				printf("MB Left\n"); MM_SBT_ButtonAction=MM_SBT_ACT_L;
				SDL_UnlockMutex(MM_SBTButtonLock);
			  }
          	  if(ind[0][1]&0x10) {
                sbt_display->mx-=(256.0-(GLdouble)ind[1][1])/7.5;
          	  } else {
                sbt_display->mx+=((GLdouble)ind[1][1])/7.5;
          	  }
          	  if(ind[0][1]&0x20) {
                sbt_display->my+=(256.0-(GLdouble)ind[2][1])/7.5;
          	  } else {
                sbt_display->my-=((GLdouble)ind[2][1])/7.5;
          	  }
            
			  if(sbt_display->taxel&&MM_SBT_Running) {
          		sbt_display->bxS=sbt_display->mx*sbt_display->ppm;
          		sbt_display->byS=sbt_display->my*sbt_display->ppm;
          		sbt_display->bxE=sbt_display->bxS+sbt_display->bw;
          		sbt_display->byE=sbt_display->byS+sbt_display->bh;
            
          		set=(sbt_display->inv)?0x00:0x01;
          		if(sbt_display->taxel) for(x=0;x<sbt_display->bn;x++) {
                  sbt_display->bd[sbt_display->bn-1-x]=0;
                  sbt_display->bv[sbt_display->bn-1-x]=0;                    
              	  for(y=0;y<8;y++) {
                    tx=((sbt_display->mx + (3.97+2.45)*x + SBT_TAXELCOOX[y]) * sbt_display->ppm);
                    ty=((sbt_display->my                 + SBT_TAXELCOOY[y]) * sbt_display->ppm);
                    if(tx>=0&&tx<sbt_display->tw && ty>=0&&ty<sbt_display->th)
                        sbt_display->bd[sbt_display->bn-1-x]|=(sbt_display->taxel[(tx+ty*sbt_display->tw)]?(set<<y):((1-set)<<y));
                    else 
                        sbt_display->bv[sbt_display->bn-1-x]|=0x01<<y;
              	  }
          		}
          	  } 
		  }
      	}
    
        if(read(sbt_display->dev_file,&in,1)==1) {
		  printf("R : %2x (%d,%d) : %2x %2x %2x : %2x %2x %2x : %2x %2x %2x\n",in,cp,cd,ind[0][0],ind[0][1],ind[0][2],ind[1][0],ind[1][1],ind[1][2],ind[2][0],ind[2][1],ind[2][2]);
		  if(data_ready) { data_ready=0; cd=cp=0; bzero(ind,9); }
          ind[cp][cd]=in;                
		  cd++;
		  if(cd==3) { cp++; cd=0; }
		  if(cp==3) { cp=0; cd=0; data_ready=1; }
		  if(data_ready) {
			if (
		  	    ((ind[0][0]!=0x03) && (ind[0][0]!=0x01) && (ind[0][0]!=0xFF))
			  ||((ind[1][0]!=0x03) && (ind[1][0]!=0x01) && (ind[1][0]!=0xFF))
	  	  	  ||((ind[2][0]!=0x03) && (ind[2][0]!=0x01) && (ind[2][0]!=0xFF))
			  ||((ind[0][2]!='.') || (ind[1][2]!='.') || (ind[2][2]!='.'))
			) {
				printf("error packet sync\n");
				ind[0][0]=ind[0][1];
				ind[0][1]=ind[0][2];
				ind[0][2]=ind[1][0];
				ind[1][0]=ind[1][1];
				ind[1][1]=ind[1][2];
				ind[1][2]=ind[2][0];
				ind[2][0]=ind[2][1];
				ind[2][1]=ind[2][2];
				data_ready=0;
				cp=2; cd=2;
        	} else if (
				(((ind[0][1]&0x08)&&!(ind[0][1]&0xC0)))
			  ||((ind[0][0]==ind[0][1]==ind[1][0]==ind[1][1]==ind[2][0]==ind[2][1]==0xFF))
			) {
				printf("error packet shift\n");				
				ind[0][0]=ind[1][0];
				ind[0][1]=ind[1][1];
				ind[0][2]=ind[1][2];
				ind[1][0]=ind[2][0];
				ind[1][1]=ind[2][1];
				ind[1][2]=ind[2][2];
				data_ready=0;
				cp=2; cd=0;
			}
		  }
        } else {
		  SDL_Delay(20);
		}
		
	}
    sbt_display->th_proc_exit=2;
    return 0;
}


mmf_sbt_texture2taxel(struct mms_sbt_display * disp, struct mms_texture * texture) {
    unsigned int x,y;
    GLubyte * texpixels_bw;
    GLubyte * texpixels_rgba;
    GLubyte * texpixels_flip;
    int sr,sg,sb,tr,tg,tb,src;

    if(texture->sbt_bw==NULL) {
      texpixels_bw=malloc(texture->w*texture->h*1);
      texpixels_rgba=malloc(texture->w*texture->h*4);
      if(disp->vflip) texpixels_flip=malloc(texture->w*texture->h*1);
    
      glEnable(GL_TEXTURE_RECTANGLE_NV);
      glBindTexture(GL_TEXTURE_RECTANGLE_NV,texture->name);

      glGetTexImage(GL_TEXTURE_RECTANGLE_NV,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,texpixels_rgba);
      for(x=0;x<(texture->w*texture->h);x++) {
        sr=texpixels_rgba[x*4+3];
        sg=texpixels_rgba[x*4+2];
        sb=texpixels_rgba[x*4+1];
        tr=disp->bgc&0xFF;
        tg=(disp->bgc>>8)&0xFF;
        tb=(disp->bgc>>16)&0xFF;
        texpixels_bw[x]=(abs(sr-tr)+abs(sg-tg)+abs(sb-tb))?(0x00):(0xFF);
      }
      free(texpixels_rgba);

      if(disp->vflip) {
        for(x=0;x<texture->w;x++) for(y=0;y<texture->h;y++) {
          texpixels_flip[x+y*texture->w]=texpixels_bw[x+(texture->h-1-y)*texture->w];
        }
        free(texpixels_bw);
        texpixels_bw=texpixels_flip;
      }
      
      texture->sbt_bw=texpixels_bw;
      mmf_gl_errors();
    }
	disp->taxel=texture->sbt_bw;
	MM_SBT_Running=1;
}


int mmf_sbt_set_image(struct mms_sbt_display * sbt_display, struct mms_texture * texture, unsigned int inv, unsigned int vflip, Uint32 bgc, Uint32 fgc) {

    if(texture->w>texture->h) {
        sbt_display->mmw=359.0;
        sbt_display->mmh=(359.0*(GLdouble)texture->h)/(GLdouble)texture->w;
        sbt_display->ppm=((GLdouble)texture->w)/359.0;
    } else {
        sbt_display->mmh=359.0;
        sbt_display->mmw=(359.0*(GLdouble)texture->w)/(GLdouble)texture->h;
        sbt_display->ppm=((GLdouble)texture->h)/359.0;
    }

    sbt_display->bxS=sbt_display->mx*sbt_display->ppm;
    sbt_display->byS=sbt_display->my*sbt_display->ppm;
    sbt_display->bxE=sbt_display->bxS+sbt_display->bw;
    sbt_display->byE=sbt_display->byS+sbt_display->bh;
    
    sbt_display->mmox=(359.0-(GLdouble)sbt_display->mmw)/2;
    sbt_display->mmoy=(359.0-(GLdouble)sbt_display->mmh)/2;

    sbt_display->bw=sbt_display->ppm*((sbt_display->bn-1)*3.97+(sbt_display->bn)*2.45);
    sbt_display->bh=sbt_display->ppm*(3*2.45);
    
    sbt_display->inv=inv;

    sbt_display->inv=inv;
    sbt_display->vflip=vflip;
    sbt_display->bgc=bgc;
    sbt_display->fgc=fgc;
	sbt_display->texture=texture;

    sbt_display->tw=texture->w;
    sbt_display->th=texture->h;
    sbt_display->taxel=texture->sbt_bw; // NULL or not
	if((texture->sbt_bw)) {
	  MM_SBT_Running=1;
	}
    return(0);
}



// clear the sbt display
int mmf_sbt_clear_display(struct mms_sbt_display * sbt_display) {
    int c;
    for(c=0;c<sbt_display->bn;c++) sbt_display->bd[c]=0x00;
    return mmf_sbt_write_data(sbt_display);
}



// rise the sbt display
int mmf_sbt_rise_display(struct mms_sbt_display * sbt_display) {
    int c;
    for(c=0;c<sbt_display->bn;c++) sbt_display->bd[c]=0xFF;
    return mmf_sbt_write_data(sbt_display);
}



// write custom data to the display, i.e. text
int mmf_sbt_write_display(struct mms_sbt_display * sbt_display, unsigned char * data) {
    int c;
    for(c=0;c<sbt_display->bn;c++)  sbt_display->bd[c]=data[c];
    return mmf_sbt_write_data(sbt_display);
}



// shutdown a sbt display
int mmf_sbt_close_display (struct mms_sbt_display * sbt_display) {
    SDL_Thread * temp_thread;
    
    if(sbt_display->th_proc) {
        temp_thread=sbt_display->th_proc;
        sbt_display->th_proc_exit=1;
        SDL_Delay(500);
        if(sbt_display->th_proc_exit!=2) SDL_KillThread(temp_thread);
    }

    if(sbt_display->th_vibe) {
        temp_thread=sbt_display->th_vibe;
        sbt_display->th_vibe_exit=1;
        SDL_Delay(500);
        if(sbt_display->th_vibe_exit!=2) SDL_KillThread(temp_thread);
    }

    mmf_sbt_clear_display(sbt_display);

    if(sbt_display->dev_name) free(sbt_display->dev_name);
    free(sbt_display);
    
    return 0;
}



// open and initialize a sbt display
struct mms_sbt_display * mmf_sbt_open_display (char * dev_name, unsigned int baud, unsigned int bn) {
    unsigned int        dev_file;
    struct termios      settings;
    struct mms_sbt_display  * sbt_display;
    
// perform some basic checks on input data
    if(!dev_name) {
        mmf_sbt_error_msg("No device name given.");
        return NULL;
    }
    if(!baud) {
        mmf_sbt_error_msg("Empty baud number.");
        return NULL;
    }
    if( (baud!=9600)    &&
        (baud!=19200)   &&
        (baud!=38400)   &&
        (baud!=57600)   &&
        (baud!=115200)  &&
        (baud!=230400)  ) {
        mmf_sbt_error_msg("Wrong baud number.");
        return NULL;
    }
    if(!bn) {
        mmf_sbt_error_msg("Empty number of braille elements.\n");
        return NULL;
    }

// allocate memories
    sbt_display=malloc(sizeof(struct mms_sbt_display));
    if(sbt_display==NULL) {
        mmf_sbt_error_msg("Can't malloc structure.");
        return NULL;
    }

    sbt_display->dev_name=malloc(strlen(dev_name)+1);
    if(sbt_display->dev_name==NULL) {
        free(sbt_display);
        mmf_sbt_error_msg("Can't malloc device name memory.");
        return NULL;    
    }

    sbt_display->bd=malloc(sizeof(unsigned char)*bn);
    if(sbt_display->bd==NULL) {
        free(sbt_display->dev_name);
        free(sbt_display);
        mmf_sbt_error_msg("Can't malloc braille data memory.");
        return NULL;    
    }

    sbt_display->bv=malloc(sizeof(unsigned char)*bn);
    if(sbt_display->bv==NULL) {
        free(sbt_display->dev_name);
        free(sbt_display->bd);
        free(sbt_display);
        mmf_sbt_error_msg("Can't malloc braille vibe memory.");
        return NULL;    
    }

// open the ports, we are using blocking mode because of multithreading
    dev_file=open(dev_name,O_RDWR|O_NOCTTY  |O_NDELAY  );
    if(dev_file==-1) {
        mmf_sbt_error_msg("Can't open device.");
        return NULL;
    }
    
    tcflush(dev_file,TCIOFLUSH);

	fcntl(dev_file,F_SETFL,FNDELAY);
    tcgetattr(dev_file,&settings);

    switch(baud) {
        case 9600:      
            cfsetispeed(&settings, B9600);
            cfsetospeed(&settings, B9600);
             break;
        case 19200:
            cfsetispeed(&settings, B19200);
            cfsetospeed(&settings, B19200);
            break;
        case 38400:
            cfsetispeed(&settings, B38400);
            cfsetospeed(&settings, B38400);
            break;
        case 57600:
            cfsetispeed(&settings, B57600);
            cfsetospeed(&settings, B57600);
            break;
        case 115200:
            cfsetispeed(&settings, B115200);
            cfsetospeed(&settings, B115200);
            break;
        case 230400:
            cfsetispeed(&settings, B230400);
            cfsetospeed(&settings, B230400);
            break;
    }
    settings.c_cflag&=~PARENB; // keine Paritaet
    settings.c_cflag&=~CSTOPB; // 2. Stopbit abschalten => 1 Stopbit
    settings.c_cflag&=~CSIZE;  // 8-Bit
    settings.c_cflag|=CS8;

    settings.c_cflag|=(CLOCAL|CREAD); // Empfang aktivieren, lokaler Modus einstellen

    settings.c_lflag&=~(ICANON|ECHO|ECHOE|ISIG); // Roh-Eingabe (c_lflag!)
    settings.c_cflag&=~CRTSCTS; // keine Flusskontrolle (FLUSS = CRTSCTS oder CNEW_RTSCTS)
    settings.c_iflag&=~(IXON | IXOFF | IXANY); // keine Flusskontrolle
    //settings.c_oflag&=~OPOST; // Roh-Ausgabe

    tcsetattr(dev_file,TCSANOW,&settings); // settings der seriellen Schnittstelle speichern

// fill the main structure
    sbt_display->dev_file=dev_file;
    sbt_display->dev_baud=baud;
//    strncpy(sbt_display->dev_name,dev_name,strlen(dev_name));
    strcpy(sbt_display->dev_name,dev_name);
    sbt_display->bn=bn;
    sbt_display->taxel=NULL;
	sbt_display->texture=NULL;
    sbt_display->inv=0;
    sbt_display->chars[0]=0x00;
    memset(sbt_display->bd,0x00,sbt_display->bn);
    memset(sbt_display->bv,0x00,sbt_display->bn);

    sbt_display->th_proc_exit=0;
    sbt_display->th_proc=NULL;
    sbt_display->th_proc=SDL_CreateThread(mmf_sbt_processor,(void *)sbt_display);
    if(!sbt_display->th_proc) {
        mmf_sbt_error_msg("Can't create processor thread.\n");
        mmf_sbt_close_display(sbt_display);
        return NULL;
    }

    sbt_display->th_vibe_exit=0;
    sbt_display->th_vibe=NULL;
    sbt_display->th_vibe=SDL_CreateThread(mmf_sbt_vibration,(void *)sbt_display);
    if(!sbt_display->th_vibe) {
        mmf_sbt_error_msg("Can't create vibration thread.\n");
        mmf_sbt_close_display(sbt_display);
        return NULL;
    }
    
    return sbt_display;
}


// MM related functions

int mmf_sbt_open() {
  if( (MM_SBT_DevName!=NULL) && (MM_SBT_DevBaud!=0) ) {
    if(MM_SBT_Display) 
      mmf_sbt_close_display(MM_SBT_Display);
    if(strlen(MM_SBT_DevName)<1)
      return(1);
    MM_SBT_Display=mmf_sbt_open_display(MM_SBT_DevName,MM_SBT_DevBaud,5);
    if(!MM_SBT_Display) mmf_sbt_error_msg("CREATING SBT FAILED\n");
    else {
      mmf_sbt_error_msg("CREATING SBT OK\n");
      mmf_sbt_load_chars(MM_SBT_Display,"text.de.tbl");
    }
    printf("dev_name: %s\n",MM_SBT_Display->dev_name);
    printf("dev_name_o: %s\n",MM_SBT_DevName);
  }
}

int mmf_sbt_set_current(struct mms_item * item) {
  struct mms_item * runner;
  Uint32 fgc,bgc;
  
  MM_SBT_Running=0;

  for(runner=MM_CurrentPage->items;runner;runner=runner->next) {
    if(runner!=item)
      runner->is_sbt=0;
  }  
  
  switch(item->type) {
    case MM_ASIAANIM:
      mmf_sbt_set_image(MM_SBT_Display,((struct mms_asiaanim *)item->data)->texture,1,1,0xFFFFFF,0x000000);
      MM_CurrentSBTItem=item;
      break;
    case MM_IMAGE_BUTTON:
    case MM_TEXT_BUTTON:
      mmf_sbt_set_image(MM_SBT_Display,((struct mms_button *)item->data)->texture_low,1,0,0xFFFFFF,0x000000);
      MM_CurrentSBTItem=item;
      break;
    case MM_TEXT:
      fgc =((Uint32)(((((struct mms_text *)item->data)->fg.red  )>>8)&0xFF));
      fgc|=((Uint32)(((((struct mms_text *)item->data)->fg.green)>>8)&0xFF))<<8;
      fgc|=((Uint32)(((((struct mms_text *)item->data)->fg.blue )>>8)&0xFF))<<16;
      bgc =((Uint32)(((((struct mms_text *)item->data)->bg.red  )>>8)&0xFF));
      bgc|=((Uint32)(((((struct mms_text *)item->data)->bg.green)>>8)&0xFF))<<8;
      bgc|=((Uint32)(((((struct mms_text *)item->data)->bg.blue )>>8)&0xFF))<<16;
      mmf_sbt_set_image(MM_SBT_Display,((struct mms_text *)item->data)->texture,1,0,bgc,fgc);
      MM_CurrentSBTItem=item;
      break;
    case MM_IMAGE:
      mmf_sbt_set_image(MM_SBT_Display,((struct mms_image *)item->data)->texture,1,0,0xFFFFFF,0x000000);
      MM_CurrentSBTItem=item;
      break;
    default:
      MM_SBT_Running=0;
      break;
  }

  
}
