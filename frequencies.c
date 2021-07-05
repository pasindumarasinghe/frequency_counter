//preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//defining constants
#define WORDSIZE 300

//unicodes that are useful in printing the bar chart
#define DASH "\u2500"
#define L "\u2514"
#define PIPE "\u2502"
#define BLOCK "\u2591"


typedef struct _{//define a structure for words
	char text[WORDSIZE];
	int count;
	struct _ * next;
}word_t;


typedef struct __{//define a structure for alphanumeric characters
	char letter;
	int count;
	struct __ * next;
}char_t;


typedef struct ___{//define a structure for filenames
	char name[20];
	struct ___ * next;
} file_t;


void process(char *word);//this function removes all the non-alphanumeric characters from the given word
void chars(FILE * fp);//this fuction returns all the occurences of alphanumeric characters.
void words(FILE * fp);//this function returns all the occurences of words in the given text file
void csort();//this function sorts the linked list with alphaneumeric characters according to the number of occurences
void creverse(); // this function reverses the linked list with alphanumberic characters
void wsort();//this function sorts the linked list with words according to the number of occurences
void wreverse(); // this function reverses the linked list with words
void xaxis(int lenght);//prints out the x axis in the graph
int percentageLength(float percentage);//this function outputs the length of the percentage number(eg - 78.23% -> 6)
int maxLength(word_t * head, int l);//this function returns the length of the longest word(which is within the specified number of words)
void usage(char ** argv);//this function display the usage message
int isNumber(char * str);//this determines whether a string is a number or not
void flags(int argc, char ** argv);//this function handles the functionality of the program according to the given arguments.
void charPrint(char letter, float current_percentage, float max_percentage, int width);//this prints out the bar chart for characters
void filling(int length);//this prints the bar
void spaces(int length);//this function prints out spaces a specified number of times
void wordPrint(char * word, float current_percentage, float max_percentage, int width , int max_length);//this prints out the bar chart for word frequencies
void files(int argc, char ** argv);//this funtion handles all the files in the command line arguments

//declaring global variables
word_t * whead = NULL;//whead - word head
char_t * chead = NULL;//chead - char head
file_t * fhead = NULL;//fhead = file head

int bars = 10;//the number of bars that should be printed(default = 10)
int toggle;//the variable which decides whether to output the word frequencies or char frequencies
int scale;//the variable which decides whether to print a scaled chart or not
int c_count, w_count;//these are used to check whether user has entered both -w and -c arguments


