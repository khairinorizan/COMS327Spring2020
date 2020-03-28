Implemented Features

README file, that describes implemented features

DEVELOPERS file, that gives an overview of your implementation, including a breakdown of source files and functions, and who authored each function.
Working Makefile. Typing make should build your check executable.

Reads from stdin if no filename argument
Reads from filename passed as argument
Normal output to stdout
Correctly formatted output on valid inputs

Reports correct number of covered cards on valid inputs
Reports correct number of stock cards on valid inputs
Reports correct number of waste cards on valid inputs
No error messages on valid inputs

Error messages to stderr

Formatting error messages include line numbers
Appropriate error messages for formatting errors
Check that the tableau piles are legal
Check for duplicate cards
Check for missing cards (counts number of cards and checks if the total is 52, if less, cards are missing)

HOW THE PROGRAM WORKS;
1. solitaire.c will take in a file from the command line if it exists, otherwise takes input from stdin.

2. solitaire.c then processes the file, checking for any formatting errors and extracting certain necessary information. 
   It outputs to stderr and stdout respectively.

3. Once the file has been taken in from file or stdin, it is passed into a function called proc()

4. proc searches through the file using helper methods like keywordcheck to determine whether or not the input file has proper syntax. 
   Any cards found along the way are recorded into a 2d array representing the deck.

5. Proc also handles the error checking for missing cards, duplicate cards, and tableau piles by using the global tableau and deck 2d 
   arrays.

6. Keywordcheck parses through the input file one character at a time using fgetc, until it hits a newline or a ' ' character, and puts
   that string into a global char array called globalword. Proc then uses globalword to determine if the input is correct syntactically. 
   Keywordcheck also ignores comments by checking for the '#' symbol and ignoring everything until it hits a newline char.

7. Checkfrank, checkrank, and checksuit are functions used to make sure that a character is of the correct set. Suit must be one of
   char suit[] = { 'c', 'd', 'h', 's' }; for example

8. Tableaucheck functions like keywordcheck, except it also pipes the card strings it finds into a 2d array representing the tableau. 
   Additionally, it records the amount of cards in the cover. It will end when it finds the end of the file or 'STOCK:'. It also 
   records all cards found into the deck 2d array.

9. Stockcheck is like tableaucheck except it terminates on EOF or 'MOVES'. It parses for cards and the | symbol, and increments a 
   counter for waste and stock.