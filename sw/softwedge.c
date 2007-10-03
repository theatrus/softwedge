
/*
    softwedge - serial to X11 keyboard software wedge
    Copyright (C) 2007 Yann Ramin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "softwedge.h"

static int serialPort;
static Display *dpy;

static void xtest_key_press(unsigned char letter) {
  unsigned int shiftcode = XKeysymToKeycode(dpy, XStringToKeysym("Shift_L"));
  int upper = 0;
  int skip_lookup = 0;
  char s[2];
  s[0] = letter;
  s[1] = 0;
  KeySym sym = XStringToKeysym(s);
  KeyCode keycode;



  if (sym == 0) {
    sym = letter;
  }


  if (sym == '\n') {
    sym = XK_Return;
    skip_lookup = 1;

  } else if (sym == '\t') {
    sym = XK_Tab;
    skip_lookup = 1;
  }

  keycode = XKeysymToKeycode(dpy, sym);
  if (keycode == 0) {
    sym = 0xff00 | letter;
    keycode = XKeysymToKeycode(dpy, sym);
    
  }

  if (!skip_lookup) {
    // Here we try to determine if a keysym
    // needs a modifier key (shift), such as a
    // shifted letter or symbol.
    // The second keysym should be the shifted char
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
  }

  if (upper)
    XTestFakeKeyEvent(dpy, shiftcode, True, 0);	

  
  XTestFakeKeyEvent(dpy, keycode, True, 0);	
  XTestFakeKeyEvent(dpy, keycode, False, 0);

  if (upper)
    XTestFakeKeyEvent(dpy, shiftcode, False, 0);	

  

}

static void press_keys(char* string) {
  int len = strlen(string);
  int i = 0;
  for (i = 0; i < len; i++) {
    xtest_key_press(string[i]);
  }
  XFlush(dpy);
}


int sw_open_serial(const char *port) {
  serialPort = open(port, O_RDONLY);
  if (serialPort < 0) {
    fprintf(stderr, "Can't open serial port: %s\n", port);
    exit(-1);
  }

  return 0;
}

void sw_init() {

  int xtest_major_version = 0;
  int xtest_minor_version = 0;
  int dummy;

  
  /*
   * Open the display using the $DISPLAY environment variable to locate
   * the X server.  See Section 2.1.
   */
  if ((dpy = XOpenDisplay(NULL)) == NULL) {
    fprintf(stderr, "%s: can't open %s\en", "softwedge", XDisplayName(NULL));
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

}


void sw_read_loop() {

  char readbuf[2];
  readbuf[1] = 0;

  while(read(serialPort, readbuf, 1) > 0) {

    press_keys(readbuf);

  }


  // We're done now
  close(serialPort);





}
