GtkWidget *b_newpage;
GtkWidget *b_newresource;
GtkWidget *b_globals;
GtkWidget *b_load;
GtkWidget *b_save;
GtkWidget *b_exportdir;
GtkWidget *b_cleansweep;

GtkWidget *c_resources;

GtkWidget *fenster;
GtkWidget *dialog;
GtkWidget *question;

GtkWidget *file_select;
GtkWidget *editor_pages_view;
GtkWidget *editor_pages_liststore;

GtkWidget *uni_button01;
GtkWidget *uni_button02;
GtkWidget *uni_button03;
GtkWidget *uni_button04;
GtkWidget *uni_button05;
GtkWidget *uni_button06;
GtkWidget *uni_text_single01;
GtkWidget *uni_text_single02;
GtkWidget *uni_text_single03;
GtkWidget *uni_text_single04;
GtkWidget *uni_text_single05;
GtkWidget *uni_text_single06;
GtkWidget *uni_text_single07;
GtkWidget *uni_text_single08;
GtkWidget *uni_text_single09;
GtkWidget *uni_text_single10;
GtkWidget *uni_text_single11;
GtkWidget *uni_text_single12;
GtkWidget *uni_text_single13;
GtkWidget *uni_text_single14;
GtkWidget *uni_text_single15;
GtkWidget *uni_text_single16;
GtkWidget *uni_label01;
GtkWidget *uni_label02;
GtkWidget *uni_label03;
GtkWidget *uni_label04;
GtkWidget *uni_label05;
GtkWidget *uni_label06;
GtkWidget *uni_label07;
GtkWidget *uni_label08;
GtkWidget *uni_label09;
GtkWidget *uni_label10;
GtkWidget *uni_label11;
GtkWidget *uni_label12;
GtkWidget *uni_label13;
GtkWidget *uni_label14;
GtkWidget *uni_label15;
GtkWidget *uni_label16;
GtkWidget *uni_filebutton;
GtkWidget *uni_spin01;
GtkWidget *uni_spin02;
GtkWidget *uni_spin03;
GtkWidget *uni_spin04;
GtkWidget *uni_spin05;
GtkWidget *uni_spin06;
GtkWidget *uni_spin07;
GtkWidget *uni_spin08;
GtkWidget *uni_hscale01;
GtkWidget *uni_hscale02;
GtkWidget *uni_hscale03;
GtkWidget *uni_hscale04;
GtkWidget *uni_combox01;
GtkWidget *uni_combox02;
GtkWidget *uni_colbut01;
GtkWidget *uni_colbut02;
GtkWidget *uni_colbut03;
GtkWidget *uni_colbut04;
GtkWidget *uni_colbut05;
GtkWidget *uni_colbut06;
GtkWidget *uni_colbut07;
GtkWidget *uni_check01;
GtkWidget *uni_toggle01;
GtkWidget *uni_toggle02;
GtkWidget *uni_toggle03;
GtkWidget *uni_toggle04;
GtkWidget *uni_toggle05;
GtkWidget *uni_toggle06;
GtkWidget *uni_toggle07;
GtkWidget *uni_toggle08;

GtkWidget *uni_nb1;
GtkWidget *uni_nb2;
GtkWidget *uni_nb3;
GtkWidget *uni_nb4;
GtkWidget *uni_nb5;
GtkWidget *uni_nb6;
GtkWidget *uni_nb7;
GtkWidget *uni_nb8;

/*
GtkListStore *uni_store01;
GtkListStore *uni_store02;
GtkCellRenderer *uni_textrender01;
GtkCellRenderer *uni_textrender02;
GtkTreeIter uni_iter;
*/

GtkListStore *action_store[8];
GtkListStore *avalue_store[8];
GtkCellRenderer *action_textrender[8];
GtkCellRenderer *avalue_textrender[8];
GtkTreeIter action_iter[8];
GtkWidget   *action_combox[8];
GtkWidget   *avalue_combox[8];
GtkWidget   *action_label[8];


GtkWidget *ml_view;
GtkTextBuffer *ml_buffer;

GtkTreeStore  *treestore_items;
GtkTreeStore  *treestore_resources;
GtkTreeStore  *treestore_pages;

GtkWidget *viewr,*viewp,*viewi,*MM_GTKIDLER_FOCUS_WIDGET;

enum { TARGET_STRING, TARGET_URL};
static GtkTargetEntry targetentries[]= {
  { "STRING",0,TARGET_STRING },
  { "text/plain",0,TARGET_STRING },
  { "text/uri-list",0,TARGET_URL },
};



enum {
  COL_RES_TYPE = 0,
  COL_RES_NAME,
  COL_RES_ID,
  NUM_RES_COLS
} ;

enum {
  COL_PGS_NAME = 0,
  COL_PGS_ID,
  NUM_PGS_COLS
} ;

enum {
  COL_ITM_NAME = 0,
  COL_ITM_TYPE,
  COL_ITM_ID,
  NUM_ITM_COLS
} ;


GLuint MM_RROW_0,MM_RROW_1;
struct mms_resource * MM_RROW_FocusResource;

GLuint MM_IROW_0;
struct mms_item * MM_IROW_FocusItem;

GLuint MM_PROW_0;
struct mms_page * MM_PROW_FocusPage;
