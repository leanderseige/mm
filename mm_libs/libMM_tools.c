struct mms_texture * mmf_new_texture() {
  struct mms_texture *ret,*run;
  
  ret=malloc(sizeof(struct mms_texture));
  if(ret==NULL) return(NULL);
  
  ret->next=NULL;
  if(MM_Textures==NULL) {
	MM_Textures=ret;
	ret->prev=NULL;
  } else {
	for(run=MM_Textures;run->next;run=run->next);
	run->next=ret;
	ret->prev=run;
  }
  return(ret);
}

void mmf_realpath(char * in) {
  if(realpath(in,MM_PathBuf)==NULL)
    MM_PathBuf[0]=0x00;
}

void mmf_gl_errors() {
  GLuint error;
  while((error=glGetError())!=GL_NO_ERROR) 
    fprintf(stderr,"GLERROR: %s\n",gluErrorString(error));  
}

