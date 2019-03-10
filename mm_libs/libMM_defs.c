// just for when we need it :)
#define PI 3.1415926535897932384626433832795

// resource types
#define MM_TEXT                 1
#define MM_IMAGE                2
#define MM_VIDEO                3
#define MM_TEXT_BUTTON          4
#define MM_IMAGE_BUTTON         5
#define MM_FONT                 6
#define MM_SOUND                7
#define MM_ASIAANIM             9

// actions
#define MM_NO_ACTION            0
#define MM_JUMP_TO_PAGE         1
#define MM_VIDEO_PLAY           2
#define MM_VIDEO_STOP           3
#define MM_VIDEO_PAUSE          4
#define MM_AAD_PLAY_FRAME       5
#define MM_AAD_PLAY_ANIMATION   6
#define MM_AAD_SHOW_STILL       7
#define MM_AAD_CLEAR            8
#define MM_SOUND_PLAY           9
#define MM_SOUND_STOP           10
#define MM_SOUND_PAUSE          11
#define MM_SET_SBT              12

// video states
#define MM_VIDEO_STOPPED        0
#define MM_VIDEO_PLAYING        1
#define MM_VIDEO_PLEASE_STOP    2

// sbt action indices
#define MM_SBT_ACT_L            0
#define MM_SBT_ACT_M            1
#define MM_SBT_ACT_R            2
#define MM_SBT_ACT_E            3
#define MM_SBT_ACT_D            4
#define MM_SBT_ACT_NONE         99

#define MM_MODE_EDITOR          1
#define MM_MODE_PLAYER          2

// text alignments
#define MM_BLOCK                1
#define MM_LEFT                 2
#define MM_RIGHT                3
#define MM_CENTER               4

#define MM_PAGE                 1
#define MM_ITEM                 2
#define MM_RESOURCE             3
#define MM_SBT                  4

// aad mode flags
#define MM_AD_PLAYANIM          1
#define MM_AD_PLAYFRAME         2
#define MM_AD_CLEAR             3
#define MM_AD_STILL             4

#define MM_SBT_TextureSize      256 // 1024
#define MM_SBT_PointFactor      10 // 40

#define MM_TM_SURFACE		1
#define MM_TM_UPLOADED		2
#define MM_TM_REMOVE		3
#define MM_TM_VIDEOUPDATE	4
#define MM_TM_VIDEOGENUPD   5
#define MM_TM_VIDEOWAIT     6
#define MM_TM_RENDERAAD		7
#define MM_TM_GETTEXIMAGE	8

SDL_Thread *                MM_GTK_Thread;

SDL_mutex *					MM_RendererLock;

SDL_Surface *				MM_SDL_Window;
GLubyte						MM_SDL_FullScreen;
GLuint 						MM_ScreenBPP;
Uint32                      MM_ScreenWidthEditor;
Uint32                      MM_ScreenHeightEditor;
Uint32                      MM_ScreenWidthDisplay;
Uint32                      MM_ScreenHeightDisplay;
Uint32                      MM_ScreenWidthMax;
Uint32                      MM_ScreenHeightMax;
Uint32                      MM_ScreenFlags;
GLdouble                    MM_ScreenAspect;

Uint32                      MM_PATHMAX;
char *                      MM_PathBuf;

GLdouble                    MM_CameraDistance;
GLdouble                    MM_CameraAngle;

GLuint                      MM_FrameCounter;

GLuint                      MM_Run_Mode;
GLuint                      MM_EditorActionMode;

GLuint                      MM_Editor_Manual_Update;

GLuint                      MM_SBT_Running;
char *                      MM_SBT_DevName;
GLuint                      MM_SBT_DevBaud;

GLdouble                    MM_GridX;
GLdouble                    MM_GridY;
GLdouble                    MM_GridSizeX;
GLdouble                    MM_GridSizeY;
GLdouble                    MM_Grid_Snap;
GLdouble                    MM_Item_Snap;
GLdouble					MM_ModuloN;
GLdouble					MM_ModuloM;

GLdouble                    MM_DEF_GridX;
GLdouble                    MM_DEF_GridY;
GLdouble                    MM_DEF_TextLineHeight;
GLdouble                    MM_DEF_TextItemWidth;
GLdouble                    MM_DEF_TextBorderWidth;
GdkColor                    MM_DEF_FGL;
GdkColor                    MM_DEF_BGL;
GdkColor                    MM_DEF_FGH;
GdkColor                    MM_DEF_BGH;
GdkColor                    MM_DEF_PBG;
GdkColor                    MM_DEF_AFG;
GdkColor                    MM_DEF_ABG;

