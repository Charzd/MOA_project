/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "global.h"
# include "rand.h"

float strtof(const char *str) {
    float result = 0.0;
    float decimal_factor = 0.1;
    int sign = 1;
    int in_decimal = 0;

    while (*str == ' ') str++;

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str) {
        if (*str >= '0' && *str <= '9') {
            if (!in_decimal) {
                result = result * 10 + (*str - '0');
            } else {
                result = result + (*str - '0') * decimal_factor;
                decimal_factor *= 0.1;
            }
        } else if (*str == '.') {
            if (in_decimal) break; 
            in_decimal = 1;
        } else {
            break;  
        }
        str++;
    }

    return result * sign;
}

void findDef(FILE *f, char *def) {
    char word[1024];
    /* assumes no word exceeds length of 1023 */
    while (fscanf(f, " %1023s", word) == 1) {
        if(strcmp(word,def) == 0) break;
    }
}

void removeSemicolon(char *line){
    strtok(line, ";");
}

int countWords(char *line){
    int words;
    char linet[1024], *token;
    strcpy(linet, line);

    words = 0;
    token = strtok(linet, " ");

    while( token != NULL ) {
        words ++;
        token = strtok(NULL, " ");
    }
   return words;
}
void readEntradas(FILE *f, problem_instance *pi) {
    int debug = 0, id = 0;
    char line[1024];
    char buffer[8192] = "";
    char *token;


    while (fgets(line, sizeof(line), f)) {
        if (strchr(line, ';') != NULL) {
            strcat(buffer, line);
            break;
        }
        strcat(buffer, line);
    }

    removeSemicolon(buffer);
    if (debug) printf("Buffer (todo el set): %s\n", buffer);

    pi->nEntradas = countWords(buffer);
    pi->entradas = malloc(pi->nEntradas * sizeof(entrada));

    token = strtok(buffer, " \t\n");
    while (token != NULL && id < pi->nEntradas) {
        pi->entradas[id].id = id;
        pi->entradas[id].nombre = malloc(strlen(token) + 1);
        strcpy(pi->entradas[id].nombre, token);
        token = strtok(NULL, " \t\n");
        id++;
    }

    if (debug) {
        for (id = 0; id < pi->nEntradas; id++)
            printf("%d, %s\n", pi->entradas[id].id, pi->entradas[id].nombre);
    }
}

void readnumFondos(FILE *f, problem_instance *pi){
    int debug=0;
    int num;
    char *token;
    char line[1024];

    fgets(line, sizeof(line), f);
    if (debug) printf("Line: %s\n", line);

    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);

    token = strtok(line, " \t\n");
    if (token != NULL) {
        num = atoi(token); 
    } else {
        fprintf(stderr, "No se pudo leer número de fondos\n");
        exit(1);
    }

    
    pi->nFondos = num;

    if (debug) printf("Número de fondos leído: %d\n", num);
    

}

