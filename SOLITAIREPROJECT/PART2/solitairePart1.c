/* 
    Author: Muhammad Khairi Norizan (Khairi)
    Purpose: Project Solitaire Part 1 & 2
    Language: C
*/

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#define tableauColumns 7
#define totalCards 52

struct card {
    int rank;
    char suit;
    char color;
};

/* FUNCTION PROTOTYPES */
bool checkIsNumber(char *stringInt);
void skipWhiteSpace(FILE *inFile);
void readComments(FILE *inFile);
void checkRules(FILE *inFile);
void checkFoundations(FILE *inFile);
void checkTableau(FILE *inFile);
void checkStock(FILE *inFile);
void checkMoves(FILE *inFile);
bool checkSuit(char suit);
bool checkRank(char rank);
bool movePile(char src, char dest);
bool checkFoundationPile(struct card **foundationSuit, int foundationSize);
bool checkUncoverTableauPile(struct card **tableauUncover, int tableauSize);
bool compareRank(char r1, char r2);
bool compareColor(char c1, char c2);
bool compareRankMoveFoundation(char r1, char r2);
int findMaxNumberofCardinUncover(int t1Uncover, int t2Uncover, int t3Uncover, int t4Uncover, int t5Uncover, int t6Uncover, int t7Uncover);
int findMaxNumberofCardinCover(int t1Cover, int t2Cover, int t3Cover, int t4Cover, int t5Cover, int t6Cover, int t7Cover);
void printResultTest();

/* END OF FUNCTION PROTOTYPES */

/* GLOBAL VARIABLE */

/* MOVE */
int moveCount;
char **moveList; //2D Array
int maxMoves;

/* FOUNDATION IS READ FROM TOP TO BOTTOM; TOP IS foundation1 */
struct card **foundationClub; //2D Array
int sizeoffoundationClub;
struct card **foundationDiamond; //2D Array
int sizeoffoundationDiamond;
struct card **foundationHeart; //2D Array
int sizeoffoundationHeart;
struct card **foundationSpade; //2D Array
int sizeoffoundationSpade;

/* STOCK */
struct card **stock;
int sizeofStock;
/* WASTE */
struct card **waste;
int sizeofWaste;
/* TABLUEAU COL 1 */
struct card **tableau1Uncover;
struct card **tableau1Cover;
int t1coverSize;
int t1uncoverSize;
/* TABLUEAU COL 2 */
struct card **tableau2Uncover;
struct card **tableau2Cover;
int t2coverSize;
int t2uncoverSize;
/* TABLUEAU COL 3 */
struct card **tableau3Uncover;
struct card **tableau3Cover;
int t3coverSize;
int t3uncoverSize;
/* TABLUEAU COL 4 */
struct card **tableau4Uncover;
struct card **tableau4Cover;
int t4coverSize;
int t4uncoverSize;
/* TABLUEAU COL 5 */
struct card **tableau5Uncover;
struct card **tableau5Cover;
int t5coverSize;
int t5uncoverSize;
/* TABLUEAU COL 6 */
struct card **tableau6Uncover;
struct card **tableau6Cover;
int t6coverSize;
int t6uncoverSize;
/* TABLUEAU COL 7 */
struct card **tableau7Uncover;
struct card **tableau7Cover;
int t7coverSize;
int t7uncoverSize;

/* DECK */
struct card *deck[52];
int deckCount = 0;

/* STRING OF A LINE OF INPUT FILE */
char readLine[100];
int lineCount;

/* SECTION FLAGS */
bool readRules;
bool readFoundations;
bool readTableau;
bool readStock;
bool readMoves;

/* RULES SECTION VARIABLES */
int turn;
int limit;

/* FOUNDATIONS SECTION VARIABLES */



/* END OF GLOBAL VARIABLE */
bool checkFoundationPile(struct card **foundationSuit, int foundationSize){
    for(int i=0; i<foundationSize-1; i++){
        struct card *currentCard = (struct card*)malloc(1*sizeof(struct card*));
        memcpy(currentCard, foundationSuit[i], 1*sizeof(struct card*));
        
        struct card *cardBelow = (struct card*)malloc(1*sizeof(struct card*));
        memcpy(cardBelow, foundationSuit[i+1], 1*sizeof(struct card*));
        

        if(currentCard->color != cardBelow->color && currentCard->suit != cardBelow->color){
            return false;
        }else{
            if(compareRank(currentCard->rank, cardBelow->rank) == false){
                return false;
            }
        }
    }

    return true;

}

bool compareColor(char c1, char c2){
    if(c1 == 'R' && c2 == 'R'){
        return false;
    }else if(c1 == 'B' && c2 == 'B'){
        return false;
    }else if(c1 == 'R' && c2 == 'B'){
        return true;
    }else if(c1 == 'B' && c2 == 'R'){
        return true;
    }else{
        return false;
    }
}

