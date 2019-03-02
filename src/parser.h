#include "parserDef.h"

void ComputeFirstAndFollow();
void initializeParser(FILE* fp);
void getGrammar(FILE* fp);
void computeFirst(int nonTerm);
void push(int nonTerm,int t);
void merge(int nonTerm, int t,int epsFlag);
//void push();
int indexNonTermLHS(NonTerminal nt);
int visited[numNonTerminals];
int containsNullProd(int i);