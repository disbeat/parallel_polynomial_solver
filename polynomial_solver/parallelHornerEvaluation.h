
ResultType parallelHornerEvaluation(CoefType* coef, int degree, TestType test) {
    
	int i, aux, tid, it, tmp;

	int k = NUM_THREADS;

	TestType testK = test;

	// Result of the polynom
	ResultType result = 0;
	// Shared array
	TestType * resultSpace = (TestType *) malloc( k * sizeof(TestType) );
    TestType resultAux;

	if ( degree < 2*k )
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