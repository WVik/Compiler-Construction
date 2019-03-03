#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "parser.h"

void initializeParser(FILE* fp)
{
	
  //Initialize Grammar Production Rules
  g = (Grammar)malloc(sizeof(Production)*numRules);

	RHSRuleIndices = (int**)malloc(numNonTerminals*sizeof(int*));
  currentIndex = (int*)malloc(sizeof(int)*numNonTerminals);

  for(int i=0;i<numNonTerminals;i++)
    currentIndex[i] = 0;

  for(int i=0;i<numNonTerminals;i++)
      {
        RHSRuleIndices[i] = (int*)malloc(sizeof(int)*maxRHSIndices);
        for(int j=0;j<maxRHSIndices;j++)
          RHSRuleIndices[i][j] = -1;
      }

  for(int i=0;i<numNonTerminals;i++)
    visited[i] = 0;


  //Parse the grammar and make liked lists corresponding to each rule
   getGrammar(fp);

	
  //Definitions and delcations for First and Follow computation
  Ft = (FirstFollowNode*)malloc(numNonTerminals*sizeof(FirstFollowNode));
  for(int i=0;i<numNonTerminals;i++)
    Ft->head = NULL;

  Fl = (FirstFollowNode*)malloc(numNonTerminals*sizeof(FirstFollowNode));
  for(int i=0;i<numNonTerminals;i++)
    Fl->head = NULL;

  RhsNode dollar = (RhsNode)malloc(sizeof(struct rhsNode));
  (dollar->term).t = $;
  dollar->next = NULL;
  dollar->id = terminal;

  push(Fl,0,dollar);
  ComputeFirstAndFollow();

  //Definitions and delcations for Predictive Parsing Table
  ParseTable = (int**)malloc(sizeof(int*)*numNonTerminals);
  for(int i=0;i<numNonTerminals;i++)
    {
      ParseTable[i] = (int*)malloc(sizeof(int)*numTerminals);
      for(int j=0;j<numTerminals;j++)
        ParseTable[i][j] = -1;
    }

  createParseTable();

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

void printFirstFollow(FirstFollowNode * list, int i){
  printf("%s: ", nonTerminalString[i]);
  RhsNode temp = list[i].head;
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
		{
      if(visited[i]==0)
			   computeFirst(i);
       //printFirstFollow(Ft,i);
    }
  

  
  //Reset Visited rules for Follow
  for(int i=0;i<numNonTerminals;i++)
    visited[i] = 0;  


  for(int i=0;i<numNonTerminals;i++)
  {  if(visited[i]==0)
        computeFollow(i);
      printFirstFollow(Fl,i);
  }
      
  //}
}



void computeFollow(int nonTerm)
{
    if(visited[nonTerm]==1)
      return;

    visited[nonTerm] = 1;
    for(int i=0;i<currentIndex[nonTerm];i++)
    {
        computeFollowHelper(RHSRuleIndices[nonTerm][i], nonTerm);
    }

}


void computeFollowHelper(int RuleNum, int nonTerm)
{
  RhsNode temp = g[RuleNum]->head;
  
  //Flag to check if we encountered the nonterminal yet
  int sameNonTerm = 0;

  while(temp!=NULL)
  {
    

    if(sameNonTerm == 1)
    {
      //Encounter a Terminal
      if(temp->id == terminal)
        {
          push(Fl,nonTerm,temp);
          sameNonTerm = 0;
          temp = temp->next;
          continue;
        }
      
      //If it is a non terminal, merge the first of temp->term
      merge(Fl,Ft,nonTerm,(temp->term).nt,0);
      
      //If this nonterminal doesn't go to eps, reset the flag
      if(!containsNullProd((temp->term).nt))
        sameNonTerm = 0;

    }
    else if(temp->id == terminal || (temp->id == nonterminal && nonTerm != (temp->term).nt))
    {
      temp = temp->next;
      continue;
    }

    if(temp->id == nonterminal && nonTerm == (temp->term).nt)
      sameNonTerm = 1;

    /*Recursive follow case:
        1) The last nonterminal goes to eps
        2) The last nonterminal is the one whose follow is to be computed
    */

    if(temp->next == NULL && ((sameNonTerm==1) || (nonTerm == (temp->term).nt && temp->id == nonterminal)))
      {
        
        //If follow isn't computed yet
        if(Fl[g[RuleNum]->lhs].head == NULL)
          computeFollow(g[RuleNum]->lhs);

        merge(Fl,Fl,nonTerm,g[RuleNum]->lhs,0);
        return;
      }

    //We encountered the non-terminal whose follow is to be computed
    
    //sameNonTerm = 1;

    temp = temp->next;
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
          push(Ft,nonTerm,rhs);
          TermFlag = 1;     
          break;       
        }

  		
  				if(Ft[(rhs->term).nt].head == NULL)
  					computeFirst((rhs->term).nt);

          int epsFlag = 0;
          if(rhs->next == NULL)
            epsFlag = 1;
  				merge(Ft,Ft,nonTerm,(rhs->term).nt,epsFlag);

  				if(!containsNullProd((rhs->term).nt))
  					break;
  				rhs = rhs->next;
  		}
  		
  		temp++;
      if(indexNonTermLHS(nonTerm)+temp>=numRules)
            break;

  	}
}




