#include "zyre.h"

char const * const fx_execute(char const * const command)
{
	return command;
}

void zce_actor (zsock_t *pipe, void *args)
{
    zyre_t *node = zyre_new ((char *) args);
    if (!node) return;

    zyre_start (node);
    zyre_join (node, "EXECUTION");
    zsock_signal (pipe, 0);

    bool terminated = false;
    zpoller_t *poller = zpoller_new (pipe, zyre_socket (node), NULL);
    while (!terminated) {
        void *which = zpoller_wait (poller, -1);
        if (which == pipe) {
            zmsg_t *msg = zmsg_recv (which);
            if (!msg) break;

            char *command = zmsg_popstr (msg);
            if (streq (command, "$TERM"))
                terminated = true;
            else if (streq (command, "SHOUT")) { 
                char *string = zmsg_popstr (msg);
                // zyre_shouts (node, "EXECUTION", "%s", string);
                zyre_shouts (node, "EXECUTION", "%s", fx_execute(string));
            } else {
                puts ("E: invalid message to actor");
                assert (false);
            }
            free (command);
            zmsg_destroy (&msg);
        }
        else
        if (which == zyre_socket (node)) {
            zmsg_t *msg = zmsg_recv (which);
            char *event = zmsg_popstr (msg);
            char *peer = zmsg_popstr (msg);
            char *name = zmsg_popstr (msg);
            char *group = zmsg_popstr (msg);
            char *message = zmsg_popstr (msg);

            if (streq (event, "ENTER"))
                printf ("%s has joined the network\n", name);
            else if (streq (event, "EXIT"))
                printf ("%s has left the network\n", name);
            else if (streq (event, "SHOUT"))
                printf ("%s: %s\n", name, message);
            else if (streq (event, "EVASIVE"))
                printf ("%s is being evasive\n", name);
            else if (streq (event, "SILENT"))
                printf ("%s is being silent\n", name);

            free (event);
            free (peer);
            free (name);
            free (group);
            free (message);
            zmsg_destroy (&msg);
        }
    }
    zpoller_destroy (&poller);
    zyre_stop (node);
    zclock_sleep (100);
    zyre_destroy (&node);
}

