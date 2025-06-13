Se ejecuta el algoritmo de la manera habitual:

>>> make all 
>>> ./nsga2r random_seed instance_route popsize ngen nobj pcross_real pmut_real


Actualmente el código esta diseñado para el caso de prueba 2 (dado que desactiva la restricción siete)
Cabe detallar que para aplicar bien la restricción del caso de prueba 1 se debe hacer lo siguiente:
- Descomentar la linea 55 de initialize.c
- Comentar la línea 188 de eval.c