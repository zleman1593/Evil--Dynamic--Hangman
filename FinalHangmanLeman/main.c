//
//  main.c
//  LemanHangmanHard
//
//  Created by Zackery Leman on 10/02/13.
//  Copyright (c) 2013 Zleman. All rights reserved.
// This is a version of hangman that allows the user to guess letters to uncover a hidden word of length that the user chooses. The user allots him or herself a total number of guesses to uncover the letter. The user also chooses how difficult that game will be. The program "changes" the hidden word to make the game as hard as possible by dynamically generating a new pool of possible words to fit the current pattern  of revealed letters. The game plays until the user guesses all the letters, or runs out of guesses. Note: Error occurs when 16 letters is chosen as word length.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Header.h"


int main(int argc, const char * argv[])
{
   
    userLoop();// initiates game
}


/* Calls relevant functions to play the game and allows user to guess letter after letter
 * and checks if the user has won or lost after each guess.*/
void userLoop(){
    createLists(); // This reads the dictionary and generates an array of all lists of all word lengths
    char guess;    // This is the variable where the current letter being guessed is kept
    int next=0;    // Prevents duplication of code on first pass.
    int won=0;     // Keeps track of whether the game has been won or lost
    int hard= difficulty();  //User chosen difficulty level
    int numberOfLetter=numberOfLetters();
    //number of wrong guesses left before game is over
    int guessNumber=numberOfGuesses();
    
    
    printf("To play, type in a single letter as a guess.\n");
    printf("You have %i guesses remaining.\n", guessNumber);
    // game plays while the user still has some  guesses left and has not won
    
    guess = obtainGuess();      //Gets first guess from user
    insertInList(&head, guess );//Puts guessed letter in list of guessed letters
                                //All word families of user defined length are created
    generateDescriptions(&lists[numberOfLetter],numberOfLetter,guess);
    currentStatus();            // The status of game is shown to the user
    getNewList(hard);               /*Finds the biggest members list from current
                                *families and makes it the new main list of possible words*/
    
    
   //This is the loop that allows the game to proceed. The first part of the loop is similar to above.
    while ((guessNumber)>0 && won==0) {
        if (next==1) {// prevents and initial duplication
            printf("You have %i guesses remaining.", guessNumber);
            // prints the status of the uncovered letters
            guess = obtainGuess();       //Gets first guess from user
            insertInList(&head, guess ); //Adds the guessed letter to the list containing all guessed letters
            currentFamily=NULL;          //Resets list of current families
            
            //All word families of user defined length are created
            generateDescriptions(&currentWordList,numberOfLetter,guess);
            //All word families of user defined length are created
            /*Finds the biggest members list from current
             *families and makes it the new main list*/
            getNewList(hard);
            currentStatus();
        }
        // Allows the first part of the loop to be run on all subsequent guesses.
        next=1;
        
        // Lets the use know if they uncovered a letter.
        if (checkIfFound(guess,numberOfLetter)) {
            printf("\n \n \n Wrong!\n");
        }
        else {
            printf("\n \n \n Correct.\n");
            
        }
        //  The number of  guesses remaining decreases after each guess
        guessNumber--;
        // Checks to see if the user has uncovered all letters
        if (checkWin(numberOfLetter)) {
            printf("You Win!!");
            won=1;
        }
        
    }
    printf("Game over. You uncovered:");
    //Shows final game state
    currentStatus();
    //Shows user a word that fits the currently visible letter pattern.
    printf("The word was: %s\n",currentFamily->members->word);
    
}


/* This reads the dictionary and builds the initial array of
 * lists for each word length. */
void createLists(){
    char mode1[] = "r";
    char file1[] = "dictionary.txt";
    FILE *fileptr1;
    fileptr1= fopen(file1, mode1);
    char buff[50];
    int position=0;
    if(!fileptr1){
        printf("Cannot open file");
    }
    while (fscanf(fileptr1, "%s", buff) != EOF){
        /*word is read and put in buffer
         *and then memory for the word is
         *allocated and the string is put there */
        char *newWord=malloc(strlen(buff));
        position =strlen(buff);
        strcpy (newWord,buff);
        // Add that string to your array of lists data structure.
        insertWordInListAtArrayPosition(&lists[position], newWord);
        //updates how many words are  of each length
        listsAux[position]= 1 + listsAux[position];
    }
}


/*Obtains user guess and verifies that it is valid.
 *Returns the valid character guessed*/
