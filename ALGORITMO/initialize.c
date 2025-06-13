/* Data initializtion routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to initialize a population randomly */
void initialize_pop (population *pop, problem_instance *pi)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]), pi);
    }
    return;
}

/* Function to initialize an individual feasible */
void initialize_ind(individual *ind, problem_instance *pi) {
    int d, s, i, id, dia, start, end, dias;
    char tipo;
    int total_dias = pi->nDias;
    int total_semanas = pi->nSemanas;
    int fcount;

    int *fondos_usados = malloc(pi->nFondos * sizeof(int));
    int *entradas_usadas = malloc(pi->nEntradas * sizeof(int));
    int *postres_usados = malloc(pi->nPostres * sizeof(int));

    int *fondos_seleccionados = malloc(total_dias * sizeof(int));
    int *entradas_sem = malloc(5 * sizeof(int));
    int *postres_sem = malloc(5 * sizeof(int));

    for (i = 0; i < pi->nFondos; i++) fondos_usados[i] = 0;

    fcount = 0;
    while (fcount < total_dias) {
        id = rnd(0, pi->nFondos - 1);
        if (fondos_usados[id]){
            continue;
        } 

        dia = fcount;
        tipo = pi->platos_fondo[id].tipo;

        int es_lunes = dia % 5 == 0;
        int es_miercoles = dia % 5 == 2;

        if (es_lunes && tipo == 'M') {continue; }

        /* Comentado para el caso de prueba 2*/
        /*if (es_miercoles && tipo != 'L') {continue;} */ 

        fondos_seleccionados[dia] = id;
        fondos_usados[id] = 1;
        fcount++;
    }

    for (s = 0; s < total_semanas; s++) {
        start = s * 5;
        end = (s + 1) * 5;
        if (end > total_dias) end = total_dias;
        dias = end - start;

        for (i = 0; i < pi->nEntradas; i++){
            entradas_usadas[i] = 0;
        } 
        for (i = 0; i < dias; ) {
            int id = rnd(0, pi->nEntradas - 1);
            if (entradas_usadas[id]) continue;
            entradas_usadas[id] = 1;
            entradas_sem[i++] = id;
        }


        for (i = 0; i < pi->nPostres; i++) postres_usados[i] = 0;
        for (i = 0; i < dias; ) {
            id = rnd(0, pi->nPostres - 1);
            if (postres_usados[id]) continue;
            postres_usados[id] = 1;
            postres_sem[i++] = id;
        }

        
        for (d = 0; d < dias; d++) {
            int index = (start + d) * 3;
            ind->xreal[index + 0] = entradas_sem[d];
            ind->xreal[index + 1] = fondos_seleccionados[start + d];
            ind->xreal[index + 2] = postres_sem[d];
        }
    }

    free(fondos_usados);
    free(entradas_usadas);
    free(postres_usados);
    free(fondos_seleccionados);
    free(entradas_sem);
    free(postres_sem);
}