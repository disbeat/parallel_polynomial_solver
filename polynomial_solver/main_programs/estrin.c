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

ResultType estrinEvaluation(CoefType* coef, int degree, TestType test) {
    
	int n, i, it, length;

	TestType * swap;
	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * values = (TestType *) malloc( (degree+1) * sizeof(TestType) );
	TestType * copy = (TestType *) malloc( (degree+1) * sizeof(TestType) );

	memcpy( values, coef, (degree + 1) * sizeof(TestType) );

	// number of iterations
	n = (int) ceil( log( degree + 1 ) / log( 2 ) );
	
	// Set one thread per degree
	omp_set_num_threads(NUM_THREADS);	

	length = degree+1;
	
	for( it = 0; it < n; it++ )
	{	

		#pragma omp parallel for
		for ( i = 0; i < length / 2; i++ )
			copy[ i ] = values[ i * 2 ] + values[ i * 2 + 1 ] * test;

		//copy last value if length is odd
		if ( length % 2 == 1 )
			copy[ length/2 ] = values[ length - 1 ];


		test *= test;
		length = ceil(length/2.0);
		
		swap = copy;
		copy = values;
		values = swap;

	}
	
	result = values[0];


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
    
    evaluate(polynomial, degree, instances, num_instances, estrinEvaluation);
    ticks2 = clock();
   	printf("seconds estrin = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);

    
    
    return 0;
}













