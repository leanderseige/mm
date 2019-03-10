void mmf_delete_texture(struct mms_texture * texture) {
  glDeleteTextures(1,&texture->name);
  if(texture->sbt_bw!=NULL) free(texture->sbt_bw);
  if(texture->surf) SDL_FreeSurface(texture->surf);
  
  if(texture==MM_Textures) {
	if(texture->next) {
	  MM_Textures=texture->next;
	  MM_Textures->prev=NULL;
	} else {
	  MM_Textures=NULL;
	}
  } else {
	texture->prev->next=texture->next;
	if(texture->next) {
	  texture->next->prev=texture->prev;
	}
  }
  
  free(texture);
}

void mmf_delete_res_text(struct mms_text * text) {
  text->texture->status=MM_TM_REMOVE;
  free(text->text);
  free(text->full_text);
  free(text);
}

void mmf_delete_res_image(struct mms_image * image) {
  image->texture->status=MM_TM_REMOVE;
  free(image->text);
  free(image->path);
  free(image);
}

void mmf_delete_res_button(struct mms_button * button) {
  button->texture_low->status=MM_TM_REMOVE;
  button->texture_high->status=MM_TM_REMOVE;
  if(button->text) free(button->text);
  if(button->path) free(button->path);
  free(button);
}

void mmf_delete_res_sound(struct mms_sound * sound) {
  SDL_FreeWAV(sound->buffer);
  if(sound->text) free(sound->text);
  if(sound->path) free(sound->path);
  free(sound);
}

void mmf_delete_res_font(struct mms_font * font) {
  if(font->text) free(font->text);
  if(font->path) free(font->path);
  free(font);
}

void mmf_delete_res_video(struct mms_video * video) {
  int c;
//  video->texture->status=MM_TM_REMOVE; FIXEM
  dv_decoder_free(video->decoder);
  close(video->filedesc);
  for(c=0;c<4;c++)
    if(video->audio_buffers[c])
      free(video->audio_buffers[c]);
  if(video->pixels) free(video->pixels);
  if(video->text) free(video->text);
  if(video->path) free(video->path);
  free(video);
}

void mmf_delete_res_aad(struct mms_asiaanim * aad) {
  int c,i;
  
  for(c=0;c<aad->nf;c++) {
    for(i=0;i<aad->all_frames[c]->ns;i++) {
      free(aad->all_frames[c]->s[i]);
    }
    free(aad->all_frames[c]);
  }
  free(aad->all_frames);
  if(aad->text) free(aad->text);
  if(aad->path) free(aad->path);
  free(aad);
}