bool compareRank(char r1, char r2){
    char allRank[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
    int r1Index = 0;
    int r2Index = 0;

    for(int i = 0; i < 13; i++){
        if(r1 == allRank[i]){
            r1Index = i;
        }
        if(r2 == allRank[i]){
            r2Index = i;
        }
    }

    if(r1Index > r2Index){
        return true;
    }else{
        return false;
    }

}

bool compareRankMoveFoundation(char r1, char r2){
    char allRank[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
    int r1Index = 0;
    int r2Index = 0;

    for(int i = 0; i < 13; i++){
        if(r1 == allRank[i]){
            r1Index = i;
        }
        if(r2 == allRank[i]){
            r2Index = i;
        }
    }

    if(r2Index == r1Index-1){
        return true;
    }else{
        return false;
    }

}

bool movePile(char src, char dest){
    if(src == '1'){
        if(t1uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau1Uncover[t1uncoverSize-1];

                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau1uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau1uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau1uncoverbottomCard->suit;

                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau1Uncover[t1uncoverSize-1]);
                            t1uncoverSize--;

                            if(t1uncoverSize < 0){
                                return false;
                            }

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                        }else{
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            
                            if(compareRankMoveFoundation(tableau1uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){

                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau1uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau1uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau1uncoverbottomCard->suit;                                    

                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau1Uncover[t1uncoverSize-1]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */                             
                                
                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau1uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau1uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau1uncoverbottomCard->suit;

                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau1Uncover[t1uncoverSize-1]);
                            t1uncoverSize--;

                            if(t1uncoverSize < 0){
                                return false;
                            }

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB TOP CARD AT CLUB FOUNDATION */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            // struct card *foundationClubTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            // memcpy(foundationClubTopCard, foundationClub[sizeoffoundationClub-1], 1*sizeof(struct card*));
                            /* GRAB BOTTOM CARD AT TABLEAU 1 */
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            // struct card *tableau1uncoverbottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            // memcpy(tableau1uncoverbottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                           

                            if(compareRankMoveFoundation(tableau1uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                //int cpyt1uncoverSize = t1uncoverSize-1;
                                //for(int i = cpyt1uncoverSize; i >= 0; i--){
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau1uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau1uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau1uncoverbottomCard->suit;
                                    // newfoundationClubCard->color = tableau1Uncover[i]->color;
                                    // newfoundationClubCard->rank = tableau1Uncover[i]->rank;
                                    // newfoundationClubCard->suit = tableau1Uncover[i]->suit;
                                    

                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau1Uncover[t1uncoverSize-1]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau1uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau1uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau1uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau1Uncover[t1uncoverSize-1]);
                            t1uncoverSize--;

                            if(t1uncoverSize < 0){
                                return false;
                            }

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD OF HEARTFOUNDATION */
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                            // struct card *foundationHeartTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            // memcpy(foundationHeartTopCard, foundationHeart[sizeoffoundationHeart-1], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD OF TABLEAU 1 */
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            // struct card *tableau1uncoverbottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            // memcpy(tableau1uncoverbottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            if(compareRankMoveFoundation(tableau1uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                //int cpyt1uncoverSize = t1uncoverSize-1;
                                //for(int i = cpyt1uncoverSize; i >= 0; i--){
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau1uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau1uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau1uncoverbottomCard->suit;
                                    // newfoundationHeartCard->color = tableau1Uncover[i]->color;
                                    // newfoundationHeartCard->rank = tableau1Uncover[i]->rank;
                                    // newfoundationHeartCard->suit = tableau1Uncover[i]->suit;
                                    

                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau1Uncover[t1uncoverSize-1]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }

                    }else if(checkCard->suit == 's'){
                        if(sizeoffoundationSpade == 0){
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];
                            struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationSpadeCard->color = tableau1uncoverbottomCard->color;
                            newfoundationSpadeCard->rank = tableau1uncoverbottomCard->rank;
                            newfoundationSpadeCard->suit = tableau1uncoverbottomCard->suit;

                            foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;

                            foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                            free(tableau1Uncover[t1uncoverSize-1]);
                            t1uncoverSize--;

                            if(t1uncoverSize < 0){
                                return false;
                            }

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));



                        }else{
                            /* GRAB THE TOP CARD FROM SPADE FOUNDATION */
                            struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                            /* GRAB THE BOTTOM CARD FROM SPADE FOUNDATION */
                            struct card *tableau1uncoverbottomCard = tableau1Uncover[t1uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau1uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                /* CREATE A NEW FOUNDATION CARD FOR SPADE FOUNDATION */
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau1uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau1uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau1uncoverbottomCard->suit;
                                    
                                /* ADD THE NEW FOUNDATION CARD AT THE TOP OF SPADE FOUNDATION PILE */
                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau1Uncover[t1uncoverSize-1]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else{
                        return false;
                    }                    
                }else if(dest == '2'){
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1, tableau1Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation2Card->color = topCardTableau1->color;
                                newfoundation2Card->rank = topCardTableau1->rank;
                                newfoundation2Card->suit = topCardTableau1->suit;                                    

                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau1Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                                                                                
                                        newfoundation2Card->color = tableau1UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau1UncoverTopCard->suit;                                            

                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1, tableau1Uncover[i], 1*sizeof(struct card*));
                                
            
                                newfoundation3Card->color = topCardTableau1->color;
                                newfoundation3Card->rank = topCardTableau1->rank;
                                newfoundation3Card->suit = topCardTableau1->suit;                                    

                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau1Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau3UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau3Uncover[t3uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation3Card->color = tableau1UncoverTopCard->color;
                                        newfoundation3Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation3Card->suit = tableau1UncoverTopCard->suit;                                            

                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }

                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1, tableau1Uncover[i], 1*sizeof(struct card*));
                                
                                
                                newfoundation4Card->color = topCardTableau1->color;
                                newfoundation4Card->rank = topCardTableau1->rank;
                                newfoundation4Card->suit = topCardTableau1->suit;                                    

                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                            }
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t4uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                                                                                

                                        newfoundation4Card->color = tableau1UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau1UncoverTopCard->suit;                                            

                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1, tableau1Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation5Card->color = topCardTableau1->color;
                                newfoundation5Card->rank = topCardTableau1->rank;
                                newfoundation5Card->suit = topCardTableau1->suit;
                                    
                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }

                            }                            
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau1Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau5UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverBottomCard, tableau5Uncover[t5uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                                        

                                        newfoundation5Card->color = tableau1UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau1UncoverTopCard->suit;                                            

                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }

                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1Uncover, tableau1Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation6Card->color = topCardTableau1Uncover->color;
                                newfoundation6Card->rank = topCardTableau1Uncover->rank;
                                newfoundation6Card->suit = topCardTableau1Uncover->suit;                                    

                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }
                                                                
                            }
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau1Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                            
                                        newfoundation6Card->color = tableau1UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau1UncoverTopCard->suit;                                            

                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            int cpyt1uncoverSize = t1uncoverSize;
                            for(int i = 0; i < cpyt1uncoverSize; i++){
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau1Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau1Uncover, tableau1Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation7Card->color = topCardTableau1Uncover->color;
                                newfoundation7Card->rank = topCardTableau1Uncover->rank;
                                newfoundation7Card->suit = topCardTableau1Uncover->suit;
                                    
                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau1Uncover[i]);
                                t1uncoverSize--;

                                if(t1uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        if(checkUncoverTableauPile(tableau1Uncover, t1uncoverSize) == true){
                            struct card *tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverTopCard, tableau1Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau1UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau1UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt1uncoverSize = t1uncoverSize;
                                    for(int i = 0; i < cpyt1uncoverSize; i++){
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau1UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1UncoverTopCard, tableau1Uncover[i], 1*sizeof(struct card*));
                                    
                                        newfoundation7Card->color = tableau1UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau1UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau1UncoverTopCard->suit;
                                            
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau1Uncover[i]);
                                        t1uncoverSize--;

                                        if(t1uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        /* OPEN COVERED CARD */ 
        if(t1coverSize > 0 && t1uncoverSize == 0){
            struct card t1CoveredCard = *tableau1Cover[t1coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t1CoveredCard.color;
            toAddCard->rank = t1CoveredCard.rank;
            toAddCard->suit = t1CoveredCard.suit;
            
            tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau1Uncover[t1uncoverSize], toAddCard, 1*sizeof(struct card*));
            t1uncoverSize++;

            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

            free(tableau1Cover[t1coverSize-1]);
            t1coverSize--;
            tableau1Cover = realloc(tableau1Cover, (t1coverSize+1) * sizeof(struct card));


        }
        /* DONE OPEN COVERED CARD */

    }else if(src == '2'){
        if(t2uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize)){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau2Uncover[t2uncoverSize-1];
                        
                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));;
                            newfoundationDiamondCard->color = tableau2uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau2uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau2uncoverbottomCard->suit;

                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau2Uncover[t2uncoverSize-1]);
                            t2uncoverSize--;

                            if(t2uncoverSize < 0){
                                return false;
                            }

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                        }else{
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];

                            if(compareRankMoveFoundation(tableau2uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){

                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));;
                                newfoundationDiamondCard->color = tableau2uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau2uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau2uncoverbottomCard->suit;

                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau2Uncover[t2uncoverSize-1]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */
                                    
                            }else{
                                return false;
                            }
                        }

                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau2uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau2uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau2uncoverbottomCard->suit;

                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau2Uncover[t2uncoverSize-1]);
                            t2uncoverSize--;

                            if(t2uncoverSize < 0){
                                return false;
                            }

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));



                        }else{
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];
                            

                            if(compareRankMoveFoundation(tableau2uncoverbottomCard->rank, foundationClubTopCard->rank) == true){

                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau2uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau2uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau2uncoverbottomCard->suit;

                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau2Uncover[t2uncoverSize-1]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau2uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau2uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau2uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau2Uncover[t2uncoverSize-1]);
                            t2uncoverSize--;

                            if(t2uncoverSize < 0){
                                return false;
                            }

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                        }else{
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];

                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau2uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){

                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau2uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau2uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau2uncoverbottomCard->suit;                                    

                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau2Uncover[t2uncoverSize-1]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
          
                            }else{
                                return false;
                            }
                        }

                    }else if(checkCard->suit == 's'){
                        if(sizeoffoundationSpade == 0){
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];
                            struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau2uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau2uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau2uncoverbottomCard->suit;
                                    

                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau2Uncover[t2uncoverSize-1]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                        }else{
                            struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                            struct card *tableau2uncoverbottomCard = tableau2Uncover[t2uncoverSize-1];

                            if(compareRankMoveFoundation(tableau2uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau2uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau2uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau2uncoverbottomCard->suit;
                                    

                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau2Uncover[t2uncoverSize-1]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else{
                        return false;
                    }
                }else if(dest == '1'){
                    /* NO NEED ANY VALIDATION TO INSERT TO TABLEAU 1 */
                    if(t1uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardtableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardtableau2, tableau2Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation1Card->color = topCardtableau2->color;
                                newfoundation1Card->rank = topCardtableau2->rank;
                                newfoundation1Card->suit = topCardtableau2->suit;                                    

                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* DO VALIDATION TO CHECK COLOR AND RANK THE BOTTOM CARD OF TABLEAU 1 BOTTOM CARD */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = tableau2Uncover[0];
                            /* GRAB THE BOTTOM CARD AT TABLEAU 1 */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 2 TO TABLEAU 1*/
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2UncoverTopCard, tableau2Uncover[i], 1*sizeof(struct card*));                            

                                        newfoundation1Card->color = tableau2UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau2UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau2UncoverTopCard->suit;                            

                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    /* NO VALIDATION NEED TO MOVE CARDS */
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardtableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardtableau2, tableau2Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation3Card->color = topCardtableau2->color;
                                newfoundation3Card->rank = topCardtableau2->rank;
                                newfoundation3Card->suit = topCardtableau2->suit;                                    

                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        /* VALIDATION ON RANK AND COLOR TO MOVE */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverTopCard, tableau2Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD TABLEAU 3 */
                            struct card *tableau3UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverBottomCard, tableau3Uncover[t3uncoverSize-1], 1*sizeof(struct card*));                           

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 2 TO TABLEAU 3 */
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        /* CREATE NEW CARD TO BE MOVED */
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        newfoundation3Card->color = tableau2Uncover[i]->color;
                                        newfoundation3Card->rank = tableau2Uncover[i]->rank;
                                        newfoundation3Card->suit = tableau2Uncover[i]->suit;
                                            
                                        /* MOVE CARD TO TABLEAU 3 at Index */
                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    /* RESIZE TABLEAU 2 */
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    /* NO VALIDATION NEEDED TO MOVED AT TABLEAU 4 */
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau2, tableau2Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation4Card->color = topCardTableau2->color;
                                newfoundation4Card->rank = topCardTableau2->rank;
                                newfoundation4Card->suit = topCardTableau2->suit;                                    

                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                        /* DO VALIDATION FOR RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverTopCard, tableau2Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 4 */
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t4uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 2 TO TABLEAU 4 */
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        /* CREATE NEW CARD TO BE MOVED */
                                        struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2UncoverTopCard, tableau2Uncover[i], 1*sizeof(struct card*));

                                        newfoundation4Card->color = tableau2UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau2UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau2UncoverTopCard->suit;
                                            
                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    /* EMPTY TABLEAU NO VALIDATION NEED */
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            /* MOVE ALL THE CARDS FROM TABLEAU 2 TO TABLEAU 5 */
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardtableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardtableau2, tableau2Uncover[i], 1*sizeof(struct card*));

                                newfoundation5Card->color = topCardtableau2->color;
                                newfoundation5Card->rank = topCardtableau2->rank;
                                newfoundation5Card->suit = topCardtableau2->suit;                                    

                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = tableau2Uncover[0];

                            /* GRAB THE BOTTOM CARD AT TABLEAU 5 */
                            struct card *tableau5UncoverBottomCard = tableau5Uncover[t5uncoverSize-1];                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2UncoverTopCard, tableau2Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation5Card->color = tableau2UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau2UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau2UncoverTopCard->suit;                                            

                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    /* TABLEAU EMPTY NO VALIDATION NEED */
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardtableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardtableau2, tableau2Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation6Card->color = topCardtableau2->color;
                                newfoundation6Card->rank = topCardtableau2->rank;
                                newfoundation6Card->suit = topCardtableau2->suit;                                    

                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverTopCard, tableau2Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 6 */
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        /* CREATE A NEW CARD TO MOVE */
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2UncoverTopCard, tableau2Uncover[i], 1*sizeof(struct card*));

                                        newfoundation6Card->color = tableau2UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau2UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau2UncoverTopCard->suit;
                                            
                                        /* MOVE CARD TO THE BOTTOM PILE OF TABLEAU 6 */
                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    /* TABLEAU 7 EMPTY NO VALIDATION NEED */
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            int cpyt2uncoverSize = t2uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 2 TO TABLEAU 7 */
                            for(int i = 0; i < cpyt2uncoverSize; i++){
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardtableau2 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardtableau2, tableau2Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation7Card->color = topCardtableau2->color;
                                newfoundation7Card->rank = topCardtableau2->rank;
                                newfoundation7Card->suit = topCardtableau2->suit;                                    

                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau2Uncover[i]);
                                t2uncoverSize--;

                                if(t2uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 7 NOT EMPTY NEED VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau2Uncover, t2uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverTopCard, tableau2Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 7 */
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau2UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau2UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt2uncoverSize = t2uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 2 TO TABLEAU 7 */
                                    for(int i = 0; i < cpyt2uncoverSize; i++){
                                        /* CREATE NEW CARD TO BE MOVED */
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau2UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2UncoverTopCard, tableau2Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation7Card->color = tableau2UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau2UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau2UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM PILE OF TABLEAU 7 */
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau2Uncover[i]);
                                        t2uncoverSize--;

                                        if(t2uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        /* OPEN COVERED CARD */ 
        if(t2coverSize > 0 && t2uncoverSize == 0){
            struct card t2CoveredCard = *tableau2Cover[t2coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t2CoveredCard.color;
            toAddCard->rank = t2CoveredCard.rank;
            toAddCard->suit = t2CoveredCard.suit;
            
            tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau2Uncover[t2uncoverSize], toAddCard, 1*sizeof(struct card*));
            t2uncoverSize++;
                        
            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

            free(tableau2Cover[t2coverSize-1]);
            t2coverSize--;
            tableau2Cover = realloc(tableau2Cover, (t2coverSize+1) * sizeof(struct card));

        }
        /* DONE OPEN COVERED CARD */

    }else if(src == '3'){
        if(t3uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize)){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau3Uncover[t3uncoverSize-1];
                        
                    /* CHECK IF CARD TO BE MOVED IS DIAMOND SUIT */
                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau3uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau3uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau3uncoverbottomCard->suit;

                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau3Uncover[t3uncoverSize-1]);
                            t3uncoverSize--;

                            if(t3uncoverSize < 0){
                                return false;
                            }

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD OF DIAMOND FOUNDATION */
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 3 */
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];
                                
                            if(compareRankMoveFoundation(tableau3uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){
                                /* CREATE A NEW CARD TO MOVE */
                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau3uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau3uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau3uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OP DIAMOND FOUNDATION */
                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau3Uncover[t3uncoverSize-1]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */
                                    
                            }else{
                                return false;
                            }

                            }
                            

                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau3uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau3uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau3uncoverbottomCard->suit;

                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau3Uncover[t3uncoverSize-1]);
                            t3uncoverSize--;

                            if(t3uncoverSize < 0){
                                return false;
                            }

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));


                        }else{
                            /* GRAB TOP CARD AT CLUB FOUNDATION */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            /* GRAB BOTTOM CARD AT TABLEAU 3 */
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau3uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                /* CREATE A NEW CARD */
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau3uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau3uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau3uncoverbottomCard->suit; 

                                /* MOVE NEW CARD TO THE TOP OF CLUB FOUNDATION */
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau3Uncover[t3uncoverSize-1]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                  
                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau3uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau3uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau3uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau3Uncover[t3uncoverSize-1]);
                            t3uncoverSize--;

                            if(t3uncoverSize < 0){
                                return false;
                            }

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD AT HEART FOUNDATION */
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                            /* GRAB THE BOTTOM CARD AT TABLEAU 3 */
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau3uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                /* CREATE NEW CARD TO MOVE */
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau3uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau3uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau3uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OF HEART FOUNDATION */
                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau3Uncover[t3uncoverSize-1]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            

                    }else if(checkCard->suit == 's'){
                        if(sizeoffoundationSpade == 0){
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];
                            struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationSpadeCard->color = tableau3uncoverbottomCard->color;
                            newfoundationSpadeCard->rank = tableau3uncoverbottomCard->rank;
                            newfoundationSpadeCard->suit = tableau3uncoverbottomCard->suit;
                                        

                            foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;

                            foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                            free(tableau3Uncover[t3uncoverSize-1]);
                            t3uncoverSize--;

                            if(t3uncoverSize < 0){
                                return false;
                            }

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD OF SPADE FOUNDATION */
                            struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 3 */
                            struct card *tableau3uncoverbottomCard = tableau3Uncover[t3uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau3uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau3uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau3uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau3uncoverbottomCard->suit;
                                        

                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau3Uncover[t3uncoverSize-1]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            

                    }else{
                        return false;
                    }
                    
                }else if(dest == '1'){
                    /* TABLEAU 1 IS EMPTY NO VALIDATION NEED */
                    if(t1uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            /* MOVE ALL THE CARDS FROM TABLEAU 3 TABLEAU 1 */
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE A NEW CARD */
                                struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation1Card->color = topCardTableau3->color;
                                newfoundation1Card->rank = topCardTableau3->rank;
                                newfoundation1Card->suit = topCardTableau3->suit;

                                /* ADD NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 1 IS NOT EMPTY VALIDATION NEED FOR RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            /* GRAB THE TOP CARD OF TABLEAU 3 */
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD OF TABLEAU 3 */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 1 */
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE A NEW CARD */
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation1Card->color = tableau3UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau3UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '2'){
                    /* TABLEAU 2 IS EMPTY NO VALIDATION NEED */
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            /* MOVE ALL THE CARDS FROM TABLEAU 3 TO TABLEAU 2 */
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation2Card->color = topCardTableau3->color;
                                newfoundation2Card->rank = topCardTableau3->rank;
                                newfoundation2Card->suit = topCardTableau3->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 3 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 3 */
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 2 */
                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 2 */
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation2Card->color = tableau3UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau3UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    /* TABLEAU 4 IS EMPTY NO VALIDATION NEED */
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 4 */
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE A NEW CARD */
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                
                                
                                newfoundation4Card->color = topCardTableau3->color;
                                newfoundation4Card->rank = topCardTableau3->rank;
                                newfoundation4Card->suit = topCardTableau3->suit;
                                   
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 3 */
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 4 */
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t4uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation4Card->color = tableau3UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau3UncoverTopCard->suit;

                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    /* TABLEAU 5 IS EMPTY NO VALIDATION NEED */
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation5Card->color = topCardTableau3->color;
                                newfoundation5Card->rank = topCardTableau3->rank;
                                newfoundation5Card->suit = topCardTableau3->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 5 IS NOT EMPTY VALIDATION NEED ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 3 */
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 5 */
                            struct card *tableau5UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverBottomCard, tableau5Uncover[t5uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    /* MOVE ALL THE CARDS FROM TABLEAU 3 TO TABLEAU 5 */
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation5Card->color = tableau3UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau3UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    /* TABLEAU 6 IS EMPTY NO VALIDATION NEED */
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 3 TO TABLEAU 6 */
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation6Card->color = topCardTableau3->color;
                                newfoundation6Card->rank = topCardTableau3->rank;
                                newfoundation6Card->suit = topCardTableau3->suit;
                                    
                                /* MOVE NEW CARD AT THE BOTTOM OF TABLEAU 6 */
                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 6 IS NOT EMPTY VALIDATION OF RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            /* GRAB THE TOP CARD OF TABLEAU 3 */
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD OF TABLEAU 6 */
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 6 */
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation6Card->color = tableau3UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau3UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    /* TABLEAU 7 IS EMPTY NO VALIDATION NEED */
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            int cpyt3uncoverSize = t3uncoverSize;
                            /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 7 */
                            for(int i = 0; i < cpyt3uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau3 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau3, tableau3Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation7Card->color = topCardTableau3->color;
                                newfoundation7Card->rank = topCardTableau3->rank;
                                newfoundation7Card->suit = topCardTableau3->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau3Uncover[i]);
                                t3uncoverSize--;

                                if(t3uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 7 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau3Uncover, t3uncoverSize) == true){
                            struct card *tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverTopCard, tableau3Uncover[0], 1*sizeof(struct card*));
                            
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau3UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau3UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt3uncoverSize = t3uncoverSize;
                                    /* MOVE ALL THE CARD FROM TABLEAU 3 TO TABLEAU 7 */
                                    for(int i = 0; i < cpyt3uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau3UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3UncoverTopCard, tableau3Uncover[i], 1*sizeof(struct card*));

                                        newfoundation7Card->color = tableau3UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau3UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau3UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau3Uncover[i]);
                                        t3uncoverSize--;

                                        if(t3uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        /* OPEN COVERED CARD */ 
        if(t3coverSize > 0 && t3uncoverSize == 0){
            struct card t3CoveredCard = *tableau3Cover[t3coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t3CoveredCard.color;
            toAddCard->rank = t3CoveredCard.rank;
            toAddCard->suit = t3CoveredCard.suit;
            
            tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau3Uncover[t3uncoverSize], toAddCard, 1*sizeof(struct card*));
            t3uncoverSize++;
            // *tableau6Uncover[t6uncoverSize] = toAddCard;
            // t6uncoverSize++;
                        
            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

            free(tableau3Cover[t3coverSize-1]);
            t3coverSize--;
            tableau3Cover = realloc(tableau3Cover, (t3coverSize+1) * sizeof(struct card));


        }
        /* DONE OPEN COVERED CARD */
        
    }else if(src == '4'){
        if(t4uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize)){
            //printf("PASSED TABLEAU VALIDATION!\n");
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau4Uncover[t4uncoverSize-1];
                        
                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau4uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau4uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau4uncoverbottomCard->suit;

                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau4Uncover[t4uncoverSize-1]);
                            t4uncoverSize--;

                            if(t4uncoverSize < 0){
                                return false;
                            }

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD OF DIAMOND FOUNDATION */
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 4 */
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau4uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau4uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau4uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau4uncoverbottomCard->suit;
                                    
                                /* MOVE NEW CARD TO THE TOP OF DIAMOND FOUNDATION */
                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau4Uncover[t4uncoverSize-1]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau4uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau4uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau4uncoverbottomCard->suit;

                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau4Uncover[t4uncoverSize-1]);
                            t4uncoverSize--;

                            if(t4uncoverSize < 0){
                                return false;
                            }

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                        }else{
                             /* GRAB THE TOP CARD OF CLUB FOUNDATION */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 4 */
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau4uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                //int cpyt1uncoverSize = t1uncoverSize-1;
                                //for(int i = cpyt1uncoverSize; i >= 0; i--){
                                /* CREATE A NEW CARD */
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau4uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau4uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau4uncoverbottomCard->suit;
                                    
                                /* MOVE NEW CARD TO THE TOP OF CLUB FOUNDATION */
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationClub = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card*));
                                free(tableau4Uncover[t4uncoverSize-1]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                           
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau4uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau4uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau4uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau4Uncover[t4uncoverSize-1]);
                            t4uncoverSize--;

                            if(t4uncoverSize < 0){
                                return false;
                            }

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP CARD OF HEART FOUNDATION */
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 4 */
                            struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau4uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau4uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau4uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau4uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OF HEART FOUNDATION */
                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card*));
                                free(tableau4Uncover[t4uncoverSize-1]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }

                    }else if(checkCard->suit == 's'){
                            if(sizeoffoundationSpade == 0){
                                struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau4uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau4uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau4uncoverbottomCard->suit;

                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau4Uncover[t4uncoverSize-1]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));


                            }else{
                                /* GRAB THE TOP CARD OF SPADE FOUNDATION */
                                struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                                /* GRAB THE BOTTOM CARD OF TABLEAU 4 */
                                struct card *tableau4uncoverbottomCard = tableau4Uncover[t4uncoverSize-1];                                

                                if(compareRankMoveFoundation(tableau4uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                    /* CREATE NEW CARD */
                                    struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                    newfoundationSpadeCard->color = tableau4uncoverbottomCard->color;
                                    newfoundationSpadeCard->rank = tableau4uncoverbottomCard->rank;
                                    newfoundationSpadeCard->suit = tableau4uncoverbottomCard->suit;
                                        
                                    /* MOVE NEW CARD TO THE TOP OF SPADE FOUNDATION */
                                    foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                    memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                    sizeoffoundationSpade++;

                                    foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                    free(tableau4Uncover[t4uncoverSize-1]);
                                    t4uncoverSize--;

                                    if(t4uncoverSize < 0){
                                        return false;
                                    }

                                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                }else{
                                    return false;
                                }
                            }

                        }else{
                            return false;
                        }
                }else if(dest == '1'){
                    /* TABLEAU 1 IS EMPTY */
                    if(t1uncoverSize == 0){
                        /* VALIDATE TABLEAU 4 PILE */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation1Card =  (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(newfoundation1Card, tableau4Uncover[i], 1*sizeof(struct card*));
                                
                                struct card *topCardTableau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation1Card->color = topCardTableau4->color;
                                newfoundation1Card->rank = topCardTableau4->rank;
                                newfoundation1Card->suit = topCardTableau4->suit;

                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;
                                /* DONE INSERTING NEW CARD TO TABLEAU 1 && REMOVING THE PILE FROM TABLEAU 4 */
                                if(t4uncoverSize < 0){
                                    return false;
                                }

                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 1 IS NOT EMPTY */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB THE TOP CARD OF TABLEAU 4 */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD OF TABLEAU 1 */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));

                                        newfoundation1Card->color = tableau4UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau4UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '2'){
                    /* TABLEAU 2 IS EMPTY NO VALIDATION NEED */
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 4 TO TABLEAU 2 */
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                
                                struct card *topCardTableau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation2Card->color = topCardTableau4->color;
                                newfoundation2Card->rank = topCardTableau4->rank;
                                newfoundation2Card->suit = topCardTableau4->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB THE TOP CARD OF TABLEAU 4 */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD OF TABLEAU 2 */
                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    /* MOVE ALL THE CARDS FROM TABLEAU 4 to TABLEAU 2 */
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));

                                        newfoundation2Card->color = tableau4UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau4UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    /* TABLEAU 3 IS NOT EMPTY NO VALIDATION NEED */
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            /* MOVE ALL THE CARD FROM TABLEAU 4 TO TABLEAU 3 */
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation3Card->color = topCardTableau4->color;
                                newfoundation3Card->rank = topCardTableau4->rank;
                                newfoundation3Card->suit = topCardTableau4->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 4 */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 3 */
                            struct card *tableau3UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverBottomCard, tableau3Uncover[t3uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 4 TO TABLEAU 3 */
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation3Card->color = tableau4UncoverTopCard->color;
                                        newfoundation3Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation3Card->suit = tableau4UncoverTopCard->suit;                                            

                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    /* TABLEAU 5 IS NOT EMPTY NO VALIDATION */
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            /* MOVE ALL THE CARDS FROM TABLEAU 4 TO TABLEAU 5 */
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTablueau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTablueau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation5Card->color = topCardTablueau4->color;
                                newfoundation5Card->rank = topCardTablueau4->rank;
                                newfoundation5Card->suit = topCardTablueau4->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF THE TABLEAU 5 */
                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 5 NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB TABLEAU 4 TOP CARD */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB TABLEAU 5 BOTTOM CARD */
                            struct card *tableau5UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverBottomCard, tableau5Uncover[t5uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));

                                        newfoundation5Card->color = tableau4UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau4UncoverTopCard->suit;
                                        
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 4 TO TABLEAU 6 */
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTablueau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTablueau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation6Card->color = topCardTablueau4->color;
                                newfoundation6Card->rank = topCardTablueau4->rank;
                                newfoundation6Card->suit = topCardTablueau4->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 6 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB TABLEAU 4 TOP CARD */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    /* MOVE ALL THE CARDS FROM TABLEAU 4 TO TABLEAU 6 */
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));

                                        newfoundation6Card->color = tableau4UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau4UncoverTopCard->suit;
                                        
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                }
                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    /* TABLEAU 7 IS EMPTY NOT VALIDATION NEED */
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            int cpyt4uncoverSize = t4uncoverSize;
                            for(int i = 0; i < cpyt4uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTablueau4 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTablueau4, tableau4Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation7Card->color = topCardTablueau4->color;
                                newfoundation7Card->rank = topCardTablueau4->rank;
                                newfoundation7Card->suit = topCardTablueau4->suit;
                                    
                                /* MOVE NEW ARD TO THE BOTTOM OF TABLEAU 7 */
                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau4Uncover[i]);
                                t4uncoverSize--;

                                if(t4uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 7 IS NOT EMPTY VALIDATION ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau4Uncover, t4uncoverSize) == true){
                            /* GRAB TOP CARD IN TABLEAU 4 */
                            struct card *tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverTopCard, tableau4Uncover[0], 1*sizeof(struct card*));
                            /* GRAB BOTTOM CARD IN TABLEAU 7 */
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau4UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau4UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt4uncoverSize = t4uncoverSize;
                                    /* MOVE ALL CARD FROM TABLEAU 4 TO TABLEAU 7 */
                                    for(int i = 0; i < cpyt4uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau4UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4UncoverTopCard, tableau4Uncover[i], 1*sizeof(struct card*));

                                        newfoundation7Card->color = tableau4UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau4UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau4UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau4Uncover[i]);
                                        t4uncoverSize--;

                                        if(t4uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                }
                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        
        /* OPEN COVERED CARD */ 
        if(t4coverSize > 0 && t4uncoverSize == 0){
            struct card t4CoveredCard = *tableau4Cover[t4coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t4CoveredCard.color;
            toAddCard->rank = t4CoveredCard.rank;
            toAddCard->suit = t4CoveredCard.suit;
            
            tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau4Uncover[t4uncoverSize], toAddCard, 1*sizeof(struct card*));
            t4uncoverSize++;
            // *tableau6Uncover[t6uncoverSize] = toAddCard;
            // t6uncoverSize++;
                        
            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

            free(tableau4Cover[t4coverSize-1]);
            t4coverSize--;
            tableau4Cover = realloc(tableau4Cover, (t4coverSize+1) * sizeof(struct card));

            
        }
        /* DONE OPEN COVERED CARD */
        
    }else if(src == '5'){
        //checkUncoverTableauPile(tableau5Uncover, t5uncoverSize);
        if(t5uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize)){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau5Uncover[t5uncoverSize-1];                        

                    if(checkCard->suit == 'd'){
                        /* GET THE TOP CARD OF DIAMOND FOUNDATION */
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau5uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau5uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau5uncoverbottomCard->suit;
                                        
                            /* MOVE NEW CARD TO THE TOP OF THE DIAMOND FOUNDATION PILE */
                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau5Uncover[t5uncoverSize-1]);
                            t5uncoverSize--;

                            if(t5uncoverSize < 0){
                                return false;
                            }

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                        }else{
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            /* GET THE BOTTOM CARD OF TABLEAU 5 */
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau5uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){

                                /* CREATE NEW CARD */
                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau5uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau5uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau5uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OF THE DIAMOND FOUNDATION PILE */
                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau5Uncover[t5uncoverSize-1]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */

                            }else{
                                return false;
                            }
                        }
                        
                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau5uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau5uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau5uncoverbottomCard->suit;
                            
                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau5Uncover[t5uncoverSize-1]);
                            t5uncoverSize--;

                            if(t5uncoverSize < 0){
                                return false;
                            }

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB TOP CARD OF CLUB FOUNDATION */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            /* GRAB BOTTOM CARD OF TABLEAU 5 */
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau5uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau5uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau5uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau5uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OF CLUB FOUNDATION PILE */
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau5Uncover[t5uncoverSize-1]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                        
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau5uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau5uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau5uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau5Uncover[t5uncoverSize-1]);
                            t5uncoverSize--;

                            if(t5uncoverSize < 0){
                                return false;
                            }

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* GRAB THE TOP OF HEART FOUNDATION */
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                            /* GRAB THE BOTTOM CARD OF TABLEAU 5 */
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau5uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau5uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau5uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau5uncoverbottomCard->suit;
                                    
                                /* MOVE NEW CARD TO THE TOP PILE OF HEART FOUNDATION */
                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau5Uncover[t5uncoverSize-1]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }

                        }
                            
                    }else if(checkCard->suit == 's'){
                        if(sizeoffoundationSpade == 0){
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];
                            struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationSpadeCard->color = tableau5uncoverbottomCard->color;
                            newfoundationSpadeCard->rank = tableau5uncoverbottomCard->rank;
                            newfoundationSpadeCard->suit = tableau5uncoverbottomCard->suit;

                            /* MOVE NEW CARD TO THE TOP PILE OF SPADE FOUNDATION */
                            foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));
                            sizeoffoundationSpade++;

                            foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                            free(tableau5Uncover[t5uncoverSize-1]);
                            t5uncoverSize--;

                            if(t5uncoverSize < 0){
                                return false;
                            }

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            /* TOP CARD OF SPADE FOUNDATION */
                            struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                    
                            /* BOTTOM CARD OF TABLEAU 5 */
                            struct card *tableau5uncoverbottomCard = tableau5Uncover[t5uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau5uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau5uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau5uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau5uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP PILE OF SPADE FOUNDATION */
                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau5Uncover[t5uncoverSize-1]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                        
                    }else{
                        return false;
                    }
                }else if(dest == '1'){
                    /* TABLEAU 1 IS NOT EMPTY NO VALIDATION NEED */
                    if(t1uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation1Card->color = topCardTableau5->color;
                                newfoundation1Card->rank = topCardTableau5->rank;
                                newfoundation1Card->suit = topCardTableau5->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 1 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* GRAB THE TOP CARD AT TABLEAU 5 */
                            struct card *tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverTopCard, tableau5Uncover[0], 1*sizeof(struct card*));
                            /* GRAB THE BOTTOM CARD AT TABLEAU 1 */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    /* MOVE ALL CARDS AT TABLEAU 5 TO TABLEAU 1 */
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation1Card->color = tableau5UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau5UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '2'){
                    /* TABLEAU 2 IS EMPTY NO VALIDATION NEED */
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation2Card->color = topCardTableau5->color;
                                newfoundation2Card->rank = topCardTableau5->rank;
                                newfoundation2Card->suit = topCardTableau5->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 2 IS NOT EMPTY VALIDATION RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* CREATE NEW CARD */
                            struct card *tableau5UncoverTopCard = tableau5Uncover[0];

                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 5 TO TABLEAU 1 */
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));

                                        newfoundation2Card->color = tableau5UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau5UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    /* TABLEAU 3 IS NOT EMPTY NO VALIDATION NEED */
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 5 TO TABLEAU 1 */
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));

                                newfoundation3Card->color = topCardTableau5->color;
                                newfoundation3Card->rank = topCardTableau5->rank;
                                newfoundation3Card->suit = topCardTableau5->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 5 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* TOP CARD OF TABLEAU 5 */
                            struct card *tableau5UncoverTopCard = tableau5Uncover[0];
                            /* BOTTOM CARD OF TABLEAU 3 */
                            struct card *tableau3UncoverBottomCard = tableau3Uncover[t3uncoverSize-1];
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 5 TO TABLEAU 3 */
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = tableau5Uncover[i];                                        

                                        newfoundation3Card->color = tableau5UncoverTopCard->color;
                                        newfoundation3Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation3Card->suit = tableau5UncoverTopCard->suit;

                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    /* TABLEAU 4 IS EMPTY NO NEED VALIDATION */
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation4Card->color = topCardTableau5->color;
                                newfoundation4Card->rank = topCardTableau5->rank;
                                newfoundation4Card->suit = topCardTableau5->suit;

                                /* MOVE NEW CARD TO TABLEAU 4 */
                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* TABLEAU 5 TOP CARD */
                            struct card *tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverTopCard, tableau5Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 4 BOTTOM CARD */
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t4uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 5 TO TABLEAU 4 */
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));

                                        newfoundation4Card->color = tableau5UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau5UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    /* TABLEAU 6 IS EMPTY NO VALIDATION NEED */
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 5 TO TABLEAU 6 */
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation6Card->color = topCardTableau5->color;
                                newfoundation6Card->rank = topCardTableau5->rank;
                                newfoundation6Card->suit = topCardTableau5->suit;

                                /* MOVE NEW CARD TO THE BOTTTOM OF TABLEAU 6 */
                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 6 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* TABLEAU 5 TOP CARD */
                            struct card *tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverTopCard, tableau5Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 5 TO TABLEAU 6 */
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));

                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation6Card->color = tableau5UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau5UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    /* TABLEAU 7 IS EMPTY NO VALIDATION NEED */
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            int cpyt5uncoverSize = t5uncoverSize;
                            for(int i = 0; i < cpyt5uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau5 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau5, tableau5Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation7Card->color = topCardTableau5->color;
                                newfoundation7Card->rank = topCardTableau5->rank;
                                newfoundation7Card->suit = topCardTableau5->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau5Uncover[i]);
                                t5uncoverSize--;

                                if(t5uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 7 NOT EMPTY VALIDATE RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau5Uncover, t5uncoverSize) == true){
                            /* TABLEAU 5 TOP CARD */
                            struct card *tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverTopCard, tableau5Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 7 BOTTOM CARD */
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau5UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau5UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt5uncoverSize = t5uncoverSize;
                                    for(int i = 0; i < cpyt5uncoverSize; i++){
                                        /* NEW CARD */
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau5UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5UncoverTopCard, tableau5Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation7Card->color = tableau5UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau5UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau5UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau5Uncover[i]);
                                        t5uncoverSize--;

                                        if(t5uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }
        
        /* OPEN COVERED CARD */ 
        if(t5coverSize > 0 && t5uncoverSize == 0){
            struct card *t5CoveredCard = tableau5Cover[t5coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));

            toAddCard->color = t5CoveredCard->color;
            toAddCard->rank = t5CoveredCard->rank;
            toAddCard->suit = t5CoveredCard->suit;

            tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau5Uncover[t5uncoverSize], toAddCard, 1*sizeof(struct card*));

            t5uncoverSize++;
                        
            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

            free(tableau5Cover[t5coverSize-1]);
            t5coverSize--;
            tableau5Cover = realloc(tableau5Cover, (t5coverSize+1) * sizeof(struct card));

        }
        /* DONE OPEN COVERED CARD */
        
    }else if(src == '6'){
        if(t6uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize)){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau6Uncover[t6uncoverSize-1];
                                            
                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau6uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau6uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau6uncoverbottomCard->suit;

                            /* MOVE NEW CARD TO THE TOP PILE OF DIAMOND FOUNDATION */
                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau6Uncover[t6uncoverSize-1]);
                            t6uncoverSize--;

                            if(t6uncoverSize < 0){
                                return false;
                            }

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));


                        }else{
                            /* DIAMOND FOUNDATION TOP CARD */
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            /* TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau6uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau6uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau6uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau6uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP PILE OF DIAMOND FOUNDATION */
                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau6Uncover[t6uncoverSize-1]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */

                                    
                            }else{
                                return false;
                            }

                        }
                            
                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau6uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau6uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau6uncoverbottomCard->suit;

                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau6Uncover[t6uncoverSize-1]);
                            t6uncoverSize--;

                            if(t6uncoverSize < 0){
                                return false;
                            }

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* CLUB FOUNDATION TOP CARD */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            /* TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];
                                
                            if(compareRankMoveFoundation(tableau6uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau6uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau6uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau6uncoverbottomCard->suit;

                                /* MOVE NEW CARD TO THE TOP PILE OF CLUB FOUNDATION */
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau6Uncover[t6uncoverSize-1]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                    }else if(checkCard->suit == 'h'){
                        if(sizeoffoundationHeart == 0){
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];
                            struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationHeartCard->color = tableau6uncoverbottomCard->color;
                            newfoundationHeartCard->rank = tableau6uncoverbottomCard->rank;
                            newfoundationHeartCard->suit = tableau6uncoverbottomCard->suit;

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                            free(tableau6Uncover[t6uncoverSize-1]);
                            t6uncoverSize--;

                            if(t6uncoverSize < 0){
                                return false;
                            }

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* HEART FOUNDATION TOP CARD */
                            struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                            /* TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau6uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau6uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau6uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau6uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO HEART FOUNDATION */
                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau6Uncover[t6uncoverSize-1]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            

                    }else if(checkCard->suit == 's'){
                        if(sizeoffoundationSpade == 0){
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];
                            struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationSpadeCard->color = tableau6uncoverbottomCard->color;
                            newfoundationSpadeCard->rank = tableau6uncoverbottomCard->rank;
                            newfoundationSpadeCard->suit = tableau6uncoverbottomCard->suit;
                                        
                            /* MOVE NEW CARD TO SPADE FOUNDATION TOP OF THE PILE */
                            foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;

                            foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                            free(tableau6Uncover[t6uncoverSize-1]);
                            t6uncoverSize--;

                            if(t6uncoverSize < 0){
                                return false;
                            }

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));


                        }else{
                            /* SPADE FOUNDATION TOP CARD */
                            struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                            /* TABLEAU 6 BOTTOM CARD */
                            struct card *tableau6uncoverbottomCard = tableau6Uncover[t6uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau6uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                if(foundationSpadeTopCard->color == tableau6uncoverbottomCard->color){
                                    /* CREATE NEW CARD */
                                    struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                    newfoundationSpadeCard->color = tableau6uncoverbottomCard->color;
                                    newfoundationSpadeCard->rank = tableau6uncoverbottomCard->rank;
                                    newfoundationSpadeCard->suit = tableau6uncoverbottomCard->suit;
                                            
                                    /* MOVE NEW CARD TO SPADE FOUNDATION TOP OF THE PILE */
                                    foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                    memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                    sizeoffoundationSpade++;

                                    foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card*));
                                    free(tableau6Uncover[t6uncoverSize-1]);
                                    t6uncoverSize--;

                                    if(t6uncoverSize < 0){
                                        return false;
                                    }

                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card*));

                                }else{
                                    return false;
                                }
                                    

                            }else{
                                return false;
                            }
                        }                            

                    }else{
                        return false;
                    }
                }else if(dest == '1'){
                    /* TABLEAU 1 IS EMPTY NO VALIDATION NEED */
                    if(t1uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 6 TO TABLEAU 1 */
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau6Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6Uncover, tableau6Uncover[i], 1*sizeof(struct card*));
                                

                                newfoundation1Card->color = topCardTableau6Uncover->color;
                                newfoundation1Card->rank = topCardTableau6Uncover->rank;
                                newfoundation1Card->suit = topCardTableau6Uncover->suit;
                                // newfoundation1Card->color = tableau6Uncover[i]->color;
                                // newfoundation1Card->rank = tableau6Uncover[i]->rank;
                                // newfoundation1Card->suit = tableau6Uncover[i]->suit;
                                /* MOVE NEW CARD TO TABLEAU 1 BOTTOM PILE */
                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 1 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 1 BOTTOM CARD */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 6 TO TABLEAU 1 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*));
                                                                                
                                        newfoundation1Card->color = tableau6UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '2'){
                    /* TABLEAU 2 IS EMPTY NO VALIDATION REQUIRES */
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau6Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6Uncover, tableau6Uncover[i], 1*sizeof(struct card*));                                

                                newfoundation2Card->color = topCardTableau6Uncover->color;
                                newfoundation2Card->rank = topCardTableau6Uncover->rank;
                                newfoundation2Card->suit = topCardTableau6Uncover->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 2 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*)); 
                            /* TABLEAU 2 BOTTOM CARD */
                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*)); 
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 6 TO TABLEAU 2 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*)); 
                                        
                                        newfoundation2Card->color = tableau6UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    /* TABLEAU 3 IS NOT EMPTY NO VALIDATION REQUIRES */
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau6Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6Uncover, tableau6Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation3Card->color = topCardTableau6Uncover->color;
                                newfoundation3Card->rank = topCardTableau6Uncover->rank;
                                newfoundation3Card->suit = topCardTableau6Uncover->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 3 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 3 BOTTOM CARD */
                            struct card *tableau3UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverBottomCard, tableau3Uncover[t3uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL CARD FROM TABLEAU 6 TO TABLEAU 3 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation3Card->color = tableau6UncoverTopCard->color;
                                        newfoundation3Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation3Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    /* TABLEAU 5 IS EMPTY NOT VALIDATION REQUIRES */
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau6Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6Uncover, tableau6Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation5Card->color = topCardTableau6Uncover->color;
                                newfoundation5Card->rank = topCardTableau6Uncover->rank;
                                newfoundation5Card->suit = topCardTableau6Uncover->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 5 IS NOT EMPTY VALIDATE ON RAND AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 5 BOTTOM CARD */
                            struct card *tableau5UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverBottomCard, tableau5Uncover[t5uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL THE CARDS FROM TABLEAU 6 TO TABLEAU 5 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation5Card->color = tableau6UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    /* TABLEAU 4 IS EMPTY NO VALIDATION REQUIRES */
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));;
                                struct card *topCardTableau6Uncover = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6Uncover, tableau6Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation4Card->color = topCardTableau6Uncover->color;
                                newfoundation4Card->rank = topCardTableau6Uncover->rank;
                                newfoundation4Card->suit = topCardTableau6Uncover->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 4 BOTTOm CARD */
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t6uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 6 TO TABLEAU 4 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation4Card= (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation4Card->color = tableau6UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '7'){
                    /* TABLEAU 7 IS EMPTY NOT VALIDATION REQUIRES */
                    if(t7uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            int cpyt6uncoverSize = t6uncoverSize;
                            for(int i = 0; i < cpyt6uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau6 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau6, tableau6Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation7Card->color = topCardTableau6->color;
                                newfoundation7Card->rank = topCardTableau6->rank;
                                newfoundation7Card->suit = topCardTableau6->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                t7uncoverSize++;

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                free(tableau6Uncover[i]);
                                t6uncoverSize--;

                                if(t6uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 7 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau6Uncover, t6uncoverSize) == true){
                            /* TABLEAU 6 TOP CARD */
                            struct card *tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverTopCard, tableau6Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 7 BOTTOM CARD */
                            struct card *tableau7UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverBottomCard, tableau7Uncover[t7uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau6UncoverTopCard->rank, tableau7UncoverBottomCard->rank) == false){
                                if(tableau6UncoverTopCard->color != tableau7UncoverBottomCard->color){
                                    int cpyt6uncoverSize = t6uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 6 TO TABLEAU 7 */
                                    for(int i = 0; i < cpyt6uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau6UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6UncoverTopCard, tableau6Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation7Card->color = tableau6UncoverTopCard->color;
                                        newfoundation7Card->rank = tableau6UncoverTopCard->rank;
                                        newfoundation7Card->suit = tableau6UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 7 */
                                        tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                                        t7uncoverSize++;

                                        tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau6Uncover[i]);
                                        t6uncoverSize--;

                                        if(t6uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        /* OPEN COVERED CARD */ 
        if(t6coverSize > 0 && t6uncoverSize == 0){
            struct card t6CoveredCard = *tableau6Cover[t6coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t6CoveredCard.color;
            toAddCard->rank = t6CoveredCard.rank;
            toAddCard->suit = t6CoveredCard.suit;
            
            tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau6Uncover[t6uncoverSize], toAddCard, 1*sizeof(struct card*));
            t6uncoverSize++;
                        
            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

            free(tableau6Cover[t6coverSize-1]);
            t6coverSize--;
            tableau6Cover = realloc(tableau6Cover, (t6coverSize+1) * sizeof(struct card));

        }
        /* DONE OPEN COVERED CARD */
        
    }else if(src == '7'){
        if(t7uncoverSize == 0){
            return false;
        }else if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize)){
            if(dest != src){
                if(dest == 'f'){
                    struct card *checkCard = tableau7Uncover[t7uncoverSize-1];                        

                    if(checkCard->suit == 'd'){
                        if(sizeoffoundationDiamond == 0){
                            struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];
                            struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationDiamondCard->color = tableau7uncoverbottomCard->color;
                            newfoundationDiamondCard->rank = tableau7uncoverbottomCard->rank;
                            newfoundationDiamondCard->suit = tableau7uncoverbottomCard->suit;

                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationDiamond, (sizeoffoundationDiamond+1) * sizeof(struct card));

                            /* REMOVE CARD AT TABLEAU 1 */
                            free(tableau7Uncover[t7uncoverSize-1]);
                            t7uncoverSize--;

                            if(t7uncoverSize < 0){
                                return false;
                            }

                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));



                        }else{
                            struct card *foundationDiamondTopCard = foundationDiamond[sizeoffoundationDiamond-1];
                            struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];                                

                            if(compareRankMoveFoundation(tableau7uncoverbottomCard->rank, foundationDiamondTopCard->rank) == true){

                                struct card *newfoundationDiamondCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationDiamondCard->color = tableau7uncoverbottomCard->color;
                                newfoundationDiamondCard->rank = tableau7uncoverbottomCard->rank;
                                newfoundationDiamondCard->suit = tableau7uncoverbottomCard->suit;                                        

                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationDiamondCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationDiamond+1) * sizeof(struct card));

                                /* REMOVE CARD AT TABLEAU 1 */
                                free(tableau7Uncover[t7uncoverSize-1]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                /* DONE REMOVE CARD AT TABLEAU 1 */
                                    
                            }else{
                                return false;
                            }
                        }
                        
                    }else if(checkCard->suit == 'c'){
                        if(sizeoffoundationClub == 0){
                            struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];
                            struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                            newfoundationClubCard->color = tableau7uncoverbottomCard->color;
                            newfoundationClubCard->rank = tableau7uncoverbottomCard->rank;
                            newfoundationClubCard->suit = tableau7uncoverbottomCard->suit;

                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                            free(tableau7Uncover[t7uncoverSize-1]);
                            t7uncoverSize--;

                            if(t7uncoverSize < 0){
                                return false;
                            }

                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                        }else{
                            /* CLUB FOUNDATION TOP CARD */
                            struct card *foundationClubTopCard = foundationClub[sizeoffoundationClub-1];
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];                            

                            if(compareRankMoveFoundation(tableau7uncoverbottomCard->rank, foundationClubTopCard->rank) == true){
                                /* CREATE NEW CARD */
                                struct card *newfoundationClubCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationClubCard->color = tableau7uncoverbottomCard->color;
                                newfoundationClubCard->rank = tableau7uncoverbottomCard->rank;
                                newfoundationClubCard->suit = tableau7uncoverbottomCard->suit;
                                    
                                /* MOVE NEW CARD TO THE TOP OF CLUB FOUNDATION */
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationClubCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationDiamond = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card));
                                free(tableau7Uncover[t7uncoverSize-1]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                            }else{
                                return false;
                            }
                        }
                            
                        }else if(checkCard->suit == 'h'){
                            if(sizeoffoundationHeart == 0){
                                struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];
                                struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationHeartCard->color = tableau7uncoverbottomCard->color;
                                newfoundationHeartCard->rank = tableau7uncoverbottomCard->rank;
                                newfoundationHeartCard->suit = tableau7uncoverbottomCard->suit;

                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                free(tableau7Uncover[t7uncoverSize-1]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));


                            }else{
                                /* HEART FOUNDATION TOP CARD */
                                struct card *foundationHeartTopCard = foundationHeart[sizeoffoundationHeart-1];
                                /* TABLEAU 7 BOTTOM CARD */
                                struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];                                

                                if(compareRankMoveFoundation(tableau7uncoverbottomCard->rank, foundationHeartTopCard->rank) == true){
                                    /* CREATE NEW CARD */
                                    struct card *newfoundationHeartCard = (struct card*)malloc(1*sizeof(struct card*));
                                    newfoundationHeartCard->color = tableau7uncoverbottomCard->color;
                                    newfoundationHeartCard->rank = tableau7uncoverbottomCard->rank;
                                    newfoundationHeartCard->suit = tableau7uncoverbottomCard->suit;
                                        
                                    /* MOVE NEW CARD TO THE TOP OF HEART FOUNDATION */
                                    foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                    memcpy(foundationHeart[sizeoffoundationHeart], newfoundationHeartCard, 1*sizeof(struct card*));

                                    sizeoffoundationHeart++;

                                    foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card));
                                    free(tableau7Uncover[t7uncoverSize-1]);
                                    t7uncoverSize--;

                                    if(t7uncoverSize < 0){
                                        return false;
                                    }

                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                }else{
                                    return false;
                                }

                            }
                            
                        }else if(checkCard->suit == 's'){
                            if(sizeoffoundationSpade == 0){
                                struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];
                                struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                newfoundationSpadeCard->color = tableau7uncoverbottomCard->color;
                                newfoundationSpadeCard->rank = tableau7uncoverbottomCard->rank;
                                newfoundationSpadeCard->suit = tableau7uncoverbottomCard->suit;
                                        
                                /* MOVE NEW CARD TO THE TOP OF SPADE FOUNDATION */
                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                free(tableau7Uncover[t7uncoverSize-1]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }

                                tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                            }else{
                                /* SPADE FOUNDATION TOP CARD */
                                struct card *foundationSpadeTopCard = foundationSpade[sizeoffoundationSpade-1];
                                /* TABLEAU 7 BOTTOM CARD */
                                struct card *tableau7uncoverbottomCard = tableau7Uncover[t7uncoverSize-1];                                

                                if(compareRankMoveFoundation(tableau7uncoverbottomCard->rank, foundationSpadeTopCard->rank) == true){
                                    /* CREATE NEW CARD */
                                    struct card *newfoundationSpadeCard = (struct card*)malloc(1*sizeof(struct card*));
                                    newfoundationSpadeCard->color = tableau7uncoverbottomCard->color;
                                    newfoundationSpadeCard->rank = tableau7uncoverbottomCard->rank;
                                    newfoundationSpadeCard->suit = tableau7uncoverbottomCard->suit;
                                        
                                    /* MOVE NEW CARD TO THE TOP OF SPADE FOUNDATION */
                                    foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                    memcpy(foundationSpade[sizeoffoundationSpade], newfoundationSpadeCard, 1*sizeof(struct card*));

                                    sizeoffoundationSpade++;

                                    foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card));
                                    free(tableau7Uncover[t7uncoverSize-1]);
                                    t7uncoverSize--;

                                    if(t7uncoverSize < 0){
                                        return false;
                                    }

                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                }else{
                                    return false;
                                }

                            }
                            
                        }else{
                            return false;
                        }
                }else if(dest == '1'){
                    /* TABLEAU 1 IS EMPTY NOT VALIDATION REQUIRES */
                    if(t1uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARD FROM TABLEAU 7 TO TABLEAU 1 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau7, tableau7Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation1Card->color = topCardTableau7->color;
                                newfoundation1Card->rank = topCardTableau7->rank;
                                newfoundation1Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                t1uncoverSize++;

                                tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 1 IS NOT EMPTY VALIDATE RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 1 BOTTOM CARD */
                            struct card *tableau1UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1UncoverBottomCard, tableau1Uncover[t1uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau1UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau1UncoverBottomCard->color){
                                     int cpyt7uncoverSize = t7uncoverSize;
                                     /* MOVE ALL CARD FROM TABLEAU 7 TO TABLEAU 1 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation1Card->color = tableau7UncoverTopCard->color;
                                        newfoundation1Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation1Card->suit = tableau7UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 1 */
                                        tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                                        t1uncoverSize++;

                                        tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                               
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '2'){
                    /* TABLEAU 2 IS EMPTY NO VALIDATION REQUIRES */
                    if(t2uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 2 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], tableau7Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation2Card->color = topCardTableau7->color;
                                newfoundation2Card->rank = topCardTableau7->rank;
                                newfoundation2Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                t2uncoverSize++;

                                tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 2 IS NOT EMPTY VALIDATE RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 2 BOTTOM CARD */
                            struct card *tableau2UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2UncoverBottomCard, tableau2Uncover[t2uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau2UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau2UncoverBottomCard->color){
                                    int cpyt7uncoverSize = t7uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 2 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation2Card->color = tableau7UncoverTopCard->color;
                                        newfoundation2Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation2Card->suit = tableau7UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 2 */
                                        tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                                        t2uncoverSize++;

                                        tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '3'){
                    /* TABLEAU 3 IS NOT EMPTY NO VALIDATION REQUIRES */
                    if(t3uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 3 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau7, tableau7Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation3Card->color = topCardTableau7->color;
                                newfoundation3Card->rank = topCardTableau7->rank;
                                newfoundation3Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                t3uncoverSize++;

                                tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 3 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 3 BOTTOM CARD */
                            struct card *tableau3UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3UncoverBottomCard, tableau3Uncover[t3uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau3UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau3UncoverBottomCard->color){
                                    int cpyt7uncoverSize = t7uncoverSize;
                                    /* MOVE ALL CARDS FROM TABLEAU 7 to TABLEAU 3 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation3Card->color = tableau7UncoverTopCard->color;
                                        newfoundation3Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation3Card->suit = tableau7UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 3 */
                                        tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                                        t3uncoverSize++;

                                        tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '5'){
                    /* TABLEAU 5 IS EMPTY */
                    if(t5uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 5 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau7, tableau7Uncover[i], 1*sizeof(struct card*));
                                                                
                                newfoundation5Card->color = topCardTableau7->color;
                                newfoundation5Card->rank = topCardTableau7->rank;
                                newfoundation5Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                t5uncoverSize++;

                                tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 5 IS NOT EMPTY VALIDATE ON RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 5 BOTTOM CARD */
                            struct card *tableau5UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5UncoverBottomCard, tableau5Uncover[t5uncoverSize-1], 1*sizeof(struct card*));
                            

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau5UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau5UncoverBottomCard->color){
                                    int cpyt7uncoverSize = t7uncoverSize;
                                    /* MOVE ALL CARD FROM TABLEAU 7 TO TABLEAU 5 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));
                                        
                                        newfoundation5Card->color = tableau7UncoverTopCard->color;
                                        newfoundation5Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation5Card->suit = tableau7UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 5 */
                                        tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                                        t5uncoverSize++;

                                        tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '4'){
                    /* TABLEAU 4 IS EMPTY NOT VALIDATION REQUIRES */
                    if(t4uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 4 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau7, tableau7Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation4Card->color = topCardTableau7->color;
                                newfoundation4Card->rank = topCardTableau7->rank;
                                newfoundation4Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                t4uncoverSize++;

                                tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 4 IS NOT EMPTY VALIDATE RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            /* TABLEAU 7 TOP CARD */
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            /* TABLEAU 4 BOTTOM CARD */
                            struct card *tableau4UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4UncoverBottomCard, tableau4Uncover[t4uncoverSize-1], 1*sizeof(struct card*));
                            
                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau4UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau4UncoverBottomCard->color){
                                     int cpyt7uncoverSize = t7uncoverSize;
                                     /* MOVE ALL THE CARDS FROM TABLEAU 7 TO TABLEAU 4 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));

                                        newfoundation4Card->color = tableau7UncoverTopCard->color;
                                        newfoundation4Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation4Card->suit = tableau7UncoverTopCard->suit;
                                                                                    
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 4 */
                                        tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                                        t4uncoverSize++;

                                        tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                               
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }else if(dest == '6'){
                    /* TABLEAU 6 IS EMPTY NO VALIDATION REQUIRES */
                    if(t6uncoverSize == 0){
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            int cpyt7uncoverSize = t7uncoverSize;
                            /* MOVE ALL CARDS FROM TABLEAU 7 TO TABLEAU 6 */
                            for(int i = 0; i < cpyt7uncoverSize; i++){
                                /* CREATE NEW CARD */
                                struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *topCardTableau7 = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(topCardTableau7, tableau7Uncover[i], 1*sizeof(struct card*));
                                
                                newfoundation6Card->color = topCardTableau7->color;
                                newfoundation6Card->rank = topCardTableau7->rank;
                                newfoundation6Card->suit = topCardTableau7->suit;
                                    
                                /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                t6uncoverSize++;

                                tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                                free(tableau7Uncover[i]);
                                t7uncoverSize--;

                                if(t7uncoverSize < 0){
                                    return false;
                                }
                            }
                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                        }else{
                            return false;
                        }
                    }else{
                    /* TABLEAU 6 IS NOT EMPTY VALIDATE RANK AND COLOR */
                        if(checkUncoverTableauPile(tableau7Uncover, t7uncoverSize) == true){
                            struct card *tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7UncoverTopCard, tableau7Uncover[0], 1*sizeof(struct card*));
                            struct card *tableau6UncoverBottomCard = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6UncoverBottomCard, tableau6Uncover[t6uncoverSize-1], 1*sizeof(struct card*));

                            /* VALID MOVE */
                            if(compareRankMoveFoundation(tableau7UncoverTopCard->rank, tableau6UncoverBottomCard->rank) == false){
                                if(tableau7UncoverTopCard->color != tableau6UncoverBottomCard->color){
                                    int cpyt7uncoverSize = t7uncoverSize;
                                    /* MOVE ALL THE CARDS FROM TABLEAU 7 TO TABLEAU 6 */
                                    for(int i = 0; i < cpyt7uncoverSize; i++){
                                        /* CREATE NEW CARD */
                                        struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                                        tableau7UncoverTopCard = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau7UncoverTopCard, tableau7Uncover[i], 1*sizeof(struct card*));

                                        newfoundation6Card->color = tableau7UncoverTopCard->color;
                                        newfoundation6Card->rank = tableau7UncoverTopCard->rank;
                                        newfoundation6Card->suit = tableau7UncoverTopCard->suit;
                                            
                                        /* MOVE NEW CARD TO THE BOTTOM OF TABLEAU 6 */
                                        tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                                        memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                                        t6uncoverSize++;

                                        tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));
                                        
                                        free(tableau7Uncover[i]);
                                        t7uncoverSize--;

                                        if(t7uncoverSize < 0){
                                            return false;
                                        }
                                    }
                                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));
                                }else{
                                    return false;
                                }
                                
                            }else{
                                return false;
                            }
                            
                        }else{
                            return false;
                        }
                    }

                }
            }
        }else{
            return false;
        }

        /* OPEN COVERED CARD */ 
        if(t7coverSize > 0 && t7uncoverSize == 0){
            struct card t7CoveredCard = *tableau7Cover[t7coverSize-1];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));
            
            toAddCard->color = t7CoveredCard.color;
            toAddCard->rank = t7CoveredCard.rank;
            toAddCard->suit = t7CoveredCard.suit;
            
            tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(tableau7Uncover[t7uncoverSize], toAddCard, 1*sizeof(struct card*));
            t7uncoverSize++;
            // *tableau6Uncover[t6uncoverSize] = toAddCard;
            // t6uncoverSize++;
                        
            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

            free(tableau7Cover[t7coverSize-1]);
            t7coverSize--;
            tableau7Cover = realloc(tableau7Cover, (t7coverSize+1) * sizeof(struct card));

        }
        /* DONE OPEN COVERED CARD */
        
    }else if(src == 'w'){
        if(sizeofWaste == 0){
            return false;
        }else{
            if(dest == '1'){
                if(t1uncoverSize == 0){
                    struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    

                    newfoundation1Card->color = wasteTopCard->color;
                    newfoundation1Card->rank = wasteTopCard->rank;
                    newfoundation1Card->suit = wasteTopCard->suit;

                    tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                    t1uncoverSize++;

                    tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    if(sizeofWaste < 0){
                        return false;
                    }

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                }else{
                    struct card *newfoundation1Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    

                    newfoundation1Card->color = wasteTopCard->color;
                    newfoundation1Card->rank = wasteTopCard->rank;
                    newfoundation1Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol1 = tableau1Uncover[t1uncoverSize-1];                    

                    if(compareRankMoveFoundation(bottomCardCol1->rank, newfoundation1Card->rank) == true){
                        if(bottomCardCol1->color != newfoundation1Card->color){
                            tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1Uncover[t1uncoverSize], newfoundation1Card, 1*sizeof(struct card*));

                            t1uncoverSize++;

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }
            }else if(dest == '2'){
                if(t2uncoverSize == 0){
                    struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation2Card->color = wasteTopCard->color;
                    newfoundation2Card->rank = wasteTopCard->rank;
                    newfoundation2Card->suit = wasteTopCard->suit;

                    tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                    t2uncoverSize++;

                    tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                    if(sizeofWaste < 0){
                        return false;
                    }

                }else{
                    struct card *newfoundation2Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation2Card->color = wasteTopCard->color;
                    newfoundation2Card->rank = wasteTopCard->rank;
                    newfoundation2Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol2 = tableau2Uncover[t2uncoverSize-1];
                    
                    if(compareRankMoveFoundation(bottomCardCol2->rank, newfoundation2Card->rank) == true){
                        if(bottomCardCol2->color != newfoundation2Card->color){
                            tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2Uncover[t2uncoverSize], newfoundation2Card, 1*sizeof(struct card*));

                            t2uncoverSize++;

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }

            }else if(dest == '3'){
                if(t3uncoverSize == 0){
                    struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation3Card->color = wasteTopCard->color;
                    newfoundation3Card->rank = wasteTopCard->rank;
                    newfoundation3Card->suit = wasteTopCard->suit;

                    tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                    t3uncoverSize++;

                    tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                    if(sizeofWaste < 0){
                        return false;
                    }

                }else{
                    struct card *newfoundation3Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                 
                    
                    newfoundation3Card->color = wasteTopCard->color;
                    newfoundation3Card->rank = wasteTopCard->rank;
                    newfoundation3Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol3 = tableau3Uncover[t3uncoverSize-1];                    

                    if(compareRankMoveFoundation(bottomCardCol3->rank, newfoundation3Card->rank) == true){
                        if(bottomCardCol3->color != newfoundation3Card->color){
                            tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3Uncover[t3uncoverSize], newfoundation3Card, 1*sizeof(struct card*));

                            t3uncoverSize++;

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }


            }else if(dest == '4'){
                if(t4uncoverSize == 0){
                    struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation4Card->color = wasteTopCard->color;
                    newfoundation4Card->rank = wasteTopCard->rank;
                    newfoundation4Card->suit = wasteTopCard->suit;

                    tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                    t4uncoverSize++;

                    tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                    if(sizeofWaste < 0){
                        return false;
                    }

                }else{
                    struct card *newfoundation4Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation4Card->color = wasteTopCard->color;
                    newfoundation4Card->rank = wasteTopCard->rank;
                    newfoundation4Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol4 = tableau4Uncover[t4uncoverSize-1];                    

                    if(compareRankMoveFoundation(bottomCardCol4->rank, newfoundation4Card->rank) == true){
                        if(bottomCardCol4->color != newfoundation4Card->color){
                            tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4Uncover[t4uncoverSize], newfoundation4Card, 1*sizeof(struct card*));

                            t4uncoverSize++;

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1) * sizeof(struct card*));
                            
                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));
                            
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }

            }else if(dest == '5'){
                if(t5uncoverSize == 0){
                    struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];
                                        
                    newfoundation5Card->color = wasteTopCard->color;
                    newfoundation5Card->rank = wasteTopCard->rank;
                    newfoundation5Card->suit = wasteTopCard->suit;

                    tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                    t5uncoverSize++;

                    tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                    if(sizeofWaste < 0){
                        return false;
                    }

                }else{
                    struct card *newfoundation5Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation5Card->color = wasteTopCard->color;
                    newfoundation5Card->rank = wasteTopCard->rank;
                    newfoundation5Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol5 = tableau5Uncover[t5uncoverSize-1];                    

                    if(compareRankMoveFoundation(bottomCardCol5->rank, newfoundation5Card->rank) == true){
                        if(bottomCardCol5->color != newfoundation5Card->color){
                            tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5Uncover[t5uncoverSize], newfoundation5Card, 1*sizeof(struct card*));

                            t5uncoverSize++;

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }

            }else if(dest == '6'){
                if(t6uncoverSize == 0){
                    struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation6Card->color = wasteTopCard->color;
                    newfoundation6Card->rank = wasteTopCard->rank;
                    newfoundation6Card->suit = wasteTopCard->suit;

                    tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                    t6uncoverSize++;

                    tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));

                    if(sizeofWaste < 0){
                        return false;
                    }

                }else{
                    struct card *newfoundation6Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation6Card->color = wasteTopCard->color;
                    newfoundation6Card->rank = wasteTopCard->rank;
                    newfoundation6Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol6 = tableau6Uncover[t6uncoverSize-1];                    

                    if(compareRankMoveFoundation(bottomCardCol6->rank, newfoundation6Card->rank) == true){
                        if(bottomCardCol6->color != newfoundation6Card->color){
                            tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6Uncover[t6uncoverSize], newfoundation6Card, 1*sizeof(struct card*));

                            t6uncoverSize++;

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card));
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }

            }else if(dest == '7'){
                if(t7uncoverSize == 0){
                    struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];                    
                    
                    newfoundation7Card->color = wasteTopCard->color;
                    newfoundation7Card->rank = wasteTopCard->rank;
                    newfoundation7Card->suit = wasteTopCard->suit;

                    tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                    memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                    t7uncoverSize++;

                    tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                    free(waste[sizeofWaste-1]);
                    sizeofWaste--;

                    if(sizeofWaste < 0){
                        return false;
                    }

                    waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                    

                }else{
                    struct card *newfoundation7Card = (struct card*)malloc(1*sizeof(struct card*));
                    struct card *wasteTopCard = waste[sizeofWaste-1];
                    
                    
                    newfoundation7Card->color = wasteTopCard->color;
                    newfoundation7Card->rank = wasteTopCard->rank;
                    newfoundation7Card->suit = wasteTopCard->suit;

                    struct card *bottomCardCol7 = tableau7Uncover[t7uncoverSize-1];
                    

                    if(compareRankMoveFoundation(bottomCardCol7->rank, newfoundation7Card->rank) == true){
                        if(bottomCardCol7->color != newfoundation7Card->color){
                            tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7Uncover[t7uncoverSize], newfoundation7Card, 1*sizeof(struct card*));

                            t7uncoverSize++;

                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1) * sizeof(struct card));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));
                            
                            if(sizeofWaste < 0){
                                return false;
                            }

                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }

            }else if(dest == 'f'){
                struct card *newfoundationCard = (struct card*)malloc(1*sizeof(struct card*));
                struct card *wasteTopCard = waste[sizeofWaste-1];

                newfoundationCard->color = wasteTopCard->color;
                newfoundationCard->rank = wasteTopCard->rank;
                newfoundationCard->suit = wasteTopCard->suit;

                if(newfoundationCard->suit == 'd'){
                    if(sizeoffoundationDiamond == 0){
                        if(newfoundationCard->rank == 'A'){
                            foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;

                            foundationDiamond = realloc(foundationDiamond, (sizeoffoundationDiamond+1) * sizeof(struct card*));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                            if(sizeofWaste < 0){
                                return false;
                            }
                        }else{
                            return false;
                        }      

                    }else{
                        struct card *topCardDiamondFoundation = foundationDiamond[sizeoffoundationClub-1];

                        if(compareRankMoveFoundation(newfoundationCard->rank, topCardDiamondFoundation->rank) == true){
                            if(topCardDiamondFoundation->suit == newfoundationCard->suit){
                                foundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationDiamond[sizeoffoundationDiamond], newfoundationCard, 1*sizeof(struct card*));

                                sizeoffoundationDiamond++;

                                foundationDiamond = realloc(foundationDiamond, (sizeoffoundationDiamond+1) * sizeof(struct card*));

                                free(waste[sizeofWaste-1]);
                                sizeofWaste--;

                                if(sizeofWaste < 0){
                                    return false;
                                }

                                waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }

                }else if(newfoundationCard->suit == 'c'){
                    if(sizeoffoundationClub == 0){
                        if(newfoundationCard->rank == 'A'){
                            foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationClub[sizeoffoundationClub], newfoundationCard, 1*sizeof(struct card*));

                            sizeoffoundationClub++;

                            foundationClub = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card*));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            if(sizeofWaste < 0){
                                return false;
                            }

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                        }else{
                            return false;
                        }
                        

                    }else{
                        struct card *topCardClubFoundation = foundationClub[sizeoffoundationClub-1];

                        if(compareRankMoveFoundation(newfoundationCard->rank, topCardClubFoundation->rank) == true){
                            if(topCardClubFoundation->suit == newfoundationCard->suit){
                                foundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationClub[sizeoffoundationClub], newfoundationCard, 1*sizeof(struct card*));

                                sizeoffoundationClub++;

                                foundationClub = realloc(foundationClub, (sizeoffoundationClub+1) * sizeof(struct card*));

                                free(waste[sizeofWaste-1]);
                                sizeofWaste--;

                                if(sizeofWaste < 0){
                                    return false;
                                }

                                waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));
                                
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }

                }else if(newfoundationCard->suit == 'h'){
                    if(sizeoffoundationHeart == 0){
                        if(newfoundationCard->rank == 'A'){

                            foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationHeart[sizeoffoundationHeart], newfoundationCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;

                            foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card*));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            if(sizeofWaste < 0){
                                return false;
                            }

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                        }else{
                            return false;
                        }
                        
                    }else{
                        struct card *topCardHeartFoundation = foundationHeart[sizeoffoundationHeart-1];

                        if(compareRankMoveFoundation(newfoundationCard->rank, topCardHeartFoundation->rank) == true){
                            if(topCardHeartFoundation->suit == newfoundationCard->suit){
                                foundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationHeart[sizeoffoundationHeart], newfoundationCard, 1*sizeof(struct card*));

                                sizeoffoundationHeart++;

                                foundationHeart = realloc(foundationHeart, (sizeoffoundationHeart+1) * sizeof(struct card*));

                                free(waste[sizeofWaste-1]);
                                sizeofWaste--;

                                if(sizeofWaste < 0){
                                    return false;
                                }

                                waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));
                                
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }

                }else if(newfoundationCard->suit == 's'){
                    if(sizeoffoundationSpade == 0){
                        if(newfoundationCard->rank == 'A'){
                            foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(foundationSpade[sizeoffoundationSpade], newfoundationCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;

                            foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card*));

                            free(waste[sizeofWaste-1]);
                            sizeofWaste--;

                            if(sizeofWaste < 0){
                                return false;
                            }

                            waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));

                        }
                    
                    }else{
                        struct card *topCardSpadeFoundation = foundationSpade[sizeoffoundationSpade-1];

                        if(compareRankMoveFoundation(newfoundationCard->rank, topCardSpadeFoundation->rank) == true){
                            if(topCardSpadeFoundation->suit == newfoundationCard->suit){
                                foundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(foundationSpade[sizeoffoundationSpade], newfoundationCard, 1*sizeof(struct card*));

                                sizeoffoundationSpade++;

                                foundationSpade = realloc(foundationSpade, (sizeoffoundationSpade+1) * sizeof(struct card*));

                                free(waste[sizeofWaste-1]);
                                sizeofWaste--;
  
                                if(sizeofWaste < 0){
                                    return false;
                                }

                                waste = realloc(waste, (sizeofWaste+1) * sizeof(struct card*));
                              
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }

                }else{
                    return false;
                }



            }else{
                return false;
            }
        }
    }else{
        printf("ERROR!\n");
        return false;
    }

    return true;
}