GLuint                      MM_GTKIDLER_REBUILD_ITEMS;
GLuint                      MM_GTKIDLER_REBUILD_PAGES;
GLuint                      MM_GTKIDLER_REBUILD_RESOURCES;
GLuint						MM_GTKIDLER_SHOW_WINDOW;
GLuint						MM_GTKIDLER_HIDE_WINDOW;

GLuint                      MM_SBT_ButtonAction;
GLuint						MM_SBT_LastButton;
SDL_mutex					* MM_SBTButtonLock;

void                        (*MM_EditorFlash)();

GLdouble					MM_CP_MINX,MM_CP_MAXX,MM_CP_MINY,MM_CP_MAXY;

SDL_Cursor                  * MM_CursorEmpty;
SDL_Cursor                  * MM_CursorDefault;

SDL_mutex                   * MM_SoundLock;
SDL_mutex                   * MM_VideoLock;
SDL_mutex                   * MM_VideoReadyMutex;
SDL_cond                    * MM_VideoReadyCond;

GLfloat                     MM_LightAmbient[]  = { 0.85, 0.85, 0.85, 1.0 };
GLfloat                     MM_LightDiffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat                     MM_LightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat                     MM_LightPosition[] = { 1.0, 1.0, -1000, 0.0 };
GLfloat                     MM_LightDirection[] = { 0.5, 0.5, 1.0 };

//-----------------------------------------------------------------------------

struct mms_texture {
  GLuint				status;
  GLuint                name;
  GLuint                w,h,a;
  GLubyte *          	sbt_bw;
  GLubyte *             rawpixels;
  SDL_Surface *			surf;
  struct mms_texture *	to_clone;
  struct mms_texture	*next,*prev;
}
mm_shadow_center,
mm_shadow_corner,
mm_shadow_edge,
* MM_Textures;

//-----------------------------------------------------------------------------

struct mms_page {
  GLuint                    id;
  char *                    name;
  struct mms_item *         items;
  struct mms_page *         next;
  struct mms_page *         prev;
  GdkColor                  bg_color;
  GLuint                    action;
  GLuint                    avalue;
} * MM_CurrentPage, * MM_Pages;


//-----------------------------------------------------------------------------

// structure holding everything for one sbt display
struct mms_sbt_display {
  char *              dev_name;       // filename of the device
  unsigned int        dev_baud;       // baudrate
  unsigned int        dev_file;       // file descriptor
  unsigned int        bn;             // number of braille elements
  unsigned char *     bd;             // current braille data
  unsigned char *     bv;             // current braille vibration pattern
  SDL_Thread *        th_proc;        // processing thread 
  SDL_Thread *        th_vibe;        // vibration thread
  unsigned int        th_proc_exit;   // thread proc exit signal
  unsigned int        th_vibe_exit;   // thread vibe exit signal
//  GLuint              texture;        // gl texture
  GLint               tw,th;          // gl texture dimensiones
  unsigned char *     taxel;          // texture as taxel map
  GLdouble            bxS,byS,bxE,byE;// current pixel sbt position, start and and
  GLdouble            bw,bh;          // width and height of braille mouse in pixel
  GLdouble            mx,my;          // current physical sbt position (upper left taxel)
  GLdouble            mmw,mmh;        // physical picture size in mm
  GLdouble            mmox,mmoy;      // physical picture offset in mm
  GLdouble            ppm;            // pixel per mm
  unsigned int        inv;            // invert the display?
  unsigned char       chars[256];     // ascii
  unsigned char *     text;           // view text
  unsigned char       lock;           // lock display for custom or text viewing
  struct mms_texture *texture;
  Uint32				vflip,bgc,fgc;
} * MM_SBT_Display;
                                                                                    

//-----------------------------------------------------------------------------

struct mms_evalanim {
  void * eval_rx;
  void * eval_ry;
  void * eval_rz;
  void * eval_tx;
  void * eval_ty;
  void * eval_tz;
  void * eval_sw;
  void * eval_sh;
  char * str_rx;
  char * str_ry;
  char * str_rz;
  char * str_tx;
  char * str_ty;
  char * str_tz;
  char * str_sw;
  char * str_sh;
  GLdouble  playtime; // total time in ms
  GLdouble  starttime;  // sdl ticks at start/1000
  GLdouble  timescale;   // 1...0 of animation
  GLdouble  random;
} * MM_AnimationClipboard;

//-----------------------------------------------------------------------------

