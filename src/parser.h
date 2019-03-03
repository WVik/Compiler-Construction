#include "parserDef.h"

void ComputeFirstAndFollow();
void initializeParser(FILE* fp);
void getGrammar(FILE* fp);
void computeFirst(int nonTerm);
void push(FirstFollowNode* list,int nonTerm, RhsNode t);
void merge(FirstFollowNode* insertInto, FirstFollowNode* insertFrom, int nonTerm, int t, int epsFlag);
int indexNonTermLHS(NonTerminal nt);
int visited[numNonTerminals];
int containsNullProd(int i);
void computeFollow(int nonTerm);
void computeFollowHelper(int RuleNum, int nonTerm);
void printFirstFollow(FirstFollowNode * list, int i);