
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
	size += sizeof(size_t);
	if(size < PAGE_SIZE){
		//Try to find a free space in that page
		node* prev = NULL;
		node* curr = mem;
		while(curr){
			
		}
	}
	//Else create a page

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