void readFondos(FILE *f, problem_instance *pi) {
    int debug = 0;
    int id = 0, i;
    char line[1024];
    char *p;
    char *token;

    pi->platos_fondo = malloc(pi->nFondos * sizeof(plato_fondo));
    if (!pi->platos_fondo) {
        fprintf(stderr, "Error de memoria para platos_fondo\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), f)) {
        if (strchr(line, ';') != NULL) {
            p = strchr(line, ';');
            *p = ' ';
        }

        token = strtok(line, " \t\n");
        while (token != NULL && id < pi->nFondos) {
            pi->platos_fondo[id].id = id;
            pi->platos_fondo[id].nombre = malloc(strlen(token) + 1);
            if (!pi->platos_fondo[id].nombre) {
                fprintf(stderr, "Error de memoria para nombre\n");
                exit(1);
            }
            strcpy(pi->platos_fondo[id].nombre, token);
            id++;

            if (id >= pi->nFondos) break;

            token = strtok(NULL, " \t\n");
        }

        if (id >= pi->nFondos) break;
    }

    if (debug) {
        for (i = 0; i < pi->nFondos; i++) {
            printf("%d, %s\n", pi->platos_fondo[i].id, pi->platos_fondo[i].nombre);
        }
    }
}


void readPostres(FILE *f, problem_instance *pi) {
    int debug = 0, id = 0;
    char line[1024];
    char buffer[8192] = "";
    char *token;


    while (fgets(line, sizeof(line), f)) {
        if (strchr(line, ';') != NULL) {
            strcat(buffer, line);
            break;
        }
        strcat(buffer, line);
    }

    removeSemicolon(buffer);
    if (debug) printf("Buffer (todo el set): %s\n", buffer);

    pi->nPostres = countWords(buffer);
    pi->postres = malloc(pi->nPostres * sizeof(postre));

    token = strtok(buffer, " \t\n");
    while (token != NULL && id < pi->nPostres) {
        pi->postres[id].id = id;
        pi->postres[id].nombre = malloc(strlen(token) + 1);
        strcpy(pi->postres[id].nombre, token);
        token = strtok(NULL, " \t\n");
        id++;
    }

    if (debug) {
        for (id = 0; id < pi->nPostres; id++)
            printf("%d, %s\n", pi->postres[id].id, pi->postres[id].nombre);
    }
}

void readtype_Fondo(FILE *f, problem_instance pi, char type_fondo) {
    int debug = 0;
    char line[1024];
    char *token;
    char *p;

    while (fgets(line, sizeof(line), f)) {
        p = strchr(line, ';');
        if (p != NULL) *p = ' ';

        token = strtok(line, " \t\n");
        while (token != NULL) {
            int id = getNodeId(token, pi, 'F');
            if (id >= 0 && id < pi.nFondos) {
                pi.platos_fondo[id].tipo = type_fondo;
                if (debug) printf("Asignado %c a %s (id=%d)\n", type_fondo, pi.platos_fondo[id].nombre, id);
            } else {
                printf("WARNING: Fondo '%s' no encontrado\n", token);
            }
            token = strtok(NULL, " \t\n");
        }

        if (p != NULL) break;
    }

    if (debug) getchar();
}

void readDias(FILE *f, problem_instance *pi){
    int debug=0, id=0;
    int tipo, semana;
    char *token;
    char line[1024];
    fgets(line, sizeof(line), f);
    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nDias=countWords(line);
    pi->dias=malloc(pi->nDias*sizeof(dia));
    if (debug) printf("nDias: %d\n", pi->nDias);


    token=strtok(line, " ");
    tipo= 1;  /*//1,2,3,4,5 -> Lunes, Martes, Miercoles, Jueves, Viernes*/
    semana = 1;

    while( token != NULL && id < pi->nDias) {
        if (debug) printf("%s\n", token);

        pi->dias[id].id=id;
        pi->dias[id].nombre = malloc(strlen(token) + 1);
        strcpy(pi->dias[id].nombre, token); 
        pi->dias[id].tipo = tipo;
        pi->dias[id].semana = semana;

        tipo++;
        if (tipo > 5) {
            tipo = 1;
            semana++;
        }

        token=strtok(NULL, " ");
        id++;
    }
    pi->nSemanas = semana-1;
    if (debug) {
        for ( id=0; id<pi->nDias; id++ )
            printf("%d, %s, tipo: %d, semana: %d\n",
                   pi->dias[id].id, pi->dias[id].nombre,
                   pi->dias[id].tipo, pi->dias[id].semana);
    }
    if (debug) getchar();
}


void readNutrientes(FILE *f, problem_instance *pi){
    int debug=0, id=0;
    char *token;
    char line[1024];
    fgets(line, sizeof(line), f);
    if (debug) printf("Line: %s\n", line);
    removeSemicolon(line);
    if (debug) printf("Line: %s\n", line);
    pi->nNutrientes=countWords(line);

    pi->nutrientes=malloc(pi->nNutrientes*sizeof(nutriente));

    if (debug) printf("nNutrientes: %d\n", pi->nNutrientes);

    token=strtok(line, " ");
    while( token != NULL && id < pi->nNutrientes) {
        if (debug) printf("%s\n", token);
        pi->nutrientes[id].id=id;
        pi->nutrientes[id].nombre = malloc(strlen(token) + 1);
        strcpy(pi->nutrientes[id].nombre, token); 
        token=strtok(NULL, " ");
        id++;
    }
    if (debug) {
        for ( id=0; id<pi->nNutrientes; id++ )
            printf ("%d, %s\n", pi->nutrientes[id].id, pi->nutrientes[id].nombre);
    }
    if (debug) getchar();
}

void read_u_na(FILE *f, problem_instance *pi) {
    int debug=0;
    int i, id;
    char line[2100];
    char *token;
    char *nombre;
    char type;
    float value;
    while (fgets(line, sizeof(line), f)) {
        if (debug) printf("Line: %s\n", line);
        if (line[0] == ';') break;

        token = strtok(line, " \t\n");
        if (!token) continue;

        nombre = token;
        type = getplat(nombre, *pi);
        id = getNodeId(nombre, *pi, type);

        if (id == -1 || type == -1) {
            printf("WARNING: Plato '%s' no encontrado\n", nombre);
            continue;
        }
        if (type == 'E') {
            pi->entradas[id].nutrientes = malloc(pi->nNutrientes * sizeof(float));
        } else if (type == 'F') {
            pi->platos_fondo[id].nutrientes = malloc(pi->nNutrientes * sizeof(float));
        } else if (type == 'P') {
            pi->postres[id].nutrientes = malloc(pi->nNutrientes * sizeof(float));
        }

        for (i = 0; i < 8; i++) {
            token = strtok(NULL, " \t\n");
            if (!token) {
                printf("WARNING: Faltan nutrientes para %s\n", nombre);
                break;
            }

            value = strtof(token);

            if (debug) printf("nombre: %s, tipo: %c nutriente: %d valor: %f\n", nombre, type, i, value);
            if (type == 'E')
                pi->entradas[id].nutrientes[i] = value;
            else if (type == 'F')
                pi->platos_fondo[id].nutrientes[i] = value;
            else if (type == 'P')
                pi->postres[id].nutrientes[i] = value;
        }
    }
}

void read_el_a_y_rc_a(FILE *f, problem_instance *pi) {
    int debug = 0;
    char line[2100];
    char *token;
    char *nombre;
    char type;
    int id;
    float emision, costo;

    while (fgets(line, sizeof(line), f)) {
        if (debug) printf("Line: %s\n", line);
        if (line[0] == ';') break;

        token = strtok(line, " \t\n");
        if (!token) continue;

        nombre = token;
        type = getplat(nombre, *pi);
        id = getNodeId(nombre, *pi, type);

        if (id == -1 || type == -1) {
            printf("WARNING: Plato '%s' no encontrado\n", nombre);
            continue;
        }

        token = strtok(NULL, " \t\n");
        if (!token) {
            printf("WARNING: Falta emision para %s\n", nombre);
            continue;
        }
        emision = strtof(token);

        token = strtok(NULL, " \t\n");
        if (!token) {
            printf("WARNING: Falta costo para %s\n", nombre);
            continue;
        }
        costo = strtof(token);

        if (debug)
            printf("Plato: %s, Tipo: %c, Emision: %.2f, Costo: %.2f\n", nombre, type, emision, costo);

        if (type == 'E') {
            pi->entradas[id].emisiones = emision;
            pi->entradas[id].costo = costo;
        } else if (type == 'F') {
            pi->platos_fondo[id].emisiones = emision;
            pi->platos_fondo[id].costo = costo;
        } else if (type == 'P') {
            pi->postres[id].emisiones = emision;
            pi->postres[id].costo = costo;
        }
    }
}

void read_r_a(FILE *f, problem_instance *pi) {
    int debug = 0;
    char line[2100];
    char *token;
    char *nombre;
    char type;
    int id, valor;

    while (fgets(line, sizeof(line), f)) {
        if (debug) printf("Line: %s\n", line);
        if (line[0] == ';') break;

        token = strtok(line, " \t\n");
        if (!token) continue;

        nombre = token;
        type = getplat(nombre, *pi);
        id = getNodeId(nombre, *pi, type);

        if (id == -1) {
            printf("WARNING: Plato '%s' no encontrado\n", nombre);
            continue;
        }

        if (type != 'F') {
            printf("WARNING: Plato '%s' no es de tipo fondo ('F')\n", nombre);
            continue;
        }

        token = strtok(NULL, " \t\n");
        if (!token) {
            printf("WARNING: Falta valor r_a para %s\n", nombre);
            continue;
        }

        valor = strtof(token);

        if (debug) printf("Plato fondo: %s, ID: %d, r_a: %d\n", nombre, id, valor);

        pi->platos_fondo[id].r = valor;
    }
}

void read_cotas_nutr(FILE *f, problem_instance *pi) {
    int debug = 0;
    char line[1024];
    char *token;
    char *nombre;
    char type;
    int id;
    float cota_inf, cota_sup;

    while (fgets(line, sizeof(line), f)) {
        if (debug) printf("Line: %s\n", line);
        if (line[0] == ';') break;

        token = strtok(line, " \t\n");
        if (!token) continue;

        nombre = token;
        type = getplat(nombre, *pi);
        id = getNodeId(nombre, *pi, type);

        if (type != 'N' || id == -1) {
            printf("WARNING: Nutriente '%s' no encontrado o no es tipo 'N'\n", nombre);
            continue;
        }

        token = strtok(NULL, " \t\n");
        if (!token) {
            printf("WARNING: Falta nota inferior para %s\n", nombre);
            continue;
        }
        cota_inf = strtof(token);

        token = strtok(NULL, " \t\n");
        if (!token) {
            printf("WARNING: Falta cota superior para %s\n", nombre);
            continue;
        }
        cota_sup = strtof(token);

        if (debug)
            printf("Nutriente: %s, cota_inf: %.2f, cota_sup: %.2f\n", nombre, cota_inf, cota_sup);

        pi->nutrientes[id].min_d = cota_inf;
        pi->nutrientes[id].max_d = cota_sup;
    }
}

int readInputFile(char* filePath, problem_instance *pi) {
    int debug=0;
    int i;
    FILE* fh=fopen(filePath, "r");

    /*check if file exists*/
    if ( fh==NULL ){
        printf("File does not exists %s", filePath);
        return 0;
    }

    if(debug) printf("Reading: %s \n", filePath);

    findDef(fh, "NF:=");
    readnumFondos(fh, pi);
    if(debug) printf("TOTAL DE FONDOS LEIDOS! \n");

    findDef(fh, "ENTR:=");
    readEntradas(fh, pi);
    if(debug) printf("End Entradas! \n");

    findDef(fh, "PRIN:=");
    readFondos(fh, pi);
    if(debug) printf("End Fondos! \n");

    findDef(fh, "PSTR:=");
    readPostres(fh, pi);
    if(debug) printf("End Postres! \n");

    findDef(fh, "MM:=");
    readtype_Fondo(fh, *pi, 'M');
    if(debug) printf("End MM! \n");

    findDef(fh, "ML:=");
    readtype_Fondo(fh, *pi, 'L');
    if(debug) printf("End ML! \n");

    findDef(fh, "DIAS:=");
    readDias(fh, pi);
    if(debug) printf("End DIAS! \n");

    findDef(fh, "NUTR:=");
    readNutrientes(fh, pi);
    if(debug) printf("End NUTRI! \n");

    findDef(fh, "u_na:=");
    read_u_na(fh, pi);
    if(debug) printf("End nutrition data! \n");

    findDef(fh, "el_a_y_rc_a:=");
    read_el_a_y_rc_a(fh, pi);
    if(debug) printf("End emi_cost data! \n");

    findDef(fh, "r_a:=");
    read_r_a(fh, pi);
    if(debug) printf("End r data! \n");

    findDef(fh, "cotas_nutr:=");
    read_cotas_nutr(fh, pi);
    if(debug) printf("End cotas data! \n");

    fclose(fh);
    if(debug) printf("End Reading! \n");


    printProblemInstance(pi);
    if(debug) getchar();

    
    nreal = 3*pi->nDias;
    ncon = 7;
    
    min_realvar = (double *)malloc(nreal*sizeof(double));
    max_realvar = (double *)malloc(nreal*sizeof(double));
    
    y_over = (double *)malloc(pi->nNutrientes*sizeof(double));
    y_under = (double *)malloc(pi->nNutrientes*sizeof(double));
    
    for(i=0; i<nreal;i++){
        min_realvar[i] = 0;
        if (i%3 == 0)  max_realvar[i] = pi->nEntradas-1;
        else if (i%3 == 1) max_realvar[i] = pi->nFondos-1;
        else  max_realvar[i] = pi->nPostres-1;
    }
    
    
    return 0;
}