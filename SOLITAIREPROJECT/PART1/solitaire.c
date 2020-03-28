//
//  solitaire_1.c
//
//
//  Created by Dan Savage on 2/6/20.
//	Modified by Khairi on 2/10/2020
//

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define tableauColumns 7

//Function prototype
void proc(FILE *inf);
void keywordcheck();
void tableaucheck();
void stockcheck();
bool uncoverOrderCheck(char* arr);
bool suitCheck(char c1, char c2);
//END OF FUNCTION PROTOTYPE

char globalword[100];

FILE *inf;
int linecount;

unsigned covered, stock, limit;
int waste = 0;
bool checkfrank(char c);
bool checkrank(char c);
bool checksuit(char c);

char frank[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q',
		'K', '_' };
char rank[] =
		{ 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
char suit[] = { 'c', 'd', 'h', 's' };
char deck[52][3]; //can store all cards represented by rank and suit
int deckcount = 0;
char tableau[7][100];
char cover[7][100];
char uncover[7][100];


int main(int argc, char *argv[]) {

	//if there is no argument, use stdin.
	if (argc == 1) {
		proc(stdin);
	}
	//if there is one argument, take in the file.
	else if (argc == 2) {
		inf = fopen(argv[1], "r");
		if (inf == NULL) {
			fprintf(stderr, "Error: Couldn't read file\n");
			return 0;
		}
		proc(inf);
		fclose(inf);
	} else {
		fprintf(stderr, "ERROR: Please use only one file for input.\n");
		return 0;
	}
}

//MODIFIED AND FIXED BY KHAIRI
void proc(FILE *infile) {
	inf = infile;
	int deckincrement = 0;

	keywordcheck();
	if (strcmp("RULES:", globalword) == 0) {
		keywordcheck();
		if (strcmp("turn", globalword) == 0) {
			keywordcheck();
			if (strcmp("1", globalword) != 0 && strcmp("3", globalword) != 0) {
				fprintf(stderr, "Error near line %d: expecting ’1' or '3'.\n", linecount);
			} else {
				keywordcheck();
				if (strcmp("unlimited", globalword) == 0) {
					
				} else if (strcmp("limit", globalword) == 0) {
					keywordcheck();
					if (!isdigit(globalword[0])) {
						fprintf(stderr,
								"Error near line %d: expecting a digit from 0-9.\n",
								linecount);
					} else {
						printf("limit: %c", globalword[0]);
						limit = globalword[0]; //record limit for future use
					}
				} else {
					fprintf(stderr,
							"Error near line %d: expecting 'unlimited' or 'limit N'.\n",
							linecount);
				}
			}
		} else {
			fprintf(stderr, "Error near line %d: expecting 'turn'.\n", linecount);
		}
	} else {
		fprintf(stderr, "Error near line %d: expecting 'RULES:'.\n", linecount);
	}

	keywordcheck();

	if (strcmp("FOUNDATIONS:", globalword) == 0) {
		
		for (int i = 0; i < 4; i++) {
			keywordcheck();
			
			if (strlen(globalword) != 2) {
				fprintf(stderr, "Error near line %d: card must be 2 characters\n",
						linecount);
			} else {
				//fixed by KHAIRI
				if (checkfrank(globalword[0]) == false) {
					fprintf(stderr, "Error near line %d: invalid rank.\n", linecount);
				} else if (checksuit(globalword[1]) == false) {

					fprintf(stderr, "Error near line %d: invalid suit.\n",
							linecount);
							
					if (globalword[1] == 'c' && i != 0) {
						fprintf(stderr, "Error near line %d: expected suit c.\n",
								linecount);
					}
					if (globalword[1] == 'd' && i != 1) {
						fprintf(stderr, "Error near line %d: expected suit d.\n",
								linecount);
					}
					if (globalword[1] == 'h' && i != 2) {
						fprintf(stderr, "Error near line %d: expected suit h.\n",
								linecount);
					}
					if (globalword[1] == 's' && i != 3) {
						fprintf(stderr, "Error near line %d: expected suit s.\n",
								linecount);
					}
					
				//add cards to deck
				}else if(globalword[0] != '_'){

					if(globalword[0] == 'A')
					{
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						strcpy(deck[deckcount], globalword);
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

					}
					else if(isdigit(globalword[0]))
					{
						char number[2];
						number[0] = globalword[0];
						number[1] = '\0';

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						for(int i = 2; i <= atoi(number); i++)
						{
							if(deckcount >= 52)
							{
								fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
							}
							strcpy(deck[deckcount], globalword);
							deck[deckcount][2] = '\0';
							deckcount++;
						}
									
						deck[deckcount][0] = 'A';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}
					}
					else if(globalword[0] == 'T')
					{
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'T';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						for(int i = 2; i <= 9; i++)
						{
							if(deckcount >= 52)
							{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
							}
							deck[deckcount][0] = i + '0';
							deck[deckcount][1] = globalword[1];
							deck[deckcount][2] = '\0';
							deckcount++;
						}

						deck[deckcount][0] = 'A';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}
					}
					else if(globalword[0] == 'J')
					{
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'J';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'T';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						for(int i = 2; i <= 9; i++)
						{
							if(deckcount >= 52)
							{
								fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
							}
							deck[deckcount][0] = i + '0';
							deck[deckcount][1] = globalword[1];
							deck[deckcount][2] = '\0';
							deckcount++;
						}

						deck[deckcount][0] = 'A';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;
							
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

					}
					else if(globalword[0] == 'Q')
					{
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}	
						deck[deckcount][0] = 'Q';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;
									
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'J';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'T';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						for(int i = 2; i <= 9; i++)
						{
							if(deckcount >= 52)
							{
								fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
							}
							deck[deckcount][0] = i + '0';
							deck[deckcount][1] = globalword[1];
							deck[deckcount][2] = '\0';
							deckcount++;
						}

						deck[deckcount][0] = 'A';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

					}
					else if(globalword[0] == 'K')
					{
						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'K';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'Q';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'J';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						deck[deckcount][0] = 'T';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}

						for(int i = 2; i <= 9; i++)
						{
							if(deckcount >= 52)
							{
								fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
							}
							deck[deckcount][0] = i + '0';
							deck[deckcount][1] = globalword[1];
							deck[deckcount][2] = '\0';
							deckcount++;
						}

						deck[deckcount][0] = 'A';
						deck[deckcount][1] = globalword[1];
						deck[deckcount][2] = '\0';
						deckcount++;

						if(deckcount >= 52)
						{
							fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
						}	
					}
				} else {
					continue;
				}
				
			}
		}
		
	} else {
		fprintf(stderr, "Error near line %d: expected 'FOUNDATIONS:'\n", linecount);
	}

	keywordcheck();
	if (strcmp("TABLEAU:", globalword) == 0) {

		tableaucheck();

		//Khairi adds
		//seperates the uncover and cover
    	for(int i = 1; i <= 7; i++){
			char *token;

        	if(strcmp(tableau[i], "|") == 0){
            	strcpy(cover[i], "");
            	strcpy(uncover[i], "");
            
        	}else if(tableau[i][0] == '|'){

            	strcpy(cover[i], "");
            	token = strtok(tableau[i], "|");
            	strcpy(uncover[i], token);            

        	}else{
            	token = strtok(tableau[i], "|");
        
            	strcpy(cover[i], token);

            	token = strtok(NULL, "|");

            	strcpy(uncover[i], token);	

        	}

				
		}

		bool trytest = true;
		char uncoverWithSpaces[7][100];
		int charCount = 1;
		int tracker = 0;
		char space = ' ';

		for(int i = 1; i <= 7; i++){
			for(int j = 0; j < 20; j++){
				if(charCount != 3 && charCount < 3){
					uncoverWithSpaces[i][tracker] = uncover[i][j];
				}else if(charCount == 3){
					uncoverWithSpaces[i][tracker] = ' ';
					charCount = 0;
					j--;
				}else if(j > strlen(uncover[i])){
					uncoverWithSpaces[i][tracker] = '\0';
					break;
				}
				charCount++;
				tracker++;
			}
			tracker = 0;
			charCount = 1;
		}


    	for(int i = 1; i <= 7; i++){
			char* uncoverAtIndex = calloc(100, sizeof(1));
			strcpy(uncoverAtIndex, uncoverWithSpaces[i]);

			trytest = uncoverOrderCheck(uncoverAtIndex);

			if(trytest == false){
				fprintf(stderr, "Invalid tableau pile order at %d.\n", i);
			}
		}

	} else {
		fprintf(stderr, "Error near line %d expected 'TABLEAU:'\n", linecount);
	}
	
	stockcheck();

	//movecheck();//not to be implemented yet

	//additional errorchecks
	if(deckcount != 52)
	{
		printf("Deck contains %d cards\n", deckcount);
		fprintf(stderr, "Deck does not contain 52 cards.\n");
	}
	
	//check duplicates in deck (KHAIRI)
	for(int i = 0; i < deckcount; i++){
		int count = 0;

		while(count < deckcount){
			if(i == count){
				count++;
				continue;
			}

			if(deck[i] == deck[count]){
				fprintf(stderr, "Deck contains duplicate card %s.\n", deck[i]);
			}
			count++;
		}
		
	}

	//print the output to stdout
	fprintf(stdout, "input file is valid\n");
	fprintf(stdout, "%u covered cards\n",covered);
	fprintf(stdout, "%u stock cards\n",stock);
	fprintf(stdout, "%u waste cards\n",waste);
	return;
}

