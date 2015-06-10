/*
 * my-malloc.c 	-- Implementation de malloc, free, calloc, realloc
 *
 * Implémentation first-fit pour malloc
 *
 * Author: Simon Paris
 * Author: Pascal Tung
 */

#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include "my-malloc.h"
#include "list.h"


static int nb_alloc           = 0; /* nombre de fois où on alloué     */
static int nb_dealloc         = 0; /* nombre de fois où on désalloué  */
static int nb_sbrk            = 0; /* nombre de fois où a appelé sbrk */
static size_t sbrk_block_size = 1024; /* taille des blocs sbrk */

static Header freelistStart  = { 0, &freelistStart }; /* le bloc initial de taiile nulle */
static Header* freelist = &freelistStart; /* pointeur sur la liste des blocs libres */


/*
 * Ajoute un  bloc à la freelist dans l'ordre croissant des pointeurs.
 */
void insert(Header* insert)
{
  Header* header = freelist;
  Header* next = header->info.next;
  do
  {
    if (header < insert && insert < next)
    {
      header->info.next = insert;
      insert->info.next = next;
      fusion(header);
      return;
    }
    header = next;
    next = header->info.next;
  } while (0 != next->info.size);
  // insertion en fin de liste
  header->info.next = insert;
  insert->info.next = freelist;
  fusion(header);
}

/*
 * Trouve un bloc libre de taille supérieure ou égale à size. Coupe ou supprime le bloc.
 */
void* getBlock(size_t size)
{
  Header* prev   = freelist;
  Header* header = freelist->info.next;
  while (0 != header->info.size) // le 1er block a une taille 0
  {
    if (size <= header->info.size)
    {
      if (size + sizeof(Header) >= header->info.size)
      {
        // supprimer le bloc
        prev->info.next = header->info.next;
        setPtr(header, 0);
      }
      else
      {
        // couper le bloc : attention il faut avoir la place pour le header
        Header* newHeader = (Header*)((void*)header + size + sizeof(Header));
        setSizePtr(newHeader, header->info.size - size - sizeof(Header), header->info.next);
        prev->info.next = newHeader;
        setSizePtr(header, size, 0);
      }
      return getRealPtr(header);
    }
    prev   = header;
    header = prev->info.next;
  }

  return 0;
}

//----------------------------------------------------------------------
void *mymalloc(size_t size) {
  nb_alloc += 1;

  void* result;

  while (0 == (result = getBlock(size)))
  {
    void* free_mem = sbrk(sbrk_block_size);

    setSizePtr(free_mem, sbrk_block_size - sizeof(Header), 0);
    insert(free_mem);

    nb_sbrk += 1;
  }

  return result;
}


void myfree(void *ptr) {
  nb_dealloc += 1;
  insert(getHeaderPtr(ptr));
}

void *mycalloc(size_t nmemb, size_t size) {
  nb_alloc += 1;
  void* result = mymalloc(nmemb * size);
  if (result == 0)
  {
    return 0;
  }
  bzero(result, nmemb * size);
  return result;
}


void *myrealloc(void *ptr, size_t size) {
  Header* header = getHeaderPtr(ptr);
  size_t minSize = header->info.size;
  if (size < minSize)
  {
    minSize = size;
  }
  myfree(ptr);
  void* result = mymalloc(size);
  if (result == 0)
  {
    return 0;
  }
  for (size_t i = 0; i < minSize; i++)
  {
    *((unsigned char*)(result + i)) = *((unsigned char*)(ptr + i));
  }
  return result;
}

#ifdef MALLOC_DBG
void mymalloc_infos(char *msg, void** tab, int tabLength) {
  if (msg) fprintf(stderr, "**********\n*** %s\n", msg);

  fprintf(stderr, "# allocs = %3d - # deallocs = %3d - # sbrk = %3d\n",
          nb_alloc, nb_dealloc, nb_sbrk);

  Header* header = freelist;
  do {
    fprintf(stderr, "Block @ %p (size = %3zu, next %p)\n", header, header->info.size, header->info.next);
    // Block @ 0x601570 (size=   0, next 0x13270a0)
    header = header->info.next;
  } while (0 != header->info.size);

  /*for (int i = 0; i < tabLength; i++) {
    if (tab[i] != 0) {
      Header* h = (Header*)(tab[i] - sizeof(Header));
      fprintf(stderr, "Tab @ %p (size = %3zu, next %p)\n", h, h->info.size, h->info.next);
    }
    }*/

  if (msg) fprintf(stderr, "**********\n\n");
}
#endif
