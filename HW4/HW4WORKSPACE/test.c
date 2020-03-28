#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
    int array[6] = {2,4,6,8,10,0}; 
    int* arrayEh = array;
    //show_array(arrayEh);

    for(int i = 0 ; *(arrayEh+i) != 0; i++){
        printf("%d ", *(arrayEh+i));
    }

    return 0;
}

void show_array(int* A){
    //need to test
    for( ; *A != 0; ++A){
        printf("%d", *A);
    }
}