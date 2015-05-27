
#include "list.h"

size_t getRealSize(size_t size)
{
  return size + sizeof(Header);
}

void *getRealPtr(void *ptr)
{
  return ptr + sizeof(Header);
}

Header *getHeaderPtr(void *ptr)
{
  return (Header*)(ptr - sizeof(Header));
}

void setSizePtr(void *ptr, size_t size, Header *next)
{
  Header* header = (Header*)ptr;
  header->size = size;  // équivalent: (*header).size = size;
  header->next = next;
}

void setPtr(Header *ptr, Header *next)
{
  ptr->next = next;
}


void fusion(Header* here)
{
  if (getRealPtr(here) + here->size == here->next)
  {
    here->size = here->size + sizeof(Header) + here->next->size;
    here->next = here->next->next;
  }
  else
  {
    here = here->next;
  }
  if (getRealPtr(here) + here->size == here->next)
  {
    here->size = here->size + sizeof(Header) + here->next->size;
    here->next = here->next->next;
  }
}

