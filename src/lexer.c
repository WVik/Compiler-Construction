#include<stdio.h>
#include<stdlib.h>
#include "lexer.h"


char* getTokenString()
{
	if(state == 2)
	{
		//ID vs Keyword
	}

	if(state ==6)
	{
		if(end-begin+1 <= 20)
			return tokenList[6];
		return "ERROR";
	}

	if(state==13)
	{
		//Lookup
		if(end-begin+1 <= 30)
			return tokenList[13];
		return "ERROR";

	}

	return tokenList[state];		
}


long hash(char* c)
{
	long temp = 0;
	for(int i=0;;i++)
	{
		if(c[i]=='\0')
			break;
		temp = temp*keywordHashMul + c[i];
		temp = temp%keywordHashModulo;
	}
	return temp;
}


int getTransitionIndex(char c)
{
	if(c == '0' || c == '1' || c == '8' || c == '9')
		return 0;
	if('2'<=c && c<='7')
		return 1;

	if('A'<=c && c<='Z')
		return 2;

	if(c=='a' || ('e'<=c && c<='z'))
		return 3;

	if('b'<=c && c<='d')
		return 4;

	switch(c){
		case '<':
			return 5;
		case '-':
			return 6;
		case '%':
			return 7;
		case '_':
			return 8;
		case '#':
			return 9;
		case '[':
			return 10;
		case ']':
			return 11;
		case '.':
			return 12;
		case ',':
			return 13;
		case ';':
			return 14;
		case ':':
			return 15;
		case '(':
			return 16;
		case ')':
			return 17;
		case '+':
			return 18;
		// case '-':
		// 	return 19;
		case '*':
			return 20;
		case '/':
			return 21;
		case '&':
			return 22;
		case '@':
			return 23;
		case '~':
			return 24;
		case '>':
			return 25;
		case '=':
			return 26;
		case '!':
			return 27;
		case ' ':
			return 28;
		case '\n':
			return 29;
		default :
			return -1;

	}

}

void populateTokenList()
{
	for(int i=0;i<numStates;i++)
		tokenList[i] = (char*)malloc(maxTokenLength*sizeof(char));
		

	tokenList[6] = "TK_ID";
	tokenList[9] = "TK_RECORDID";
	tokenList[13] = "TK_FUNID";
	tokenList[16] = "TK_OR";
	tokenList[19] = "TK_AND";
	tokenList[21] = "TK_NUM";
	tokenList[24] = "TK_RNUM";
	tokenList[28] = "TK_ASSIGNOP";
	tokenList[29] = "TK_LT";
	tokenList[30] = "TK_LT";
	tokenList[32] = "TK_LT";
	tokenList[34] = "TK_GT";
	tokenList[35] = "TK_GE";
	tokenList[37] = "TK_EQ";
	tokenList[39] = "TK_NE";
	tokenList[43] = "TK_COMMENT";
	tokenList[44] = "TK_DIV";
	tokenList[45] = "TK_MUL";
	tokenList[46] = "TK_MINUS";
	tokenList[47] = "TK_PLUS";
	tokenList[48] = "TK_SQL";
	tokenList[49] = "TK_SQR";
	tokenList[50] = "TK_OP";
	tokenList[51] = "TK_CL";
	tokenList[52] = "TK_COMMA";
	tokenList[53] = "TK_SEM";
	tokenList[54] = "TK_COLON";
	tokenList[55] = "TK_DOT";
	tokenList[56] = "TK_NOT";

}

void populateKeywordList()
{

}


void initializeLexer()
{
	state = 0;
	buffer1 = (char*)malloc(bufferLength*sizeof(char));
	buffer2 = (char*)malloc(bufferLength*sizeof(char));
	inputBuffer = buffer1;
	tokenList = (char**)malloc(numStates*sizeof(char*));
	begin = end = 0;
	stateFlags = (int*)malloc(numStates*sizeof(int));

	for(int i=0;i<numStates;i++)
		for(int j=0;j<numInputs;j++)		
			{
				stateFlags[i] = 0;
			 	transitionTable[i][j] = 0;
			}
   	
   	populateTransitionTable();
   	populateTokenList();
   	populateKeywordList();

}

void populateTransitionTable()
{
	/*
	TODO:
	* For each state set the stateFlags properly
	* For each state's every transition, fill the transition accordingly
	* Handle states with 'others' transition separately
	*/

	transitionTable[0][3] = 1;
	stateFlags[0] = 1;


	transitionTable[0][20] = 45;


	transitionTable[45][20] = 45;
	stateFlags[45] = 2;


}

void getStream(FILE* fp)
{
	int numRead = fread(inputBuffer, sizeof(char), bufferLength, fp);
	printf("%s\n",inputBuffer);	
}


int main()
{
	initializeLexer();
    FILE* fp = fopen("sample.txt","r");
    //getStream(fp);

    char* t = getNextToken(fp);
    printf("%s",t);

	return 0;
}


char* getNextToken(FILE* fp)
{
	if(end == bufferLength || end == 0)
		getStream(fp);
    
	while(1)
	{
		char ch = inputBuffer[end];

		end++;

		int index = getTransitionIndex(ch);

		state = transitionTable[state][index];

		if(stateFlags[state] == 0)
			{state = 0; continue;}

		if(stateFlags[state] == 2)
			return getTokenString(); 
	}

	return NULL;
}