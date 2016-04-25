//
// Created by moslab on 4/19/16.
//
#include "dalvik_hook.h"

int dalvik_hook_setup(struct dalvik_hook_t *h, char *cls, char *meth, char *sig, int ns, void *func)
{
    if (!h)
        return 0;

    strcpy(h->clname, cls);
    strncpy(h->clnamep, cls+1, strlen(cls)-2);
    strcpy(h->method_name, meth);
    strcpy(h->method_sig, sig);
    h->n_iss = ns;
    h->n_rss = ns;
    h->n_oss = 0;
    h->native_func = func;

    h->sm = 0; // set by hand if needed

    h->af = 0x0100; // native, modify by hand if needed

    h->resolvm = 0; // don't resolve method on-the-fly, change by hand if needed

    h->debug_me = 0;

    return 1;
}
