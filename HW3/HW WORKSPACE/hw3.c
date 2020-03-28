/*
 * Author: Muhammad Khairi Norizan
 * Purpose: HW3 Display Windows
 * Language: C
 * 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct window{
    unsigned int xlow; //upper left corner coordinate
    unsigned int ylow; //upper left corner coordinate
    unsigned int xhigh; //bottom right corner coordinate
    unsigned int yhigh; //bottom right corner coordinate
    char wch; //character to be filled on the window
};

struct window displayWindow;//output screen

void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]){
    //int size = Wsize;
    unsigned int sizeofLine = (x2-x1)+1;
    //W[size];

    line[sizeofLine];
   //int k = 0;

    for(int i = 0; i < Wsize; i++){
        if(y >= W[i].ylow && y <= W[i].yhigh){
            //printf("Y of %u is within W[%d] Y ranges\n", y , i);
            /* When xhigh is out of bound */
            if(W[i].xlow >= x1 && W[i].xhigh > x2){ // xhigh out of bound
                //unsigned int arrIndex = sizeofLine - (x2 - W[i].xlow) - 1;
                unsigned int arrIndex = sizeofLine - 1;

                //printf("x low inside bound and x high for W[%d] is outside bound\n", i);

                for(unsigned int j = x2; j >= W[i].xlow; j--){
                    line[arrIndex] = W[i].wch;
                    arrIndex--;
                }

                // for(unsigned int j = W[i].xlow; j < sizeofLine-1; j++){
                //     line[arrIndex] = W[i].wch;
                // }

                // for(unsigned int k = 0; k < sizeofLine; k++){
                //     //if(k == sizeofLine-1){
                //         printf("line[%u] for height %u = %c \n", k, y, line[k]);
                //     //}else{
                //       //  printf("line[%u] for height %u = %c ", k, y, W[i].wch);
                //     //}
                    
                // }
            /* When xlow is out of bound */
            }else if(W[i].xlow <= x1 && W[i].xhigh <= x2){ // xlow out of bound
                unsigned int arrIndex = 0;

                //printf("x low is out of bound and x high for W[%d] is inside bound\n", i);

                for(unsigned int j = x1; j <= W[i].xhigh; j++){
                    line[arrIndex] = W[i].wch;
                    arrIndex++;
                }

            /* When both xlow and xhigh are inside bound */
            }else if(W[i].xlow > x1 && W[i].xhigh < x2){ // both xlow xhigh inside bound
                unsigned int xDiff = x2 - W[i].xhigh;
                unsigned int arrIndex = sizeofLine - xDiff - 1;

                //printf("Both x low and x high for W[%d] are inside bound\n", i);

                for(unsigned int j = W[i].xhigh; j >= W[i].xlow; j--){
                    line[arrIndex] = W[i].wch;
                    arrIndex--;
                }

                // for(unsigned int k = 0; k < sizeofLine; k++){
                //     //if(k == sizeofLine-1){
                //         printf("line[%u] for height %u = %c \n", k, y, line[k]);
                //     //}else{
                //       //  printf("line[%u] for height %u = %c ", k, y, W[i].wch);
                //     //}
                    
                // }

            /* When both xlow and xhigh are outside or equal the bound */
            }else if(W[i].xlow <= x1 && W[i].xhigh >= x2){ //both outside bound or equal
                for(unsigned int j = 0; j < sizeofLine; j++){
                    line[j] = W[i].wch;
                }
                
            }
        }
    }

}

