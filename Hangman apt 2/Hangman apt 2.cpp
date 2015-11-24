// Hangman apt 2.cpp : Defines the entry point for the console application.
// Author: Silas Agnew

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdio.h> // redundancy is unknown
#include <stdlib.h> // redundancy is unknown

// Interesting unintended feature: If you type in multiple characters at once it enters each one as it would if all characters were entered separately
// NOTE: I know 'system("CLS")' is bad practice, but this isn't exactly an industry standard program is it; therefore, I'm going to use it anyway

std::string *tempWordList;  // temporary(short term) list of selectable words from the disctionary
std::string word; // the secret word
std::string alphabet = "abcdefghijklmnopqrstuvwxyz"; // alphabet for validation purposes
char availLetters[26];  // list of available letters
char *findword; // temporary(long term) list of chars that the user has found in the secret word in the appropriate index location
const char boardchar[6] = { 'O', '|', '/', '\\', '/', '\\' };  // hangman 'limbs'
char currentHangman[6]; // hangman limbs the user has displayed
char wrongchar[6];  // list of wrong chars the user has entered
char userChoice; // the users char input
bool validation; // if the char is valid or not
bool inWord = false; // if the char is in the secret word
bool userPlay; // if the user wants to keep playing or not
int length; // the length of the secret word
int limbCount(0); // amount of limb the user has displayed

void instructions() // outputs instructions on how to play the game to the user
{
	std::string instruct = "You must guess what a randomly selected word is by entering the letters in the \nword.  If you guess wrong you will receive a strike and a body part will be \nadded to the man. When the man is fully formed, you lose!";
	std::cout << instruct << "\n" << std::endl;
}

void error(int error_code) // outputs results to certain errors that may occur throughout the program
{
	if (error_code == 1) { // If the dictionary of words couldn't be opened
		std::cout << "ERROR: Could not find the dictionary file: 'Dictionary.txt'." << std::endl;
	}

	else if (error_code == 2) { // if not enough memory is available to create new variables
		std::cout << "ERROR: Not enough memory available to allocate." << std::endl;
	}

	else if (error_code == 3) { // If the user inputs an invalid input
		std::cout << "ERROR: Not a valid input. Try again!" << std::endl;
	}
}


// Done to initialize
void new_word_display(int length) // creats '_ _ _ _ , etc' for the word on display
{
	findword = new (std::nothrow) char[length];   //?? may add an extra slot(s) 
	
	if (findword == nullptr) { // error 2
		error(2);
	}

	for (int i = 0; i < length; ++i) { 
		findword[i] = '_'; // underscore for unfound char
	}
}

void new_word() 
/* opens the dictionary of words ("Dictionary.txt") and adds each word in the file to an array;
Then takes it and finds a random word via an RNG and establishes the length of the word and the word itself */
{
	std::string tempword;
	
	std::ifstream dict("Dictionary.txt"); // opens file
	if (dict.is_open())
	{
		int l = 0;
		tempWordList = new std::string[126]; // new array for eligable words

		if (tempWordList == nullptr) { // error handling
			error(2);
		}

		// REDO: maybe use counter 'l' to count how many words were extracted and have a varied amount of words in the dictionary and not just a static 126
		while (dict.good() && l < 126) // adds all words to the array
		{
			getline(dict, tempword);
			tempWordList[l] = tempword;
			l++;
		}
		dict.close(); // closes file
	}

	else
		error(1); // if file wasn't opened

	srand((unsigned int) time(NULL));
	
	int rn = rand() % 126;  // RNG 
	word = tempWordList[rn];

	delete[] tempWordList; // ends array

	length = word.length();  // gets length of selected word

	new_word_display(length); // creats '_ _ _ _ ...'
}



void new_hangman()  // creats a 'hangman' to display
{
	for (int i = 0; i < 6; i++) {
		currentHangman[i] = ' '; // empty [no] hangman
	}
}

void wrong_char() // list of entered chars that are incorrect
{
	for (int i = 0; i < 6; i++) {
		wrongchar[i] = ' '; // empty list 
	}
}

void avail_char() // Creats available letters array which starts as the complete alphabet
{
	int i(0);

	for (char a : alphabet) {
		availLetters[i] = a; // all alphabet
		i++;
	}

}

// calls new_word(); new_hangman(); wrong_char(); avail_char()
void new_display() // creates new game by calling the appropriate collection of functions
{
	// variables that need to be 'reset' after every game
	userPlay = true;
	limbCount = 0;

	new_word(); 
	new_hangman(); 
	wrong_char(); 
	avail_char();
}