bool checkUncoverTableauPile(struct card **tableauUncover, int tableauSize){

    for(int i = 0; i < tableauSize-1; i++){
        struct card *tableauCard1 = tableauUncover[i];
        struct card *tableauCard2 = tableauUncover[i+1];
        
        if(compareRank(tableauCard1->rank, tableauCard2->rank) == true && compareColor(tableauCard1->color, tableauCard2->color) == true){
            continue;
        }else{
            return false;
        }
    }
    return true;
}

bool checkSuit(char suit){
    bool b = false;
    char allSuit[] = { 'c', 'd', 'h', 's' };

	for (int i = 0; i < 4; i++) {
		if (suit == allSuit[i])
			b = true;
	}

	return b;
}

bool checkRank(char rank){
    bool b = false;
    char allRank[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };

	for (int i = 0; i < 13; i++) {
		if (rank == allRank[i])
			b = true;
	}

	return b;
}

bool checkIsNumber(char *stringInt){
    char arrStringInt[100];
    strcpy(arrStringInt, stringInt);

    for(int i = 0; i < strlen(arrStringInt); i++){
        if(isdigit(arrStringInt[i]) == 0){
            return false;
        }
    }

    return true;
}

void skipWhiteSpace(FILE *inFile){
    do{
        char *token;
        char *cpyOfReadLine;

        fgets(readLine, 100, inFile);
        printf("skipWhiteSpace readLine is: %s\n", readLine);

        cpyOfReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyOfReadLine, readLine);
        printf("skipWhiteSpace cpyOfReadLine is: %s\n", readLine);

        token = strtok(cpyOfReadLine, " \t\r\n\v\f\r"); 

        printf("skipWhiteSpace Token is:%s\n", token);

        if(token != NULL){
            return;
        }

        lineCount++;

    }while(!feof(inFile));


}

