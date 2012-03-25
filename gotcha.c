#include <X11/Intrinsic.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

typedef struct _X11LogSess {
	Display *dpy;
	Window focuswin;
	FILE *fd;
} X11LogSess;

void gotcha (void);
double get_secs (void);
int main (int argc, char **argv);

void
gotcha (void)
{
	char filename[1000], cmd[1000], path[1000];

	sprintf (path, "/home/atw/gotcha");
	sprintf (filename, "%d.jpg", (int) get_secs ());
	sprintf (cmd, "uvccapture -m -o%s/%s", path, filename);

	system (cmd);

	sprintf (cmd, "chmod 400 %s/%s", path, filename);

	system (cmd);
}

double
get_secs (void)
{
        struct timeval tv;
	gettimeofday (&tv, NULL);
        return (tv.tv_sec + tv.tv_usec/1e6);
}

int
main (int argc, char **argv)
{
	X11LogSess *sess;
	FILE *fd;
	double now = get_secs ();
	double last_time = now;
	XEvent ev;
	int revert_to_return;
    
	if (!(sess = malloc (sizeof (struct _X11LogSess)))) {
		fprintf(stderr, ">> Error: %s\n", strerror(errno));
		sess = NULL;
	} else {
		sess->dpy = XOpenDisplay (getenv ("DISPLAY"));
            
		sess->focuswin = None;
        
		fd = stdout;

		sess->fd = fd;
	}
    
	if (sess == NULL) {
		fprintf (stderr, "Something went wrong. Aborting.\n");
		exit (1);
	}
    
	while (1) {
		now = get_secs ();

		if (sess->focuswin == None) {
			revert_to_return = 0;
			XGetInputFocus (sess->dpy, &(sess->focuswin),
					&revert_to_return);
			if (sess->focuswin != None) {
				XSelectInput (sess->dpy, sess->focuswin,
					      KeyPressMask | FocusChangeMask);
			}
		} else {
			XNextEvent (sess->dpy, &ev);
    
			if(ev.xany.type == FocusOut)
				sess->focuswin = None;
		}

		// seconds * minutes
		if (now - last_time > 60*10)
			gotcha ();

		last_time = now;
	}
     
	return (0);
}
