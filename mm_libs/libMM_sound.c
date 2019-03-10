void mmf_fill_sound_buffer(void *udata, Uint8 *stream, int len) {
  struct mms_item * runner;
  struct mms_sound * tsound;
  struct mms_video * tvideo;
  unsigned char joiner[DV_AUDIO_MAX_SAMPLES*4];
  Uint32    tlen;

//    SDL_LockMutex(MM_SoundLock);
  if(MM_CurrentPage)
    runner=MM_CurrentPage->items;
  else
    runner=NULL;
  while(runner) {
    switch(runner->type) {
      case MM_SOUND:
        SDL_LockMutex(MM_SoundLock);
  		if(((struct mms_sound *)runner->data)->is_playing) {
      	  tsound=(struct mms_sound *)(runner->data);
      	  tlen=(len>tsound->play_remain?tsound->play_remain:len);   
      	  SDL_MixAudio(stream,tsound->play_pos,tlen, SDL_MIX_MAXVOLUME);       
      	  tsound->play_remain-=tlen;
      	  tsound->play_pos   +=tlen;
		  if(!(tsound->play_remain)) {
			((struct mms_sound *)runner->data)->is_playing=0;
		  }
		}
        SDL_UnlockMutex(MM_SoundLock);
        break;
      case MM_VIDEO:
        tvideo=(struct mms_video *)(runner->data);
	SDL_LockMutex(MM_VideoLock);
	if(((struct mms_video *)runner->data)->is_playing!=MM_VIDEO_STOPPED) {
		SDL_UnlockMutex(MM_VideoLock);
		if(len>tvideo->play_remain) {
		tlen=tvideo->play_remain;
		memcpy(joiner,tvideo->play_pos,tlen);
		mmf_dv_read_frame(tvideo);
		memcpy(&(joiner[tlen]),tvideo->play_pos,len-tlen);
		SDL_MixAudio(stream,joiner,len, SDL_MIX_MAXVOLUME);
		tvideo->play_remain-=(len-tlen);
		tvideo->play_pos   +=(len-tlen);
		} else {    
		SDL_MixAudio(stream,tvideo->play_pos,len, SDL_MIX_MAXVOLUME);       
		tvideo->play_remain-=len;
		tvideo->play_pos   +=len;
		}
        } else {
		SDL_UnlockMutex(MM_VideoLock);
        }
        break;
      default:
        break;
    }
    runner=runner->next;
  }
//        SDL_UnlockMutex(MM_SoundLock);
  
}

int mmf_init_sound() {
  SDL_AudioSpec wanted;
      
  /* Set the audio format */
  wanted.freq = 48000;
  wanted.format = AUDIO_S16;
  wanted.channels = 2;    /* 1 = mono, 2 = stereo */
  wanted.samples = 1024;  /* 25fps => 1024 */
  wanted.callback = mmf_fill_sound_buffer;
  wanted.userdata = NULL;
                                  
  /* Open the audio device, forcing the desired format */
  if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
    return(1);
  }
  
  SDL_PauseAudio(0);

  return(0);
}

