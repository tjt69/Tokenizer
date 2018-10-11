/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global int variables that are used by the state functions, main, and TKGetNextToken.
 *All are intialized at 0 when the program begins.
 */

int t=0;		//integer value that keeps track of the last index in the char* token in TKGetNextToken.
int o=0;		//integer value that keeps track of the last index in the char* input in the TokenizerT created in the function main.
int etrue=0;		//integer value that keeps track of whether or not a 'e' or 'E' is already in the current token being made. 1 if false, 0 if true.
int dtrue=0;		//integer value that keeps track of whether or not a '.' is already in the current token being made. 1 if false, 0 if true.
int strue=0;		//integer value that keeps track of whether or not a '-' or '+' is already in the current token being made. 1 if false, 0 if true.

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 *
 * Defines a Struct containing a character pointer input.
 */

struct TokenizerT_ {

	char *input;	
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string ts).
 * 
 * TKCreate copys the arguments from char* ts into the TokenizerT object so that it is not dependent on
 * them staying immutable after returning.
 *
 * This function also ignores the spaces at the begining of the inputed string and the duplicate spaces
 * within the string. This makes the TokenizerT object return a copy of the inputed string minus the extra spaces.
 * This object is more effiecent to tokenize as the funtion TKGetNextToken recieves a smaller string to iterate through. 
 *
 * Before returning the TokenizerT object this fuction reallocates memory for its char* input, freeing any memory taking up by the extra
 * white space.
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL and prints an error message to stderr.
 *
 */  
	
TokenizerT *TKCreate( char *ts ) {
	
	int length;							//integer value storing the length of ts
	int i,j=0;							//integers i and j keep track of the indexes of the new object and ts 

	TokenizerT *temp=(TokenizerT *)malloc(sizeof(TokenizerT *));    // mallocs space for a pointer to a new TokenizerT object

	length= strlen(ts);

	temp->input= (char*)malloc(length+1);				//mallocs the max amount of space needed for the input string of temp
 
	while(ts[j]==' '){						//eliminates space characters at the beginning of the string
		j++;
	}
	
	/*For loop (int i) Copies the given string ts into the correct index
 	  of input. If there is a space followed by any number of duplicate spaces
	   it will skip the extra spaces by incrementing j by one*/

	for(i = 0;j<length;i++){
		temp->input[i]=ts[j];
		if((ts[j]==' ')){
			while(ts[j+1]==' '){
				j++;
			}
		}
		j++;
		
	}

	temp->input= (char*)realloc((temp->input),(sizeof(char*)*i));	//reallocates memory to the size of input

  return temp;
}

/*
 * Frees all dynamically allocated memory that is part of the object being destroyed, tk.
 * 
 * Does not return anything on completion.
 *
 */

void TKDestroy( TokenizerT * tk ) {

	free(tk->input);			//Frees the char* input
	free(tk);				//Frees the TokenizerT *tk

}

/*
 * Function int state_0 accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it starts with zero and, depending on the next character, will be identified as either octal, hexadecimal,
 * float, or zero.
 *
 * The return value will be 1 if octal, 2 if hexadecimal, 4 if float, or -1 if zero.
 *
 * The return value dicates which state the token will be sent to, -1 will end the state machine.
 *
 */ 

int state_0(char*token, TokenizerT *tk){
	token[t]=tk->input[o];					//copies character from tk's input to the token
	t++;
	o++;				
	
	if(isdigit(tk->input[o])&&(tk->input[o]<'8')){		//Checks to see if octal
		return 1;					
	}	

	if((tk->input[o]=='x'||tk->input[o]=='X')&&isxdigit(tk->input[o+1])){	//Checks to see if hexadecimal
		return 2;
	}

	if(tk->input[o]=='.'&&isdigit(tk->input[o+1])&&dtrue==0){		//Checks to see if decimal
		dtrue=1;							//Sets the dtrue to 1 indicating a '.' is in the token
		return 4;
	} 

	printf("\e[32m Zero: \e[0m	");
		
	return -1;						//ends the state machine
	
}

/*
 * Function int state_octal accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it starts with zero and the next character is less than 8. As long as the next character in tk's input
 * is a digit less than 8 it will stay in this state by copying the character into token and returning 1. If the next character is anthing
 * else it will exit the state machine. 
 * 
 * The return value will be 1 if octal or -1 if other.
 *
 */ 
	
