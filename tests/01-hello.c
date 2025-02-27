
#include <stdio.h>
#include <string.h>

#include "pmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    char* hello = pstrdup("Hello, World");
    printf("%s\n", hello);
    pfree(hello);

    pprintstats();
    return 0;
}



