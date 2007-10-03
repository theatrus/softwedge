#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>


void xtest_key_press(Display *dpy, unsigned char letter) {
  unsigned int shiftcode = XKeysymToKeycode(dpy, XStringToKeysym("Shift_L"));
  int upper = 0;
  char s[2];
  s[0] = letter;
  s[1] = 0;
  KeySym sym = XStringToKeysym(s);
  KeyCode keycode;



  if (sym == 0) {
    sym = letter;
  }



  keycode = XKeysymToKeycode(dpy, sym);

  
  KeySym *syms;
  int keysyms_per_keycode;
  syms = XGetKeyboardMapping(dpy, keycode, 1, &keysyms_per_keycode);
  int i = 0;
  for (i = 0; i <= keysyms_per_keycode; i++) {
    if (syms[i] == 0)
      break;

    if (i == 0 && syms[i] != letter)
      upper = 1;
    
    
  }




  if (upper)
    XTestFakeKeyEvent(dpy, shiftcode, True, 0);	

  
  XTestFakeKeyEvent(dpy, keycode, True, 0);	
  XTestFakeKeyEvent(dpy, keycode, False, 0);

  if (upper)
    XTestFakeKeyEvent(dpy, shiftcode, False, 0);	

  
  XFlush(dpy);
}

void press_keys(Display *dpy, char* string) {
  int len = strlen(string);
  int i = 0;
  for (i = 0; i < len; i++) {
    xtest_key_press(dpy, string[i]);
  }
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
  
  char s[] = "Hello world!";


  if(fork()) {
    return 0;
  }

  close(0);
  close(1);
  close(2);


  press_keys(dpy, s);



  return 0;
}
