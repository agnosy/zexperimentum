#include "czmq_library.h"
#include "zproc.h"
#include "zyre.h"

void zce_actor (zsock_t *pipe, void *args);

// run command, wait until it ends and get the (stdandard) output
char const * const fx_execute(char const * const command)
{
    bool verbose = true;
    char * file = "/bin/ls"; 
    zproc_t *self = zproc_new ();
    assert (self);
    zproc_set_verbose (self, verbose);
    
    //  join stdout of the process to zeromq socket
    //  all data will be readable from zproc_stdout socket
    assert (!zproc_stdout (self));
    zproc_set_stdout (self, NULL);
    assert (zproc_stdout (self));
    
    zproc_set_argsx (self, file, "--help", NULL);
    
    if (verbose)
        zsys_debug("zproc_test() : launching helper '%s' --help", file );
    
    int r = zproc_run (self);
    assert (r == 0);
    zframe_t *frame;
    zsock_brecv (zproc_stdout (self), "f", &frame);
    assert (frame);
    assert (zframe_data (frame));
    // TODO: real test
    if (verbose) zframe_print (frame, "1:");
    zframe_destroy (&frame);
    r = zproc_wait (self, -1);
    assert (r == 0);
    zproc_destroy (&self);
}


int main (int argc, char *argv [])
{
    if (argc < 2) {
        fprintf (stderr, "Usage: %s <node-name>\n", argv[0]);
        exit (0);
    }
    zactor_t *actor = zactor_new (zce_actor, argv [1]);
    assert (actor);
    
    while (!zsys_interrupted) {
        char message [1024];
        if (!fgets (message, 1024, stdin)) break;
        message [strlen (message) - 1] = 0;
        zstr_sendx (actor, "SHOUT", message, NULL);
    }
    fprintf(stderr, "%s is leaving the network\n", argv [1]);
    zactor_destroy (&actor);
    return 0;
}

