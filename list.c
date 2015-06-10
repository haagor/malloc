
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
  header->info.size = size;
  header->info.next = next;
}

void setPtr(Header *ptr, Header *next)
{
  ptr->info.next = next;
}


void fusion(Header* here)
{
  if (getRealPtr(here) + here->info.size == here->info.next)
  {
    here->info.size = here->info.size + sizeof(Header) + here->info.next->info.size;
    here->info.next = here->info.next->info.next;
  }
  else
  {
    here = here->info.next;
  }
  if (getRealPtr(here) + here->info.size == here->info.next)
  {
    here->info.size = here->info.size + sizeof(Header) + here->info.next->info.size;
    here->info.next = here->info.next->info.next;
  }
}

