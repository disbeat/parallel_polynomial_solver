#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "dataTypes.h"
#include "basicEvaluation.h"
#include "prefixSumEvaluation.h"
#include "hornerEvaluation.h"
#include "estrinEvaluation.h"
#include "parallelHornerEvaluation.h"



#define MAX_STRING 256






double * read_constants(int * degree) {
	int i;
	double * polynomial;
	scanf("%d", degree);
	polynomial = (double *) malloc(sizeof(double) * ((*degree) + 1));
	for (i = *degree; i >= 0; i--)
		scanf("%lf", &polynomial[i]);
	return polynomial;
}

double * read_instances(int * num) {
	int i;
	double * instances;
	scanf("%d", num);
	instances = (double *) malloc(sizeof(double) * (*num));
	for (i = 0; i < *num; i++)
		scanf("%lf", &instances[i]);
	return instances;
}

void print_polynomial(double * polynomial, int degree) {
	int i = 0, firsttime = 1;
	for (i = degree; i >= 0; i--) {
		char str[MAX_STRING] = "";
		double factor = polynomial[i];
		
		if (factor != 0) {
			if (firsttime)
				sprintf(str, "%.1lf", factor);
			else
				if (factor < 0)
					sprintf(str, " - %.1lf", -factor);
				else
					sprintf(str, " + %.1lf", factor);
			
			if (str[0] != '\0') {
				if (i > 0) {
					printf("%s x^%d", str, i);
				}
				else
					printf("%s", str);
				str[0] = '\0';
				firsttime = 0;
			}
		}
	}
}


void evaluate(double * polynomial, int degree, double * inputlist, int num_inputs, double (evaluate_function)(double *, int, double)) {
	int i;
	//printf("p(x) = ");
	//print_polynomial(polynomial, degree);
	//printf("\n");
	for (i = 0; i < num_inputs; i++) {
		double input = inputlist[i];
		double output = evaluate_function(polynomial, degree, input);
		//printf("p(%.1lf) = %lf\n", input, output);
	}
}


/* NOTA: esta função não está de acordo com os requisitos do enunciado.
   Leia cuidadosamente as instruções relativamente ao formato da saída.
*/
int main(void) {
	int degree, num_instances;
    double * polynomial = read_constants(&degree);
	double * instances = read_instances(&num_instances);
	clock_t ticks1, ticks2;	

// Basic evaluation
	printf("Basic evaluation optmimized\n");
	ticks1 = clock();
	evaluate(polynomial, degree, instances, num_instances, basicEvaluation);
	ticks2 = clock();
	printf("seconds basic = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);

// Basic evaluation dumb
    printf("Basic evaluation dumb\n");
    ticks1 = clock();
    evaluate(polynomial, degree, instances, num_instances, basicEvaluationDumb);
    ticks2 = clock();
    printf("seconds basic = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);

//Horner Evaluation
	printf("Horner evaluation\n");
	ticks1 = clock();
	evaluate(polynomial, degree, instances, num_instances, hornerEvaluation);
	ticks2 = clock();
	printf("seconds horner = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
	
// // Prefix 1 evaluation
    printf("Prefix evaluation V1\n");
    ticks1 = clock();
    evaluate(polynomial, degree, instances, num_instances, calculatePolynomPrefixSumV1);
    ticks2 = clock();
    printf("seconds prefix V1 = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
 
// // Prefix 2 evaluation
    printf("Prefix evaluation V2\n");
    ticks1 = clock();
    evaluate(polynomial, degree, instances, num_instances, calculatePolynomPrefixSumV2);
    ticks2 = clock();
    printf("seconds prefix V2 = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
         
// Estrin evaluation
	printf("Estrin evaluation V1\n");
    ticks1 = clock();
    evaluate(polynomial, degree, instances, num_instances, estrinEvaluation);
    ticks2 = clock();
    printf("seconds estrin V1 = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
	
// Horner parallel evaluation
	printf("Horner Parallel evaluation V1\n");
    ticks1 = clock();
    evaluate(polynomial, degree, instances, num_instances, parallelHornerEvaluation);
    ticks2 = clock();
    printf("seconds Horner Parallel V1 = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
		

	free(polynomial);
	free(instances);
    return 0;
}

