#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_STRING 256
#define NUM_THREADS 2

typedef double CoefType;
typedef double TestType;
typedef double ResultType;

/**
 * Calcutates the polynom using the prefix sum problem, for a given test value, with k processors
 */
ResultType calculatePolynomPrefixSumV2(CoefType* coef, int degree, TestType test)
{
	int n, offset, i, it;

	TestType * swap;
	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * values = (TestType *) malloc( degree * sizeof(TestType) );
	TestType * copy = (TestType *) malloc( degree * sizeof(TestType) );

	// number of iterations
	n = (int) ceil( log( degree ) / log( 2 ) );
	offset = 1;
	
	// Set NUM_THREADS
	omp_set_num_threads(NUM_THREADS);	

	#pragma omp parallel for
	for (i = 0; i < offset; i++)
		values[i] = test;

	#pragma omp parallel for
	for (i = 0; i < degree-offset; i++)
		values[i + offset] = test * test;
	
	offset *= 2;
	
	for( it = 1; it < n; it++ )
	{	

		#pragma omp parallel for
		for (i = 0; i < offset; i++)
			copy[i] = values[i];

		#pragma omp parallel for
		for (i = 0; i < degree-offset; i++)
			copy[i + offset] = values[i] * values[i+offset];
		
		offset *= 2;
		
		swap = copy;
		copy = values;
		values = swap;

	}
	
	#pragma omp parallel for reduction(+:result)
    for(i = 0; i < degree; i++)
		result += values[i] * coef[i + 1];

//		printf("[tid: %d] %d ^ %d = %d | ans * %d = %d\n", tid, test, tid, values[tid], coef[degree + 1], result);
	

	result += coef[0];


	free(values);
	free(copy);

	return result;
}

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
	printf("p(x) = ");
	print_polynomial(polynomial, degree);
	printf("\n");
	for (i = 0; i < num_inputs; i++) {
		double input = inputlist[i];
		double output = evaluate_function(polynomial, degree, input);
		printf("p(%.1lf) = %lf\n", input, output);
	}
}





int main(int argc, char* argv[]) {
    int degree, num_instances;
    double * polynomial = read_constants(&degree);
	double * instances = read_instances(&num_instances);
    clock_t ticks1, ticks2;
    
    ticks1 = clock();	  
    
    evaluate(polynomial, degree, instances, num_instances, calculatePolynomPrefixSumV2);
    ticks2 = clock();
    printf("seconds prefixSumV2 = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
    
    return 0;
}













