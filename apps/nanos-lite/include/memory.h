#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#ifndef PGSIZE
#define PGSIZE 4096
#endif

#ifndef PMEM_SIZE
#define PMEM_SIZE (128 * 1024 * 1024)
#endif

#define PGMASK          (PGSIZE - 1)    // Mask for bit ops
#define PGROUNDUP(sz)   (((sz)+PGSIZE-1) & ~PGMASK)
#define PGROUNDDOWN(a)  (((a)) & ~PGMASK)

void* new_page(void);
void* kmalloc(uint32_t);

#endif