struct mms_item {
  GLuint                    id;     // id
  GLuint                    res_id; // corresponding resource id
  GLuint                    type;   // button, image, video...
  void *                    data;   // mms_button, mms_image...
  GLdouble                  minx,miny; // position upper left corner
  GLdouble                  maxx,maxy; // position lower right corner
  GLuint                    action;
  GLuint                    avalue;
  GLuint                    is_background;
  GLuint                    drop_shadow;
  GLuint                    light;
  char *                    text;
  struct mms_item *         next;   // next item in page
  struct mms_item *         prev;   // previous item in page
  
  GLuint                    is_sbt;
  GLuint                    sbtact[5];
  GLuint                    sbtava[5];
  
  struct mms_evalanim       animakt;
} * MM_Edit_Item, * MM_CurrentSBTItem;

//-----------------------------------------------------------------------------

struct mms_resource {
  GLuint                    id;     // resource id
  GLuint                    type;   // button, image, video...
  void *                    data;   // mms_button, mms_image...
  char *                    text;
  struct mms_resource *     next;   // next item in page
  struct mms_resource *     prev;   // previous item in page
} * MM_Resources;

//-----------------------------------------------------------------------------

struct mms_button {
  GLuint                    flags;
  GLuint                    type;
  GLdouble                  w,h;
  GLdouble                  build_w,build_h,build_b;
  GLuint                    font_resource;
  struct mms_texture *      texture_low;
  struct mms_texture *      texture_high;
  GdkColor                  fgl,bgl,fgh,bgh;
  char *                    text;         
  char *                    path;
  GLdouble                  bow;
} * MM_Buttons;

struct mms_image {
  GLuint                    type;  
  GLdouble                  w,h;
  struct mms_texture *      texture;
  char *                    text;
  char *                    path;         
};

struct mms_text {
  GLuint                    type;
  GLuint                    flags;
  GLdouble                  w,h;
  struct mms_texture *      texture;
  char *                    text;         
  char *                    full_text;
  GLuint                    font_resource;
  GLdouble                  field_width,border_width,line_height;
  GdkColor                  fg,bg;
  GLuint                    align;
  GLuint					alpha;
  GLdouble                  bow;
};

struct mms_sound {
  GLuint                    type;
  SDL_AudioSpec             spec;
  Uint32                    len;
  Uint8 *                   buffer;
  GLuint                    is_playing;
  Uint8                     * play_pos;
  Uint32                    play_remain;
  char *                    path;
  char *                    text;
};

struct aad_stroke {
  int                   nv;
  double                v[4*2048];
  char                  name[64];
};
           
struct aad_frame {
  int                   ns;
  int                   ticks;
  struct aad_stroke *   s[2048];
  char                  name[64];
  struct aad_frame *    next;
  struct aad_frame *    prev;
};
                   

struct mms_asiaanim {
  GLuint                    flags;
  GLuint                    type;
  GLdouble                  w,h,b;
  GLdouble                  build_w,build_h,build_b;
  GLdouble                  origw,origh;
  GLuint                    is_playing;
  
  GdkColor                  bg_color;
  GdkColor                  fg_color;
  
  GLuint                    aa_totalticks;
  GLuint                    aa_currenttick;
  GLuint                    aa_oldtick;
  
  GLuint                    nf;
  struct aad_frame *        root;
  struct aad_frame *        current_frame;
  struct aad_frame **       all_frames;
  
  struct mms_texture *      texture;

  char *                    text;
  char *                    path;
};

struct mms_video {
  GLuint                    type;
  GLdouble                  w,h;
  struct mms_texture *      texture;
  int                       filedesc;
  GLuint                    norm;
  dv_decoder_t              *decoder;
  GLuint                    chunksize;
  GLuint                    framesize;
  GLuint                    fcnt,fidx;
  GLuint                    wide;
  //gint16
  Uint8                    *audio_buffers[4];
  int                       libdv_pitches;
  unsigned char             raw_in[144000];
  unsigned char             *pixels;
  char *                    text;
  char *                    path;

  Uint8                     buffer[2048*4];
  GLuint                    is_playing;
  Uint8                     * play_pos;
  Uint32                    play_remain;

};

struct mms_font {
  GLuint                    type;
  char *                    text;
  char *                    path;
};

//-----------------------------------------------------------------------------

struct mms_geometry {
  GLdouble x,y,w,h;
};

//-----------------------------------------------------------------------------

struct mms_texture  *MM_EditorPrevTexture;
struct mms_geometry MM_EditorPrevGeometry;

//-----------------------------------------------------------------------------

