#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

int main(){
    bool readNum = true;
    int maxnum = 0;
    char dest = ' ';
    char src = ' ';

    scanf("%d", &maxnum);

    scanf("%c->%c",&src, &dest);

    printf("maxnum: %d, source: %c, dest: %c\n", maxnum, src, dest);

    // while(!feof(stdin)){
    //     if(readNum == true){
    //         scanf("%d", maxnum);
    //         readNum = false;
    //     }else{
    //         scanf("%c->%c",src, dest);
    //     }
    // }

    // printf("maxnum: %d, source: %c, dest: %c\n", maxnum, src, dest);
    return 1;

}