/* 
* Author: Muhammad Khairi Norizan (Khairi)
* Purpose: HW4: Integer Factors (Prime Factorization) 
*
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function Prototypes */
unsigned* build_factors(unsigned N);
void show_array(unsigned* A);
unsigned* build_common(unsigned* A, unsigned* B);

/* Return the prime factorization of an unsigned integer, as a pointer to a dynamically allocated array of integers */
unsigned* build_factors(unsigned N){
    int index = 0;
    int primeIndex = 0;
    int retValIndex = 0;
    unsigned int* retVal;    

    if(N == 0 || N == 1){
        unsigned int arrayofRetVal[1];
        arrayofRetVal[retValIndex] = 0;

        retVal = (unsigned int*)malloc(sizeof(unsigned int));

        memcpy(retVal, arrayofRetVal, sizeof(unsigned int));

        return retVal;

    }else{
        unsigned int copyofN = N;
        unsigned int allNumbersofN[N-1];

        /* Store all the values from 2 to N into allNumbersofN */
        for(unsigned i=2; i <= N; i++){
            allNumbersofN[index] = i;
            index++;
        }

        /* Performing the Seive of erathosthenes' method */
        for(int i=0; i < index-1; i++){
            for(int j=i+1; j < index; j++){
                if((allNumbersofN[i] != 0 && allNumbersofN[j] != 0) && (allNumbersofN[j]%allNumbersofN[i] == 0)){
                    allNumbersofN[j] = 0;
                }

            }
        }

        /* Count the number of prime numbers of N */
        int count = 0;

        for(int i=0; i < index; i++){
            if(allNumbersofN[i] != 0){
                count++;
            }
        }
        
        /* Initilize primeNumberofN with a size of count */
        unsigned int primeNumberofN[count];

        /* store the prime numbers of N from allNumbersofN into primeNumberofN */
        for(int i=0; i < index; i++){
            if(allNumbersofN[i] != 0){
                primeNumberofN[primeIndex] = allNumbersofN[i];
                primeIndex++;
            }
        }

        int tracker = 0;
        int countArrayofRetVal = 0;      

        /* Count the number of possible prime factorization of N */  
        while(copyofN > 1){
            if(copyofN % primeNumberofN[tracker] == 0){
                copyofN = copyofN / primeNumberofN[tracker];
                countArrayofRetVal++;

            }else{
                tracker++;
            }

        }
         
        countArrayofRetVal++;

        tracker = 0;

        /* Initilize arrayofRetVal with a size of countArrayofRetVal */
        unsigned int arrayofRetVal[countArrayofRetVal];

        /* Store the arrayofRetVal */
        while(N > 1){
            if(N % primeNumberofN[tracker] == 0){
                N = N / primeNumberofN[tracker];
                arrayofRetVal[retValIndex] = primeNumberofN[tracker];
                retValIndex++;
            }else{
                tracker++;
            }

        }

        arrayofRetVal[retValIndex] = 0;
        retValIndex++;

        retVal = (unsigned int*)malloc(retValIndex * sizeof(unsigned int));

        memcpy(retVal, arrayofRetVal, retValIndex * sizeof(unsigned int));
        
        return retVal;

    }


}

/* Display all the elements of *A */
void show_array(unsigned* A){

    /* Break the loop when *A is pointing to element '0' */
    for(int i = 0 ; *(A+i) != 0; i++){
        printf("%u ", *(A+i));
    }

    printf("\n");

}

/* takes two 0-terminated arrays of factors, and builds and returns a new 0-terminated array of factors, that 
   contains the common elements of the input arrays */
unsigned* build_common(unsigned* A, unsigned* B){

    int arrayAIndex = 0;
    int arrayBIndex = 0;
    int retvalIndex = 0;
    int arrayASize = 0;
    int arrayBSize = 0;
    int retvalSize = 0;
    
    unsigned int* retValPtr;

    /* count elements inside pointer A */
    for(int i = 0 ; *(A+i) != 0; i++){
        arrayASize++;
    }

    arrayASize++;

    /* Initilize arrayA with the size of arrayASize */
    unsigned int arrayA[arrayASize];

    /* Store all the elements in *A into arrayA */
    for(int i = 0 ; *(A+i) != 0; i++){
        arrayA[arrayAIndex] = (unsigned int)*(A+i);
        arrayAIndex++;
    }

    //store 0 at the last index
    arrayA[arrayAIndex] = 0;
    arrayAIndex++;

    /* Count the elements inside *B */
    for(int j = 0 ; *(B+j) != 0; j++){
        arrayBSize++;
    }

    arrayBSize++;

    unsigned int arrayB[arrayBSize];

    /* Store all the elements of *B into arrayB */
    for(int i = 0 ; *(B+i) != 0; i++){
        arrayB[arrayBIndex] = (unsigned int)*(B+i);
        arrayBIndex++;
    }

    //store 0 at the last index
    arrayB[arrayBIndex] = 0;
    arrayBIndex++;
    
    int i = 0;
    int j = 0;

    /* Counts the number of mutual elements in arrayA and arrayB */
    while(arrayA[i] != 0 && arrayB[j] != 0){
        if(arrayA[i] == arrayB[j]){
            retvalSize++;
            i++;
            j++;
        }else if(arrayA[i] != arrayB[j]){
            if(arrayA[i] < arrayB[j]){
                i++;
            }else if(arrayB[j] < arrayA[i]){
                j++;
            }
        }
    }
    retvalSize++;

    unsigned int retVal[retvalSize];

    i = 0;
    j = 0;

    /* storing the mutual elements into retVal */
    while(arrayA[i] != 0 && arrayB[j] != 0){
        if(arrayA[i] == arrayB[j]){
            retVal[retvalIndex] = arrayA[i];
            retvalIndex++;
            i++;
            j++;
        }else if(arrayA[i] != arrayB[j]){
            if(arrayA[i] < arrayB[j]){
                i++;
            }else if(arrayB[j] < arrayA[i]){
                j++;
            }
        }
    }

    retVal[retvalIndex] = 0;
    retvalIndex++;

    retValPtr = (unsigned int*)malloc(retvalIndex * sizeof(unsigned int));

    memcpy(retValPtr, retVal, retvalIndex * sizeof(unsigned int));

    return retValPtr;


}


int main(){
    unsigned int intM = 0;
    unsigned int intN = 0;
    unsigned int* intMFactors;
    unsigned int* intNFactors;

    printf("Enter two integers M, N: \n");

    scanf("%u, %u", &intM, &intN);

    printf("%u has factors: ", intM);

    show_array(build_factors(intM));

    printf("%u has factors: ", intN);

    show_array(build_factors(intN));

    printf("%u and %u have common factors: ", intM, intN);

    show_array(build_common(build_factors(intM),build_factors(intN)));

    return 0;

}