char obtainGuess(){
    /*While incorrect is 1 the function will keep asking
     * the user for input, until a valid guess is entered.*/
    char incorrect=1;
    //While the entered guess is invalid repeat loop
    while (incorrect) {
        printf("Your guess:");
        char letter = getchar();
        flushStdin();
        //verifies that guess is a letter
        if (isalpha(letter)) {
            incorrect=0;
            // Takes care of matching letters of different cases
            char lowerLetter=tolower(letter);
            if (testIfAleadyGuessed(head,letter)) {
                return lowerLetter;
            }
            else {
                printf("This character has already been guessed.\n");
                incorrect = 1;
            }
        } else {
            printf("This character is not a letter. Please input a letter.\n");
        }
    }
}

/* Determines if guessed letter has already been guessed.
 * Input is pointer to list and a char to test the list against.*/
char testIfAleadyGuessed(struct node* headRef, char test){
    // acts as a boolean switch variable
    int boolean=1;
    struct node* current = headRef;
    while (current != NULL) {
        if (current->letter == tolower(test)) {
            boolean=0;
            return boolean;
        }
        current = current->next;
    }
    if (boolean!=0) {
        return boolean;
    }
}


/*Prints the current status of the game to the user*/
void currentStatus(){
    printf("\n \n \n");
    if (currentDescription==NULL) {
        printf("The hidden letters of the word will be covered with 0's\n");
    } else{
        printf(" %s \n",currentDescription);
    }
    printf("\n Letters used: ");
    // prints the list of letters already used
    printList(head);
    printf("\n");
}


/* This prompts user to enter the number of guesses
 * they would like to allow themselves */
int numberOfGuesses(){
    int number = -1;
    while(1){
        number = -1;
        printf(" \n Please enter the number of guesses you would like to allow yourself:");
        do {
            scanf("%d", &number);
            //Checks that input was a number
            if (number<0) {
                printf("That is not a number.\n");
            }
            flushStdin();
            
        } while (number < 0);
        //Checks that input was in correct range.
        if (number<lettersInAlphabet && number>minimumGuesses) {
            printf("You will have %d guesses.\n", number);
            return number;
        }
        else {
            printf("That is not possible. Please pick a number greater than 1  and less than 26.\n");
        }
    }
}


/* This prompts user to enter the number of letters
 * in the word they would like to guess */
int numberOfLetters(){
    
    int number = -1;
    while(1){
        number = -1;
        printf(" \n Please enter the number of letters you would like in the word:");
        do {
            scanf("%d", &number);
            if (number<0) {
                printf("That is not a number.\n");
            }
            flushStdin();
            
        } while (number < 0);
        /*Restrict word length by making sure there is at least one word of that
        * length in the dictionary.*/
        if (!listsAux[number]<=0) {
            printf("You will have  to guess a word %d letters long.\n", number);
            return number;
        }
        else {
            printf("That is not possible. Please pick a number greater than 1  and less than 31.\n");
        }
    }
}

/* This prompts user to enter the difficulty
 * of the game they would like to play*/
int difficulty(){
    int number = -1;
    while(1){
        number = -1;
        printf(" \n Please enter 1 if you want an easy game, or 0 if you want a difficult game:");
        do {
            scanf("%d", &number);
            //Checks that input was a number
            if (number<0) {
                printf("That is not a number.\n");
            }
            flushStdin();
            
        } while (number<0);
        //Checks that input was in correct range.
        if (number!=0 && number!=1) {
            printf("That is not possible.\n");

        }
        else {
           return number;
        }
    }
}

/*  Iterates over the initial list of words for a given length and generates 
 *the description for each word. It takes a double pointer to the head of 
 *the list, the number of letters in the word, the letter guessed*/
void  generateDescriptions(struct wordNode** headRef,int number, char letter){
    struct wordNode* current = *headRef;
    char descript[number];
    //Terminates descript with null character
    descript[number]='\0';
    while (current != NULL) {
        for (int i=0;i<number;i++) {
            if (letter==tolower(current->word[i])) {
                descript[i]=tolower(current->word[i]);
            }
            else if (!testIfAleadyGuessed(head,tolower(current->word[i]))) {
                
                /* If current word has a letter that has already been guessed,
                *include that letter in updated description instead*/
                descript[i]=tolower(current->word[i]);
            }
            else {
                //This represents a covered letter
                descript[i]='0';
            }
        }
//#ifdef DEBUG
        //printf(" %s \n",descript);
//#endif
        
        /*Checks if word description exists and inserts current* 
         word into correct existing family if that is true.
         *If not, it builds a new family and inserts the word there*/
        searchFamilyList(descript, number, (current->word));
        current = current->next;
    }
}


