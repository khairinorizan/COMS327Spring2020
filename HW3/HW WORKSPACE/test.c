#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(){

    char str[50] = "yo what you doing";
    char* strpointer = str;
    //strcpy(strpointer, str);
    char* token = strtok(strpointer, " ");
    int count = 0;
    while(token != NULL){

        if(count != 2){
            printf("%s\n", token);
        }
        count++;
        token = strtok(NULL, " ");
    }

    //printf("%s\n", token[0]);



    // unsigned int xlowVal = 0; //upper left corner coordinate
    // unsigned int ylowVal = 0; //upper left corner coordinate
    // unsigned int xhighVal = 0; //bottom right corner coordinate
    // unsigned int yhighVal = 0; //bottom right corner coordinate
    // char wchVal = ' '; //character to be filled on the window

    // for(int i=0; i<5; i++){
    //     scanf("%u,%u %u,%u %c", &xlowVal,&ylowVal,&xhighVal,&yhighVal, &wchVal);
    //     printf("%u,%u %u,%u %c\n", xlowVal,ylowVal,xhighVal,yhighVal, wchVal);        
    // }
    
    // char str;
    // char output[50];
    // char output2[10];
    // char output3[10];
    // char output4[10];
    // unsigned int o1 = 0;
    // unsigned int o2 = 0;
    // unsigned int o3 = 0;
    // unsigned int o4 = 0;

    // scanf("%u,%u %u,%u", &o1,&o2,&o3,&o4);

    // printf("%u,%u %u,%u\n", o1,o2,o3,o4);

    // scanf("%u,%u %u,%u", &o1,&o2,&o3,&o4);

    // printf("%u,%u %u,%u\n", o1,o2,o3,o4);
    //char str[50];
    // int c = 0;
    // int line = 0;

    // str = getc(stdin);

//    while(line < 2){
//         scanf("%s,%s %s,%s", &output,&output2,&output3,&output4);

//         printf("%s,%s %s,%s\n", output,output2,output3,output4);

//         line++;
//    }

    // while(output != EOF){
    //     scanf("%c", &output);

    //     printf("%c", output);
    // }

//    while(str != EOF){
//        scanf("%c", &output);

//         printf("%s", output);
//         char* strpointer = &str;
//         strcpy(strpointer, str);
//         char* token = strtok(strpointer, ", ");

//         while(token != NULL){

//             printf("%s\n", token);
//             token = strtok(NULL, ", ");
//         }
//        putc(str,stdout);
//        str = getc(stdin);
//    }
    // while (!feof(stdin))
    // {
    //     // if(!feof(stdin)){
    //     //     break;
    //     // }
    //     /* code */
    //     str = getc(stdin);
    //     putc(str, stdout);
    // }
    
    //printf("\n");
    // int val = 0;
    // char str[20];
    // strcpy(str, "yooo");
    // puts(str);

    // memset(str, 0, sizeof(str));
    // puts(str);
    // val = atoi(str);

    // printf("Val value : %d\n", val);



    // int i = -62;
    // unsigned int j;
    // j = abs(i);

    // printf("%u\n", j);
    //printf("%d", i);


    // char str[] = "5";
    

    // char* token = strtok(str, "");

    // while(token != NULL){

    //     printf("%s\n", token);
    //     token = strtok(NULL, "");
    // }
    return 0;
}