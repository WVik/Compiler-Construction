#include<stdio.h>

#define numStates 50
#define numInputs 40
#define bufferLength 500

typedef struct tNode{
	short nextState;
	short flag; 
}transitionNode;

typedef struct tInfo{
	char* lexeme;
	char* token;
	int flag;
	int lineNumber;
}tokenInfo;

//typedef struct transNode* TransNode;



int begin,end,state;
char* primaryBuffer, *secondaryBuffer;


transitionNode transitionTable[numStates][numInputs];

char* getNextToken(FILE* fp);
void getStream();
void populateTransitionTable();