void readComments(FILE *inFile){
    do{
        char *token;
        char *cpyOfReadLine;

        fgets(readLine, 100, inFile);
        printf("readComments readLine is: %s\n", readLine);

        cpyOfReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyOfReadLine, readLine);
        printf("readComments cpyOfReadLine is: %s\n", cpyOfReadLine);

        token = strtok(cpyOfReadLine, " \t\r\n\v\f\r");        
        printf("readComments Token is:%s\n", token);

        if(token != NULL && (strstr(token, "#") == NULL || strcmp(token, "#") != 0)){
            fprintf(stdout, "OUT FROM readComments!\n");
            return;
        }

        while(token != NULL){
            char *cpyToken;
            char *strstrPtr;
            cpyToken = (char *)malloc(strlen(token));
            strcpy(cpyToken, token);

            printf("readComments cpyToken is: %s\n", cpyToken);

            strstrPtr = strstr(cpyToken, "#");

            printf("readComments strstrPtr is: %s\n", strstrPtr);

            if(strstrPtr != NULL){
                break;
            }

            token = strtok(NULL, " \t\r\n\v\f\r");
        }

        lineCount++;
    }while(!feof(inFile));

    fprintf(stderr, "Invalid input file!\n");
    exit(-1);
}

void checkRules(FILE *inFile){
    printf("INSIDE checkRules!\n");

    bool checkLimit = false;
    char *cpyReadLine;
    cpyReadLine = (char *)malloc(strlen(readLine));
    strcpy(cpyReadLine, readLine);

    printf("checkRules cpyReadLine is: %s\n", cpyReadLine);

    char *token;

    token = strtok(cpyReadLine, " \t\r\n\v\f\r");

    printf("checkRules Token is:%s\n", token);

    /* START VALIDATION */
    if(strcmp(token, "RULES:") != 0){
        fprintf(stderr, "Error near line %d: expecting 'RULES:'!\n", lineCount);
        exit(-1);
    }else{
        /* SKIP ANY COMMENTS OR WHITESPACE AFTER KEYWORD 'RULES:' */
        readComments(inFile);
        //printf("DONE SKIP COMMENTS AFTER RULES:\n");
        bool readTurn = false;
        bool checkedTurn = false;

        /* READING TURN */
        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        while(token != NULL){

            if(readTurn == false){
                char *cpyToken;
                cpyToken = (char *)malloc(strlen(token));
                strcpy(cpyToken, token);

                if(strcmp(cpyToken, "turn") == 0){
                    readTurn == true;
                }else{
                    fprintf(stderr, "Error near line %d: expecting 'turn'!\n", lineCount);
                    exit(-1);
                }

                readTurn = true;
                
            }else if(checkedTurn == false && readTurn == true){
                char *cpyToken;
                cpyToken = (char *)malloc(strlen(token));
                strcpy(cpyToken, token);

                if(checkIsNumber(cpyToken) == false){
                    fprintf(stderr, "Error near line %d: expecting a number!\n", lineCount);
                    exit(-1);
                }else{
                    turn = atoi(cpyToken);

                    if(turn != 1 && turn != 3){
                        fprintf(stderr, "Error near line %d: expecting 1 or 3!\n", lineCount);
                        exit(-1);
                    }

                    checkedTurn = true;
                }
            }else if(strcmp(token, "#") == 0 || strstr(token, "#") != NULL){
                break;
            }else{
                fprintf(stderr, "Error near line %d: expecting a non-readable!\n", lineCount);
                exit(-1);
            }

            token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* DONE READING TURN */

        /* LIMITS VALIDATION */

        fgets(readLine, 100, inFile);

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        lineCount++;

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token == NULL){
            fprintf(stderr, "Error near line %d: expecting limit or unlimited!\n", lineCount);
            exit(-1);
        }

        while(token != NULL){
            char *cpyToken;
            cpyToken = (char *)malloc(strlen(token));
            strcpy(cpyToken, token);
            
            if(strcmp(cpyToken, "unlimited") == 0){
                limit = INT_MAX;
                break;

            }else if(strcmp(cpyToken, "limit") == 0){
                checkLimit = true;

            }else if(checkLimit == true){
                if(checkIsNumber(cpyToken) == true){
                    limit = atoi(cpyToken);
                }else{
                    fprintf(stderr, "Error near line %d: expecting a valid number after limit!\n", lineCount);
                    exit(-1);
                }

                checkLimit = false;

            }else if(strcmp(cpyToken, "#") == 0 || strstr(token, "#") != NULL){
                break;

            }else{
                fprintf(stderr, "Error near line %d: expecting 1 or 3!\n", lineCount);
                exit(-1);
            }


            token = strtok(NULL, " \t\r\n\v\f\r");
        }

        /* END OF LIMITS VALIDATION */

        readRules = true;

        /* CHECK FOR A SINGLE WHITE SPACE AFTER RULES SECTION */

        skipWhiteSpace(inFile);

        /* END OF CHECKING FOR A SINGLE WHITE SPACE AFTER RULES SECTION */


    }

}

