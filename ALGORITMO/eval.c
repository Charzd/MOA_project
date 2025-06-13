/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop, problem_instance *pi)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]), pi);
    }
    return;
}

void test_problem (double *xreal, double *obj, problem_instance *pi)
{
    int d;
    double emisiones = 0;
    double costos = 0;
    int id_e,id_f,id_p;

    for(d=0; d < pi->nDias; d++){

        id_e = xreal[d*3];
        id_f = xreal[d*3+1];
        id_p = xreal[d*3+2];

        emisiones  = emisiones + pi->platos_fondo[id_f].r * (pi->entradas[id_e].emisiones + pi->platos_fondo[id_f].emisiones + pi->postres[id_p].emisiones);
        costos  = costos + pi->platos_fondo[id_f].r * (pi->entradas[id_e].costo + pi->platos_fondo[id_f].costo + pi->postres[id_p].costo);
        
    }
    obj[0] = emisiones;
    obj[1] = costos;
    return;
}

void view_nutr(double *xreal, double *constr, problem_instance *pi){
    int d,i,j,k,n;
    int e,f,p;
    float valor, limite_max, limite_min;
    double exceso, falta;

    exceso = 0;
    falta = 0;

    for(d=0; d < pi->nDias; d++){
        i = d*3;
        j = i+1;
        k = i+2;

        e = xreal[i];
        f = xreal[j];
        p = xreal[k];

        for(n=0 ; n< pi->nNutrientes; n++){
            limite_max = pi->nutrientes[n].max_d;
            limite_min = pi->nutrientes[n].min_d;

            valor = pi->entradas[e].nutrientes[n] + pi->platos_fondo[f].nutrientes[n] + pi->postres[p].nutrientes[n];

            if(valor > limite_max){
                exceso += valor - limite_max;
            }
            else if (valor < limite_min){
                falta += limite_min - valor;
            }

        }
    }
    constr[0] = -exceso;
    constr[1] = -falta;
    return;
}

void view_unique_fondos(double *xreal, double *constr, problem_instance *pi){
    int *platos_fondo;
    int d,d2,f1,f2;
    int violation = 0;

    platos_fondo = malloc(pi->nDias * sizeof(int));

    for(d=0; d<pi->nDias; d++){
        platos_fondo[d] = (int)xreal[d*3+1];
    }

    for(d = 0; d< pi->nDias; d++){
        for(d2 = d+1; d2 < pi->nDias; d2++){
            f1 = platos_fondo[d];
            f2 = platos_fondo[d2];
            if(f1==f2) violation++;
        }
    }

    free(platos_fondo);

    constr[2] = -violation;
}

void view_weekly_unique(double *xreal, double *constr, problem_instance *pi){
    int *starts, *deserts;
    int s,d,d2,e1,e2,p1,p2;
    int violation_e = 0;
    int violation_p = 0;
    int inicio, fin;

    starts = malloc(pi->nDias * sizeof(int));
    deserts = malloc(pi->nDias * sizeof(int));

    for(d=0; d<pi->nDias; d++){
        starts[d] = (int)xreal[d*3];
        deserts[d] = (int)xreal[d*3+2];
    }

    for(s=0; s < pi->nSemanas; s++){
        inicio = s*5;
        fin = (s+1)*5;
        if (fin > pi->nDias) fin = pi->nDias;

        for(d=inicio; d < fin ; d++){
            for(d2=d+1; d2 < fin; d2++){
                e1 = starts[d];
                e2 = starts[d2];
                if(e1 == e2) violation_e++;

                p1 = deserts[d];
                p2 = deserts[d2];
                if(p1 == p2) violation_p++;
            }
        }
    }

    free(starts);
    free(deserts);

    constr[3] = -violation_e;
    constr[4] = -violation_p;
}

void view_restrict_diet(double *xreal, double *constr, problem_instance *pi){
    int s, m, w, f_w, f_m;
    char t_w, t_m;
    int violation_l = 0;
    int violation_nomeat = 0;

    for(s = 0; s < pi->nSemanas; s++){
        m = s*5; /*Lunes*/
        w = s*5+2;  /*Miercoles*/

        if (m >= pi->nDias) break;
        if (w >= pi->nDias) break;

        f_m = (int)xreal[m*3+1];
        f_w = (int)xreal[w*3+1];
        
        t_m = pi->platos_fondo[f_m].tipo;
        t_w = pi->platos_fondo[f_w].tipo;

        if(t_m == 'M'){
            violation_nomeat++;
        }
        if (t_w != 'L') {
            violation_l++;
        }

    }
    constr[5] = -violation_nomeat;
    constr[6] = -violation_l;
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (individual *ind, problem_instance *pi)
{
    int j;
    test_problem (ind->xreal, ind->obj, pi);
    view_nutr(ind->xreal, ind->constr, pi);
    view_unique_fondos(ind->xreal, ind->constr, pi);
    view_weekly_unique(ind->xreal, ind->constr, pi);
    view_restrict_diet(ind->xreal, ind->constr, pi);


    /* Descomentado para el caso de prueba 2*/
    ind->constr[6] = 0; 
    
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    return;
}
