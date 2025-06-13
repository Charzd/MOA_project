/* Crossover routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    if (nreal!=0)
    {
        realcross (parent1, parent2, child1, child2);
    }
    if (nbin!=0)
    {
        bincross (parent1, parent2, child1, child2);
    }
    return;
}

/*Routine for two point real variable*/
void realcross(individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i;
    int site1, site2, temp;

    if (randomperc() <= pcross_real)
    {
        nrealcross++;

        site1 = rnd(0, nreal - 1);
        site2 = rnd(0, nreal - 1);

        if (site1 > site2)
        {
            temp = site1;
            site1 = site2;
            site2 = temp;
        }

        for (i = 0; i < site1; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }

        for (i = site1; i <= site2; i++)
        {
            child1->xreal[i] = parent2->xreal[i];
            child2->xreal[i] = parent1->xreal[i];
        }

        for (i = site2 + 1; i < nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    }
    else
    {
        for (i = 0; i < nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    }
    return;
}


/* Routine for two point binary crossover */
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i, j;
    double rand;
    int temp, site1, site2;
    for (i=0; i<nbin; i++)
    {
        rand = randomperc();
        if (rand <= pcross_bin)
        {
            nbincross++;
            site1 = rnd(0,nbits[i]-1);
            site2 = rnd(0,nbits[i]-1);
            if (site1 > site2)
            {
                temp = site1;
                site1 = site2;
                site2 = temp;
            }
            for (j=0; j<site1; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
            for (j=site1; j<site2; j++)
            {
                child1->gene[i][j] = parent2->gene[i][j];
                child2->gene[i][j] = parent1->gene[i][j];
            }
            for (j=site2; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
        else
        {
            for (j=0; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
    }
    return;
}
