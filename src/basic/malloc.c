#include <stddef.h>
#include "malloc.h"
#include "os.h"
#include "error.h"
#include "time.h"
#include "../uart/uart.h"

//Temporarly used from https://github.com/jserv/mini-arm-os/tree/master

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned int size;
    } s;
    Align x;
};

typedef union header Header;

//  0 - 73741824 bytes size assigned for TEXT and DATA, HEAP STARTS at adress 73741824
static unsigned long program_break = 93741824;

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */
int mallocLock = 0;

static void *sbrk(unsigned int nbytes)
{
    unsigned char *previous_pb = program_break;
    program_break += nbytes;
    return (void *) previous_pb;
}

void *malloc(unsigned int nbytes)
{
    if(mallocLock){
        wait_msec(500);
    }
    mallocLock = 1;
    Header *p, *prevp;
    unsigned int nunits;
    void *cp;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            } else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            mallocLock = 0;
            return (void *)(p + 1);
        }

        if (p == freep) {
            cp = sbrk(nunits * sizeof(Header));
            if (cp == (void *) -1) {
                throw("Error while allocating memory. No more memory available.");
                mallocLock = 0;
                return NULL;
            } else {
                p = (Header *) cp;
                p->s.size = nunits;
                free((void *) (p + 1));
                p = freep;
            }
        }
    }
}

void free(void *ap)
{
    Header *bp, *p;
    bp = (Header *) ap - 1;

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;
    }

    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }

    freep = p;
}