void push(FirstFollowNode* list, int nonTerm,RhsNode t)
{
	
	if(list[nonTerm].head == NULL)
	{
		list[nonTerm].head = (RhsNode)malloc(sizeof(struct rhsNode));
		(list[nonTerm].head->term).nt = (t->term).nt;
		list[nonTerm].head->id = terminal;
    list[nonTerm].head->next = NULL;
		return;
	}

	RhsNode temp = list[nonTerm].head;
	
  if((temp->term).nt == (t->term).nt)
      return; 

	while(temp->next != NULL)
	{		
		temp = temp->next;
    if((temp->term).nt == (t->term).nt)
      return;
	}
	temp->next = (RhsNode)malloc(sizeof(struct rhsNode));
	(temp->next->term).nt = (t->term).nt;
	temp->id = terminal;
  temp->next->next = NULL;
}






void merge(FirstFollowNode* insertInto, FirstFollowNode* insertFrom, int nonTerm, int t, int epsFlag)
{
	
  RhsNode temp = insertFrom[t].head;
	while(temp!=NULL)
		{
			if(epsFlag == 0 && (temp->term).t==eps)
        {temp = temp->next;continue;}

      push(insertInto,nonTerm,temp);
			temp = temp->next;
		}
}


// void merge(int nonTerm, int t, int epsFlag)
// {
  
//   RhsNode temp = Ft[t].head;
//   while(temp!=NULL)
//     {
//       if(epsFlag == 0 && (temp->term).t==eps)
//         {temp = temp->next;continue;}

//       push(Ft,nonTerm,temp);
//       temp = temp->next;
//     }
// }



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

              //For follow computation
              RHSRuleIndices[ind][currentIndex[ind]] = ruleIndex;
              currentIndex[ind]++;

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
   		
    ruleIndex++;


   	}
  }


void createParseTable()
{
  for(int i=0;i<numRules;i++)
  {
    int ntIndex = g[i]->lhs; 
    RhsNode rhsTerm = g[i]->head; 
    int epsFlag = 0;
    while(rhsTerm!=NULL)
    {
      epsFlag = 0;
      if(rhsTerm->id == terminal)
      {
        if((rhsTerm->term).t == eps)
        {
          epsFlag = 1;
          break;
        }
        ParseTable[ntIndex][(rhsTerm->term).t] = i;
        break;
      }

      RhsNode firstNode = Ft[(rhsTerm->term).nt].head;
      while(firstNode!=NULL)
      {
        if((firstNode->term).nt == eps)
        { 
          epsFlag=1;
          firstNode = firstNode->next;
          continue;
        }

        ParseTable[ntIndex][(firstNode->term).t] = i;
        firstNode = firstNode->next;

      }
      if(epsFlag == 0)
        break;
      rhsTerm = rhsTerm->next;
    }

    if(epsFlag == 1)
    {
      RhsNode temp =Fl[ntIndex].head;
      
      while(temp!=NULL)
      {
        
        ParseTable[ntIndex][(temp->term).t] = i;
        temp=temp->next;
      }
    }

  }

  for(int i=0;i<numNonTerminals;i++)
  {
    RhsNode flNode = Fl[i].head;
    while(flNode!=NULL)
    {
      if(ParseTable[i][(flNode->term).t] <= 0)
        ParseTable[i][(flNode->term).t] = SYNCH;
      flNode = flNode->next;
    }
  }

  //Synchronize using a synchronize
  
  printParseTree();


}

void printParseTree(){

    for(int i=0;i<numNonTerminals;i++)
    {
      for(int j=0;j<numTerminals;j++)
        printf("%d ",ParseTable[i][j]);
      printf("\n");
    }

}


int main()
{
	FILE* fp = fopen("grammar.txt","r");
	initializeParser(fp);
	return 0;
}