//K creates function
bool uncoverOrderCheck(char* arr){

	char *token;	
	int index = 0;
	char uncoveredCards[index][3];
	char uncoveredArr[100];

	strcpy(uncoveredArr, arr);

	token = strtok(arr, " ");

	while(token != NULL){
		strcpy(uncoveredCards[index], token);	
		token = strtok(NULL, " ");
		index++;

	}

	int j = 1;

	for(int i=0; i < index-1; i++){
		if(uncoveredCards[i][0] == 'K'){
			if(uncoveredCards[j][0] == 'K'){
				if(!suitCheck(uncoveredCards[i][1],uncoveredCards[j][1])){
					return false;
				}
			}

		}else if(uncoveredCards[i][0] == 'Q'){
			if(uncoveredCards[j][0] != 'Q'){
				if(isdigit(uncoveredCards[j][0]) == 0){
					if(uncoveredCards[j][0] == 'K'){
						return false;
					}
				}

			}else if(uncoveredCards[j][0] == 'Q'){
				if(suitCheck(uncoveredCards[i][1],uncoveredCards[j][1]) == false){
					return false;
				}

			}

		}else if(uncoveredCards[i][0] == 'J'){
			if(uncoveredCards[j][0] != 'J' && isdigit(uncoveredCards[j][0]) == 0){
				if(uncoveredCards[j][0] == 'Q' || uncoveredCards[j][0] == 'K'){
					return false;
				}

			}else if(uncoveredCards[j][0] == 'J'){
				if(!suitCheck(uncoveredCards[i][1],uncoveredCards[j][1])){
					return false;
				}
			}

		}else if(uncoveredCards[i][0] == 'T'){
			if(uncoveredCards[j][0] != 'T' && isdigit(uncoveredCards[j][0]) == 0){
				return false;

			}else if(uncoveredCards[j][0] != 'T'){
				if(!suitCheck(uncoveredCards[i][1],uncoveredCards[j][1])){
					return false;
				}

			}

		}else{ //rank 1-9
			if(isdigit(uncoveredCards[j][0]) == 0){
				return false;
			}else if(uncoveredCards[i][0] < uncoveredCards[j][0]){
				return false;
			}else if(uncoveredCards[i][0] == uncoveredCards[j][0]){
				if(!suitCheck(uncoveredCards[i][1],uncoveredCards[j][1])){
					return false;
				}
			}
		}
		j++;
	}

	return true;

}

