//
//  Header.h
//  HangmanLemanHard
//
//  Created by Zleman on 10/1/13.
//  Copyright (c) 2013 Zleman. All rights reserved.
//


#ifndef FinalHangmanLeman_Header_h
#define FinalHangmanLeman_Header_h
#define DEBUG
#define lettersInAlphabet 26
#define minimumGuesses 1
#define numberOfWordLengths 30


/*Declares struct type node which stores guessed letters*/
struct node {
    char letter;
    struct node* next;
};

/*Declares struct type WordNode. Initial list of words*/
struct wordNode {
    char *word;
    struct wordNode* next;
};

/*declares struct type family.
*Members is list of words.
*Number is the number of members.
*Description is the word family description.
*Next is the next list in our list.*/
struct family {
    struct wordNode* members;
    int number;
    char *description;
    struct family *next;
};

struct family *currentFamily;// creates global currentfamily pointer to current list of families
struct node *head; // creates global head pointer to list of guessed words
struct wordNode *lists[numberOfWordLengths]; // creates array of global head pointers
struct wordNode *currentWordList; //Allows the list of words the computer can choose from to be refined and passed on
char *currentDescription; // Holds the current description of the hidden word that is displayed to the user
int listsAux[numberOfWordLengths];  //Contains number of words per initial list. i.e How many words of that length were read into a list.

void flushStdin();
void createLists();
int  numberOfLetters();
int  numberOfguesses();
void insertWordInListAtArrayPosition(struct wordNode **headRef, char *data);
void insertInList(struct node** headRef, char data);
void printList(struct node* headRef);
char testIfAleadyGuessed(struct node* headRef, char test);
char obtainGuess();
int  checkIfFound(char guess,int numberOfLetter);
void currentStatus();
void userLoop();
int  checkWin(int numberOfLetters);
void generateDescriptions(struct wordNode** headRef,int number, char letter);
void insertInListOfFamilies(char *wordDescription);
void insertInFamily(struct wordNode** headRef, char *word);
int  searchFamilyList(char *wordDescription, int numberLetters, char *word);
void getNewList(int difficulty);
int difficulty();
#endif
