/* Mutation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to perform mutation in a population */
void mutation_pop (population *pop, problem_instance *pi)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        mutation_ind(&(pop->ind[i]), pi);
    }
    return;
}

/* Function to perform mutation of an individual */
void mutation_ind (individual *ind, problem_instance *pi)
{
    if (nreal!=0)
    {
        real_mutate_ind(ind, pi);
    }
    if (nbin!=0)
    {
        bin_mutate_ind(ind);
    }
    return;
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind (individual *ind)
{
    int j, k;
    double prob;
    for (j=0; j<nbin; j++)
    {
        for (k=0; k<nbits[j]; k++)
        {
            prob = randomperc();
            if (prob <=pmut_bin)
            {
                if (ind->gene[j][k] == 0)
                {
                    ind->gene[j][k] = 1;
                }
                else
                {
                    ind->gene[j][k] = 0;
                }
                nbinmut+=1;
            }
        }
    }
    return;
}

/*Individual mutation, explote*/
void real_mutate_ind(individual *ind, problem_instance *pi)
{
    int pos, tipo, nuevo_valor, flag, i;
    int con = 20;
    double *copy_sol;
    double *copy_obj;
    
    if (randomperc() <= pmut_real)
    {
        flag = 1;
        copy_sol = (double *)malloc(nreal*sizeof(double));
        copy_obj = (double *)malloc(nobj*sizeof(double));

        while(flag && con > 0)
        {
            for(i=0; i<nreal; i++){
                copy_sol[i] = ind->xreal[i];
            }

            pos = rnd(0, nreal - 1);
            tipo = pos % 3;
            nuevo_valor = 0;

            if (tipo == 0) /* Entrada*/
            {
                nuevo_valor = rnd(0, pi->nEntradas - 1);
            }
            else if (tipo == 1) /* Fondo*/
            {
                nuevo_valor = rnd(0, pi->nFondos - 1);
            }
            else /* Postre*/
            {
                nuevo_valor = rnd(0, pi->nPostres - 1);
            }

            copy_sol[pos] = (double)nuevo_valor;
            test_problem(copy_sol,copy_obj,pi);

            if(copy_obj[0] < ind->obj[0] || copy_obj[1] < ind->obj[1]){
                
                ind->xreal[pos] = (double)nuevo_valor;
                nrealmut+=1;
                flag = 0;
            }
            con -- ;
        }
        free(copy_sol);
        free(copy_obj);
    }
    return;
}