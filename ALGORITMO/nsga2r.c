/* NSGA-II routine (implementation of the 'main' function) */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include "global.h"
# include "rand.h"
#include <time.h>

int nreal;  
int nbin;   /**/
int nobj;   
int ncon;   
int popsize;
double pcross_real;
double pcross_bin;  /**/
double pmut_real;
double pmut_bin;  /**/
double eta_c;
double eta_m;
int ngen;
int nbinmut; /**/
int nrealmut;
int nbincross;  /**/
int nrealcross;
int *nbits;  /**/
double *min_realvar;
double *max_realvar;
double *min_binvar; /**/
double *max_binvar; /**/
int bitlength; /**/
int choice;
int obj1;
int obj2;
int obj3;
int angle1;
int angle2;
double *y_under;
double *y_over;

int main (int argc, char **argv)
{
    clock_t start_time;
    start_time = clock();
    int i;
    FILE *fpt1;
    FILE *fpt2;
    FILE *fpt3;
    FILE *fpt4;
    FILE *fpt5;

    struct problem_instance * pi = malloc (sizeof(problem_instance));
    population *parent_pop;
    population *child_pop;
    population *mixed_pop;
    if (argc<2)
    {
        printf("\n Usage ./nsga2r random_seed instance_route popsize ngen nobj pcross_real pmut_real\n");
        printf("./nsga2r 0.123 Models/Model_data2.dat 1000 100 2 0.6 0.1\n");
        exit(1);
    }
    seed = (double)atof(argv[1]);
    if (seed<=0.0 || seed>=1.0){
        printf("\n Entered seed value is wrong, seed value must be in (0,1) \n");
        exit(1);
    }
    fpt1 = fopen("initial_pop.out","w");
    fpt2 = fopen("final_pop.out","w");
    fpt3 = fopen("best_pop.out","w");
    fpt4 = fopen("all_pop.out","w");
    fpt5 = fopen("params.out","w");
    fprintf(fpt1,"# This file contains the data of initial population\n");
    fprintf(fpt2,"# This file contains the data of final population\n");
    fprintf(fpt3,"# This file contains the data of final feasible population (if found)\n");
    fprintf(fpt4,"# This file contains the data of all generations\n");
    fprintf(fpt5,"# This file contains information about inputs as read by the program\n");

    char * instance_route;
    instance_route = argv[2];
    readInputFile(instance_route, pi);  

    popsize = atoi(argv[3]);
    if (popsize<4 || (popsize%4)!= 0){
        printf("\n population size read is : %d",popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }
    ngen = atoi(argv[4]);
    if (ngen<1){
        printf("\n number of generations read is : %d",ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }
    nobj = atoi(argv[5]);
    if (nobj<1){
        printf("\n number of objectives entered is : %d",nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }
    pcross_real = atof (argv[6]);
    if (pcross_real<0.0 || pcross_real>1.0){
        printf("\n Probability of crossover entered is : %e",pcross_real);
        printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
        exit (1);
    }
    pmut_real = atof (argv[7]);
    if (pmut_real<0.0 || pmut_real>1.0){
        printf("\n Probability of mutation entered is : %e",pmut_real);
        printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
        exit (1);
    }

    printf("\n Input data successfully entered, now performing initialization \n");

    fprintf(fpt5,"\n Population size = %d",popsize);
    fprintf(fpt5,"\n Number of generations = %d",ngen);
    fprintf(fpt5,"\n Number of objective functions = %d",nobj);

    fprintf(fpt5,"\n Number of constraints = %d",ncon);
    
    fprintf(fpt5,"\n Number of real variables = %d",nreal);
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %d",i+1,(int)min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %d",i+1,(int)max_realvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of real variable = %e",pcross_real);
        fprintf(fpt5,"\n Probability of mutation of real variable = %e",pmut_real);
        fprintf(fpt5,"\n Distribution index for crossover = %e",eta_c);
        fprintf(fpt5,"\n Distribution index for mutation = %e",eta_m);
    }
    
    fprintf(fpt5,"\n Number of binary variables = %d",nbin);
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,max_binvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of binary variable = %e",pcross_bin);
        fprintf(fpt5,"\n Probability of mutation of binary variable = %e",pmut_bin);
    }
    fprintf(fpt5,"\n Seed for random number generator = %e",seed);

    
    bitlength = 0;
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            bitlength += nbits[i];
        }
    }
    /*
    // fpt1 => "initial_pop.out"
    // fpt2 => "final_pop.out"
    // fpt3 => "best_pop.out"
    // fpt4 => "all_pop.out"
    // fpt5 => "params.out"
    */
    fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    nbinmut = 0;
    nrealmut = 0;
    nbincross = 0;
    nrealcross = 0;
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));

    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    randomize();
    
    initialize_pop (parent_pop, pi);
    printf("\n Initialization done, now performing first generation\n");
    decode_pop(parent_pop);
    evaluate_pop (parent_pop, pi);  /**/
    assign_rank_and_crowding_distance (parent_pop);
    report_pop (parent_pop, fpt1);
    fprintf(fpt4,"# gen = 1\n");
    report_pop(parent_pop,fpt4);
    printf("\n gen = 1");
    fflush(stdout);
    /*if (choice!=0)
        onthefly_display (parent_pop,gp,1);*/
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    sleep(1);
    for (i=2; i<=ngen; i++)
    {
        selection (parent_pop, child_pop); /*ACA SE HACE EL CROSSOVER*/
        mutation_pop (child_pop, pi);
        decode_pop(child_pop);
        evaluate_pop(child_pop, pi);
        merge (parent_pop, child_pop, mixed_pop);
        fill_nondominated_sort (mixed_pop, parent_pop);
        /* Comment following four lines if information for all
        generations is not desired, it will speed up the execution */
        fprintf(fpt4,"# gen = %d\n",i);
        report_pop(parent_pop,fpt4);
        fflush(fpt4);
        printf("\n gen = %d",i);
    }
    printf("\n Generations finished, now reporting solutions");
    report_pop(parent_pop,fpt2);
    report_feasible(parent_pop,fpt3);
    if (nreal!=0)
    {
        fprintf(fpt5,"\n Number of crossover of real variable = %d",nrealcross);
        fprintf(fpt5,"\n Number of mutation of real variable = %d",nrealmut);
    }
    if (nbin!=0)
    {
        fprintf(fpt5,"\n Number of crossover of binary variable = %d",nbincross);
        fprintf(fpt5,"\n Number of mutation of binary variable = %d",nbinmut);
    }
    fflush(stdout);
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    fclose(fpt1);
    fclose(fpt2);
    fclose(fpt3);
    fclose(fpt4);
    fclose(fpt5);
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
        free (nbits);
    }
    deallocate_memory_pop (parent_pop, popsize);
    deallocate_memory_pop (child_pop, popsize);
    deallocate_memory_pop (mixed_pop, 2*popsize);
    free (parent_pop);
    free (child_pop);
    free (mixed_pop);
    printf("\n Routine successfully exited \n");
    clock_t end_time;
    double elapsed_time;

    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("El algoritmo tardo %.4f segundos en ejecutarse.\n", elapsed_time);

    return (0);
}