int main(int argc , char ** argv){

	flags(argc, argv);//checking all the flags the user has entered

	if((c_count == 1) && (w_count == 1)){//when the user has entered both -w and -c flags the program outputs an error message
		printf("[-c] and [-w] cannot use together\n");
		usage(argv);
	}


	files(argc, argv);//putting all the valid files into a linked list

	FILE *fp;
	fp = fopen(fhead->name , "r");//opening the first file in the linked list

	if(fp == NULL){//when all the files are empty
		printf("No data to process\n");
		return 0;
	}


	file_t * ftemp = fhead;


	if(toggle  == 1){//when the user wants the alphanumeric character frequencies
		//setting up the head of the linked list
		chead = (char_t*)malloc(sizeof(char_t));//allocating memory to the head of the linked list
		char c;//c = character

		while(1){//defining the head of the linked list
			fscanf(fp,"%c",&c);
		
			if(!(isalpha(c)) && !(isdigit(c))){//skipping the non-alphanumeric characters.(a functiom -> isalphanumeric????)
				continue;
				}
			else{//defining the head of the linked list
				c = tolower(c);
				chead->letter = c;
				(chead->count)++;
				chead->next = NULL;
				break;
			}

		}

		
		while(ftemp!= NULL){//loop through all the files and find the character (non - alphanumeric) frequencies

			chars(fp);

			if(ftemp->next == NULL) break;

			fp = fopen(ftemp->next->name, "r");
			ftemp = ftemp->next;

		}

		//display the graph
		csort();//sorting the linked list.
		creverse();//reverse the sorted linked list without moving the characters with the same frequency

	
		char_t * temp_for_ctotal = chead;
		int total = 0;
		//get the total of the occurences of the alphanumeric characters
		while(temp_for_ctotal != NULL){
			total += temp_for_ctotal->count;
			temp_for_ctotal = temp_for_ctotal->next;
		}

		char_t * ctemp = chead;


		int width_for_chars = 80 - 4 - percentageLength((chead->count)*100.0/total);//calculating the max width of a bar in the bar chart


		//printing the linked list along with the char counts.
		printf("\n");//as in the sample program
		int step = 0;
		while(ctemp != NULL){
			if (step == bars) break; // handling the files which has less than 10 letters.
		
			if(scale == 1){//printing the scaled version of the bar chart
				charPrint(ctemp->letter,(ctemp->count)*100.0/total,(chead->count)*100.0/total , width_for_chars);
			}
			else{//printing the non-scaled version of the bar chart
				charPrint(ctemp->letter,(ctemp->count)*100.0/total,100.0 , width_for_chars);
			}
			ctemp = ctemp->next;
			step++;
		}

		printf("   ");//printing the required spaces before printing the x-axis
		xaxis(77);

	}


	else{//when the user wants to find out the word frequencies in the given files

		whead = (word_t*)malloc(sizeof(word_t));

		char word[150];

		//assigning the first word to the head of the linked list.
		fscanf(fp,"%s",word);
		process(word);//preprocess the head of the linked list
		strcpy(whead->text ,word);//assigning values to the head
		(whead->count)++;
		whead->next = NULL;


		while(ftemp!= NULL){//loop through all the files and find out the word frequencies

			words(fp);

			if(ftemp->next == NULL) break;

			fp = fopen(ftemp->next->name, "r");
			ftemp = ftemp->next;

		}

		//print out the chart for word frequencies
		wsort();//sorting the linked list
		wreverse();//reverse the linked list(words that have the same frequency does not get reversed)

		int max_length = maxLength(whead,bars);//determining the length of the longest word in the specified interval


		word_t * temp_for_wtotal = whead;
		int total = 0;
		//get the total of the occurences of the alphanumeric characters
		while(temp_for_wtotal != NULL){
			total += temp_for_wtotal->count;
			temp_for_wtotal = temp_for_wtotal->next;
		}
		
		word_t * wtemp = whead;

		int width_for_words = 80 - 3 - max_length - percentageLength((whead->count)*100.0/total);//calculating the max width of a bar in the bar chart
		
		printf("\n");//as in the sample program
		int step = 0;
		while(wtemp != NULL){
			if(step == bars) break; //handling file with words less than 10.
			
			if(scale == 1){//printing out the scaled version of the bar chart
				wordPrint(wtemp->text,(wtemp->count)*100.0/total,(whead->count)*100.0/total,width_for_words,max_length);
			}
			else{//printing out the nonscaled version of the bar chart
				wordPrint(wtemp->text,(wtemp->count)*100.0/total,100.0,width_for_words,max_length);
			}
			wtemp = wtemp->next;
			step++;
		}
		//printing out the x axis of the bar chart
		spaces(max_length+2);
		xaxis(78 - max_length);
		
	}
	
	
	fclose(fp);//closing the last opened file
	return 0;
}