void checkFoundations(FILE *inFile){
    printf("GOT INSIDE checkFoundations!\n");
    bool doneClub = false;
    bool doneDiamond = false;
    bool doneHeart = false;
    bool doneSpade = false;

    foundationClub = malloc(1*sizeof(struct card*));
    sizeoffoundationClub = 0;
    foundationDiamond = malloc(1*sizeof(struct card*));
    sizeoffoundationDiamond = 0;
    foundationHeart = malloc(1*sizeof(struct card*));
    sizeoffoundationHeart = 0;
    foundationSpade = malloc(1*sizeof(struct card*));
    sizeoffoundationSpade = 0;

    struct card **cpyFoundationClub;
    cpyFoundationClub = malloc(1*sizeof(struct card*));
    struct card **cpyFoundationDiamond;
    cpyFoundationDiamond = malloc(1*sizeof(struct card*));
    struct card **cpyFoundationHeart;
    cpyFoundationHeart = malloc(1*sizeof(struct card*));
    struct card **cpyFoundationSpade;
    cpyFoundationSpade = malloc(1*sizeof(struct card*));

    char *cpyReadLine;
    cpyReadLine = (char *)malloc(strlen(readLine));
    strcpy(cpyReadLine, readLine);

   // printf("checkFoundations cpyReadLine is: %s\n", cpyReadLine);

    char *token;

    token = strtok(cpyReadLine, " \t\r\n\v\f\r");

    //printf("checkFoundations Token is:%s\n", token);

    if(strcmp(token, "FOUNDATIONS:") != 0){
        fprintf(stderr, "Error near line %d: expecting 'FOUNDATIONS:'!\n", lineCount);
        exit(-1);
    }else{
        /* READING CLUB FOUNDATION */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readClubLine;
        readClubLine = (char *)malloc(strlen(readLine));
        strcpy(readClubLine, readLine);

        char *clubToken;
        clubToken = strtok(readClubLine, " \t\r\n\v\f\r");

        if(clubToken == NULL){
            fprintf(stderr, "Error near line %d: expecting club foundations!\n", lineCount);
            exit(-1);
        }

        while(clubToken != NULL){
            char *cpyClubToken;
            cpyClubToken = (char *)malloc(strlen(clubToken));
            strcpy(cpyClubToken, clubToken);

            if(doneClub == false){
                /* CHECKING NUMBER OF CHARACTHERS */
                if(strlen(cpyClubToken) != 2){
                    fprintf(stderr, "Error near line %d: Club foundation does not have two characters!\n", lineCount);
                    exit(-1);
                }

                if(strstr(cpyClubToken, "c") == NULL){
                    fprintf(stderr, "Error near line %d: Club foundation does not contain 'c' character!\n", lineCount);
                    exit(-1);
                }else{
                    char arrcpyClubToken[10];
                    strcpy(arrcpyClubToken, clubToken);

                    struct card *newCard;

                    if(arrcpyClubToken[0] == '_'){
                        sizeoffoundationClub = 0;
                    }else if(arrcpyClubToken[0] == 'A'){
                        /* ADDING Ac to **foundationClub */                        
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));

                    }else if(arrcpyClubToken[0] == 'T'){
                        /* ADDING Tc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationClub */

                        /* ADDING 9c-2c TO **foundationClub */
                        for(int i = 9; i >= 2; i--){
                            //struct card *newCard;
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 'c';

                            cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                            sizeoffoundationClub++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationClub */

                        /* ADDING Ac TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationClub */

                    }else if(arrcpyClubToken[0] == 'J'){
                        /* ADDING Jc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationClub */

                        /* ADDING Tc TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationClub */

                        /* ADDING 9c-2c TO **foundationClub */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 'c';

                            cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                            sizeoffoundationClub++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationClub */

                        /* ADDING Ac TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationClub */

                    }else if(arrcpyClubToken[0] == 'Q'){
                        /* ADDING Qc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'Q';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationClub */

                        /* ADDING Jc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationClub */

                        /* ADDING Tc TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationClub */

                        /* ADDING 9c-2c TO **foundationClub */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 'c';

                            cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                            sizeoffoundationClub++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationClub */

                        /* ADDING Ac TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationClub */

                    }else if(arrcpyClubToken[0] == 'K'){
                        /* ADDING Kc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'K';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Kc TO **foundationClub */

                        /* ADDING Qc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'Q';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationClub */

                        /* ADDING Jc to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 'c';
                        
                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationClub */

                        /* ADDING Tc TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationClub */

                        /* ADDING 9c-2c TO **foundationClub */

                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 'c';
                            
                            cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                            sizeoffoundationClub++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationClub */

                        /* ADDING Ac TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationClub */

                    }else if(isdigit(arrcpyClubToken[0]) != 0){
                        char *convert = &arrcpyClubToken[0];
                        int rankVal = atoi(convert);

                        if(rankVal < 2){
                            fprintf(stderr, "Error near line %d: Rank for Club Foundation is not valid!\n", lineCount);
                            exit(-1);
                        }

                        /* ADDING (rankVal)c to **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = rankVal +'0';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING (rankVal)cc TO **foundationClub */

                        /* ADDING (rankVal-1)c-2c TO **foundationClub */
                        for(int i = rankVal-1; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                          
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 'c';

                            cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                            sizeoffoundationClub++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationClub */

                        /* ADDING Ac TO **foundationClub */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 'c';

                        cpyFoundationClub[sizeoffoundationClub] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationClub[sizeoffoundationClub], newCard, (sizeoffoundationClub+1)*sizeof(struct card*));

                        sizeoffoundationClub++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationClub */

                    }else{
                        fprintf(stderr, "Error near line %d: Expecting a valid rank for club foundation!\n", lineCount);
                        exit(-1);
                    }
                }

                doneClub = true;
            }else{
                if(strcmp(cpyClubToken, "#") == 0){
                    break;
                }
            }

            clubToken = strtok(NULL, " \t\r\n\v\f\r");
        }

        int tracker = 0;
        for(int i = sizeoffoundationClub-1; i > 0; i--){
            struct card *cardtoAdd = (struct card*)malloc(1*sizeof(struct card*));
            struct card currentCard = *cpyFoundationClub[i];

            cardtoAdd->color = currentCard.color;
            cardtoAdd->rank = currentCard.rank;
            cardtoAdd->suit = currentCard.suit;

            foundationClub[tracker] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(foundationClub[tracker], cardtoAdd, (tracker+1)*sizeof(struct card*));
            tracker++;
        }

        tracker = 0;
        printf("CLUB FOUNDATION!\n");
        for(int i = 0; i < sizeoffoundationClub; i++){
            struct card *currentCard = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(currentCard, foundationClub[i], (tracker+1)*sizeof(struct card*));
            

            printf("%c%c\n", currentCard->rank, currentCard->suit);
        }

        /* END OF READING CLUB FOUNDATION */

        /* READING DIAMOND FOUNDATION */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readDiamondLine;
        readDiamondLine = (char *)malloc(strlen(readLine));
        strcpy(readDiamondLine, readLine);

        char *diamondToken;
        diamondToken = strtok(readDiamondLine, " \t\r\n\v\f\r");

        if(diamondToken == NULL){
            fprintf(stderr, "Error near line %d: expecting diamond foundations!\n", lineCount);
            exit(-1);
        }

        while(diamondToken != NULL){
            char *cpyDiamondToken;
            cpyDiamondToken = (char *)malloc(strlen(diamondToken));
            strcpy(cpyDiamondToken, diamondToken);

            if(doneDiamond == false){
                /* CHECKING NUMBER OF CHARACTHERS */
                if(strlen(cpyDiamondToken) != 2){
                    fprintf(stderr, "Error near line %d: Diamond foundation does not have two characters!\n", lineCount);
                    exit(-1);
                }

                if(strstr(cpyDiamondToken, "d") == NULL){
                    fprintf(stderr, "Error near line %d: Diamond foundation does not contain 'd' character!\n", lineCount);
                    exit(-1);
                }else{
                    char arrcpyDiamondToken[10];
                    strcpy(arrcpyDiamondToken, diamondToken);
                    struct card *newCard;

                    if(arrcpyDiamondToken[0] == '_'){
                        printf("ITS _\n");
                        sizeoffoundationDiamond = 0;
                    }else if(arrcpyDiamondToken[0] == 'A'){
                        /* ADDING Ac to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));
                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));

                    }else if(arrcpyDiamondToken[0] == 'T'){
                        /* ADDING Tc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));

                        /* END OF ADDING Tc TO **foundationDiamond */

                        /* ADDING 9c-2c TO **foundationDiamond */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'd';

                            cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationDiamond */

                        /* ADDING Ac TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';
                        
                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationDiamond */

                    }else if(arrcpyDiamondToken[0] == 'J'){
                        /* ADDING Jc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationDiamond */

                        /* ADDING Tc TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationDiamond */

                        /* ADDING 9c-2c TO **foundationDiamond */

                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'd';

                            cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationDiamond */

                        /* ADDING Ac TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';
                        
                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationDiamond */

                    }else if(arrcpyDiamondToken[0] == 'Q'){
                        /* ADDING Qc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'Q';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationDiamond */

                        /* ADDING Jc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'd';
                        
                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationDiamond */

                        /* ADDING Tc TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationDiamond */

                        /* ADDING 9c-2c TO **foundationDiamond */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'd';

                            cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationDiamond */

                        /* ADDING Ac TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationDiamond */

                    }else if(arrcpyDiamondToken[0] == 'K'){
                        /* ADDING Kc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'K';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Kc TO **foundationDiamond */

                        /* ADDING Qc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'Q';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationDiamond */

                        /* ADDING Jc to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationDiamond */

                        /* ADDING Tc TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationDiamond */

                        /* ADDING 9c-2c TO **foundationDiamond */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'd';

                            cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationDiamond */

                        /* ADDING Ac TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationDiamond */

                    }else if(isdigit(arrcpyDiamondToken[0]) != 0){
                        char *convert = &arrcpyDiamondToken[0];
                        int rankVal = atoi(convert);

                        if(rankVal < 2){
                            fprintf(stderr, "Error near line %d: Rank for Diamond Foundation is not valid!\n", lineCount);
                            exit(-1);
                        }

                        /* ADDING (rankVal)c to **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = rankVal +'0';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING (rankVal)cc TO **foundationDiamond */

                        /* ADDING (rankVal-1)c-2c TO **foundationDiamond */

                        for(int i = rankVal-1; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'd';

                            cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                            sizeoffoundationDiamond++;
                            cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationDiamond */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'd';

                        cpyFoundationDiamond[sizeoffoundationDiamond] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationDiamond[sizeoffoundationDiamond], newCard, 1*sizeof(struct card*));

                        sizeoffoundationDiamond++;
                        cpyFoundationDiamond = realloc(cpyFoundationDiamond, (sizeoffoundationDiamond+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationDiamond */

                    }else{
                        fprintf(stderr, "Error near line %d: Expecting a valid rank for diamond foundation!\n", lineCount);
                        exit(-1);
                    }
                }

                doneDiamond = true;
            }else{
                if(strcmp(cpyDiamondToken, "#") == 0){
                    break;
                }
            }

            diamondToken = strtok(NULL, " \t\r\n\v\f\r");
        }
        printf("INSERTED DIAMOND CARDS!\n");
        tracker = 0;
        for(int i = sizeoffoundationDiamond-1; i >= 0; i--){
            struct card *currentCard = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(currentCard, cpyFoundationDiamond[i], 1*sizeof(struct card*));
            
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));

            toAddCard->color = currentCard->color;
            toAddCard->rank = currentCard->rank;
            toAddCard->suit = currentCard->suit;

            foundationDiamond[tracker] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(foundationDiamond[tracker], toAddCard, 1*sizeof(struct card*));
            tracker++;
            foundationDiamond = realloc(foundationDiamond, (tracker+1)*sizeof(struct card*));

        }

        printf("DIAMOND FOUNDATION!\n");

        for(int i = 0; i < sizeoffoundationDiamond; i++){
            struct card currentCard = *foundationDiamond[i];

            printf("%c%c\n", currentCard.rank, currentCard.suit);
        }
        
        printf("END DIAMOND FOUNDATION!\n");
        /* END OF READING DIAMOND FOUNDATION */

        /* READING HEART FOUNDATION */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readHeartLine;
        readHeartLine = (char *)malloc(strlen(readLine));
        strcpy(readHeartLine, readLine);

        char *heartToken;
        heartToken = strtok(readHeartLine, " \t\r\n\v\f\r");

        if(heartToken == NULL){
            fprintf(stderr, "Error near line %d: expecting heart foundations!\n", lineCount);
            exit(-1);
        }

        while(heartToken != NULL){
            printf("CHECKING HEART NOW! \n");
            char *cpyHeartToken;
            cpyHeartToken = (char *)malloc(strlen(heartToken));
            strcpy(cpyHeartToken, heartToken);

            if(doneHeart == false){
                /* CHECKING NUMBER OF CHARACTHERS */
                if(strlen(cpyHeartToken) != 2){
                    fprintf(stderr, "Error near line %d: Club foundation does not have two characters!\n", lineCount);
                    exit(-1);
                }

                if(strstr(cpyHeartToken, "h") == NULL){
                    fprintf(stderr, "Error near line %d: Diamond foundation does not contain 'd' character!\n", lineCount);
                    exit(-1);
                }else{
                    char arrcpyHeartToken[10];
                    strcpy(arrcpyHeartToken, heartToken);
                    struct card *newCard;

                    if(arrcpyHeartToken[0] == '_'){
                        sizeoffoundationHeart = 0;
                    }else if(arrcpyHeartToken[0] == 'A'){
                        /* ADDING Ac to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';
                        
                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));

                    }else if(arrcpyHeartToken[0] == 'T'){
                        /* ADDING Tc to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationHeart */

                        /* ADDING 9c-2c TO **foundationHeart */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'h';

                            cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;
                            cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        }
                        /* END OF ADDING 9c-2c TO **foundationHeart */

                        /* ADDING Ac TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationHeart */

                    }else if(arrcpyHeartToken[0] == 'J'){
                        /* ADDING Jc to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationHeart */

                        /* ADDING Tc TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationHeart */

                        /* ADDING 9c-2c TO **foundationHeart */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'h';

                            cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;
                            cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationHeart */

                        /* ADDING Ac TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationHeart */

                    }else if(arrcpyHeartToken[0] == 'Q'){
                        /* ADDING Qc to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'Q';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;

                        /* ADDING Jc to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationHeart */

                        /* ADDING Tc TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationHeart */

                        /* ADDING 9c-2c TO **foundationHeart */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'h';

                            cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;
                            cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationHeart */

                        /* ADDING Ac TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationHeart */

                    }else if(arrcpyHeartToken[0] == 'K'){
                        /* ADDING Kc to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'K';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Kc TO **foundationHeart */

                        /* ADDING Qc to **foundationHeart */

                        //struct card *newCard;
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'Q';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationHeart */

                        /* ADDING Jc to **foundationHeart */

                        //struct card *newCard;
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'J';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationHeart */

                        /* ADDING Tc TO **foundationHeart */

                        //struct card *newCard;
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'T';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationHeart */

                        /* ADDING 9c-2c TO **foundationHeart */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'h';

                            cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;
                            cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationHeart */

                        /* ADDING Ac TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationHeart */

                    }else if(isdigit(arrcpyHeartToken[0]) != 0){
                        printf("CHECKING %c \n", arrcpyHeartToken[0]);
                        char *convert = &arrcpyHeartToken[0];
                        int rankVal = atoi(convert);

                        if(rankVal < 2){
                            fprintf(stderr, "Error near line %d: Rank for Heart Foundation is not valid!\n", lineCount);
                            exit(-1);
                        }

                        /* ADDING (rankVal)c to **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = rankVal +'0';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING (rankVal)cc TO **foundationHeart */

                        /* ADDING (rankVal-1)c-2c TO **foundationHeart */

                        for(int i = rankVal-1; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->rank = i +'0';
                            newCard->suit = 'h';

                            cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                            sizeoffoundationHeart++;
                            cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationHeart */

                        /* ADDING Ac TO **foundationHeart */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'R';
                        newCard->rank = 'A';
                        newCard->suit = 'h';

                        cpyFoundationHeart[sizeoffoundationHeart] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationHeart[sizeoffoundationHeart], newCard, 1*sizeof(struct card*));

                        sizeoffoundationHeart++;
                        cpyFoundationHeart = realloc(cpyFoundationHeart, (sizeoffoundationHeart+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationHeart */

                    }else{
                        fprintf(stderr, "Error near line %d: Expecting a valid rank for heart foundation!\n", lineCount);
                        exit(-1);
                    }
                }

                doneHeart = true;
            }else{
                if(strcmp(cpyHeartToken, "#") == 0){
                    break;
                }
            }

            heartToken = strtok(NULL, " \t\r\n\v\f\r");
        }

        printf("HEART FOUNDATION!\n");

        tracker = 0;
        for(int i = sizeoffoundationHeart-1; i >= 0; i--){
            struct card currentCard = *cpyFoundationHeart[i];            
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));

            toAddCard->color = currentCard.color;
            toAddCard->rank = currentCard.rank;
            toAddCard->suit = currentCard.suit;

            foundationHeart[tracker] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(foundationHeart[tracker], toAddCard, 1*sizeof(struct card*));
            tracker++;

            foundationHeart = realloc(foundationHeart, (tracker+1)*sizeof(struct card*));
        }

        for(int i = 0; i < sizeoffoundationHeart; i++){
            struct card *currentCard = foundationHeart[i];

            printf("%c%c\n", currentCard->rank, currentCard->suit);
        }

        /* END OF READING HEART FOUNDATION */

        /* READING SPADE FOUNDATION */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readSpadeLine;
        readSpadeLine = (char *)malloc(strlen(readLine));
        strcpy(readSpadeLine, readLine);

        char *spadeToken;
        spadeToken = strtok(readSpadeLine, " \t\r\n\v\f\r");

        if(spadeToken == NULL){
            fprintf(stderr, "Error near line %d: expecting spade foundations!\n", lineCount);
            exit(-1);
        }

        while(spadeToken != NULL){
            char *cpySpadeToken;
            cpySpadeToken = (char *)malloc(strlen(spadeToken));
            strcpy(cpySpadeToken, spadeToken);

            if(doneSpade == false){
                /* CHECKING NUMBER OF CHARACTHERS */
                if(strlen(cpySpadeToken) != 2){
                    fprintf(stderr, "Error near line %d: Heart foundation does not have two characters!\n", lineCount);
                    exit(-1);
                }

                if(strstr(cpySpadeToken, "s") == NULL){
                    fprintf(stderr, "Error near line %d: Spade foundation does not contain 's' character!\n", lineCount);
                    exit(-1);
                }else{
                    char arrcpySpadeToken[10];
                    strcpy(arrcpySpadeToken, spadeToken);
                    struct card *newCard;

                    if(arrcpySpadeToken[0] == '_'){
                        sizeoffoundationSpade = 0;
                    }else if(arrcpySpadeToken[0] == 'A'){
                        /* ADDING Ac to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));

                    }else if(arrcpySpadeToken[0] == 'T'){
                        /* ADDING Tc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationSpade */

                        /* ADDING 9c-2c TO **foundationSpade */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 's';

                            cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;
                            cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationSpade */

                        /* ADDING Ac TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationSpade */

                    }else if(arrcpySpadeToken[0] == 'J'){
                        /* ADDING Jc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationSpade */

                        /* ADDING Tc TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationSpade */

                        /* ADDING 9c-2c TO **foundationSpade */

                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 's';

                            cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;
                            cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationSpade */

                        /* ADDING Ac TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationSpade */

                    }else if(arrcpySpadeToken[0] == 'Q'){
                        /* ADDING Qc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'Q';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationSpade */

                        /* ADDING Jc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationSpade */

                        /* ADDING Tc TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationSpade */

                        /* ADDING 9c-2c TO **foundationSpade */

                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 's';

                            cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;
                            cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationSpade */

                        /* ADDING Ac TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationSpade */

                    }else if(arrcpySpadeToken[0] == 'K'){
                        /* ADDING Kc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'K';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Kc TO **foundationSpade */

                        /* ADDING Qc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'Q';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Qc TO **foundationSpade */

                        /* ADDING Jc to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'J';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Jc TO **foundationSpade */

                        /* ADDING Tc TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'T';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Tc TO **foundationSpade */

                        /* ADDING 9c-2c TO **foundationSpade */
                        for(int i = 9; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 's';

                            cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;
                            cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationSpade */

                        /* ADDING Ac TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationSpade */

                    }else if(isdigit(arrcpySpadeToken[0]) != 0){
                        char *convert = &arrcpySpadeToken[0];
                        int rankVal = atoi(convert);

                        if(rankVal < 2){
                            fprintf(stderr, "Error near line %d: Rank for Spade Foundation is not valid!\n", lineCount);
                            exit(-1);
                        }

                        /* ADDING (rankVal)c to **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = rankVal +'0';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING (rankVal)cc TO **foundationSpade */

                        /* ADDING (rankVal-1)c-2c TO **foundationSpade */
                        for(int i = rankVal-1; i >= 2; i--){
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->rank = i +'0';
                            newCard->suit = 's';

                            cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                            sizeoffoundationSpade++;
                            cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        }

                        /* END OF ADDING 9c-2c TO **foundationSpade */

                        /* ADDING Ac TO **foundationSpade */
                        newCard = malloc(sizeof(newCard) * 1);
                        newCard->color = 'B';
                        newCard->rank = 'A';
                        newCard->suit = 's';

                        cpyFoundationSpade[sizeoffoundationSpade] = (struct card*)malloc(1*sizeof(struct card*));
                        memcpy(cpyFoundationSpade[sizeoffoundationSpade], newCard, 1*sizeof(struct card*));

                        sizeoffoundationSpade++;
                        cpyFoundationSpade = realloc(cpyFoundationSpade, (sizeoffoundationSpade+1)*sizeof(struct card*));
                        /* END OF ADDING Ac TO **foundationSpade */

                    }else{
                        fprintf(stderr, "Error near line %d: Expecting a valid rank for spade foundation!\n", lineCount);
                        exit(-1);
                    }
                }

                doneSpade = true;
            }else{
                if(strcmp(cpySpadeToken, "#") == 0){
                    break;
                }
            }

            spadeToken = strtok(NULL, " \t\r\n\v\f\r");
        }

        tracker = 0;
        for(int i = sizeoffoundationSpade-1; i >= 0; i--){
            struct card currentCard = *cpyFoundationSpade[i];
            struct card *toAddCard = (struct card*)malloc(1*sizeof(struct card*));

            toAddCard->color = currentCard.color;
            toAddCard->rank = currentCard.rank;
            toAddCard->suit = currentCard.suit;
            
            foundationSpade[tracker] = (struct card*)malloc(1*sizeof(struct card*));
            memcpy(foundationSpade[tracker], toAddCard, 1*sizeof(struct card*));

            tracker++;
            foundationSpade = realloc(foundationSpade, (tracker+1)*sizeof(struct card*));

        }

        for(int i = 0; i < sizeoffoundationSpade; i++){
            struct card *currentCard = foundationSpade[i];

            printf("%c%c\n", currentCard->rank, currentCard->suit);
        }

        /* END OF READING SPADE FOUNDATION */

        skipWhiteSpace(inFile);
    }
    

}

