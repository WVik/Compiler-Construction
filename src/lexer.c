#include<stdio.h>
#include<stdlib.h>
#include "lexer.h"

void getIndex(char c)
{
	if('a'<=c && c<='z')
		return 
}


void initializeLexer()
{
	state = 0;
	primaryBuffer = (char*)malloc(bufferLength*sizeof(char));
	secondaryBuffer = (char*)malloc(bufferLength*sizeof(char));
	begin = end = 0;

	for(int i=0;i<numStates;i++)
		for(int j=0;j<numInputs;j++)		
			{
				transitionTable[i][j].flag = 0;
			 	transitionTable[i][j].nextState = 0;
			}
   	
   	populateTransitionTable();

}

void getStream(FILE* fp)
{
	fread(primaryBuffer, 1, bufferLength, fp);
	printf("%s\n",primaryBuffer);	
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

	return NULL;
}