void flags(int argc, char ** argv){//this funtion takes the command line arguments into consideration and act accordingly
	
	if(argc == 1){//the program needs at least one file to read.this checks whether there are command line arguments
		printf("No input files were given\n");
		usage(argv);
	}

	for(int i = 1 ; i < argc ; i++){//iterate through the arguments and checking them
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'l'){//when the argument is -l
				if(i+1 == argc){
					printf("Not enough options for [-l]\n");
					usage(argv);
				}
				if(isNumber(argv[i+1])){//if the argument after -l is a number
					bars = atoi(argv[i+1]);
					if (bars < 0){//when the number is a negative one
						printf("Invalid option(negative) for [-l]\n");//prints the error message
						usage(argv);

					}
					continue;
				}
				else{
					//printing the error message
					printf("Invalid options for [-l]\n");
					usage(argv);
				}
			}

			if(argv[i][1] == 'w'){//when the argument is -w

				w_count = 1;
				continue;
			}

			if(argv[i][1] == 'c'){//when the argument is -c
				
				toggle = 1;//here if the user wants the alphanumeric character frequencies the toggle turns on(1 - ON)
				c_count = 1;
				continue;

			}

			if( !strcmp(argv[i] , "--scaled") ){//checking whether the user wants the bar chart scaled or not
				scale = 1;
				continue;
			}

			else{//if an invalid flag is given, the program outputs an error message
				printf("Invalid option [%s]\n",argv[i]);
				usage(argv);
			}

		}
	}

}

void process(char * word){
	/*
	this function preprocesses a given word
	- non-alphanumeric characters are removed
	- every letter is transformed into a lowercase letter.
	*/

	int j = 0;
	char processed_word[100];//processed words are stored in a list

	for(int i = 0 ; i < strlen(word) ; i++){//removing the non-alphanumberic characters
		if(!(isalpha(word[i])) && !(isdigit(word[i]))){
			continue;
			}

		processed_word[j] = word[i];
		j++;
		
	}
	processed_word[j] = '\0';//insert the string terminator to the list

	for(int i = 0 ; i < strlen(processed_word) ; i++){//transforming all the uppercase letters to lowercase
		word[i] = tolower(processed_word[i]);
	}
	word[j] = '\0';//add the string terminator
}


void chars(FILE * fp){//this counts the alphanumeric character frequencies and stores them in a linked list
	

	char c;//c = character
	while(fscanf(fp,"%c",&c) != EOF){//reading the characters until the end of the file
		
		//preprocessing for chars
		if(!(isalpha(c)) && !(isdigit(c))){//skipping the non-alphanumeric characters
			continue;
			}

		c = tolower(c);

		//checking the existance of the character.
		char_t * current = chead;
		char_t * new = (char_t*)malloc(sizeof(char_t));

		while(current->next != NULL){//if the new read character is the same as the head
			if(current->letter == c){
				(current->count)++;
				break;
			}

			current = current->next;
		}
		if(current->next == NULL){
			if(current->letter == c){//when the last element of the linked list is the head.
				(current->count)++;
				continue;
			}

			//adding a non-existing non-alphanumeric character to the linked list
			current->next = new;
			new->letter = c;
			(new->count)++;
			new->next = NULL;
		}

	}

}


void words(FILE *fp){//this returns all the occurences of preprocessed words in a given text file
	

	char word[150];//define a variable to temperorily store the word that is read

	while(fscanf(fp,"%s",word) != EOF){
		
		process(word);//pre-process the scanned word

		//when there isn't anything left when the scanned word is preprocessed the programme neglects it.
		if(!strcmp(word,"")) continue;
		
		word_t * current = whead;
		word_t * new = (word_t*)malloc(sizeof(word_t));

		while(current->next != NULL){//if the scanned word has occured previously, the occurence is incremented by 1
			if(!strcmp(current->text,word)){
				(current->count)++;
				break;
			}
		
			current = current->next;
		}


		if(current->next == NULL){//checking the last node of the linked list
			if(!strcmp(current->text,word)){
				(current->count)++;
				continue;
			}

			//adding a new node to the linked list
			current->next = new;
			strcpy(new->text,word);
			(new->count)++;
			new->next = NULL;
		}

	}
	

}


