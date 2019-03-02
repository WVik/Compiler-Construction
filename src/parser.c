#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "parser.h"

void initializeParser(FILE* fp)
{
	getGrammar(fp);
	
  for(int i=0;i<numNonTerminals;i++)
    visited[i] = 0;
	//tableParseTable = (int**)malloc(sizeof(int*)*numNonTerminals);
	//for(int i=0;i<numNonTerminals;i++)
		//tableParseTable[i] = (int*)malloc(sizeof(int)*numTerminals);
	//table ParseTable = createParseTable(g);
  Ft = (FirstFollowNode*)malloc(numNonTerminals*sizeof(FirstFollowNode));
  for(int i=0;i<numNonTerminals;i++)
    Ft->head = NULL;

  ComputeFirstAndFollow();
}


int indexNonTermLHS(NonTerminal nt)
{
  for(int i=0;i<numRules;i++)
    if(g[i]->lhs == nt)
      return i;
  return -1;
}


int containsNullProd(int i)
{
  RhsNode temp = Ft[i].head;
  while(temp!=NULL)
  {
    if((temp->term).t == eps)
      return 1;
    temp = temp->next;
  }
  return 0;
}

// int indexNonTerm(int i)
// {
//   for(int i=0;i<numNonTerminals;i++)
//     if(strcmp())
// }

void printFirst(int i){
  printf("%s: ", nonTerminalString[i]);
  RhsNode temp = Ft[i].head;
  while(temp!=NULL)
  {
    printf("%s ",TerminalString[(temp->term).nt]);
    temp = temp->next;
  }
  printf("\n");
}

void ComputeFirstAndFollow()
{
	for(int i=0;i<numNonTerminals;i++)
		{if(visited[i]==0)
			{computeFirst(i);}
    

    //printFirst(i);
  }
}


void computeFirst(int nonTerm)
{
	//if(visited[nonTerm]==1)
		//return;
	visited[nonTerm] = 1;
	int temp = 0;
	
    while(1)
  	{
  		int TermFlag = 0;
      if(g[indexNonTermLHS(nonTerm)+temp]->lhs != nonTerm)
  			return;

  		RhsNode rhs = g[indexNonTermLHS(nonTerm)+temp]->head;

      while(rhs!=NULL)
      {
  		
      if(rhs->id == terminal)
  			{
          push(nonTerm,((rhs->term).nt));
          TermFlag = 1;     
          break;       
        }

  		
  				if(Ft[(rhs->term).nt].head == NULL)
  					computeFirst((rhs->term).nt);

          int epsFlag = 0;
          if(rhs->next == NULL)
            epsFlag = 1;
  				merge(nonTerm,(rhs->term).nt,epsFlag);

  				if(!containsNullProd((rhs->term).nt))
  					break;
  				rhs = rhs->next;
  		}
  		
  		temp++;
      if(indexNonTermLHS(nonTerm)+temp>=numRules)
            break;

  	}
}



void push(int nonTerm,int t)
{
	
	if(Ft[nonTerm].head == NULL)
	{
		Ft[nonTerm].head = (RhsNode)malloc(sizeof(struct rhsNode));
		(Ft[nonTerm].head->term).nt = t;
		Ft[nonTerm].head->id = terminal;
    Ft[nonTerm].head->next = NULL;
		return;
	}

	RhsNode temp = Ft[nonTerm].head;
	
  if((temp->term).nt == t)
      return; 

	while(temp->next != NULL)
	{
		if((temp->term).nt == t)
			return;
		
		temp = temp->next;
	}
	temp->next = (RhsNode)malloc(sizeof(struct rhsNode));
	(temp->next->term).nt = t;
	temp->id = terminal;
  temp->next->next = NULL;
}


void merge(int nonTerm, int t, int epsFlag)
{
	
  RhsNode temp = Ft[t].head;
	while(temp!=NULL)
		{
			if(epsFlag == 0 && (temp->term).t==eps)
        {temp = temp->next;continue;}

      push(nonTerm,(temp->term).nt);
			temp = temp->next;
		}
}

/*void printFirst()
{
	for(int i=0;i<numNonTerminals;i++)
	{
		print("%s",)
	}
}
*/