bool suitCheck(char c1, char c2){
	if(c1 == 's'){
		return true;

	}else if(c1 == 'h'){
		if(c2 == 's'){
			return false;
		}
		
		return true;

	}else if(c1 == 'c'){
		if(c2 == 's' || c2 == 'h'){
			return false;
		}

		return true;

	}else{ // c1 == 'd'
		if(c2 == 's' || c2 == 'h' || c2 == 'd'){
			return false;
		}

		return true;

	}
}

void keywordcheck() {
	char c;
	int index = 0;
	

	while (!feof(inf)) {
		c = (char) fgetc(inf);
		if (c == '\n') {
			linecount++;
		} //increment linecount
		if (c == '#') //skip comments
				{
			do {
				c = (char) fgetc(inf);
				if (c == '\n') {
					linecount++; //increment linecount
					break;
				} else {
					continue;
				}
			} while (!feof(inf));
		}
		if (c == ' ' || c == '\n' || c == '\0' || c == '\t' || c ==  '\r') {
			if (index != 0) {
				globalword[index + 1] = '\0';
				return;
			}
		} else {
			globalword[index] = c;
			globalword[index + 1] = '\0';
			index++;
		}
	}
	fprintf(stderr, "Reached end of file without finding a valid word.\n");
	return;
}

//checks for valid rank in the case of foundation.
bool checkfrank(char c) {
	bool b = false;
	for (int i = 0; i < 14; i++) {
		if (c == frank[i])
			b = true;
	}
	return b;
}

bool checkrank(char c) {
	bool b = false;
	for (int i = 0; i < 13; i++) {
		if (c == rank[i])
			b = true;
	}
	return b;
}

bool checksuit(char c) {
	bool b = false;
	for (int i = 0; i < 4; i++) {
		if (c == suit[i])
			b = true;
	}
	return b;
}

