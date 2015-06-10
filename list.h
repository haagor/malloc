

#ifndef _LIST_H_
#define _LIST_H_

/* Header de bloc */

#include <stdio.h>

typedef struct header
{
  size_t size;         /* Taille du bloc */
  struct header *next; /* Bloc libre suivant */
} Header;

size_t getRealSize(size_t size);
void *getRealPtr(void *ptr);

Header *getHeaderPtr(void *ptr);

void setSizePtr(void *ptr, size_t size, Header *next);
void setPtr(Header *ptr, Header *next);

void fusion(Header* here);

#endif
