mmef_gtk_rebuild_resources_view() {
  GtkTreePath * path;
  
  gtk_tree_store_clear(treestore_resources);
  mmef_fill_resources_tree();
  path=gtk_tree_path_new_from_indices(MM_RROW_0,MM_RROW_1,-1);
  gtk_tree_view_expand_to_path(GTK_TREE_VIEW(viewr),path);
  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(viewr),path,NULL,FALSE,0.5,0);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(viewr),path,gtk_tree_view_get_column(GTK_TREE_VIEW(viewr),1),0);
  gtk_widget_grab_focus(viewr);
}

mmef_gtk_rebuild_items_view() {
  GtkTreePath * path;
  
  gtk_tree_store_clear(treestore_items);
  mmef_fill_items_tree();
  path=gtk_tree_path_new_from_indices(MM_IROW_0,-1);
  gtk_tree_view_expand_to_path(GTK_TREE_VIEW(viewi),path);
  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(viewi),path,NULL,FALSE,0.5,0);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(viewi),path,gtk_tree_view_get_column(GTK_TREE_VIEW(viewi),1),0);
  gtk_widget_grab_focus(viewi);
}

mmef_gtk_rebuild_pages_view() {
  GtkTreePath * path;
  
  gtk_tree_store_clear(treestore_pages);
  mmef_fill_pages_tree();
  path=gtk_tree_path_new_from_indices(MM_PROW_0,-1);
  gtk_tree_view_expand_to_path(GTK_TREE_VIEW(viewp),path);
  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(viewp),path,NULL,FALSE,0.5,0);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(viewp),path,gtk_tree_view_get_column(GTK_TREE_VIEW(viewp),1),0);
  gtk_widget_grab_focus(viewp);
}