int state_octal(char*token, TokenizerT *tk){
	token[t]=tk->input[o];					//copies character from tk's input to the token
	t++;
	o++;	
		
	if(isdigit(tk->input[o])&&tk->input[o]<'8'){		//Checks to see if octal 
		return 1;
	}
		
	printf("\e[32mOctal: \e[0m	");

	return -1;						//ends the state machine
	
}

/*
 * Function int state_hex accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it starts with either 0x or 0X and the next character is hexadecimal. As long as the next
 * character in tk's input is a hexadecimal digit it will stay in this state by copying the character into token and returning 2.
 * If the next character is anthingelse it will exit the state machine. 
 * 
 * The return value will be 2 if hexadecimal or -1 if other.
 *
 */ 
	
int state_hex(char*token, TokenizerT *tk){
	token[t]=tk->input[o];					//copies character from tk's input to the token
	t++;
	o++;
		
	if(isxdigit(tk->input[o])){				//Checks to see if hexadecimal
		return 2;
	}
		
	printf("\e[32mHexadecimal:\e[0m		");
 	
	return -1;						//ends the state machine

}

/*
 * Function int state_dec accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it starts with a digit 1-9. It will stay in this state as long as the next character is a digit 0-9
 * by copying the character into token and returning 3. 
 * 
 * If the next character is a '.' followed by a digit 0-9 and dtrue is 0 it will return 4 and go to the float state.
 *
 * If the next character is a 'e' or 'E', etrue is zero, and the character that follows is a digit 0-9 or the first occurance of
 * a '+' or'-' it will also return 4.
 *
 * The return value will be 4 if float, 3 if decimal or -1 if other.
 *
 */

int state_dec(char*token, TokenizerT *tk){
	token[t]=tk->input[o];					//copies character from tk's input to the token
	t++;
	o++;

	if(isdigit(tk->input[o])){				//Checks to see if decimal
		return 3;
	}

	if(tk->input[o]=='.'&&isdigit(tk->input[o+1])&&dtrue==0){	//Checks to see if float
		dtrue=1;
		return 4;
	}
 
	/*Checks to see if float*/
	if((tk->input[o]=='e'||tk->input[o]=='E')&&etrue==0){
		if(isdigit(tk->input[o+1])){
			etrue=1;
			return 4;
		}
		if((tk->input[o+1]=='-'&&isdigit(tk->input[o+2])&&strue==0)||(tk->input[o+1]=='+'&&isdigit(tk->input[o+2])&&strue==0)){
			token[t]=tk->input[o];
			t++;
			o++;
			strue=1;
			etrue=1;
			return 4;	
		}
	}

	
	printf("\e[32mInteger:\e[0m	");
		
	return -1;						//ends the state machine

}

/*
 * Function int state_float accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it contains a '.', 'e', or 'E'. It will stay in this state as long as the next character is a digit 0-9
 * by copying the character into token and returning 4. 
 *
 * If the next character is a '.' followed by a digit 0-9 and dtrue is 0 it will return 4 and stay in the float state.
 *
 * If the next character is a 'e' or 'E', etrue is zero, and the character that follows is a digit 0-9 or the first occurance of
 * a '+' or'-' it will also return 4.
 *
 * The return value will be 4 if float or -1 if other.
 *
 */
int state_float(char*token, TokenizerT *tk){
	token[t]=tk->input[o];					//copies character from tk's input to the token
	t++;
	o++;

	if(isdigit(tk->input[o])){				//Checks to see if float
		return 4;
	}
	
	if(tk->input[o]=='.'&&isdigit(tk->input[o+1])&&dtrue==0){	//Checks to see if float
		dtrue=1;
		return 4;
	}	    

	/*Checks to see if float*/
	
	if((tk->input[o]=='e'||tk->input[o]=='E')&&etrue==0){
		if(isdigit(tk->input[o+1])){
			etrue=1;
			return 4;
		}
		if((tk->input[o+1]=='-'&&isdigit(tk->input[o+2])&&strue==0)||(tk->input[o+1]=='+'&&isdigit(tk->input[o+2])&&strue==0)){
			token[t]=tk->input[o];
			t++;
			o++;
			strue=1;
			etrue=1;
			return 4;	
		}
	}

	
	printf("\e[32mFloat:\e[0m	");

	return -1;						//ends the state machine
}

/*
 * Function int state_wspace accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it is white space.
 *
 * The white space is not included in the token and the index of the input of tk is incremented by one.
 *
 * The function then returns -1, ending the state machine.
 *
 * The outcome of this function breaks the string inputed through main by white space characters.
 *
 */
