#ifndef _PREFIXSUM_H
#define _PREFIXSUM_H

/**
 * Calcutates the polynom using the prefix sum problem, for a given test value
 *
 * Uses "degree" threads, shared memory (2 omp barrier)
 */
ResultType calculatePolynomPrefixSumV1(CoefType* coef, int degree, TestType test)
{
	int n, offset, tid, i;
	TestType aux;

	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * values = (TestType *) malloc( degree * sizeof(TestType) );

	// Set one thread per degree
	omp_set_num_threads(degree);

	// number of iterations
	n = (int) ceil( log( degree ) / log( 2 ) );
	offset = 1;

	#pragma omp parallel shared(values, n, degree, test, offset) private(tid, aux, i) reduction(+:result)
	{
		tid = omp_get_thread_num();
		
		//initialize values with test value
		values[tid] = test;

		for( i = 0; i < n; i++ )
		{
			aux = values[tid];
	
			#pragma omp barrier
			if ( tid + offset < degree )	
				values[tid + offset] *= aux;

			#pragma omp barrier

			#pragma omp master
			offset *= 2;
			
						
		}

		result = values[tid] * coef[tid + 1];

//		printf("[tid: %d] %d ^ %d = %d | ans * %d = %d\n", tid, test, tid, values[tid], coef[degree + 1], result);
	}

	result += coef[0];


	free(values);

	return result;
}


/**
 * Calcutates the polynom using the prefix sum problem, for a given test value
 *
 * Uses "degree" threads, copy memory (2 omp barrier)
 */
ResultType calculatePolynomPrefixSumV1_1(CoefType* coef, int degree, TestType test)
{
	int n, offset, tid, i;
	TestType * aux;

	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * values = (TestType *) malloc( degree * sizeof(TestType) );
	TestType * copy = (TestType *) malloc( degree * sizeof(TestType) );

	// Set one thread per degree
	omp_set_num_threads(degree);

	// number of iterations
	n = (int) ceil( log( degree ) / log( 2 ) );
	offset = 1;

	#pragma omp parallel private(tid, aux, i) reduction(+:result)
	{
		tid = omp_get_thread_num();
		
		//initialize values with test value
		values[tid] = test;
		copy[tid] = test;

		for( i = 0; i < n; i++ )
		{
			#pragma omp barrier

			if ( tid + offset < degree )	
				copy[tid + offset] = values[tid] * values[tid + offset];

			#pragma omp barrier

			#pragma omp master
			{
				offset *= 2;
				aux = copy;
				copy = values;
				values = aux;
			}
									
		}

		result = values[tid] * coef[tid + 1];

//		printf("[tid: %d] %d ^ %d = %d | ans * %d = %d\n", tid, test, tid, values[tid], coef[degree + 1], result);
	}

	result += coef[0];


	free(values);
	free(copy);

	return result;
}


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

    copy[0] = test;

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
		for (i = offset/2; i < offset; i++)
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


#endif