void checkTableau(FILE *inFile){
    printf("GOT INSIDE CHECK TABLEAU");
    tableau1Uncover = malloc(1*sizeof(struct card*));
    tableau1Cover = malloc(1*sizeof(struct card*));
    t1coverSize = 0;
    t1uncoverSize = 0;

    tableau2Uncover = malloc(1*sizeof(struct card*));
    tableau2Cover = malloc(1*sizeof(struct card*));
    t2coverSize = 0;
    t2uncoverSize = 0;

    tableau3Uncover = malloc(1*sizeof(struct card*));
    tableau3Cover = malloc(1*sizeof(struct card*));
    t3coverSize = 0;
    t3uncoverSize = 0;

    tableau4Uncover = malloc(1*sizeof(struct card*));
    tableau4Cover = malloc(1*sizeof(struct card*));
    t4coverSize = 0;
    t4uncoverSize = 0;  

    tableau5Uncover = malloc(1*sizeof(struct card*));
    tableau5Cover = malloc(1*sizeof(struct card*));
    t5coverSize = 0;
    t5uncoverSize = 0;

    tableau6Uncover = malloc(1*sizeof(struct card*));
    tableau6Cover = malloc(1*sizeof(struct card*));
    t6coverSize = 0;
    t6uncoverSize = 0;

    tableau7Uncover = malloc(1*sizeof(struct card*));
    tableau7Cover = malloc(1*sizeof(struct card*));
    t7coverSize = 0;
    t7uncoverSize = 0;

    bool readUncover = false;

    char *cpyReadLine;
    cpyReadLine = (char *)malloc(strlen(readLine));
    strcpy(cpyReadLine, readLine);

    char *token;

    token = strtok(cpyReadLine, " \t\r\n\v\f\r");

    if(strcmp(token, "TABLEAU:") != 0){
        fprintf(stderr, "Error near line %d: Expecting 'TABLEAU:'!\n", lineCount);
        exit(-1);
    }else{
        readComments(inFile);

        /* DEAL WITH TABLEAU COL 7 */
        char *readTableau7;
        readTableau7 = (char *)malloc(strlen(readLine));
        strcpy(readTableau7, readLine);

        char *tableau7Token;
        tableau7Token = strtok(readTableau7, " \t\r\n\v\f\r");


        while(tableau7Token != NULL){
            if(strcmp(tableau7Token, "#") == 0){
                /* DONE READING TABLEAU 7 */
                break;
            }else if(strcmp(tableau7Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau7Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau7Token[10];
                strcpy(arrcpytableau7Token, tableau7Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau7Token) == 2){
                    if(checkSuit(arrcpytableau7Token[1]) == true && checkRank(arrcpytableau7Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau7Token[1] == 'd' || arrcpytableau7Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau7Token[1];

                            if(arrcpytableau7Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau7Token[0];

                            tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7Uncover[t7uncoverSize], newCard, 1*sizeof(struct card*));

                            t7uncoverSize++;

                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau7Token[1];

                            if(arrcpytableau7Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau7Token[0];

                            tableau7Uncover[t7uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7Uncover[t7uncoverSize], newCard, 1*sizeof(struct card*));

                            t7uncoverSize++;

                            tableau7Uncover = realloc(tableau7Uncover, (t7uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */                

                /* CARD VALIDATION */
                char arrcpytableau7Token[10];
                strcpy(arrcpytableau7Token, tableau7Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau7Token) == 2){
                    if(checkSuit(arrcpytableau7Token[1]) == true && checkRank(arrcpytableau7Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau7Token[1] == 'd' || arrcpytableau7Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau7Token[1];

                            if(arrcpytableau7Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau7Token[0];

                            tableau7Cover[t7coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7Cover[t7coverSize], newCard, 1*sizeof(struct card*));

                            t7coverSize++;

                            tableau7Cover = realloc(tableau7Cover, (t7coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau7Token[1];

                            if(arrcpytableau7Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau7Token[0];

                            tableau7Cover[t7coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau7Cover[t7coverSize], newCard, 1*sizeof(struct card*));

                            t7coverSize++;

                            tableau7Cover = realloc(tableau7Cover, (t7coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau7Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        
        /* FINISH DEAL WITH TABLEAU COL 7 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }

        /* END OF WHITE SPACE AFTER TABLEAU */

        /* DEAL WITH TABLEAU COL 6 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau6;
        readTableau6 = (char *)malloc(strlen(readLine));
        strcpy(readTableau6, readLine);
        char *tableau6Token;
        tableau6Token = strtok(readTableau6, " \t\r\n\v\f\r");

        if(tableau6Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 6 cards!\n", lineCount);
            exit(-1);
        }

        //struct card *newCard;

        while(tableau6Token != NULL){
            if(strcmp(tableau6Token, "#") == 0){
                /* DONE READING TABLEAU 7 */
                break;
            }else if(strcmp(tableau6Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau6Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau6Token[10];
                strcpy(arrcpytableau6Token, tableau6Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau6Token) == 2){
                    if(checkSuit(arrcpytableau6Token[1]) == true && checkRank(arrcpytableau6Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau6Token[1] == 'd' || arrcpytableau6Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau6Token[1];

                            if(arrcpytableau6Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau6Token[0];

                            tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6Uncover[t6uncoverSize], newCard, 1*sizeof(struct card*));

                            t6uncoverSize++;

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau6Token[1];

                            if(arrcpytableau6Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau6Token[0];

                            tableau6Uncover[t6uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6Uncover[t6uncoverSize], newCard, 1*sizeof(struct card*));

                            t6uncoverSize++;

                            tableau6Uncover = realloc(tableau6Uncover, (t6uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau6Token[10];
                strcpy(arrcpytableau6Token, tableau6Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau6Token) == 2){
                    if(checkSuit(arrcpytableau6Token[1]) == true && checkRank(arrcpytableau6Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau6Token[1] == 'd' || arrcpytableau6Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau6Token[1];

                            if(arrcpytableau6Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau6Token[0];

                            tableau6Cover[t6coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6Cover[t6coverSize], newCard, 1*sizeof(struct card*));

                            t6coverSize++;

                            tableau6Cover = realloc(tableau6Cover, (t6coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau6Token[1];

                            if(arrcpytableau6Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau6Token[0];

                            tableau6Cover[t6coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau6Cover[t6coverSize], newCard, 1*sizeof(struct card*));

                            t6coverSize++;

                            tableau6Cover = realloc(tableau6Cover, (t6coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau6Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* FINISH DEAL WITH TABLEAU COL 6 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }

        /* END OF WHITE SPACE AFTER TABLEAU */

        /* DEAL WITH TABLEAU COL 5 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau5;
        readTableau5 = (char *)malloc(strlen(readLine));
        strcpy(readTableau5, readLine);

        char *tableau5Token;
        tableau5Token = strtok(readTableau5, " \t\r\n\v\f\r");

        if(tableau5Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 5!\n", lineCount);
            exit(-1);
        }

        while(tableau5Token != NULL){
            if(strcmp(tableau5Token, "#") == 0){
                /* DONE READING TABLEAU 5 */
                break;
            }else if(strcmp(tableau5Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau5Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau5Token[10];
                strcpy(arrcpytableau5Token, tableau5Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau5Token) == 2){
                    if(checkSuit(arrcpytableau5Token[1]) == true && checkRank(arrcpytableau5Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau5Token[1] == 'd' || arrcpytableau5Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau5Token[1];

                            if(arrcpytableau5Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau5Token[0];

                            tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5Uncover[t5uncoverSize], newCard, 1*sizeof(struct card*));

                            t5uncoverSize++;

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau5Token[1];

                            if(arrcpytableau5Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau5Token[0];

                            tableau5Uncover[t5uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5Uncover[t5uncoverSize], newCard, 1*sizeof(struct card*));

                            t5uncoverSize++;

                            tableau5Uncover = realloc(tableau5Uncover, (t5uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau5Token[10];
                strcpy(arrcpytableau5Token, tableau5Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau5Token) == 2){
                    if(checkSuit(arrcpytableau5Token[1]) == true && checkRank(arrcpytableau5Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau5Token[1] == 'd' || arrcpytableau5Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau5Token[1];

                            printf("T5 covered suit : %c\n", newCard->suit);

                            if(arrcpytableau5Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau5Token[0];

                            printf("T5 covered rank : %c\n", newCard->rank);

                            tableau5Cover[t5coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5Cover[t5coverSize], newCard, 1*sizeof(struct card*));

                            t5coverSize++;

                            tableau5Cover = realloc(tableau5Cover, (t5coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau5Token[1];

                            if(arrcpytableau5Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau5Token[0];

                            tableau5Cover[t5coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau5Cover[t5coverSize], newCard, 1*sizeof(struct card*));

                            t5coverSize++;

                            tableau5Cover = realloc(tableau5Cover, (t5coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau5Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* FINISH DEAL WITH TABLEAU COL 5 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }
        /* END OF WHITE SPACE AFTER TABLEAU */
        
        /* DEAL WITH TABLEAU COL 4 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau4;
        readTableau4 = (char *)malloc(strlen(readLine));
        strcpy(readTableau4, readLine);

        char *tableau4Token;
        tableau4Token = strtok(readTableau4, " \t\r\n\v\f\r");

        if(tableau4Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 4!\n", lineCount);
            exit(-1);
        }

        while(tableau4Token != NULL){
            if(strcmp(tableau4Token, "#") == 0){
                /* DONE READING TABLEAU 4 */
                break;
            }else if(strcmp(tableau4Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau4Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau4Token[10];
                strcpy(arrcpytableau4Token, tableau4Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau4Token) == 2){
                    if(checkSuit(arrcpytableau4Token[1]) == true && checkRank(arrcpytableau4Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau4Token[1] == 'd' || arrcpytableau4Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau4Token[1];

                            if(arrcpytableau4Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau4Token[0];

                            tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4Uncover[t4uncoverSize], newCard, 1*sizeof(struct card*));

                            t4uncoverSize++;

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau4Token[1];

                            if(arrcpytableau4Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau4Token[0];

                            tableau4Uncover[t4uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4Uncover[t4uncoverSize], newCard, 1*sizeof(struct card*));

                            t4uncoverSize++;

                            tableau4Uncover = realloc(tableau4Uncover, (t4uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau4Token[10];
                strcpy(arrcpytableau4Token, tableau4Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau4Token) == 2){
                    if(checkSuit(arrcpytableau4Token[1]) == true && checkRank(arrcpytableau4Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau4Token[1] == 'd' || arrcpytableau4Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau4Token[1];

                            if(arrcpytableau4Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau4Token[0];

                            tableau4Cover[t4coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4Cover[t4coverSize], newCard, 1*sizeof(struct card*));

                            t4coverSize++;

                            tableau4Cover = realloc(tableau4Cover, (t4coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau4Token[1];

                            if(arrcpytableau4Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau4Token[0];

                            tableau4Cover[t4coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau4Cover[t4coverSize], newCard, 1*sizeof(struct card*));

                            t4coverSize++;

                            tableau4Cover = realloc(tableau4Cover, (t4coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau4Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* FINISH DEAL WITH TABLEAU COL 4 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }
        /* END OF WHITE SPACE AFTER TABLEAU */

        /* DEAL WITH TABLEAU COL 3 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau3;
        readTableau3 = (char *)malloc(strlen(readLine));
        strcpy(readTableau3, readLine);

        char *tableau3Token;
        tableau3Token = strtok(readTableau3, " \t\r\n\v\f\r");

        if(tableau3Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 3!\n", lineCount);
            exit(-1);
        }

        while(tableau3Token != NULL){
            if(strcmp(tableau3Token, "#") == 0){
                /* DONE READING TABLEAU 3 */
                break;
            }else if(strcmp(tableau3Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau3Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau3Token[10];
                strcpy(arrcpytableau3Token, tableau3Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau3Token) == 2){
                    if(checkSuit(arrcpytableau3Token[1]) == true && checkRank(arrcpytableau3Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau3Token[1] == 'd' || arrcpytableau3Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau3Token[1];

                            if(arrcpytableau3Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau3Token[0];

                            tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3Uncover[t3uncoverSize], newCard, 1*sizeof(struct card*));

                            t3uncoverSize++;

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau3Token[1];

                            if(arrcpytableau3Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau3Token[0];

                            tableau3Uncover[t3uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3Uncover[t3uncoverSize], newCard, 1*sizeof(struct card*));

                            t3uncoverSize++;

                            tableau3Uncover = realloc(tableau3Uncover, (t3uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau3Token[10];
                strcpy(arrcpytableau3Token, tableau3Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau3Token) == 2){
                    if(checkSuit(arrcpytableau3Token[1]) == true && checkRank(arrcpytableau3Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau3Token[1] == 'd' || arrcpytableau3Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau3Token[1];

                            if(arrcpytableau3Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau3Token[0];

                            tableau3Cover[t3coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3Cover[t3coverSize], newCard, 1*sizeof(struct card*));

                            t3coverSize++;

                            tableau3Cover = realloc(tableau3Cover, (t3coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau3Token[1];

                            if(arrcpytableau3Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau3Token[0];

                            tableau3Cover[t3coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau3Cover[t3coverSize], newCard, 1*sizeof(struct card*));

                            t3coverSize++;

                            tableau3Cover = realloc(tableau3Cover, (t3coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau3Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* FINISH DEAL WITH TABLEAU COL 3 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }
        /* END OF WHITE SPACE AFTER TABLEAU */

        /* DEAL WITH TABLEAU COL 2 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau2;
        readTableau2 = (char *)malloc(strlen(readLine));
        strcpy(readTableau2, readLine);

        char *tableau2Token;
        tableau2Token = strtok(readTableau2, " \t\r\n\v\f\r");

        if(tableau2Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 2!\n", lineCount);
            exit(-1);
        }

        while(tableau2Token != NULL){
            if(strcmp(tableau2Token, "#") == 0){
                /* DONE READING TABLEAU 7 */
                break;
            }else if(strcmp(tableau2Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau2Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau2Token[10];
                strcpy(arrcpytableau2Token, tableau2Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau2Token) == 2){
                    if(checkSuit(arrcpytableau2Token[1]) == true && checkRank(arrcpytableau2Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau2Token[1] == 'd' || arrcpytableau2Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau2Token[1];

                            if(arrcpytableau2Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau2Token[0];

                            tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2Uncover[t2uncoverSize], newCard, 1*sizeof(struct card*));

                            t2uncoverSize++;

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau2Token[1];

                            if(arrcpytableau2Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau2Token[0];

                            tableau2Uncover[t2uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2Uncover[t2uncoverSize], newCard, 1*sizeof(struct card*));

                            t2uncoverSize++;

                            tableau2Uncover = realloc(tableau2Uncover, (t2uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau2Token[10];
                strcpy(arrcpytableau2Token, tableau2Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau2Token) == 2){
                    if(checkSuit(arrcpytableau2Token[1]) == true && checkRank(arrcpytableau2Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau2Token[1] == 'd' || arrcpytableau2Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau2Token[1];

                            if(arrcpytableau2Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau2Token[0];

                            tableau2Cover[t2coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2Cover[t2coverSize], newCard, 1*sizeof(struct card*));

                            t2coverSize++;

                            tableau2Cover = realloc(tableau2Cover, (t2coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau2Token[1];

                            if(arrcpytableau2Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau2Token[0];

                            tableau2Cover[t2coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau2Cover[t2coverSize], newCard, 1*sizeof(struct card*));

                            t2coverSize++;

                            tableau2Cover = realloc(tableau2Cover, (t2coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }
            tableau2Token = strtok(NULL, " \t\r\n\v\f\r");
        } 
        /* FINISH DEAL WITH TABLEAU COL 2 */
        readUncover = false;
        /* WHITE SPACE AFTER TABLEAU */
        fgets(readLine, 100, inFile);
        lineCount++;

        cpyReadLine = (char *)malloc(strlen(readLine));
        strcpy(cpyReadLine, readLine);

        token = strtok(cpyReadLine, " \t\r\n\v\f\r");

        if(token != NULL){
            fprintf(stderr, "Error near line %d: Expecting no character at this line!\n", lineCount);
            exit(-1);
        }
        /* END OF WHITE SPACE AFTER TABLEAU */

        /* DEAL WITH TABLEAU COL 1 */
        fgets(readLine, 100, inFile);
        lineCount++;

        char *readTableau1;
        readTableau1 = (char *)malloc(strlen(readLine));
        strcpy(readTableau1, readLine);

        char *tableau1Token;
        tableau1Token = strtok(readTableau1, " \t\r\n\v\f\r");

        if(tableau1Token == NULL){
            fprintf(stderr, "Error near line %d: Expecting tableau 2!\n", lineCount);
            exit(-1);
        }

        while(tableau1Token != NULL){
            if(strcmp(tableau1Token, "#") == 0){
                /* DONE READING TABLEAU 7 */
                break;
            }else if(strcmp(tableau1Token, "|") == 0){
                readUncover = true;
            }else if(readUncover == true && strcmp(tableau1Token, "#") != 0){
                /* CHECK FOR UNCOVERED CARDS */

                char arrcpytableau1Token[10];
                strcpy(arrcpytableau1Token, tableau1Token);

                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau1Token) == 2){
                    if(checkSuit(arrcpytableau1Token[1]) == true && checkRank(arrcpytableau1Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau1Token[1] == 'd' || arrcpytableau1Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau1Token[1];

                            if(arrcpytableau1Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau1Token[0];

                            tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1Uncover[t1uncoverSize], newCard, 1*sizeof(struct card*));

                            t1uncoverSize++;

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau1Token[1];

                            if(arrcpytableau1Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau1Token[0];

                            tableau1Uncover[t1uncoverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1Uncover[t1uncoverSize], newCard, 1*sizeof(struct card*));

                            t1uncoverSize++;

                            tableau1Uncover = realloc(tableau1Uncover, (t1uncoverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }else{
                /* CHECK FOR COVERED CARDS */

                /* CARD VALIDATION */
                char arrcpytableau1Token[10];
                strcpy(arrcpytableau1Token, tableau1Token);
                struct card *newCard;

                /* CARD VALIDATION */
                if(strlen(arrcpytableau1Token) == 2){
                    if(checkSuit(arrcpytableau1Token[1]) == true && checkRank(arrcpytableau1Token[0]) == true){
                        /* DEALING WITH COLORED CARDS */
                        if(arrcpytableau1Token[1] == 'd' || arrcpytableau1Token[1] == 'h'){
                            /* DEAL WITH ONLY RED COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'R';
                            newCard->suit = arrcpytableau1Token[1];

                            if(arrcpytableau1Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau1Token[0];

                            tableau1Cover[t1coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1Cover[t1coverSize], newCard, 1*sizeof(struct card*));

                            t1coverSize++;

                            tableau1Cover = realloc(tableau1Cover, (t1coverSize+1)*sizeof(struct card*));

                        }else{
                            /* DEAL WITH ONLY BLACK COLOR CARDS */
                            newCard = malloc(sizeof(newCard) * 1);
                            newCard->color = 'B';
                            newCard->suit = arrcpytableau1Token[1];

                            if(arrcpytableau1Token[0] == '_'){
                                fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                exit(-1);
                            }

                            newCard->rank = arrcpytableau1Token[0];

                            tableau1Cover[t1coverSize] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(tableau1Cover[t1coverSize], newCard, 1*sizeof(struct card*));

                            t1coverSize++;

                            tableau1Cover = realloc(tableau1Cover, (t1coverSize+1)*sizeof(struct card*));

                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                        exit(-1);
                    }
                }else{
                    fprintf(stderr, "Error near line %d: Invalid card input!\n", lineCount);
                    exit(-1);
                }

            }

            tableau1Token = strtok(NULL, " \t\r\n\v\f\r");
        }
        /* FINISH DEAL WITH TABLEAU COL 1 */

        skipWhiteSpace(inFile);
    }
}

void checkStock(FILE *inFile){
    struct card **copyOfStock = malloc(1*sizeof(struct card*));
    struct card **copyOfWaste = malloc(1*sizeof(struct card*));
    stock = malloc(1*sizeof(struct card*));
    sizeofStock = 0;
    waste = malloc(1*sizeof(struct card*));
    sizeofWaste = 0;
    bool readStock = false;

    char *cpyReadLine;
    cpyReadLine = (char *)malloc(strlen(readLine));
    strcpy(cpyReadLine, readLine);

    char *token;
    token = strtok(cpyReadLine, " \t\r\n\v\f\r");

    if(strcmp(token, "STOCK:") != 0){
        fprintf(stderr, "Error near line %d: Expecting 'STOCK:'!\n", lineCount);
        exit(-1);
    }else{
        fgets(readLine, 100, inFile);

        char *stockReadLine;
        stockReadLine = (char *)malloc(strlen(readLine));
        strcpy(stockReadLine, readLine);

        char *stockToken;
        stockToken = strtok(stockReadLine, " \t\r\n\v\f\r");

        while(stockToken != NULL){
            char *cpystockReadLine;
            cpystockReadLine = (char *)malloc(strlen(readLine));
            strcpy(cpystockReadLine, readLine);

            char *cpystockToken;
            cpystockToken = strtok(cpystockReadLine, " \t\r\n\v\f\r");

            while(cpystockToken != NULL){
                if(strcmp(cpystockToken, "#") == 0){
                    break;
                }else if(strcmp(cpystockToken, "|") == 0){
                    readStock = true;
                }else if(readStock == true && strcmp(cpystockToken, "|") != 0 && strcmp(cpystockToken, "#") != 0){
                    /* CHECK STOCK CARDS */
                    char arrStockToken[10];
                    strcpy(arrStockToken, cpystockToken);
                    struct card *newCard;

                    if(strlen(arrStockToken) == 2){
                        if(checkSuit(arrStockToken[1]) == true && checkRank(arrStockToken[0]) == true){
                            if(arrStockToken[1] == 'd' || arrStockToken[1] == 'h'){
                                /* DEAL WITH ONLY RED COLOR CARDS */
                                newCard = malloc(sizeof(newCard) * 1);
                                newCard->color = 'R';
                                newCard->suit = arrStockToken[1];

                                if(arrStockToken[0] == '_'){
                                    fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                    exit(-1);
                                }

                                newCard->rank = arrStockToken[0];

                                copyOfStock[sizeofStock] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(copyOfStock[sizeofStock], newCard, 1*sizeof(struct card*));

                                sizeofStock++;
                                
                                copyOfStock = realloc(copyOfStock, (sizeofStock+1)*sizeof(struct card*));
                            }else{
                                /* DEAL WITH ONLY BLACK COLOR CARDS */
                                newCard = malloc(sizeof(newCard) * 1);
                                newCard->color = 'B';
                                newCard->suit = arrStockToken[1];

                                if(arrStockToken[0] == '_'){
                                    fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                    exit(-1);
                                }

                                newCard->rank = arrStockToken[0];

                                copyOfStock[sizeofStock] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(copyOfStock[sizeofStock], newCard, 1*sizeof(struct card*));

                                sizeofStock++;

                                copyOfStock = realloc(copyOfStock, (sizeofStock+1)*sizeof(struct card*));
                            }
                        }
                    }
                }else{
                    /* CHECK WASTE CARDS */
                    char arrwasteToken[10];
                    strcpy(arrwasteToken, cpystockToken);
                    struct card *newCard;

                    if(strlen(arrwasteToken) == 2){
                        if(checkSuit(arrwasteToken[1]) == true && checkRank(arrwasteToken[0]) == true){
                            if(arrwasteToken[1] == 'd' || arrwasteToken[1] == 'h'){
                                /* DEAL WITH ONLY RED COLOR CARDS */
                                newCard = (struct card*)malloc(1*sizeof(struct card*));
                                newCard->color = 'R';
                                newCard->suit = arrwasteToken[1];

                                if(arrwasteToken[0] == '_'){
                                    fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                    exit(-1);
                                }

                                newCard->rank = arrwasteToken[0];

                                copyOfWaste[sizeofWaste] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(copyOfWaste[sizeofWaste], newCard, 1*sizeof(struct card*));

                                sizeofWaste++;

                                copyOfWaste = realloc(copyOfWaste, (sizeofWaste+1)*sizeof(struct card*));
                            }else{
                                /* DEAL WITH ONLY BLACK COLOR CARDS */
                                newCard = (struct card*)malloc(sizeof(struct card*) * 1);
                                newCard->color = 'B';
                                newCard->suit = arrwasteToken[1];

                                if(arrwasteToken[0] == '_'){
                                    fprintf(stderr, "Error near line %d: '_' is an invalid rank!\n", lineCount);
                                    exit(-1);
                                }

                                newCard->rank = arrwasteToken[0];

                                copyOfWaste[sizeofWaste] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(copyOfWaste[sizeofWaste], newCard, 1*sizeof(struct card*));

                                sizeofWaste++;

                                copyOfWaste = realloc(copyOfWaste, (sizeofWaste+1)*sizeof(struct card*));
                            }
                        }else{
                            fprintf(stderr, "Error near line %d: Invalid rank or suit!\n", lineCount);
                            exit(-1);
                        }
                    }else{
                        fprintf(stderr, "Error near line %d: Invalid waste card!\n", lineCount);
                        exit(-1);
                    }
                }
                cpystockToken = strtok(NULL, " \t\r\n\v\f\r");    
            }

            fgets(readLine, 100, inFile);

            stockReadLine = (char *)malloc(strlen(readLine));
            strcpy(stockReadLine, readLine);

            stockToken = strtok(stockReadLine, " \t\r\n\v\f\r");
        }
    }

    /* MAKE THE FIRST CARD OF STOCK AND WASTE TO BE AT THE LAST INDEX */
    int tracker = 0;
    for(int i = sizeofStock-1; i >= 0; i--){
        struct card *newCard = (struct card*)malloc(1*sizeof(struct card*));
        struct card copyOfStockAtIndex = *copyOfStock[i];

        newCard->color = copyOfStockAtIndex.color;
        newCard->rank = copyOfStockAtIndex.rank;
        newCard->suit = copyOfStockAtIndex.suit;

        stock[tracker] = (struct card*)malloc(1*sizeof(struct card*));
        memcpy(stock[tracker], newCard, 1*sizeof(struct card*));

        tracker++;

        stock = realloc(stock, (tracker+1)*sizeof(struct card*));

    }

    tracker = 0;
    for(int i = 0; i < sizeofWaste; i++){
        struct card *newCard = (struct card*)malloc(1*sizeof(struct card*));
        struct card copyOfWasteAtIndex = *copyOfWaste[i];

        
        newCard->color = copyOfWasteAtIndex.color;
        newCard->rank = copyOfWasteAtIndex.rank;
        newCard->suit = copyOfWasteAtIndex.suit;

        waste[tracker] = (struct card*)malloc(1*sizeof(struct card*));
        memcpy(waste[tracker], newCard, 1*sizeof(struct card*));

        tracker++;

        waste = realloc(waste, (tracker+1)*sizeof(struct card*));
    }

    skipWhiteSpace(inFile);    
    
}

void checkMoves(FILE *inFile){    
    moveCount = 0;
   
    moveList = malloc(1*sizeof(char*)); //2D Array
    int moveListIndex = 0;
    char source = ' ';
    char destination = ' ';

    char *cpyReadLine;
    cpyReadLine = (char *)malloc(strlen(readLine));
    strcpy(cpyReadLine, readLine);

    char *token;
    token = strtok(cpyReadLine, " \t\r\n\v\f\r");

    if(strcmp(token, "MOVES:") != 0){
        fprintf(stderr, "Error near line %d: Expecting 'MOVES:'!\n", lineCount);
        exit(-1);
    }else{
        /* STORE ALL MOVES TO moveList */
        while(!feof(inFile)){

            fgets(readLine, 100, inFile);
            lineCount++;

            cpyReadLine = (char *)malloc(strlen(readLine));
            strcpy(cpyReadLine, readLine);

            token = strtok(cpyReadLine, " \t\r\n\v\f\r");


            /* Check if there any comment(s) between line of moves */
            if(strcmp(token, "#") == 0){
                continue;
            }else if(token == NULL){
                continue;
            }else{
                cpyReadLine = (char *)malloc(strlen(readLine));
                strcpy(cpyReadLine, readLine);

                token = strtok(cpyReadLine, " \t\r\n\v\f\r");

                /* COUNT THE NUMBER OF MOVE(S) */ 
                while(token != NULL){
                    char *cpyToken;
                    moveCount++;

                    token = strtok(NULL, " \t\r\n\v\f\r");
                }
                
                printf("New Line\n");
                printf("moveCount is: %d\n", moveCount);

                cpyReadLine = (char *)malloc(strlen(readLine));
                strcpy(cpyReadLine, readLine);

                token = strtok(cpyReadLine, " \t\r\n\v\f\r");

                /* REALLOCATE THE SIZE OF moveList to moveCount */
                moveList = realloc(moveList, moveCount*sizeof(token));

                /* COPY ALL THE MOVES INTO moveList */
                while(token != NULL){
                    
                    moveList[moveListIndex] = (char *)malloc(strlen(token));
                    strcpy(moveList[moveListIndex], token);

                    token = strtok(NULL, " \t\r\n\v\f\r");

                    moveListIndex++;
                }
            }   
        }

        

        /* CHECK IF THE MOVE IS VALID OR NOT */
        for(int i = 0; i < (moveCount-1); i++){
            if(i > maxMoves){
                break;
            }else{
                printf("MOVE %s MoveCount is: %d\n", moveList[i], moveListIndex);
                char *cpyMove;
                cpyMove = (char *)malloc(strlen(moveList[i]));
                strcpy(cpyMove, moveList[i]);
                
                /* IF MOVE CONTAINS '->' */
                if(strstr(cpyMove, "->") != NULL){
                    int charCount = 0;
                    bool checkSource = true;

                    cpyMove = (char *)malloc(strlen(moveList[i]));
                    strcpy(cpyMove, moveList[i]);

                    token = strtok(cpyMove, "->");

                    while(token != NULL){
                        printf("");
                        /* CHECK SOURCE */
                        if(checkSource == true){
                            char sourceToken[10];
                            strcpy(sourceToken, token);

                            if(strlen(sourceToken) == 1){
                                if(checkIsNumber(sourceToken)){

                                    /* CHECK FOR VALID TABLEAU COLUMNS */
                                    if(atoi(sourceToken) > tableauColumns || atoi(sourceToken) <= 0){
                                        fprintf(stderr, "Move %d is invalid : %s\n", i+1, sourceToken);
                                        fprintf(stderr, "Foundations\n");
                                        fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                                        fprintf(stderr, "Tableau\n");
                                        fprintf(stderr, "Waste Top\n");

                                        exit(-1);
                                    }

                                    source = sourceToken[0];

                                }else if(sourceToken[0] == 'w'){
                                    source = sourceToken[0];
                                }else{
                                    fprintf(stderr, "Move %d is invalid : %s\n", i+1, sourceToken);
                                    fprintf(stderr, "Foundations\n");
                                    fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                                    fprintf(stderr, "Tableau\n");
                                    fprintf(stderr, "Waste Top\n");

                                    exit(-1);
                                }

                            }else{
                                fprintf(stderr, "Move %d is invalid : %s\n", i+1, sourceToken);
                                fprintf(stderr, "Foundations\n");
                                fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                                fprintf(stderr, "Tableau\n");
                                fprintf(stderr, "Waste Top\n");

                                exit(-1);
                            }

                            checkSource = false;
                            //printf("PASS CHECK SOURCE\n");
                        }else{
                        /* CHECK DESTINATION */                            
                            char destToken[10];
                            strcpy(destToken, token);

                            if(strlen(destToken) == 1){
                                if(checkIsNumber(destToken)){
                                    /* CHECK FOR VALID TABLEAU COLUMNS */
                                    if(atoi(destToken) > tableauColumns || atoi(destToken) <= 0){
                                        fprintf(stderr, "Move %d is invalid : %s\n", i+1, destToken);
                                        fprintf(stderr, "Foundations\n");
                                        fprintf(stderr, "Tableau\n");
                                        fprintf(stderr, "Waste Top\n");

                                        exit(-1);
                                    }

                                    destination = destToken[0];

                                }else if(destToken[0] == 'w'){
                                    destination = destToken[0];
                                }else if(destToken[0] == 'f'){
                                    destination = destToken[0];
                                }else{
                                    fprintf(stderr, "Move %d is invalid : %s\n", i+1, destToken);
                                    fprintf(stderr, "Foundations\n");
                                    fprintf(stderr, "Tableau\n");
                                    fprintf(stderr, "Waste Top\n");

                                    exit(-1);
                                }

                            }else{
                                fprintf(stderr, "Move %d is invalid : %s\n", i+1, destToken);
                                fprintf(stderr, "Foundations\n");
                                fprintf(stderr, "Tableau\n");
                                fprintf(stderr, "Waste Top\n");

                                exit(-1);
                            }
                        }

                        /* Number of characters before and after '->' more than 2 */
                        if(charCount > 2){
                            fprintf(stderr, "Move %d is illegal : %s\n", i+1, moveList[i]);
                            fprintf(stderr, "Foundations\n");
                            fprintf(stderr, "Tableau\n");
                            fprintf(stderr, "Waste Top\n");

                            exit(-1);
                        }

                        token = strtok(NULL, "->");
                    }                    

                    if(movePile(source, destination) == false){
                        fprintf(stderr, "Move %d is illegal : %s\n", i+1, moveList[i]);
                        fprintf(stderr, "Foundations\n");
                        //fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                        fprintf(stderr, "Tableau\n");
                        fprintf(stderr, "Waste Top\n");

                        exit(-1);
                    }
                    free(cpyMove);
                
                /* IF MOVE CONTAINS '.' */
                }else if(strstr(cpyMove, ".") != NULL){
                    if(sizeofStock != 0){
                        /* DRAW CARD FROM STOCK TO WASTE */
                        //printf("TURN IS %d \n", turn);

                        for(int i = 0; i < turn; i++){
                            struct card *newWasteTop = (struct card*)malloc(1*sizeof(struct card*));
                            struct card *stockTop = stock[sizeofStock-1];

                            /* ADD NEW CARD ON TOP OF THE WASTE PILE */
                            newWasteTop->color = stockTop->color;
                            newWasteTop->rank = stockTop->rank;
                            newWasteTop->suit = stockTop->suit;
                            
                            waste[sizeofWaste] = (struct card*)malloc(1*sizeof(struct card*));
                            memcpy(waste[sizeofWaste], newWasteTop, 1*sizeof(struct card*));
                            sizeofWaste++;
                            waste = realloc(waste, (sizeofWaste+1)*sizeof(struct card*));

                            /* REMOVE TOP CARD FROM STOCK PILE */
                            free(stock[sizeofStock-1]);
                            sizeofStock--;
                            /* DONE REMOVE TOP CARD FROM STOCK PILE */

                        }

                        /* RESIZE STOCK */
                        stock = realloc(stock, (sizeofStock+1)*sizeof(struct card*));

                    }else{
                        fprintf(stderr, "Move %d is illegal : %s\n", i+1, cpyMove);
                        fprintf(stderr, "Foundations\n");
                        //fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                        fprintf(stderr, "Tableau\n");
                        fprintf(stderr, "Waste Top\n");

                        exit(-1);
                    }
                    
                
                /* IF MOVE CONTAINS 'r' */
                }else if(strstr(cpyMove, "r") != NULL){
                    if(sizeofStock == 0){
                        if(sizeofWaste != 0){
                            for(int i = sizeofWaste-1; i >= 0; i--){
                                struct card *newStockCard = (struct card*)malloc(1*sizeof(struct card*));
                                struct card *wasteAtIndex = waste[i];
                                /* ADD NEW CARD TO STOCK PILE */
                                newStockCard->color = wasteAtIndex->color;
                                newStockCard->rank = wasteAtIndex->rank;
                                newStockCard->suit = wasteAtIndex->suit;

                                stock[sizeofStock] = (struct card*)malloc(1*sizeof(struct card*));
                                memcpy(stock[sizeofStock], newStockCard, 1*sizeof(struct card*));
                                
                                sizeofStock++;

                                stock = realloc(stock, (sizeofStock+1)*sizeof(struct card*));
                                /* DONE ADD NEW CARD TO STOCK PILE */

                                /* REMOVE TOP CARD FROM WASTE PILE */
                                free(waste[i]);
                                sizeofWaste--;
                                /* DONE REMOVE TOP CARD FROM WASTE PILE */

                            }

                            if(sizeofWaste != 0){
                                fprintf(stderr, "Invalid move! There is still waste card on waste pile!\n");
                                exit(-1);
                            }
                            
                            /* RESIZE WASTE PILE */
                            waste = realloc(waste, (sizeofWaste+1)*sizeof(struct card*));

                        }else{
                            fprintf(stderr, "Move %d is illegal : %s\n", i+1, cpyMove);
                            fprintf(stderr, "Foundations\n");
                            //fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                            fprintf(stderr, "Tableau\n");
                            fprintf(stderr, "Waste Top\n");

                            exit(-1);
                        }

                    }else{
                        fprintf(stderr, "Move %d is illegal : %s\n", i+1, cpyMove);
                        fprintf(stderr, "Foundations\n");
                        //fprintf(stderr, "PRINT ALL FOUNDATIONS HERE\n");
                        fprintf(stderr, "Tableau\n");
                        fprintf(stderr, "Waste Top\n");

                        exit(-1);
                    }
                }
            }

            printf("Post Move %d\n", i+1);

            //printResultTest();
            

        }
    }
}

void printResultTest(){
    
    fprintf(stdout, "Foundations\n");

    if(sizeoffoundationClub != 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart != 0 && sizeoffoundationSpade != 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, heartFoundationTopCard->rank, heartFoundationTopCard->suit, spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);

    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond == 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade == 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, '_', 'd', '_', 'h', '_', 's');
    }else if(sizeoffoundationDiamond != 0 && sizeoffoundationClub == 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade == 0){
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, '_', 'h', '_', 's');

    }else if(sizeoffoundationHeart != 0 && sizeoffoundationDiamond == 0 && sizeoffoundationClub == 0 && sizeoffoundationSpade == 0){
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', '_', 'd', heartFoundationTopCard->rank, heartFoundationTopCard->suit, '_', 's');

    }else if(sizeoffoundationSpade != 0 && sizeoffoundationDiamond == 0 && sizeoffoundationClub == 0 && sizeoffoundationHeart == 0){
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', '_', 'd', '_', 'h', spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);
    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade == 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, '_', 'h', '_', 's');

    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond == 0 && sizeoffoundationHeart != 0 && sizeoffoundationSpade == 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, '_', 'd', heartFoundationTopCard->rank, heartFoundationTopCard->suit, '_', 's');
    
    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond == 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade != 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, '_', 'd', '_', 'h', spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);

    }else if(sizeoffoundationClub == 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart != 0 && sizeoffoundationSpade == 0){
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, heartFoundationTopCard->rank, heartFoundationTopCard->suit, '_', 's');

    }else if(sizeoffoundationClub == 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade != 0){
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, '_', 'h', spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);

    }else if(sizeoffoundationClub == 0 && sizeoffoundationDiamond == 0 && sizeoffoundationHeart != 0 && sizeoffoundationSpade != 0){
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];

        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', '_', 'd', heartFoundationTopCard->rank, heartFoundationTopCard->suit, spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);

    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart != 0 && sizeoffoundationSpade == 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
        struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];
        
        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, heartFoundationTopCard->rank, heartFoundationTopCard->suit, '_', 's');

    }else if(sizeoffoundationClub != 0 && sizeoffoundationDiamond != 0 && sizeoffoundationHeart == 0 && sizeoffoundationSpade != 0){
        struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
        struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
        struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];
        
        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, '_', 'h', spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);
    }else{
        fprintf(stdout, "%c%c %c%c %c%c %c%c\n", '_', 'c', '_', 'd', '_', 'h', '_', 's');
    }
    

    fprintf(stdout, "Tableau\n");

    fprintf(stdout, "Print Covered\n");
    fprintf(stdout, "T1 T2 T3 T4 T5 T6 T7\n");

    int maxCovered = findMaxNumberofCardinCover(t1coverSize, t2coverSize, t3coverSize, t4coverSize, t5coverSize, t6coverSize, t7coverSize);

    for(int i = 0; i < maxCovered; i++){
        if(t1coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t1coverSize){
                struct card *currT1Cover = tableau1Cover[i];
                
                fprintf(stdout, "%c%c ", currT1Cover->rank, currT1Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t2coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t2coverSize){
                struct card *currT2Cover = tableau2Cover[i];
                
                fprintf(stdout, "%c%c ", currT2Cover->rank, currT2Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t3coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t3coverSize){
                struct card *currT3Cover = tableau3Cover[i];
                
                fprintf(stdout, "%c%c ", currT3Cover->rank, currT3Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t4coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t4coverSize){
                struct card *currT4Cover = tableau4Cover[i];
                
                fprintf(stdout, "%c%c ", currT4Cover->rank, currT4Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t5coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t5coverSize){
                struct card *currT5Cover = tableau5Cover[i];
                
                fprintf(stdout, "%c%c ", currT5Cover->rank, currT5Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t6coverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t6coverSize){
                struct card *currT6Cover = tableau6Cover[i];
                
                fprintf(stdout, "%c%c ", currT6Cover->rank, currT6Cover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t7coverSize <= 0){
            fprintf(stdout, "..\n");
        }else{
            if(i < t7coverSize){
                struct card *currT7Cover = tableau7Cover[i];
                
                fprintf(stdout, "%c%c\n", currT7Cover->rank, currT7Cover->suit);
            }else{
                fprintf(stdout, "..\n");
            }
        }

    }

    fprintf(stdout, "Print Uncovered\n");
    fprintf(stdout, "T1 T2 T3 T4 T5 T6 T7\n");

    int maxUncovered = findMaxNumberofCardinCover(t1uncoverSize, t2uncoverSize, t3uncoverSize, t4uncoverSize, t5uncoverSize, t6uncoverSize, t7uncoverSize);

    for(int i = 0; i < maxUncovered; i++){
        if(t1uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t1uncoverSize){
                struct card *currT1UnCover = tableau1Uncover[i];
                
                fprintf(stdout, "%c%c ", currT1UnCover->rank, currT1UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t2uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t2uncoverSize){
                struct card *currT2UnCover = tableau2Uncover[i];
                
                fprintf(stdout, "%c%c ", currT2UnCover->rank, currT2UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t3uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t3uncoverSize){
                struct card *currT3UnCover = tableau3Uncover[i];
                
                fprintf(stdout, "%c%c ", currT3UnCover->rank, currT3UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t4uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t4uncoverSize){
                struct card *currT4UnCover = tableau4Uncover[i];
                
                fprintf(stdout, "%c%c ", currT4UnCover->rank, currT4UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t5uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t5uncoverSize){
                struct card *currT5UnCover = tableau5Uncover[i];
                
                fprintf(stdout, "%c%c ", currT5UnCover->rank, currT5UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t6uncoverSize <= 0){
            fprintf(stdout, ".. ");
        }else{
            if(i < t6uncoverSize){
                struct card *currT6UnCover = tableau6Uncover[i];
                
                fprintf(stdout, "%c%c ", currT6UnCover->rank, currT6UnCover->suit);
            }else{
                fprintf(stdout, ".. ");
            }
        }

        if(t7uncoverSize <= 0){
            fprintf(stdout, "..\n");
        }else{
            if(i < t7uncoverSize){
                struct card *currT7UnCover = tableau7Uncover[i];
                
                fprintf(stdout, "%c%c\n", currT7UnCover->rank, currT7UnCover->suit);
            }else{
                fprintf(stdout, "..\n");
            }
        }

    }

    fprintf(stdout, "Waste Top\n");
    
    if(sizeofWaste == 0){
        fprintf(stdout, "(Empty)\n");
    }else{
        struct card *wasteTop = waste[sizeofWaste-1];
        fprintf(stdout, "%c%c\n", wasteTop->rank, wasteTop->suit);
    }

}

// void printResult(){
//     struct card *clubFoundationTopCard = foundationClub[sizeoffoundationClub-1];
//     struct card *diamondFoundationTopCard = foundationDiamond[sizeoffoundationDiamond-1];
//     struct card *heartFoundationTopCard = foundationHeart[sizeoffoundationHeart-1];
//     struct card *spadeFoundationTopCard = foundationSpade[sizeoffoundationSpade-1];
//     fprintf(stdout, "Foundations\n");
//     fprintf(stdout, "%c%c %c%c %c%c %c%c\n", clubFoundationTopCard->rank, clubFoundationTopCard->suit, diamondFoundationTopCard->rank, diamondFoundationTopCard->suit, heartFoundationTopCard->rank, heartFoundationTopCard->suit, spadeFoundationTopCard->rank, spadeFoundationTopCard->suit);

//     struct card **tableau1 = malloc(1*sizeof(struct card*));
//     struct card **tableau2 = malloc(1*sizeof(struct card*));
//     struct card **tableau3 = malloc(1*sizeof(struct card*));
//     struct card **tableau4 = malloc(1*sizeof(struct card*));
//     struct card **tableau5 = malloc(1*sizeof(struct card*));
//     struct card **tableau6 = malloc(1*sizeof(struct card*));
//     struct card **tableau7 = malloc(1*sizeof(struct card*));



//     fprintf(stdout, "Tableau\n");
// }

int findMaxNumberofCardinUncover(int t1Uncover, int t2Uncover, int t3Uncover, int t4Uncover, int t5Uncover, int t6Uncover, int t7Uncover){
    int retVal = 0;
    int uncoverSizeArr[7] = {t1Uncover, t2Uncover, t3Uncover, t4Uncover, t5Uncover, t6Uncover, t7Uncover};

    for(int i = 0; i < 7; i++){
        if(retVal <= uncoverSizeArr[i]){
            retVal = uncoverSizeArr[i];
        }
    }

    return retVal;
}

int findMaxNumberofCardinCover(int t1Cover, int t2Cover, int t3Cover, int t4Cover, int t5Cover, int t6Cover, int t7Cover){
    int retVal = 0;
    int coverSizeArr[7] = {t1Cover, t2Cover, t3Cover, t4Cover, t5Cover, t6Cover, t7Cover};

    for(int i = 0; i < 7; i++){
        if(retVal <= coverSizeArr[i]){
            retVal = coverSizeArr[i];
        }
    }

    return retVal;
}

// struct card** makeTableau(struct card **tableauCover, struct card **tableauUncover, int coverSize, int uncoverSize){
//     struct card **retCard = malloc(1*sizeof(struct card*));

    
// }

// struct card** makeTableau(struct card **tableauCover, struct card **tableauUncover, int coverSize, int uncoverSize){
//     struct card **retCard = malloc(1*sizeof(struct card*));


// }

int runProgram(FILE *inFile){
    /* SKIP COMMENTS IF ANY */
    readComments(inFile);

    /*------------------------------- PART 1 -------------------------------*/

    /* RULES VALIDATION */
    checkRules(inFile);

    /* FOUNDATIONS VALIDATION */
    checkFoundations(inFile);

    /* FOUNDATIONS VALIDATION */
    checkTableau(inFile);

    /* STOCK VALIDATION */
    checkStock(inFile);

    checkUncoverTableauPile(tableau7Uncover, t7uncoverSize);

    /*---------------------------- END OF PART 1 ---------------------------*/

    /*-------------------------------- PART 2 ------------------------------*/

    /* MOVES VALIDATION */
    checkMoves(inFile);

    /*---------------------------- END OF PART 2 ---------------------------*/


}

int main(int argc, char *argv[]){
    /* GLOBAL VARIABLES INITIALIZATION */
    lineCount = 0;
    limit = 0;
    maxMoves = INT_MAX;

    readRules = false;
    readFoundations = false;
    readTableau = false;
    readStock = false;
    readMoves = false;

    /* END OF GLOBAL VARIABLES INITIALIZATION */

    /* NO FILE BEING PASSED. READ FROM STDIN */
    if(argc == 1){
        printf("Reading from stdin\n");
        runProgram(stdin);
    /* FILE IS PASSED */
    }else if(argc == 2){
        FILE *inputFile = fopen(argv[1], "r");

        if(inputFile == NULL){
            fprintf(stderr, "Error: Could not read file!\n");
			exit(-1);
        }else{
            /* RUN THE FILE */
            runProgram(inputFile);

            /* CLOSE THE GIVEN FILE */
            fclose(inputFile);
        }

    }else{
        fprintf(stderr, "Incorrect number of argument!\n");
        exit(-1);
    }
}
