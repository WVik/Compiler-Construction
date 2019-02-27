#include<stdio.h>

#define numStates 50
#define numInputs 40
#define bufferLength 500
#define keywordHashModulo 7919
#define keywordHashMul 271
#define maxTokenLength 15

typedef struct tNode{
	short nextState;
	short flag; 
}transitionNode;

typedef struct tInfo{
	char* lexeme;
	char* token;

	//flag = 0 => ERROR | flag = 1 => GOTO NEXT | flag = 2 => ACCEPT
	int flag;

	int lineNumber;
}tokenInfo;

//typedef struct transNode* TransNode;



int begin,end,state;
char* buffer1, *buffer2,*inputBuffer;
char** tokenList;

transitionNode transitionTable[numStates][numInputs];

char* getNextToken(FILE* fp);
void getStream();
void populateTransitionTable();
long hash(char* c);
char* getTokenString();
int getTransitionIndex();
void populateTransitionTable();
void populateTokenTable();