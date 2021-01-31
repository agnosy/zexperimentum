#include <czmq.h>

int main (int argc, char *argv [])
{
    char * file = "/bin/ls"; 
    zproc_t *self = zproc_new ();
    zproc_set_verbose (self, false);
    zproc_set_stdout (self, NULL);
    
    zproc_set_argsx (self, file, "-laFtrh", NULL);
    
    assert (zproc_run (self) == 0);
    zframe_t *frame;
    zsock_brecv (zproc_stdout (self), "f", &frame);
	fprintf(stderr, "frame-data: [%s]\n", (char *) zframe_data(frame));
    zframe_destroy (&frame);
    assert (zproc_wait (self, -1) == 0);
    zproc_destroy (&self);
}