unsigned fill_windows(struct window W[], unsigned wmax){
    unsigned int xlowVal = 0; //upper left corner coordinate
    unsigned int ylowVal = 0; //upper left corner coordinate
    unsigned int xhighVal = 0; //bottom right corner coordinate
    unsigned int yhighVal = 0; //bottom right corner coordinate
    char wchVal = ' '; //character to be filled on the window

    /* Scan from standard input the coordinates of the windows for wmax number of times */
    for(int i = 0; i < wmax; i++){
        scanf("%u,%u %u,%u%*c%c", &xlowVal,&ylowVal,&xhighVal,&yhighVal, &wchVal);
        //printf("test%c", wchVal);
        W[i].xlow = xlowVal;
        W[i].ylow = ylowVal;
        W[i].xhigh = xhighVal;
        W[i].yhigh = yhighVal;
        W[i].wch = wchVal;
    }

    return wmax;
}

int main(){

    unsigned int xlowVal = 0; //upper left corner coordinate
    unsigned int ylowVal = 0; //upper left corner coordinate
    unsigned int xhighVal = 0; //bottom right corner coordinate
    unsigned int yhighVal = 0; //bottom right corner coordinate
    unsigned int wnum = 0;
    char wchVal = ' '; //character to be filled on the window

    /* Scan from standard input the coordinates of the screen to display */
    scanf("%u,%u %u,%u", &xlowVal,&ylowVal,&xhighVal,&yhighVal);

    displayWindow.xlow = xlowVal;
    displayWindow.ylow = ylowVal;
    displayWindow.xhigh = xhighVal;
    displayWindow.yhigh = yhighVal;
    displayWindow.wch = wchVal;

    /* Scan from standard input the number of window */
    scanf("%u", &wnum);

    struct window W[wnum];

    /* Call fill_windows function to fill up the windows with coordinates from standard input */
    fill_windows(W,wnum);

    //printf("Display window; xlow: %u, ylow: %u, xhigh: %u, yhigh: %u\n", displayWindow.xlow, displayWindow.ylow, displayWindow.xhigh, displayWindow.yhigh);

    //printf("WNum = %u\n", wnum);

    // for(int i = 0; i < wnum; i++){
    //     printf("Window %d = ", i);
    //     printf("xlow: %u, ylow: %u, xhigh: %u, yhigh: %u, wch: %c\n", W[i].xlow, W[i].ylow, W[i].xhigh, W[i].yhigh, W[i].wch);
    // }

    unsigned int lineSize = (displayWindow.xhigh - displayWindow.xlow) + 1;
    char line[lineSize];

    for(unsigned int i = 0; i < lineSize; i++){
        line[i] = displayWindow.wch;
    }

    for(unsigned int j = displayWindow.ylow; j <= displayWindow.yhigh; j++){
        fill_line(W, wnum, displayWindow.xlow, displayWindow.xhigh, j, line);

        //printf("Line at height %u\n", j);

        // for(unsigned int k = 0; k < lineSize; k++){
        //     printf("line[%u] = '%c'\n", k, line[k]);
        // }
        for(unsigned int k = 0; k < lineSize; k++){
            printf("%c", line[k]);
        }

        printf("\n");

        for(unsigned int k = 0; k < lineSize; k++){
            line[k] = ' ';
        }
    }

    // for(unsigned int j = displayWindow.yhigh; j > displayWindow.ylow; j--){
    //     fill_line(W, wnum, displayWindow.xlow, displayWindow.xhigh, j, line);

    //     //printf("Line at height %u\n", j);

    //     // for(unsigned int k = 0; k < lineSize; k++){
    //     //     printf("line[%u] = '%c'\n", k, line[k]);
    //     // }
    //     for(unsigned int k = 0; k < lineSize; k++){
    //         printf("%c", line[k]);
    //     }

    //     printf("\n");

    //     for(unsigned int k = 0; k < lineSize; k++){
    //         line[k] = ' ';
    //     }
    // }

    // //if(displayWindow.ylow == 0){
    // fill_line(W, wnum, displayWindow.xlow, displayWindow.xhigh, displayWindow.ylow, line);

    // //printf("Line at height %u\n", displayWindow.ylow);

    // for(unsigned int k = 0; k < lineSize; k++){
    //     printf("%c", line[k]);
    // }

    // printf("\n");
    //}


    return 0;

}