void display() // displays the game board
{
	/*	
		page layout (1-4 correspond to positions in code for reference)

		Wrong letters:     (1)
		*letters that the user entered that are false*

			+---+          (2)
			O	|
	       /|\	|
		   / \	|		
				|
				|
			========

		_ _ _ _ _ *underscores that sum to the length of the word*  (3)

		Letters Available:         (4)
		*Letters that the user still hasn't tried*
	*/
	std::cout << "	Wrong Letters:" << std::endl;  //1
	std::cout << "	";
	for (int q = 0; q < 6; q++) {
		std::cout << wrongchar[q] << " ";    // wrong chars
	}
	std::cout << "\n" << std::endl;
	std::cout << "		 " << "+---+" << std::endl;      //2
	std::cout << "		 " << currentHangman[0] << "   |" << std::endl;
	std::cout << "		" << currentHangman[2] << currentHangman[1] << currentHangman[3] << "  |" << std::endl;
	std::cout << "		" << currentHangman[4] << " " << currentHangman[5] << "  |" << std::endl;
	std::cout << "		 " << "    |" << std::endl;
	std::cout << "		 " << "    |" << std::endl;
	std::cout << "		 " << "=========\n" << std::endl; //lines spaced
	std::cout << "	";                        // 3
	for (int l = 0; l < length; l++) {
		std::cout << findword[l];
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "\n  	Letters still available:" << std::endl;     //4 
	std::cout << "	";
	for (int p = 0; p < 26; p++) {
		std::cout << availLetters[p] << " ";   // available chars
	}
	std::cout << "\n\n" << std::endl;
}

// Done every turn 
bool validate_input(char letter) // checksif the input is an alphabetical character
{
	for (int i = 0; i < 26; i++) { // runs through each char in the alphabet 
		if (letter == alphabet[i]) { 
			return true;
		}
	}
	return false;
}

bool validate_use(char letter) // checks if the char has been used previously
{
	for (int i = 0; i < 26; i++) { // runs through available letters
		if (letter == availLetters[i]) {
			availLetters[i] = ' ';
			return true;
		}
	}
	return false;
}

char get_input() // asks the user for an input; checks validity
{
	do { // loops until a valid char is entered
		std::cout << "Enter a letter of your choosing:";
		std::cin >> userChoice;
		std::cout << std::endl;

		bool valid = validate_input(userChoice);

		// FIX: This could be represented by 1 set of 'if' statements
		// if it is a valid char
		if (valid) {
			validation = true;
		}

		else if (!valid) {  // calls an error of invalid input
			error(3); 
			validation = false;
		}

		bool valid2 = validate_use(userChoice);

		// if used already
		if (valid2 && validation) { // if previously valid and now valid
			validation = true;
		}

		else { 
			validation = false;
		}
	} while (!validation);

	return userChoice;
}

void update_display(char userChoice) // Updates the display every input
{
	inWord = false;

	for (int i = 0; i < length; i++) {    // runs through the chars in the string word; if none continue to next 'if'
		if (userChoice == word[i]) {	// if the users choice is in the word
			findword[i] = userChoice;	// replaces corresponding '_' with the entered char
			inWord = true;				// bypass next 'if' statement		
		}
	}
	if (!inWord) {											// if the users choice isn't in the word
		currentHangman[limbCount] = boardchar[limbCount];	// adds a limb to the hangman
		wrongchar[limbCount] = userChoice;					// adds the entered char to the wrong char list
		limbCount++;										// keeps track of the amount of limbs on the hangman
	}
}

char check_wl() // checks if the user won/lost/hasn't finished the game
/*
'n' = no winner or loser yet
'w' = win
'l' = lose
*/
{
	bool tempwin = true;
	int c_charCount(0);
	int lCount(0);

	// Not win or lose
	for (int i = 0; i < length; i++) { // if there are no blank spots
		if (findword[i] == '_') { 
			tempwin = false; // if there's unfound words left
		}
	}

	for (int i = 0; i < 6; i++) {  // if theres blanks left in wrong char list
		if (wrongchar[i] == ' ' && !tempwin) { 
			return 'n';
		}
	}

	// Win
	for (int i = 0; i < length; i++) {
		if (findword[i] == word[i]) { // if the char is in the word
			c_charCount++; // increments amount of correct chars
		}
		if (c_charCount == length) { // if the correct char count is equal to the length of the word
			return 'w';
		}
	}

	// Lose
	for (int i = 0; i < 6; i++) { 
		if (currentHangman[i] != ' ') {  // if the hangman has limbs, count them
			lCount++;
		}
		if (lCount == 6) { // if the hangman has all of his limbs
			return 'l';
		}
	}
	return 'n';
}

void gameover_win() { // Win result
	std::cout << "You got it right! The word was " << word << "!\n" << std::endl;
}

void gameover_lose() { // Lose result
	std::cout << "You are out of chances! The word was " << word << ". Try again next time!\n" << std::endl;
}

void decide_result(char result) // decide which result to call
{
	if (result == 'w') {
		gameover_win();
	}

	else if (result == 'l') {
		gameover_lose();
	}
}

// Update the game after every turn
char update_game() // calls the appropriate collection of functions
{
	char tempChoice = get_input(); // returns the users input
	update_display(tempChoice); // adds the results of the users input to the display
	system("CLS"); // clears the screens
	display(); // re-displays the new changes
	char status = check_wl(); // returns w/l status
	decide_result(status); // if status = 'w' or 'l'
	return status; // returns status to main 
}

int _tmain(int argc, _TCHAR* argv[])
{
	// temporary variable for user input
	char input;

	do {

		char userStatus;

		// 1 time display upon execution
		instructions(); 
		new_display();
		display();

		do { // loops until a winner or loser is determined
			userStatus = update_game();
		} while (userStatus == 'n');

		delete[] findword; // deletes the found chars of the secret word temporary variable

		while (true) {  // loops until user inputs if they want to continue playing or not
			std::cout << "Would you like to play again? [y/n]: ";
			std::cin >> input;
			std::cout << std::endl;

			if (input == 'y') { // if the user wants to play again; break to parent loop and repeat
				userPlay = true;
				break;
			}
			else if (input == 'n') { // if the user wants to exit breaks to parent loop and breaks from the parent loop to end at main
				userPlay = false;
				break;
			}
			else // if input isn't 'y' or 'n'
				continue;
		}

		system("CLS"); // clears the screens

	} while (userPlay); 

	return 0;
}

