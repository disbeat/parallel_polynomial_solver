
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