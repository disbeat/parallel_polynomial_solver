#ifndef _BASICEVALUATION_H
#define _BASICEVALUATION_H



// Calcutates the polynom using the basic algorithm, for a given test value
// This version uses dynamic programming, keeping the previous power value to aply in the new coeficient 

ResultType basicEvaluation(CoefType* coef, int degree, TestType test) {
    ResultType result = 0;
    int i;
    TestType powerValue = 1;
    
    for(i = 0; i <= degree; i++) {    
        result += (powerValue * coef[i]); 
		
		//calculate the next power value from the current
        powerValue *= test;
    }
    
    return result;
    

}

// Calcutates the polynom using the basic algorithm, for a given test value
ResultType basicEvaluationDumb(CoefType* coef, int degree, TestType test) {
    int i;
	ResultType result = 0;    
	
	// simply multiply the coeficients with the power of the test to the degree i (0..n)
    for(i = 0; i <= degree; i++)     
        result += ( coef[i] * pow(test, i) ); 
    
    return result;
}

#endif



