int mmef_lock_player() {
  SDL_LockMutex(MM_RendererLock);
}


int mmef_unlock_player() {
  SDL_UnlockMutex(MM_RendererLock);
}


int mmef_yesno (GtkWidget * elterliches,char * msg) {
  int result;

  question=gtk_message_dialog_new(GTK_WINDOW(elterliches), GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,msg);
  
  result = gtk_dialog_run(GTK_DIALOG(question));
  switch (result) {
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_YES:
    case GTK_RESPONSE_NONE:
      gtk_widget_destroy(question);
      return(1);
      break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_NO:    
    case GTK_RESPONSE_CANCEL:
    default:
      gtk_widget_destroy(question);
      return(0);
      break;
  }  
}

int mmef_error (GtkWidget * elterliches,char * msg) {
  int result;

  question=gtk_message_dialog_new(GTK_WINDOW(elterliches), GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,msg);
  
  result = gtk_dialog_run(GTK_DIALOG(question));
  gtk_widget_destroy(question);
  return(0);
}

int mmef_info (GtkWidget * elterliches,char * msg) {
  int result;

  question=gtk_message_dialog_new(GTK_WINDOW(elterliches), GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO, GTK_BUTTONS_OK,msg);
  
  result = gtk_dialog_run(GTK_DIALOG(question));
  gtk_widget_destroy(question);
  return(0);
}


void ende_cb(GtkObject *object) {
	gtk_main_quit();
}

int thread_gtk(int argc, char **argv) {
  GtkWidget *box,*box0,*box00,*box01,*box000,*box001,*boxV00;
  GtkWidget *swr,*swp,*swi;
  GdkScreen * defscreen;
  GtkSettings * defsets;

  gtk_init(&argc, &argv);

  defscreen=gdk_screen_get_default();
  defsets=gtk_settings_get_for_screen(defscreen);
  gtk_settings_set_string_property(defsets,"gtk-font-name","arial unicode 10","XProperty");

  fenster = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  b_newpage  = gtk_button_new_with_label("Neue Seite");
  b_newresource  = gtk_button_new_with_label("Neu");
  b_globals =  gtk_button_new_with_label("Globale Parameter");
  b_load =  gtk_button_new_with_label("Laden");
  b_save =  gtk_button_new_with_label("Speichern");
  b_exportdir =  gtk_button_new_with_label("Exportieren");
  b_cleansweep =  gtk_button_new_with_label("Alles loeschen");

  c_resources = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Text Button");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Grafik Button");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Bild/Grafik");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Video");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Zeichensatz");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Text");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"Sound");
  gtk_combo_box_append_text(GTK_COMBO_BOX(c_resources),"AnimatedAsiaDraw");

  box = gtk_vbox_new(FALSE,0);
  box0 = gtk_hbox_new(FALSE,0);
  box00= gtk_vbox_new(FALSE,0);
  box01= gtk_vbox_new(FALSE,0);
    box000=gtk_hbox_new(FALSE,0);
    box001=gtk_hbox_new(FALSE,0);
    boxV00=gtk_hbox_new(FALSE,0);

	gtk_signal_connect_object(GTK_OBJECT(b_newpage), "clicked",
		GTK_SIGNAL_FUNC(mmef_newpage_clicked), GTK_OBJECT(fenster));

	gtk_signal_connect_object(GTK_OBJECT(b_newresource), "clicked",
		GTK_SIGNAL_FUNC(mmef_newresource_clicked), GTK_OBJECT(fenster));

	gtk_signal_connect_object(GTK_OBJECT(b_globals), "clicked",
		GTK_SIGNAL_FUNC(mmef_globals_clicked), GTK_OBJECT(fenster));

	gtk_signal_connect_object(GTK_OBJECT(b_save), "clicked",
		GTK_SIGNAL_FUNC(mmef_save_clicked), GTK_OBJECT(fenster));

	gtk_signal_connect_object(GTK_OBJECT(b_load), "clicked",
		GTK_SIGNAL_FUNC(mmef_load_clicked), GTK_OBJECT(fenster));

	gtk_signal_connect_object(GTK_OBJECT(b_cleansweep), "clicked",
		GTK_SIGNAL_FUNC(mmef_cleansweep_clicked), GTK_OBJECT(fenster));

	gtk_window_set_title(GTK_WINDOW(fenster), "MM Editor");
    gtk_window_set_default_size(GTK_WINDOW(fenster),600,400);

//	gtk_window_set_position(GTK_WINDOW(fenster),GTK_WIN_POS_CENTER);

    viewr=mmef_build_resources_table();
    viewp=mmef_build_pages_table();
    viewi=mmef_build_items_table();
    
    swr=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(swr),viewr);
    swp=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(swp),viewp);
    swi=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(swi),viewi);

	gtk_container_set_border_width(GTK_CONTAINER(fenster),1);

	gtk_box_pack_start(GTK_BOX(box000), b_newpage, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(box000), b_globals, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(boxV00), b_load, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(boxV00), b_save, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(boxV00), b_exportdir, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(boxV00), b_cleansweep, TRUE, TRUE, 1);

	gtk_box_pack_start(GTK_BOX(box001), c_resources, TRUE,TRUE, 1);
	gtk_box_pack_start(GTK_BOX(box001), b_newresource, TRUE, TRUE, 1);

	gtk_box_pack_start(GTK_BOX(box00), box000, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(box00), GTK_WIDGET(swp), TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(box00), GTK_WIDGET(swi), TRUE, TRUE, 1);

	gtk_box_pack_start(GTK_BOX(box01), box001, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(box01), GTK_WIDGET(swr), TRUE, TRUE, 1);

	gtk_box_pack_start(GTK_BOX(box0), box01, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(box0), box00, TRUE, TRUE, 1);

	gtk_box_pack_start(GTK_BOX(box),box0, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(box),boxV00, FALSE, FALSE, 1);

	gtk_container_add(GTK_CONTAINER(fenster),box);

	gtk_widget_show_all(fenster);

    g_timeout_add_full(G_PRIORITY_DEFAULT,20,mmef_gtk_idle_func,NULL,NULL);  // HIGH_IDLE

	gtk_main();
	
	printf("gtk window done.\n");
	
	return 0;
}


int mmef_launch_editor() {
    MM_GTK_Thread=SDL_CreateThread((void *)thread_gtk,NULL);
    return(0);
}