int parseWithSpaces(char* string, char** parsed) 
  { 

  	int len  = strlen(string);
  	char* str = (char*)malloc(sizeof(char)*MAX);
  	strcpy(str,string);
    	
   	int i; 

  	for (i = 0; i < MAX; i++) { 
  		parsed[i] = strsep(&str, " "); 

  		if (parsed[i] == NULL) 
  			{
  				break; 
  			}
  		if (strlen(parsed[i]) == 0) 
  			i--; 
  	}

  	return i;
  } 



  int getIndexNonTerminal(char* string)
  {
  	for(int i=0;i<numNonTerminals;i++)
  	{
  		if(strcmp(string,nonTerminalString[i])==0)
  			return i;
  	}

  	return -1;

  }

  int getIndexTerminal(char* string)
  {

  	for(int i=0;i<numTerminals;i++)
  	{
  		if(strcmp(string,TerminalString[i])==0)
  			return i;
  	}

  	return -1;
  }




void getGrammar(FILE* fp)
  {
	
  	if (fp==NULL) 
        {
            printf("no such file \n");  
            return;
        }

   	g = (Grammar)malloc(sizeof(Production)*numRules);

   	int ruleIndex = 0;
   	char line[MAX];

   	while(fgets(line, MAX, fp)!=NULL)
   	{
   		line[strlen(line)-2] = '\0';
   		
   		char* parsed[MAX];
   		int size = parseWithSpaces(line,parsed);

   		g[ruleIndex] = (Production)malloc(sizeof(struct production));

   		g[ruleIndex]->head = NULL;

   		for(int i=0;i<size;i++)
   		{
   				int len = strlen(parsed[i]);
   				
   				if(parsed[i][0]=='-')
   					continue;

   				if(parsed[i][0]=='<' && parsed[i][len-1]=='>')
   				{
   					// case of Non Terminal
   					parsed[i][len-1] = '\0';
   					parsed[i]++;

   					//printf(" %s",parsed[i]);
   					int ind = getIndexNonTerminal(parsed[i]);

   					//printf("Index = %d ", ind);

   					if(i==0)
   					{
   						// LHS
   						g[ruleIndex]->lhs = ind;
   					}
   					else
   					{
   						// RHS
   						RhsNode r = (RhsNode)malloc(sizeof(struct rhsNode));
    						RhsNode temp = g[ruleIndex]->head;

    						if(temp == NULL){
      							g[ruleIndex]->head = r;
      						  (r->term).nt = ind;
        				    r->id = nonterminal;
        				    r->next = NULL; 
    						}
    						else
    						{
        						while(temp->next != NULL)
        							 temp = temp->next;

        						temp->next = r;
      						  (r->term).nt = ind;
    	  				    r->id = nonterminal;
    	  				    r->next = NULL;

  	  				    }
   					}

   				}
   				else
   				{
   					// case of Terminal
   					//printf(" %s",parsed[i]);
   					int ind = getIndexTerminal(parsed[i]);

   					//printf("Index = %d ", ind);

   					RhsNode r = (RhsNode)malloc(sizeof(struct rhsNode));
    					RhsNode temp = g[ruleIndex]->head;
    						
    						if(temp == NULL){

    							 g[ruleIndex]->head = r;
  							   (r->term).t = ind;
  	  				    	r->id = terminal;
  	  				    	r->next = NULL;

    						}

    						else
    						{

        						while(temp->next != NULL)
        						{
        							temp = temp->next;
        						}

        						temp->next = r;

        						(r->term).t = ind;
      	  				  r->id = terminal;
      	  				  r->next = NULL;

  	  				    }
   				} 	
   		}
   		
/*
        printf("%d ",g[ruleIndex]->lhs);

    RhsNode temp = g[ruleIndex]->head;

    while(temp!=NULL)
    {
      if(temp->id == nonterminal)
        printf("%d ",(temp->term).t);

      else
        printf("%d ",(temp->term).nt);

      temp = temp->next;
    }
    printf("\n");
    //ruleNum++;
    */
    ruleIndex++;


   	}
  }




int main()
{
	FILE* fp = fopen("grammar.txt","r");
	initializeParser(fp);
	return 0;
}
