
#include <stdio.h>
#include <string.h>

#include "pmalloc.h"
#include "tools.h"

int
main(int _ac, char* _av[])
{
    int** xs = pmalloc(1000 * sizeof(int*));

    for (int ii = 0; ii < 1000; ++ii) {
        xs[ii] = pmalloc(smax(5 * ii, sizeof(int)));
        memset(xs[ii], 1, 5 * ii);
        *xs[ii] = ii;
    }

    int sum = 0;

    for (int ii = 0; ii < 1000; ++ii) {
        sum += *xs[ii];
        pfree(xs[ii]);
    }

    printf("Sum = %d\n", sum);

    pfree(xs);

    pprintstats();
    return 0;
}



