
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

int main(int argc, char**argv)
{
  int c;
  int dontDaemon = 0;
  char *sport = NULL;

  while ((c = getopt (argc, argv, "fvc:")) != -1)
    switch (c)
      {
      case 'f':
	fprintf(stderr, "softwedge not daemonizing...\n");
	dontDaemon = 1;
	break;
      case 'v':
	fprintf(stderr, "softwedge v %s: The serial softwedge X11 helper. ", SOFTWEDGE_VERSION);
	fprintf(stderr, "(c) 2007 Yann Ramin <atrus@stackworks.net>\n(Exiting...)\n");
	exit(0);
      case 'c':
	sport = optarg;
	break;
      case '?':
	if (optopt == 'c')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	return 1;
      default:
	abort ();
      }

  if (sport == NULL)
    sport = DEFAULT_SERIAL;

  sw_init();

  sw_open_serial(sport);



  if (!dontDaemon) {
    if(fork()) {
      return 0;
    }
    
    close(0);
    close(1);
    close(2);
  }

  // Loop forever reading
  sw_read_loop();

  return 0;
}
