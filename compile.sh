gcc mm.c -o mm \
  $(xml2-config --libs --cflags) \
  $(pkg-config gtk+-2.0 --libs --cflags) \
  $(pkg-config gdk-2.0 --libs --cflags) \
  $(sdl-config --libs --cflags) \
  -L/usr/X11R6/lib \
  -lm -lGL -lGLU \
  -lSDL_ttf -lSDL_image -lSDL_gfx\
  -ldv \
  -lmatheval \
  -mtune=athlon64 -O99 -msse2 -msse \
  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64\

