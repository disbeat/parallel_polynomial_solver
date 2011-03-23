

ResultType hornerEvaluation(CoefType* coefs, int degree, TestType test) {
    int i;
    ResultType result = 0;
  
    result = coefs[degree];

    for(i = degree - 1; i >= 0; i--)
        result = coefs[i] + test * result;
   
    return result;
    
}
