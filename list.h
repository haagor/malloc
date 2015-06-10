

#ifndef _LIST_H_
#define _LIST_H_

/* Header de bloc */

#include <stdio.h>

#define MOST_RESTRICTING_TYPE double

typedef union header
{
    struct {
        unsigned int size;  /* Taille du bloc */
        union header *next; /* Bloc libre suivant */
    } info;
    MOST_RESTRICTING_TYPE dummy;
} Header;

size_t getRealSize(size_t size);
void *getRealPtr(void *ptr);

Header *getHeaderPtr(void *ptr);

void setSizePtr(void *ptr, size_t size, Header *next);
void setPtr(Header *ptr, Header *next);

void fusion(Header* here);

#endif
