#include<stdio.h>
#include "parser.h"

void initializeParser()
{
	Grammar g;// = getGrammar(fp);
	ComputeFirstAndFollow(g);
	//tableParseTable = (int**)malloc(sizeof(int*)*numNonTerminals);
	for(int i=0;i<numNonTerminals;i++)
		tableParseTable[i] = (int*)malloc(sizeof(int)*numTerminals);
	//table ParseTable = createParseTable(g);
	First = (FirstFollowNode*)malloc(numNonTerminals*sizeof(FirstFollowNode));
}

void ComputeFirstAndFollow(Grammar g)
{
	for(int i=0;i<numNonTerminals;i++)
		if(visited[i]==0)
			computeFirst(i,Grammar g);
}


void computeFirst(Term nonTerm)
{
	if(visited[nonTerm]==1)
		return;

	visited[nonTerm] = 1;
	int temp = 0;
	while(1)
	{
		if(Grammar[indexNonTermLHS[nonTerm]+temp]->lhs != nonTerm)
			return;

		RhsNode rhs = Grammar[indexNonTermLHS[nonTerm]+temp]->head;

		if(rhs->TermorNonTerm == Terminal)
			{push(nonTerm,rhs->term);temp++;continue;}

		while(rhs!=NULL)
			{
				if(First[rhs->term].head == NULL)
					computeFirst(rhs->term);

				merge(nonTerm,rhs->term);

				if(!containsNullProd(rhs->term))
					break;
				rhs = rhs->next;
			}
		
		temp++;

	}
}



void push(Term nonTerm,Term t)
{
	
	if(First[nonTerm].head == NULL)
	{
		First[nonTerm].head = (RhsNode)malloc(sizeof(struct rhsNode));
		First[nonTerm].head->term = t;
		First[nonTerm].id = Terminal;
		return;
	}

	RhsNode temp = First[nonTerm].head;
	
	while(temp->next != NULL)
	{
		if(temp->term == t)
			return;
		
		temp = temp->next;
	}
	temp->next = (RhsNode)malloc(sizeof(struct rhsNode));
	temp->term = t;
	temp->id = Terminal;
}


void merge(Term nonTerm, Term t)
{
	RhsNode head = First[t].head;
	while(head->next!=NULL)
		{
			push(nonTerm,head->term);
			head = head->next;
		}
}

void printFirst()
{
	for(int i=0;i<numNonTerminals;i++)
	{
		print("%s",)
	}
}


int main()
{
	return 0;
}