int state_wspace(char*token, TokenizerT *tk){
	o++;	
	return -1;						//ends the state machine

}

/*
 * Function int state_mal accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it does not belong in any other state. It will be saved into a character c and printed out
 * as an error because it is a malformed token.
 *
 * It will return -1 to end the state machine.
 *
 */

int state_mal(char*token, TokenizerT *tk){
	char c = tk->input[o];					//copies character from tk's input to c
		
	printf("\e[31mError Mal Character:\e[0m %c \n",c);
	o++;
		
	return -1;						//ends the state machine

}

/*
 * Function int state_es accepts a char* token and a TokenizerT *tk and is part of a state machine in TKGetNextToken that
 * copies characters in tk to a token string. 
 *
 * A token is sent to this state if it is a control character like '0x02'. It will be saved into a character c and print out the hexadecimal
 * value in brackets [0xhh].
 *
 * It will return -1 to end the state machine.
 *
 */

int state_es(char* token, TokenizerT *tk){
	char c = tk->input[o];					//copies character from tk's input to c

	printf("\e[36;5;91mEscape Character:\e[0m	[0x%x]\n",c);
	o++;
		
	return -1;						//ends the state machine

}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string. Space for the returned token is at first dynamically
 * allocated. Right before returning the token space is reallocated in case 
 * the returned token is smaller than the TokenizerT tk's input string.
 *
 * A state machine with 8 separate cases (0-7) is used to determine the token 
 * where an inital if statement determines which state to start at.
 *
 * The function returns a C string (delimited by '\0') containing the token.
 *
 */

char *TKGetNextToken( TokenizerT * tk ) {
	char *token = (char *)malloc(strlen(tk->input));		//initally allocates memory for a char* the size of the string in ipput
	int state=0;
	
	/*Decides which state to begin the state machine in*/	
	if(tk->input[o]=='0'){
		state=0;
	}else if(isdigit(tk->input[o])){
		state=3;
	}else if(isspace(tk->input[o])){
		state=5;
	}else if(iscntrl(tk->input[o])){
		state=7;
	}else{
		state=6;
	}
	
	/*Calls the functions based on the state, switching states until state is -1*/
	do{
	
		switch(state){
			case 0:state = state_0(token,tk);break;
			case 1:state = state_octal(token,tk);break;
			case 2:state = state_hex(token,tk);break;
			case 3:state = state_dec(token,tk);break;
			case 4:state = state_float(token,tk);break;
			case 5:state = state_wspace(token,tk);break;
			case 6:state = state_mal(token,tk);break;
			case 7:state = state_es(token,tk);break;
		}

	}while(state!=-1);

	/*Resets values for next run*/
	dtrue=0;
	etrue=0;		
	strue=0;

	char * ftoken=(char*)realloc(token,(t+1)*sizeof(char));				//reallocates memory to better fit the token's length
	
	ftoken[t]='\0';									//adds a delimiter'\0' to the last index
	

	t=0;
		
  return ftoken;
}

/*
 * main accepts a string argument as a parammeter (in argv[1]) from the user that is then broken into tokens and identified.
 *
 * Memory is dynamically allocated for a char* p.
 *
 * The string in argv[1] is checked to see if it is empty printing to stderr and exiting with a failure if no string is present.
 *
 * If there is a string in argv[1], p is set to point to it. Then a TokenizerT object is created by calling TKCreate on p.
 *
 * The tokens are then printed out on a seperate line by calling TKGetNextToken and freed until the TokenizerT's input string is exhausted.
 *
 * The TokenizerT object is then freed with TKDestroy and main returns 0;
 * 
 */

int main(int argc, char **argv) {

	char *p=(char*)malloc(sizeof(argv[1]));			//allocates space for p the size of the initial string
	
	if(argv[1] == NULL){					//checks to see if input has a character*
		fprintf(stderr, "No Input! Aborting...\n");
		exit(EXIT_FAILURE);				//exits if there is no string
	}

	p = argv[1];
	
	TokenizerT *chan = TKCreate(p);				//creates a TokenizerT object out of p
	int leng = strlen(chan->input);

	printf("Tokenizer: %s\n\n",chan->input);
	while(o<leng){

		char *nxTk=TKGetNextToken(chan);		//gets next token
		printf("%s",nxTk);
		free(nxTk);					//frees the memory allocate for the token
 		printf("\n\n");
	}

	TKDestroy(chan);					//frees the memory for TokenizerT and the char* contained in the object
	
 return 0;

}
