int mmf_dv_get_audio_specs(struct mms_video * vs) {
    printf("Audio is %.1f kHz, %d bits quantization, "
              "%d channels, emphasis %s\n",
            (float)vs->decoder->audio->frequency / 1000.0,
                  vs->decoder->audio->quantization,
                 vs->decoder->audio->num_channels,
                (vs->decoder->audio->emphasis ? "on" : "off"));
}

int mmf_dv_read_frame(struct mms_video * vs) {
    GLuint i, samples;
    gint16 *left,*right,*stereo;

    SDL_LockMutex(MM_VideoLock);
	if(vs->is_playing==MM_VIDEO_PLEASE_STOP) {
	  vs->fidx=0;
	  vs->is_playing=MM_VIDEO_STOPPED;
	}
    SDL_UnlockMutex(MM_VideoLock);


    // dv frames have a fixed size, we can simply seek to a certain frame
    lseek(vs->filedesc,vs->fidx*vs->chunksize,SEEK_SET);
    if(vs->chunksize!=read(vs->filedesc,vs->raw_in,vs->chunksize)) {
        fprintf(stderr,"Error reading frame.\n");
        return(1);
    }
    vs->fidx++;
    vs->fidx%=vs->fcnt;

    dv_parse_header(vs->decoder,vs->raw_in);

    dv_decode_full_audio(vs->decoder,vs->raw_in,(gint16 **)(vs->audio_buffers));
    left=(gint16 *)vs->audio_buffers[0];
    right=(gint16 *)vs->audio_buffers[1];
    stereo=(gint16 *)vs->buffer;
    for (i = 0, samples = dv_get_num_samples (vs->decoder);i < samples; i++) {
      stereo[2*i+0]=right[i];
      stereo[2*i+1]=left[i];
    } 
    vs->play_remain=samples*4; // 1920*4;
    vs->play_pos=vs->buffer;
//  printf("samples: %d/%d\n",samples,DV_AUDIO_MAX_SAMPLES);
//  mmf_dv_get_audio_specs(vs);


    SDL_LockMutex(MM_VideoReadyMutex);
    if(vs->texture->status==MM_TM_VIDEOUPDATE) {
//      printf("EEEK!\n");
      SDL_CondWait(MM_VideoReadyCond,MM_VideoReadyMutex);
    }
    SDL_LockMutex(MM_VideoLock);
    SDL_UnlockMutex(MM_VideoReadyMutex);

/*
    if(vs->texture->status==MM_TM_VIDEOUPDATE) {
      printf("EEEK!\n");
      SDL_UnlockMutex(MM_VideoLock);
      while(vs->texture->status!=MM_TM_UPLOADED) SDL_Delay(10);
      SDL_LockMutex(MM_VideoLock);
    }
*/
    dv_decode_full_frame(vs->decoder,vs->raw_in,e_dv_color_rgb,&(vs->pixels),&vs->libdv_pitches);
    if(vs->texture->status!=MM_TM_VIDEOGENUPD)
      vs->texture->status=MM_TM_VIDEOUPDATE;
    SDL_UnlockMutex(MM_VideoLock);

    return(0);
}

int mmf_dv_open_file(struct mms_video * vs, char * filename) {
    struct stat fstats;    
    GLuint fs;
    struct mms_texture * temptexture;

    vs->texture=mmf_new_texture();
    if(!vs->texture) {
      fprintf(stderr,"Cannot alloc texture structure.\n");
      return(1);
    }
    vs->texture->sbt_bw=NULL;
//    glEnable(GL_TEXTURE_RECTANGLE_NV);  
//    glGenTextures(1,&(vs->texture->name));
    vs->texture->name=-1;
    vs->texture->status=MM_TM_VIDEOGENUPD;
    
    stat(filename,&fstats);
    fs=fstats.st_size;
    
    fprintf(stderr,"trying: >>%s<<\n",filename);
    vs->filedesc=open(filename,O_RDONLY);
    if(vs->filedesc==-1) {
        fprintf(stderr,"cannot open dv file %s.\n",filename);
        return(1);
    }
    // read the first 4 bytes for determining the norm
    if(4!=read(vs->filedesc,vs->raw_in,4)) {
        fprintf(stderr,"Cannot check file.\n");
        return(1);
    }
    // determine the norm and set appropriate values
    if(vs->raw_in[3]&0x80) {
        vs->norm=1; //PAL
        vs->chunksize=144000;
        vs->framesize=720*576*3;
        vs->texture->w=720;
        vs->texture->h=576;
        vs->texture->a=1;
    } else {
        vs->norm=0; //NTSC
        vs->chunksize=120000;
        vs->framesize=720*480*3;
        vs->texture->w=720;
        vs->texture->h=480;
        vs->texture->a=1;
    }
    // rewind
    lseek(vs->filedesc,0,SEEK_SET);
    vs->fidx=0;
    vs->fcnt=floor(fs/vs->chunksize);

    vs->libdv_pitches=720*3;
    
    vs->decoder=dv_decoder_new(vs->norm?TRUE:FALSE,FALSE,FALSE);
    if(!vs->decoder) {
        close(vs->filedesc);
        return(1);
    }
    vs->decoder->quality=DV_QUALITY_BEST; 
//    if(dv_format_wide(vs->decoder)) vs->wide=1; else vs->wide=0;

    vs->play_pos=vs->buffer;
    vs->play_remain=0;    

    return(0);
}