void csort(){//this function sorts the linked list with alphanumeric characters.(bubble sort)
	char_t * i , *j;

	//declare variables to store the characters and counts when swapping
	char temp_letter;
	int temp_count;

	for(i = chead ; i != NULL ; i = i->next){
		for(j = chead ; j->next != NULL ; j = j->next){
			if((j->count) > (j->next->count)){//comparing the character counts
				//swap the two nodes.

				//storing values of one node in temporary variables
				temp_letter = j->letter;
				temp_count = j->count;

				//transfer values from one node to another
				j->count = j->next->count;
				j->letter = j->next->letter;

				//assign the values stored in the temporary variables to the other node
				j->next->count = temp_count;
				j->next->letter = temp_letter;
			}
		}
	}

}



void wsort(){//this function sorts the linked list with words(bubble sort)
	word_t * i , *j;

	//declare variables to store the characters and counts when swapping
	char temp_word[WORDSIZE];
	int temp_count;

	for(i = whead ; i != NULL ; i = i->next){
		for(j = whead ; j->next != NULL ; j = j->next){
			if((j->count) > (j->next->count)){//comparing the character counts
				//swap the two nodes.

				//storing values of one node in temporary variables
				strcpy(temp_word ,j->text);
				temp_count = j->count;

				//transfer values from one node to another
				j->count = j->next->count;
				strcpy(j->text,j->next->text);

				//assign the values stored in the temporary variables to the other node
				j->next->count = temp_count;
				strcpy(j->next->text, temp_word);
			}
		}
	}

}

void xaxis(int length){//this function prints out the x axis.
	printf(L);
	for(int i = 0 ; i < length-1 ; i++){
		printf(DASH);
	}
	printf("\n");
}

int maxLength(word_t * head , int l){//this function returns the length of the longest word(which is within the specified number of words)
	int max,count;
	word_t * temp = head;
	max = strlen(temp->text);
	temp = temp->next;

	for(count = 1 ; count < l ; count++){//here count starts with 1 because the head is already taken into consideration
		if(temp == NULL) break; //when there are words less than l in the file
		if(strlen(temp->text) > max){
			max = strlen(temp->text);
		}
		temp = temp->next;
	}

	return max;
}


int percentageLength(float percentage){//this function prints out the length(number of characters) of the percentage number
	if(percentage < 10.0) return 5;//ex -> 5.34%   <- 5 characters
	else if(percentage < 100.0) return 6;//ex -> 15.34%   <- 6 characters
	else return 7;//ex - 100.00% <---7 characters
}

void usage(char ** argv){//this prints out the usage error message
	printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
	exit(0);
}

int isNumber(char * str){//this checks whether a string is a number or not
	if(str[0] == '-'){//for negative numbers
		for(int i = 1 ; str[i] != '\0' ; i++){
			
			if( !isdigit(str[i]) ){
				return 0;//returns 0 if there isn't a number
			}
		}
		return 1;//returns 1 if it is a number
	}
	else{ 
		for(int i = 0 ; str[i] != '\0' ; i++){
			
			if( !isdigit(str[i]) ){
				return 0;//returns 0 if there isn't a number
			}
		}
		return 1;//returns 1 if it is a number
	}	
}

void filling(int length){
	for(int j = 0 ; j < length ; j++){//printing the block
				printf(BLOCK);
			}
}

void spaces(int length){//this function prints spaces
	for(int j = 0 ; j < length ; j++){//printing spaces
				printf(" ");
			}
}


void charPrint(char letter, float current_percentage, float max_percentage, int width){//this function prints out the character frequency bar chart
	int length = width*current_percentage/max_percentage;//calculating the length of the bar
	for(int i = 0 ; i < 4 ; i++){//printing the bar chart
		if( (i == 0) || (i == 2) ){
			spaces(3);
			printf(PIPE);
			filling(length);
			
		}

		else if(i == 1){
			printf(" %c ",letter);
			printf(PIPE);
			filling(length);
			printf("%.2f%%",current_percentage);
		}

		else{
			spaces(3);
			printf(PIPE);
		}

	printf("\n");
	}
}


