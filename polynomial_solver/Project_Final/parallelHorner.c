#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_STRING 256
#define NUM_THREADS 2

typedef double CoefType;
typedef double TestType;
typedef double ResultType;

ResultType hornerEvaluation(CoefType* coefs, int degree, TestType test) {
    int i;
    ResultType result = 0;
  
    result = coefs[degree];

    for(i = degree - 1; i >= 0; i--)
        result = coefs[i] + test * result;
   
    return result;
    
}

ResultType parallelHornerEvaluation(CoefType* coef, int degree, TestType test) {
    
	int i, aux, tid, it, tmp;

	int k = NUM_THREADS;

	TestType testK = test;

	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * resultSpace = (TestType *) malloc( k * sizeof(TestType) );
    TestType resultAux;

	if (degree < k)
		return hornerEvaluation(coef, degree, test);

	// Set one thread per degree
	omp_set_num_threads(NUM_THREADS);	

	aux = (degree - k + 1) / k;

	//length = degree+1;
		
	//#pragma omp parallel for reduction(*:testK)
	for (i = 1; i < k; i++)	
		testK *= test;
//	printf("x^k = %.1lf\n", testK);

	#pragma omp parallel private(tid, it, tmp, resultAux)
	{
		//thread id from 0 to k
		tid = omp_get_thread_num();

		//copy first k coefs to values
		resultAux = coef[ degree - k - tid ] + testK * coef[ degree - tid ];

		for (it = 1; it < aux; it++)
			resultAux = coef[ degree - k - it * k - tid ]    +   testK  *  resultAux;
		
		
		if (tid < (degree - k + 1) % k)
			resultAux = coef[ tid ] + testK * resultAux;	

        resultSpace[tid] = resultAux;
	}

	for (i = k - 2; i >= 0; i--)
		resultSpace[i] = test * resultSpace[i+1] + resultSpace[i];

	result = resultSpace[0];

	free(resultSpace);

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
    
    evaluate(polynomial, degree, instances, num_instances, parallelHornerEvaluation);
    ticks2 = clock();
    printf("seconds parallelHorner = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
    
    return 0;
}













