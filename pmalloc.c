
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

#include "pmalloc.h"

const size_t PAGE_SIZE = 4096;
static pm_stats stats; // This initializes the stats to 0.
static struct node *mem;

long
free_list_length()
{
    // TODO: Calculate the length of the free list.
    return 0;
}

pm_stats*
pgetstats()
{
    stats.free_length = free_list_length();
    return &stats;
}

void
pprintstats()
{
    stats.free_length = free_list_length();
    fprintf(stderr, "\n== panther malloc stats ==\n");
    fprintf(stderr, "Mapped:   %ld\n", stats.pages_mapped);
    fprintf(stderr, "Unmapped: %ld\n", stats.pages_unmapped);
    fprintf(stderr, "Allocs:   %ld\n", stats.chunks_allocated);
    fprintf(stderr, "Frees:    %ld\n", stats.chunks_freed);
    fprintf(stderr, "Freelen:  %ld\n", stats.free_length);
}

static
size_t
div_up(size_t xx, size_t yy)
{
    // This is useful to calculate # of pages
    // for large allocations.
    size_t zz = xx / yy;

    if (zz * yy == xx) {
        return zz;
    }
    else {
        return zz + 1;
    }
}

void*
pmalloc(size_t size)
{
    stats.chunks_allocated += 1;
    size += sizeof(size_t);
    if (size<16) size=16;
    if (mem==NULL) {
	    mem = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	    // set the first bytes to be our size - size desired to be allocated - variable: size
	    mem = size;
	    //printf("%d\n", &mem);
	    printf("%d\n", mem);
	    //printf("%d\n", sizeof(size_t));
	    //printf("%d\n", &mem+sizeof(size_t));
	    //*(&mem+sizeof(size_t))
	    //mem->size=size;
	    // *mem = *mem + size + sizeof(size_t)
	    // return *m
	    //*mem -> pointer to our new page
	    void *pointer = *(&mem+(sizeof(size_t)/8));
	    mem = *(&mem+size);
	    mem=4096-size;

	    return pointer;
    }
    else {
		printf("%d\n", mem);
	    // Transverse linked list, looking for block that is just big enough
	    // if no space is available that is large enough, call mmap(), move last free pointer to new page
	    // Else,
	    // Set size for newly allocated chunk, move free pointer, and update size accordingly
    }
    // TODO: Actually allocate memory with mmap and a free list.
    return (void*) 0xDEADBEEF;
}

void
pfree(void* item)
{
    stats.chunks_freed += 1;
	item = &mem;
	mem = *(&item-(sizeof(size_t)/8));
	//*(&item-sizeof(size_t))
    // check if null
    // set *item->next = *mem;
    // *mem = *item-sizeof(size_t)
	
    // TODO: Actually free the item.
}

int main () {
	// TODO: Test our malloc
	pmalloc(5);
	pmalloc(45);
	return 0;
}
