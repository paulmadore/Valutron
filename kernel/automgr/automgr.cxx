/*
 * Automgr Tracing GC for Valutron
 * Copyright (c) 2015 D. Mackay. All rights reserved.
 * Use is subject to licence terms.
 */

#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include "automgr.h"

typedef struct _zoneTblEnt
{
    void * start;
    size_t length : 26;
    /* If it is root, then it is never un-marked. */
    bool marked : 1, traced : 1, root : 1;

    struct _zoneTblEnt * next;
} zoneTblEnt;

typedef struct _Automgr
{
    bool enabled;
    unsigned short aCnt;
    zoneTblEnt * zoneTbl;
} Automgr;

static Automgr mgr;
static size_t wordSize = sizeof (uintptr_t);

void AMGR_delete (zoneTblEnt * toFree);

#define AM_alloc(bytes) malloc (bytes)
#define AM_free(addr) free (addr)

Automgr * AMGR_init ()
{
    mgr.zoneTbl = 0;
    mgr.enabled = false;
    return &mgr;
}

void AMGR_enable () { mgr.enabled = true; }

void AMGR_disable () { mgr.enabled = false; }

void AMGR_add_zone (void * start, size_t length, bool isRoot)
{
    zoneTblEnt * newEntry = (zoneTblEnt *)AM_alloc (sizeof (zoneTblEnt));

    newEntry->start  = start;
    newEntry->length = length;
    newEntry->root   = isRoot;
    newEntry->marked = false;
    newEntry->traced = false;

    newEntry->next = mgr.zoneTbl;
    mgr.zoneTbl    = newEntry;
}

void AMGR_remove_zone (void * location)
{
    zoneTblEnt ** it = &mgr.zoneTbl;
    while (*it)
    {
        if ((*it)->start == location)
        {
            zoneTblEnt * toFree = *it;
            *it = toFree->next;
            AM_free (toFree);
        }
        else
            it = &(*it)->next;
    }
}

zoneTblEnt * AMGR_find_zone (void * location)
{
    zoneTblEnt ** it;

    for (it = &mgr.zoneTbl; *it; it = &(*it)->next)
    {
        if ((*it)->start == location)
            return *it;
    }
    return 0;
}

void AMGR_remove_all_zones ()
{
    zoneTblEnt ** it = &mgr.zoneTbl;
    while (*it)
    {
        zoneTblEnt * toFree = *it;
        *it = toFree->next;
        AM_free (toFree);
    }
}

/* CStdLib equivalents */

inline void AMGR_checkCycle ()
{
    if (mgr.aCnt++ >= 48)
    {
        mgr.aCnt = 0;
        AMGR_cycle ();
    }
}

void * AMGR_oalloc (size_t bytes)
{
    void * ptr = calloc (1, bytes);
    AMGR_checkCycle ();
    AMGR_add_zone (ptr, bytes, false);

    return ptr;
}

void * AMGR_realloc (void * location, size_t newBytes)
{
    zoneTblEnt * zone = AMGR_find_zone (location);
    void * newPtr     = realloc (location, newBytes);

    zone->start  = newPtr;
    zone->length = newBytes;

    return newPtr;
}

void AMGR_free (void * location)
{
    AMGR_remove_zone (location);
    free (location);
}

/* This method will, if `location' matches the address of a zone,
 * mark it.
 * Should this check if location falls within the range of a zone, or
 * just equality? */
zoneTblEnt * AMGR_ptr_for_address (uintptr_t location)
{
    zoneTblEnt ** it;

    for (it = &mgr.zoneTbl; *it != 0; it = &(*it)->next)
    {
        if ((*it)->start == (void *)location)
        {
            (*it)->marked = true;
            return *it;
        }
    }
    return 0;
}

void AMGR_unmark_all_zones ()
{
    zoneTblEnt ** it;

    for (it = &mgr.zoneTbl; *it != 0; it = &(*it)->next)
        ((*it)->marked = false, (*it)->traced = false);
}

/* Tracing */

void AMGR_trace_zone (zoneTblEnt * zone)
{
    uintptr_t * curPtr;

    if (zone->traced)
        return;
    else
        zone->traced = true;

    for (curPtr = (uintptr_t *)zone->start;
         curPtr <= (uintptr_t *)zone->start + zone->length;
         curPtr = (uintptr_t *)(((char *)curPtr) + wordSize))
    {
        zoneTblEnt * found;

        if (*curPtr % wordSize)
            continue;
        else if ((found = AMGR_ptr_for_address (*curPtr)))
            AMGR_trace_zone (found);
    }
}

void AMGR_trace_roots ()
{
    zoneTblEnt ** it;
    for (it = &mgr.zoneTbl; *it != 0; it = &(*it)->next)
        if ((*it)->root)
            AMGR_trace_zone (*it);
}

void AMGR_trace ()
{
    AMGR_unmark_all_zones ();
    AMGR_trace_roots ();
}

/* Sweeping */

void AMGR_delete (zoneTblEnt * toFree)
{
    void * loc;

    loc = toFree->start;

    free (loc);
}

void AMGR_sweep ()
{
    zoneTblEnt ** it = &mgr.zoneTbl;
    while (*it)
    {
        if (!(*it)->marked && !(*it)->root)
        {
            zoneTblEnt * toFree = *it;
            *it = toFree->next;
            AMGR_delete (toFree);
            AM_free (toFree);
        }
        else
            it = &(*it)->next;
    }
}

void AMGR_cycle ()
{
    if (!mgr.enabled)
        return;
    AMGR_trace ();
    AMGR_sweep ();
}