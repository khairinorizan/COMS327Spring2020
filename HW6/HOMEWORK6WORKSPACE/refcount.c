/*

Author: Muhammad Khairi Norizan
Purpose: HW6

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct Node{
    char type;
    int nodeNum;
    int incomingPtr;
    struct Node *pointTo;
};

struct Node **varList;
struct Node **nodeList;
int varCount;
int nodeCount;

void makeConnection(char variableInputFile, int source, int dest);

void makeConnection(char variableInputFile, int source, int dest){
    if(variableInputFile == 'V'){
        if(dest != 0){
            struct Node *currVarNode = varList[source-1];

            if(currVarNode->pointTo == 0){
                currVarNode->pointTo = nodeList[dest-1];
        
                struct Node *destNode = nodeList[dest-1];
                int newValIncomingPointer = destNode->incomingPtr + 1;
                destNode->incomingPtr = newValIncomingPointer;

                struct Node *test = nodeList[dest-1];
            }else{
                struct Node *currPointTo = currVarNode->pointTo;
                currPointTo->incomingPtr = currPointTo->incomingPtr - 1;

                /* Copy new node to currVarNode->pointTo */
                currPointTo = nodeList[dest-1];
                currVarNode->pointTo = currPointTo;

                /* Increase the number of incoming pointer to the new currVarNode->pointTo */
                struct Node destNode = *nodeList[dest-1];
                destNode.incomingPtr = destNode.incomingPtr+1;
                *nodeList[dest-1] = destNode;

            }
            
        }else{
            struct Node *currVarNode = varList[source-1];            

            if(currVarNode->pointTo == 0){
                currVarNode->pointTo = 0;

            }else{
                struct Node *currPointTo = (struct Node*)malloc(1 * sizeof(struct Node));
                currPointTo = currVarNode->pointTo;

                currPointTo->incomingPtr = currPointTo->incomingPtr - 1;
                memcpy(currVarNode->pointTo, currPointTo, 1 * sizeof(struct Node));

                currVarNode->pointTo = 0;

            }
        }                               

    }else{
        if(dest != 0){
            struct Node *currNode = nodeList[source-1];

            if(currNode->pointTo == 0){
                currNode->pointTo = nodeList[dest-1];
                
                struct Node destNode = *nodeList[dest-1];
                destNode.incomingPtr++;

                *nodeList[dest-1] = destNode;
                
            }else{
                /* Take currNode->pointTo Node and decrease the number of incoming pointer by 1 */
                struct Node currPointTo = *currNode->pointTo;
                currPointTo.incomingPtr--;

                /* Add a new now to currNode->pointTo */
                free(currNode->pointTo);
                currPointTo = *nodeList[dest-1];

                *currNode->pointTo = currPointTo;

                struct Node destNode = *nodeList[dest-1];
                destNode.incomingPtr++;
                *nodeList[dest-1] = destNode;
            }

        }else{
            struct Node *currVarNode = nodeList[source-1];

            if(currVarNode->pointTo == 0){
                currVarNode->pointTo = 0;

            }else{
                struct Node currPointTo = *currVarNode->pointTo;
                currPointTo.incomingPtr--;

                currPointTo.pointTo = 0;

                *currVarNode->pointTo = currPointTo;

            }
        }   

    }
}

