#include<stdio.h>

#define numStates 50
#define numInputs 40
#define bufferLength 500
#define keywordHashModulo 7919
#define keywordHashMul 271
#define maxTokenLength 15




typedef struct tInfo{
	char* lexeme;
	char* token;
	int flag;
	int lineNumber;
}tokenInfo;

//typedef struct transNode* TransNode;



int begin,end,state;
char *buffer1, *buffer2,*inputBuffer;
char** tokenList;
char** keyWordTable;
int transitionTable[numStates][numInputs];

//flag = 0 => ERROR | flag = 1+x => ACCEPT + x RETRACT
int* stateFlags;


char* getNextToken(FILE* fp);
void getStream();
void populateTransitionTable();
long hash(char* c);
char* getTokenString();
int getTransitionIndex(char c);
void populateTransitionTable();
void populateTokenList();