void wordPrint(char * word, float current_percentage, float max_percentage, int width, int max_length){
	//this function prints out the bar chart for the word frequencies
	int length = width*current_percentage/max_percentage;//calculating the length of the bar
	for(int i = 0 ; i < 4 ; i++){//printing the bar chart
		if( (i == 0) || (i == 2) ){
			spaces(max_length+2);
			printf(PIPE);
			filling(length);
			
		}

		else if(i == 1){//printing out the middle line of a bar with the word and the percentage of the occurence
			printf(" %s ",word);
			spaces(max_length - strlen(word));
			printf(PIPE);
			filling(length);
			printf("%.2f%%",current_percentage);
		}

		else{//print the corresponding y-axis part
			spaces(max_length+2);
			printf(PIPE);
		}

	printf("\n");
	}
}



void files(int argc, char ** argv){//this function handles all the files
	/*all the valid files in the command line arguments are stored in a linked list*/

	FILE * fp;//creating a file pointer
	int i,k;//here k is used to get the integer value of the "val" variable
	char val;	

	for(i = 1 ; i < argc ; i++){//iterate through the arguments and find out the filenames
		if( (argv[i][0] != '-') && (strcmp(argv[i-1] , "-l")) ){
			fp = fopen(argv[i],"r");

			if(fp == NULL){//check whether the file exists or not.
				printf("Cannot open one or more given files\n");
				exit(0);
			}

			//Ignore the empty file.	
			fscanf(fp , "%c" , &val);
			k = val; //typecasting

			if(k == 0){//check whether the file is empty or not
				fclose(fp);
				continue;
			}

			//if the file is non-existing non-empty file, it is stored in a linked list
			if(fhead == NULL){//assigning data to the head of the linked list
				fhead = (file_t*)malloc(sizeof(file_t));
				strcpy(fhead->name,argv[i]);
				fhead->next = NULL;
			}
			else{
				file_t * current = fhead;
				file_t * new = (file_t*)malloc(sizeof(file_t));

				while(current != NULL){
					if(!strcmp(current->name,argv[i])){//check whether the file already exists or not
						current = current->next;
						break; //if the name already exists
					}
						
					if(current->next == NULL){
						current->next = new;
						strcpy(new->name , argv[i]);
						new->next = NULL;
					}
					current = current->next;

				}

			}

		}

	}

}

void wreverse(){//this function reverses t linked list(but the elements with the same occurence are not moved)
    	word_t * i , *j;

	//declare variables to store the characters and counts when swapping
	char temp_word[WORDSIZE];
	int temp_count;

	for(i = whead ; i != NULL ; i = i->next){
		for(j = whead ; j->next != NULL ; j = j->next){
			if((j->count) < (j->next->count)){//comparing the character counts
				//swap the two nodes.

				//storing values of one node in temporary variables
				strcpy(temp_word ,j->text);
				temp_count = j->count;

				//transfer values from one node to another
				j->count = j->next->count;
				strcpy(j->text,j->next->text);

				//assign the values stored in the temporary variables to the other node
				j->next->count = temp_count;
				strcpy(j->next->text, temp_word);
			}
		}
	}


    }


void creverse(){//this function reverses a linked list
    char_t * i , *j;
	//char_t * temp = (char_t*)malloc(sizeof(char_t));

	//declare variables to store the characters and counts when swapping
	char temp_letter;
	int temp_count;

	for(i = chead ; i != NULL ; i = i->next){
		for(j = chead ; j->next != NULL ; j = j->next){
			if((j->count) < (j->next->count)){//comparing the character counts
				//swap the two nodes.

				//storing values of one node in temporary variables
				temp_letter = j->letter;
				temp_count = j->count;

				//transfer values from one node to another
				j->count = j->next->count;
				j->letter = j->next->letter;

				//assign the values stored in the temporary variables to the other node
				j->next->count = temp_count;
				j->next->letter = temp_letter;
			}
		}
	}


}
