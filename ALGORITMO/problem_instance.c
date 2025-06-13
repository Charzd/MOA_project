/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"


int getNodeId(char *name, problem_instance pi, char type_plat){
    int i;
    int count=0;
    if(type_plat == 'E'){
        for (i=0; i<pi.nEntradas; i++){
            if(strcmp(name, pi.entradas[i].nombre) == 0)
                return count;
            count++;
        }
    }
    else if (type_plat == 'F'){
        for (i=0; i<pi.nFondos; i++){
            if(strcmp(name, pi.platos_fondo[i].nombre) == 0)
                return count;
            count++;
        }
    }
    else if (type_plat =='P'){
        for (i=0; i<pi.nPostres; i++){
            if(strcmp(name, pi.postres[i].nombre) == 0)
                return count;
            count++;
        }
    }
    
    else if (type_plat == 'N'){
        for (i=0; i<pi.nNutrientes; i++){
            if(strcmp(name, pi.nutrientes[i].nombre) == 0)
                return count;
            count++;
        }
    }
    return -1;
}

char getplat(char *name, problem_instance pi){
    int i;
    for (i=0; i<pi.nEntradas; i++){
        if(strcmp(name, pi.entradas[i].nombre) == 0)
            return 'E';
    }
    for (i=0; i<pi.nFondos; i++){
        if(strcmp(name, pi.platos_fondo[i].nombre) == 0)
            return 'F'; 
    }
    for (i=0; i<pi.nPostres; i++){
        if(strcmp(name, pi.postres[i].nombre) == 0)
            return 'P';
    }
    for (i=0; i<pi.nNutrientes; i++){
        if(strcmp(name, pi.nutrientes[i].nombre) == 0)
            return 'N';
    }
    return -1;
}
void printProblemInstance(problem_instance *pi){
    int i, j;
    printf("Entradas: %d\n", pi->nEntradas);
    for (i=0; i<pi->nEntradas; i++){
        printf("%d: %s costo: %.2f emisiones %.2f", pi->entradas[i].id, pi->entradas[i].nombre, pi->entradas[i].costo, pi->entradas[i].emisiones);
        printf("\n Nutrientes: ");
        for(j=0; j<8; j++){
            printf("%f ", pi->entradas[i].nutrientes[j]);
        }
        printf("\n");
    }

    printf("Fondos: %d\n", pi->nFondos);
    for (i=0; i<pi->nFondos; i++){
        printf("%d: %s costo: %.2f emisiones %.2f r: %d tipo: %c", pi->platos_fondo[i].id, pi->platos_fondo[i].nombre, pi->platos_fondo[i].costo, pi->platos_fondo[i].emisiones, pi->platos_fondo[i].r, pi->platos_fondo[i].tipo);
        printf("\n Nutrientes: ");
        for(j=0; j<8; j++){
            printf("%f ", pi->platos_fondo[i].nutrientes[j]);
        }
        printf("\n");
    }

    printf("Postres: %d\n", pi->nPostres);
    for (i=0; i<pi->nPostres; i++){
        printf("%d: %s costo: %.2f emisiones %.2f", pi->postres[i].id, pi->postres[i].nombre, pi->postres[i].costo, pi->postres[i].emisiones);
        printf("\n Nutrientes: ");
        for(j=0; j<8; j++){
            printf("%f ", pi->postres[i].nutrientes[j]);
        }
        printf("\n");
    }

    printf("Nutrientes: %d\n", pi->nNutrientes);
    for (i=0; i<pi->nNutrientes; i++){
        printf("%d: %s cota_inf: %.2f cota_sup %.2f", pi->nutrientes[i].id, pi->nutrientes[i].nombre, pi->nutrientes[i].min_d, pi->nutrientes[i].max_d);
        printf("\n");
    }

    printf("Dias: %d\n", pi->nDias);
    printf("Semanas: %d\n", pi->nSemanas);
    for (i=0; i<pi->nDias; i++){
        printf("%d: %s semana: %d tipo %d", pi->dias[i].id, pi->dias[i].nombre, pi->dias[i].semana, pi->dias[i].tipo);
        printf("\n");
    }
    return;

}