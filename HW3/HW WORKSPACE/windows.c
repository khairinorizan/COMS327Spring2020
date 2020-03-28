/*
 * Author: Muhammad Khairi Norizan (Khairi)
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
    unsigned int sizeofLine = (x2-x1)+1;

    line[sizeofLine];

    for(int i = 0; i < Wsize; i++){
        if(y >= W[i].ylow && y <= W[i].yhigh){
            /* When xhigh is out of bound */
            if(W[i].xlow >= x1 && W[i].xhigh > x2){ // xhigh out of bound
                unsigned int arrIndex = sizeofLine - 1;

                for(unsigned int j = x2; j >= W[i].xlow; j--){
                    line[arrIndex] = W[i].wch;
                    arrIndex--;
                }

            /* When xlow is out of bound */
            }else if(W[i].xlow <= x1 && W[i].xhigh <= x2){ // xlow out of bound
                unsigned int arrIndex = 0;

                for(unsigned int j = x1; j <= W[i].xhigh; j++){
                    line[arrIndex] = W[i].wch;
                    arrIndex++;
                }

            /* When both xlow and xhigh are inside bound */
            }else if(W[i].xlow > x1 && W[i].xhigh < x2){ // both xlow xhigh inside bound
                unsigned int xDiff = x2 - W[i].xhigh;
                unsigned int arrIndex = sizeofLine - xDiff - 1;

                for(unsigned int j = W[i].xhigh; j >= W[i].xlow; j--){
                    line[arrIndex] = W[i].wch;
                    arrIndex--;
                }

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

    unsigned int lineSize = (displayWindow.xhigh - displayWindow.xlow) + 1;
    char line[lineSize];

    /* Fill up line[] with empty char */
    for(unsigned int i = 0; i < lineSize; i++){
        line[i] = displayWindow.wch;
    }

    /* Do what fill_line supposed to do */
    for(unsigned int j = displayWindow.ylow; j <= displayWindow.yhigh; j++){
        fill_line(W, wnum, displayWindow.xlow, displayWindow.xhigh, j, line);

        for(unsigned int k = 0; k < lineSize; k++){
            printf("%c", line[k]);
        }

        printf("\n");

        for(unsigned int k = 0; k < lineSize; k++){
            line[k] = ' ';
        }
    }

    return 0;

}