//MODIFIED AND FIXED BY KHAIRI
void tableaucheck() {
	char c;
	int index = 0;
	bool iscover = true;
	bool stockcheck = false;
	int columncount = 7;
	int rowcount = 0;
	bool initcommentblock = true;
	while (!feof(inf)) {
		c = (char) fgetc(inf);

		if (c == '#') //skip comments
		{
			do {
				c = (char) fgetc(inf);
				if (c == '\n') {
					linecount++; //increment linecount

					rowcount = 0;
					break;
				} else {continue;}

			} while (!feof(inf));
		}
		if (c == ' ' || c == '\n' || c == '\0' || c == '\t' || c ==  '\r') {
			if (c == '\n') {
				linecount++;

				if(!initcommentblock){
					columncount--;	
					initcommentblock = true;
				}
				
				rowcount = 0;
				iscover = true;
			} //increment linecount and decrement columncount
		
			if(strcmp("STOCK:", globalword) == 0)
			{
				return;
			}

			if (index != 0 && iscover == true && index <= 2) {

				if (strcmp("|", globalword) == 0) {
					iscover = false;
					tableau[columncount][rowcount] = globalword[0];
					rowcount++;
				}else if(!checkrank(globalword[0]) && index > 1) {
					fprintf(stderr, "Error near line %d: invalid rank.\n", linecount);	
				}else if(!checksuit(globalword[1]) && index > 1) {
					fprintf(stderr, "Error near line %d: invalid suit.\n", linecount);	
				}else{
					tableau[columncount][rowcount] = globalword[0];
					rowcount++;
					tableau[columncount][rowcount] = globalword[1];
					rowcount++;

					strcpy(deck[deckcount], globalword);
					deck[deckcount][2] = '\0';
					deckcount++;
					covered++;
				}
				
				if(deckcount >= 52)
				{
					fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
				}
			
			} else if (index != 0 && iscover == false && index <= 2) {
				if(!checkrank(globalword[0])) {	fprintf(stderr, "Error near line %d: invalid rank.\n", linecount);	}
				if(!checksuit(globalword[1])) {	fprintf(stderr, "Error near line %d: invalid suit.\n", linecount);	}
				tableau[columncount][rowcount] = globalword[0];
				rowcount++;
				tableau[columncount][rowcount] = globalword[1];
				rowcount++;
				if(deckcount >= 52)
				{
					fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
				}

				strcpy(deck[deckcount], globalword);
				deck[deckcount][2] = '\0';
				deckcount++;

			} else if (index > 2) {
				fprintf(stderr,
						"Error near line %d: string length detected > 2.\n", linecount);
			}
			index = 0;
		} else {
			globalword[index] = c;
			globalword[index + 1] = '\0';
			initcommentblock = false;
			index++;
		}
	}
	if(iscover)	{	fprintf(stderr, "Error near line %d: Never found | symbol.\n",linecount);	}
	if(!stockcheck) {	fprintf(stderr, "Error near line %d Expected 'STOCK:'.\n", linecount);	}
	return;
}


void stockcheck()
{
	char c;
	int index = 0;
	bool iswaste = true;
	int try = 0;
	waste = 0;
	while (!feof(inf)) {
		c = (char) fgetc(inf);
		if (c == '#') //skip comments
		{
			do {
				c = (char) fgetc(inf);
				if (c == '\n') {
					linecount++; //increment linecount
					break;
				} else {continue;}

			} while (!feof(inf));
		}
		if (c == ' ' || c == '\n' || c == '\0' || c == '\t' || c ==  '\r') {
			if (c == '\n') {
				linecount++;
			} //increment linecount and decrement

			if(strcmp("MOVES:", globalword) == 0)
			{
				return;
			}

			if (index != 0 && iswaste == true && index <= 2) {
				if (strcmp("|", globalword) == 0) {
					iswaste = false;
				}else if(!checkrank(globalword[0]) && index > 1) {
					fprintf(stderr, "Error near line %d: invalid rank.\n", linecount);	
				}else if(!checksuit(globalword[1]) && index > 1) {
					fprintf(stderr, "Error near line %d: invalid suit.\n", linecount);	

				}else{
					deck[deckcount][0] = globalword[0];
					deck[deckcount][1] = globalword[1];
					deckcount++;
					waste++;
				}

				if(deckcount >= 52)
				{
					fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
				}
				
			} else if (index != 0 && iswaste == false && index <= 2) {
				if(!checkrank(globalword[0])) {	fprintf(stderr, "Error near line %d: invalid rank.\n", linecount);	}
				if(!checksuit(globalword[1])) {	fprintf(stderr, "Error near line %d: invalid suit.\n", linecount);	}
				if(deckcount >= 52)
				{
					fprintf(stderr, "Attempting to add more than 52 cards to the deck around line %d.\n",linecount);
				}
				deck[deckcount][0] = globalword[0];
				deck[deckcount][1] = globalword[1];
				deckcount++;
				stock++;
			} else if (index > 2) {
				fprintf(stderr,
						"Error near line %d: string length detected more than 2.\n", linecount);
			} 
			index = 0;
		} else {
			globalword[index] = c;
			globalword[index + 1] = '\0';

			index++;
		}
	}
	if(iswaste)	{	fprintf(stderr, "Error near line %d: Never found | symbol.\n",linecount);	}
	fprintf(stderr, "Error near line %d: expected 'MOVES:'\n", linecount);
	return;
}
