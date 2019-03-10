int mmf_action_jump_to_page(GLuint id) {
  struct mms_page * target;
  struct mms_item * runner;
  struct mms_sound * temp;
  struct mms_video * tempv;

  for(runner=MM_CurrentPage->items;runner;runner=runner->next) {
  	if(runner->type==MM_SOUND) {
      SDL_LockMutex(MM_SoundLock);
	  temp=((struct mms_sound *)runner->data);
	  temp->is_playing=0;
	  temp->play_pos=temp->buffer;
	  temp->play_remain=temp->len;
      SDL_UnlockMutex(MM_SoundLock);
	}
  	if(runner->type==MM_VIDEO) {
      SDL_LockMutex(MM_VideoLock);
	  tempv=((struct mms_video *)runner->data);
  	  tempv->is_playing=MM_VIDEO_PLEASE_STOP;
	  tempv->fidx=0;  
      SDL_UnlockMutex(MM_VideoLock);	  
	}
  }
  target=mmf_get_page_by_id(id);
  if(target)
    MM_CurrentPage=target;
  else
    return(1);
  
  MM_PROW_FocusPage=MM_CurrentPage;
  MM_PROW_FocusPage=target;
  MM_GTKIDLER_REBUILD_ITEMS=1;
  MM_GTKIDLER_REBUILD_PAGES=1;

  for(runner=MM_CurrentPage->items;runner;runner=runner->next) {
    if(runner->is_sbt) {
      mmf_sbt_set_current(runner);
	  if(MM_SBT_ButtonAction!=MM_SBT_ACT_NONE) {
		if(runner->sbtact[MM_SBT_ACT_E]!=MM_NO_ACTION) {
		  mmf_call_action(runner->sbtact[MM_SBT_ACT_E],runner->sbtava[MM_SBT_ACT_E]);
		}
	  }
      runner=NULL;
	  MM_SBT_ButtonAction=MM_SBT_ACT_NONE;
      return(0);
    } 
}
  MM_SBT_Running=0;
  MM_SBT_ButtonAction=MM_SBT_ACT_NONE;
}
                                                      
