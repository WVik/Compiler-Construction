#include<stdio.h>
#include<stdlib.h>
#include "lexer.h"

char* getTokenString()
{
	
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

	if(c=='a' || ('e'<=c && c<='z')
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
		case '-':
			return 19;
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

	}

}

void populateTokenTable()
{
	for(int i=0;i<numStates;i++)
		{tokenList[i] = (char*)malloc(maxTokenLength*sizeof(char));
		 tokenList[i] = NULL;

	tokenList[6] = "TK_ID";
	tokenList[9] = "TK_RECORDID";
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


void initializeLexer()
{
	state = 0;
	primaryBuffer = (char*)malloc(bufferLength*sizeof(char));
	secondaryBuffer = (char*)malloc(bufferLength*sizeof(char));
	tokenList = (char**)malloc(numStates*sizeof(char*));
	begin = end = 0;

	for(int i=0;i<numStates;i++)
		for(int j=0;j<numInputs;j++)		
			{
				transitionTable[i][j].flag = 0;
			 	transitionTable[i][j].nextState = 0;
			}
   	
   	populateTransitionTable();
   	populateTokenTable();

}

void populateTransitionTable()
{
	
	//----------STATE 0 - START----------------
	transitionTable[0][3].nextState = 1;
	transitionTable[0][3].flag = 1;

	transitionTable[0][4].nextState = 3;
	transitionTable[0][4].flag = 1;

	transitionTable[0][getIndex('#')].nextState = 7;
	transitionTable[0][getIndex('#')].flag = 1;

	transitionTable[0][getIndex('_')].nextState = 10;
	transitionTable[0][getIndex('_')].flag = 1;

	transitionTable[0][getIndex('@')].nextState = 14;
	transitionTable[0][getIndex('@')].flag = 1;

	transitionTable[0][getIndex('/')].nextState = 44;
	transitionTable[0][getIndex('/')].flag = 2;

	transitionTable[0][getIndex('*')].nextState = 45;
	transitionTable[0][getIndex('*')].flag = 2;

	transitionTable[0][getIndex('-')].nextState = 46;
	transitionTable[0][getIndex('-')].flag = 2;

	transitionTable[0][getIndex('+')].nextState = 47;
	transitionTable[0][getIndex('+')].flag = 2;

	transitionTable[0][getIndex('[')].nextState = 48;
	transitionTable[0][getIndex('[')].flag = 2;

	transitionTable[0][getIndex(']')].nextState = 49;
	transitionTable[0][getIndex(']')].flag = 2;

	transitionTable[0][getIndex('(')].nextState = 50;
	transitionTable[0][getIndex('(')].flag = 2;

	transitionTable[0][getIndex(')')].nextState = 51;
	transitionTable[0][getIndex(')')].flag = 2;

	transitionTable[0][getIndex(',')].nextState = 52;
	transitionTable[0][getIndex(',')].flag = 2;

	transitionTable[0][getIndex(';')].nextState = 53;
	transitionTable[0][getIndex(';')].flag = 2;

	transitionTable[0][getIndex(':')].nextState = 54;
	transitionTable[0][getIndex(':')].flag = 2;

	transitionTable[0][getIndex('.')].nextState = 55;
	transitionTable[0][getIndex('.')].flag = 2;

	transitionTable[0][getIndex('~')].nextState = 56;
	transitionTable[0][getIndex('~')].flag = 2;


}

void getStream(FILE* fp)
{
	fread(inputBuffer, 1, bufferLength, fp);
	printf("%s\n",inputBuffer);	
}


int main()
{
	initializeLexer();
    FILE* fp = fopen("sample.txt","r");
    getStream(fp);

	return 0;
}


char* getNextToken(FILE* fp)
{
	if(end == bufferLength || end == 0)
		getStream(fp);

	while(true)
	{
		char ch = inputBuffer[end];
		state = transitionTable[state][getIndex(ch)].nextState;

		if(transitionTable[state][getIndex(ch)].flag == 0)
			printf("Error");

		if(transitionTable[state][getIndex(ch)].flag == 1)
			continue;

		if(transitionTable[state][getIndex(ch)].flag == 2)
			return getTokenString(); 
	}

	return NULL;
}