#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>


void key_press(Display *dpy, unsigned int keycode) {
  XTestFakeKeyEvent(dpy, keycode, True, 0);	
  XTestFakeKeyEvent(dpy, keycode, False, 0);	
  
  XFlush(dpy);
}

int main(int argc, char**argv)
{
  Display    *dpy;            /* X server connection */
  int xtest_major_version = 0;
  int xtest_minor_version = 0;
  int dummy;
  
  /*
   * Open the display using the $DISPLAY environment variable to locate
   * the X server.  See Section 2.1.
   */
  if ((dpy = XOpenDisplay(NULL)) == NULL) {
    fprintf(stderr, "%s: can't open %s\en", argv[0], XDisplayName(NULL));
    exit(1);
  }
  
  Bool success = XTestQueryExtension(dpy, &dummy, &dummy,
				     &xtest_major_version, &xtest_minor_version);
  if(success == False || xtest_major_version < 2 ||
     (xtest_major_version <= 2 && xtest_minor_version < 2))
    {
      fprintf(stderr,"XTEST extension not supported. Can't continue\n");
      exit(1);
    }
  
  key_press(dpy, 'D');
  return 0;
}