/*Searches the list of families for existing family types.
 * Returns whether the family exists or not.
 Input is the description of the word, the number of letters of the words being
 * guessed, and the word being tested*/
int searchFamilyList(char *wordDescription, int numberLetters, char *word){
    int value=0;
    struct family* current = currentFamily;
    while (current != NULL) {
        value=0;
        for (int i=0;i<numberLetters;i++) {
            if (wordDescription[i]!=current->description[i]) {
                value++;
            }
        }
        if ( value==0){
            //Inserts word into existing family
            insertInFamily(&(current->members),word);
            current->number++;
            return 0;
        }
        current = current->next;
    }
    insertInListOfFamilies(wordDescription);
    //Inserts word into family just created. Becomes first word in that family.
    insertInFamily(&(currentFamily->members),word);
    //Keeps track of number of words in family
    currentFamily->number++;
    return 1;
    
}

/*Shows the list of guessed letters to user.
 *Input is pointer to list of guessed letters*/
void printList(struct node* headRef){
    struct node* current = headRef;
    while (current != NULL) {
        printf("%c ", current->letter);
        current = current->next;
    }
}

/* This inserts every word in the dictionary into the proper
 * list in the initial array of lists for each word length.
 *Input is double pointer to head of list for the words of that length,
 * and the word being added*/
void insertWordInListAtArrayPosition(struct wordNode **headRef, char *data){
    struct wordNode* newWord=malloc(sizeof(struct wordNode));
    newWord->word=data;
    newWord->next=*headRef;
    *headRef=newWord;
}

/*inserts guessed letter into the list.
 * Input is a double pointer to the list and a char as the guess*/
void insertInList(struct node** headRef, char data){
    struct node* newnode=malloc(sizeof(struct node));
    newnode->letter=data;
    newnode->next=*headRef;
    *headRef=newnode;
}


/*Inserts all possible family types into the list of families.
  *Input is a pointer to head of the list of all families*/
void insertInListOfFamilies(char *wordDescription){
    struct family* newFamily=malloc(sizeof(struct family));
    char *newDescription=malloc(strlen(wordDescription));
    strcpy (newDescription,wordDescription);
    newFamily->description=newDescription;
    newFamily->number=0;
    newFamily->next=currentFamily;
    newFamily->members=NULL;
    currentFamily=newFamily;
    
}

/*Inserts words of a given family into the members list
 *for a given family. 
 *Input is double pointer to head of list for the words of a family,
 * and the word being added to that family*/
void insertInFamily(struct wordNode** headRef, char *word){
    struct wordNode* newFamilymember=malloc(sizeof(struct wordNode));
    char *newWord=malloc(strlen(word));
    strcpy (newWord,word);
    newFamilymember->word=newWord;
    newFamilymember->next=*headRef;
    *headRef=newFamilymember;
}


/*Finds the biggest or samllest members list from current families
 *and makes it the new main list. Input is whether user wants an easy or hard game*/
void getNewList(int difficulty){
    struct family *current = currentFamily;
    int max=0;
    int min=10000;
    if (difficulty==1) {
        while (current != NULL) {
            if (min>current->number) {
                min=current->number;
                max=current->number;
                currentFamily=current;
            }
            current = current->next;
        }

    } else {
        while (current != NULL) {
            if (max<current->number) {
                max=current->number;
                currentFamily=current;
            }
            current = current->next;
        }
    }

#ifdef DEBUG
    printf("There are %d words left", max);
#endif
    /*global variable for the remaining pool of avalible words
    * is updated*/
    currentWordList=(currentFamily->members);
    //global variable for current description is updated
    currentDescription=currentFamily->description;
}



/* Checks if the guessed letter is in the description
 * and returns whether it is or not
 * Input is the guessed letter and the number of letters
 * in the hidden word */
int checkIfFound(char guess, int numberOfLetter){
    int value=0;
    for(int i = 0; i < numberOfLetter+1; i++)
    {
        if (tolower(guess)==tolower(currentDescription[i])) {
            value++;
        }
    }
    if ( value==0){
        return 1;}
    else{
        return 0;
    }
}


/* Checks if the user has won. Returns 0 or 1 to
 *signify yes or no that the user has won*/
int checkWin(int numberOfLetters){
    char character;
    for (int i=0;i<numberOfLetters+1;i++) {
        character=currentDescription[i];
        if (character=='0') {
            return 0; //returning false
        }
    }
    return 1; //returning true
}


/*  gets rid of any additional characters and \n after the typed letter*/
void flushStdin() {
    char c ;
    while (c!='\n'){
        c = getchar();
    }
}