int main(){
    char readLine[100];
    char cpyReadLine[100];
    varList = malloc(1 * sizeof(struct Node));
    nodeList = malloc(1 * sizeof(struct Node));
    varCount = 0;
    nodeCount = 0;
    bool readNumofNodes = true;
    int nodePtrNum = 0;

    for(int i = 0; i < 9; i++){
        struct Node *newVarNode = (struct Node*)malloc(1 * sizeof(struct Node));

        newVarNode->type = 'V';
        newVarNode->nodeNum = i+1;
        newVarNode->incomingPtr = 0;
        newVarNode->pointTo = 0;

        varList[varCount] = (struct Node*)malloc(1 * sizeof(struct Node));
        memcpy(varList[varCount], newVarNode, 1 * sizeof(struct Node));

        varCount++;
        varList = realloc(varList, (varCount+1) * sizeof(struct Node));
    }

    while(!feof(stdin)){

        if(readNumofNodes == true){
            fgets(readLine, 100, stdin);
            strcpy(cpyReadLine, readLine);

            char *trimReadLine = strtok(cpyReadLine," \t\r\n\v\f\r");
            strcpy(cpyReadLine, trimReadLine);

            for(int i = 0; i < strlen(cpyReadLine); i++){
                if(isdigit(cpyReadLine[i]) == 0){
                    fprintf(stderr, "Enter a valid number for Number of Nodes\n");
                    exit(-1);
                }
            }

            nodePtrNum = atoi(cpyReadLine);

            for(int i = 0; i < nodePtrNum; i++){
                struct Node *newNodePtr = (struct Node*)malloc(1 * sizeof(struct Node));

                newNodePtr->type = 'L';
                newNodePtr->nodeNum = i+1;
                newNodePtr->incomingPtr = 0;
                newNodePtr->pointTo = 0;

                nodeList[nodeCount] = (struct Node*)malloc(1 * sizeof(struct Node));
                memcpy(nodeList[nodeCount], newNodePtr, 1 * sizeof(struct Node));

                nodeCount++;
                nodeList = realloc(nodeList, (nodeCount+1) * sizeof(struct Node));
            }

            readNumofNodes = false;

        }else{
            bool readProcess = false;
            char variableInputFile = ' ';
            int source = 0;
            int dest = 0;

            fgets(readLine, 100, stdin);
            strcpy(cpyReadLine, readLine);

            char *token;
            token = strtok(cpyReadLine, " \t\r\n\v\f\r"); 

            while(token != NULL){
                if(!readProcess){
                    char cpyToken[10];
                    strcpy(cpyToken, token);

                    if(strlen(cpyToken) != 1){
                        fprintf(stderr, "Invalid variable!\n");
                        exit(-1);
                    }else{
                        if(cpyToken[0] == 'V' || cpyToken[0] == 'L'){
                            variableInputFile = cpyToken[0];
                        }else{
                            fprintf(stderr, "Invalid variable!\n");
                            exit(-1);
                        }
                    }

                    readProcess = true;
                }else{
                    char cpyToken[10];
                    strcpy(cpyToken, token);

                    if(strstr(cpyToken, "->") == NULL){
                        fprintf(stderr, "Invalid map!\n");
                        exit(-1);
                    }else{
                        int countSrc = 0;
                        bool readDest = false;

                        char cpyToken[10];
                        strcpy(cpyToken, token);

                        if(cpyToken[0] == '-'){
                            fprintf(stderr, "Invalid map!\n");
                            exit(-1);
                        }

                        int len = strlen(cpyToken)-1;

                        for(int i = len; len >= 0; i--){
                            if(cpyToken[i] == '>'){
                                break;
                            }

                            if(cpyToken[i] == '-'){
                                fprintf(stderr, "Invalid map!\n");
                                exit(-1);
                            }
                        }

                        char *mapToken;
                        mapToken = strtok(cpyToken, "->");

                        while(mapToken != NULL){

                            if(readDest == false){
                                if(variableInputFile == 'V'){
                                    char cpyMapToken[10];
                                    strcpy(cpyMapToken, mapToken);

                                    if(strlen(cpyMapToken) != 1){
                                        fprintf(stderr, "Invalid map!\n");
                                        exit(-1);
                                    }else{
                                        if(isdigit(cpyMapToken[0]) != 0){
                                            source = atoi(cpyMapToken);

                                            if(source < 0 || source > 9){
                                                fprintf(stderr, "Invalid map!\n");
                                                exit(-1);
                                            }
                                            
                                        }else{
                                            fprintf(stderr, "Invalid map!\n");
                                            exit(-1);
                                        }
                                    }
                                }else{
                                    char cpyMapToken[10];
                                    strcpy(cpyMapToken, mapToken);

                                    for(int i = 0; i < strlen(cpyMapToken); i++){
                                        if(isdigit(cpyMapToken[i] == 0)){
                                            fprintf(stderr, "Invalid map!\n");
                                            exit(-1);
                                        }
                                    }

                                    source = atoi(cpyMapToken);

                                    if(source == 0 || source > nodePtrNum){
                                        fprintf(stderr, "Invalid map!\n");
                                        exit(-1);
                                    }

                                }

                                readDest = true;
                            }else{
                                char cpyMapToken[10];
                                strcpy(cpyMapToken, mapToken);


                                for(int i = 0; i < strlen(cpyMapToken); i++){
                                    if(isdigit(cpyMapToken[i] == 0)){
                                        fprintf(stderr, "Invalid map!\n");
                                        exit(-1);
                                    }
                                }

                                dest = atoi(cpyMapToken);

                                if(dest > nodePtrNum){
                                    fprintf(stderr, "Invalid map!\n");
                                    exit(-1);
                                }

                            }

                            countSrc++;

                            if(countSrc > 2){
                                fprintf(stderr, "Invalid map!\n");
                                exit(-1);
                            }

                            mapToken = strtok(NULL, "->");
                        }

                        makeConnection(variableInputFile, source, dest);

                    }
                }

                token = strtok(NULL, " \t\r\n\v\f\r");
            }


        }
        
    }

    for(int i = 0; i < nodePtrNum; i++){
        struct Node *currNode = nodeList[i];

        if(currNode->incomingPtr != 0){
            printf("Node %d has %d incoming pointers\n", currNode->nodeNum, currNode->incomingPtr);
        }
    }

    return 1;
}