//Sure this is it newest one
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#include "pmalloc.h"

const size_t PAGE_SIZE = 4096;
static pm_stats stats;  
static node *mem = NULL; 

char *pstrdup(char *arg) {
	int i=0;
	for(; arg[i]!=0; i++);
	char *buf = pmalloc(i+1);
	for(int j=0; j<=i; j++) buf[j]=arg[j];
	return buf;
}

long free_list_length() {
    long length = 0;
    node *current = mem;
    while (current) {
        length++;
        current = current->next;
    }
    stats.free_length += length;
    return length;
}

pm_stats* pgetstats() {
    stats.free_length = free_list_length();
    return &stats;
}

void pprintstats() {
    stats.free_length = free_list_length();
    fprintf(stderr, "\n== Panther Malloc Stats ==\n");
    fprintf(stderr, "Mapped:   %ld\n", stats.pages_mapped);
    fprintf(stderr, "Unmapped: %ld\n", stats.pages_unmapped);
    fprintf(stderr, "Allocs:   %ld\n", stats.chunks_allocated);
    fprintf(stderr, "Frees:    %ld\n", stats.chunks_freed);
    fprintf(stderr, "Freelen:  %ld\n", stats.free_length);
}

static size_t div_up(size_t xx, size_t yy) {
    size_t zz = xx / yy;
    if (zz * yy == xx) {
        return zz;
    } else {
        return zz + 1;
    }
}

void* pmalloc(size_t size) {
    size += sizeof(header); // Add space for storing the size.

    if (size < PAGE_SIZE) {
        // Try to find a free block in the list
        node* prev = NULL;
        node* curr = mem; // Head of free list
        while (curr) {
            if (curr->size >= size) {
                // Found a large enough block
                if (prev) {
                    prev->next = curr->next;
                } else {
                    mem = curr->next; // Remove from head
                }
                stats.chunks_allocated += 1;
                /*
               if(curr->next == NULL){
                    mem = (void*)((char*)curr+size);
                    mem->size -= size;
                }*/
                return (void*)((char*)curr + sizeof(header)); // Return pointer after size header
            }
            prev = curr;
            curr = curr->next;
        }

        // No suitable block found, allocate new page
        node* new_block = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (new_block == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }
        stats.pages_mapped += 1;
        new_block->size = size; //PAGE_SIZE - sizeof(header); // Adjust for the header
        mem = (void*)((char*)new_block + size);
        mem->size = PAGE_SIZE - sizeof(header);
        stats.chunks_allocated += 1;
        return (void*)((char*)new_block + sizeof(header));
    }

    // Handle large allocation (>= 1 page)
    size_t pages_needed = div_up(size, PAGE_SIZE);
    node* new_block = mmap(0, pages_needed * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (new_block == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    stats.pages_mapped += pages_needed;
    new_block->size = pages_needed * PAGE_SIZE - sizeof(header); // Adjust for header
    return (void*)((char*)new_block + sizeof(header)); // Return pointer after size header
}

void pfree(void* item) {
    stats.chunks_freed += 1;
    node* block = (node*)((char*)item - sizeof(header)); // Get the header part

    // Add the freed block back to the free list
    block->next = mem;
    mem = block;

    // working on it adding code that merges free blocks
}

/*int main() {
    // Example usage of the custom malloc/free
    void* p1 = pmalloc(100);
    void* p2 = pmalloc(200);
    pfree(p1);
    pfree(p2);

    // Print stats
    pprintstats();
    return 0;
}*/
