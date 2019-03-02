#include "parser.h"

# define MAX 500
# define numNonTerminal 51
# define numTerminal 54
int ruleNum = 1;

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

void removeSpaces(char *str) 
{ 
    // To keep track of non-space character count 
    int count = 0; 
  
    // Traverse the given string. If current character 
    // is not space, then place it at index 'count++' 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ') 
            str[count++] = str[i]; // here count is 
                                   // incremented 
    str[count] = '\0'; 
} 

int getIndexNonTerminal(char* string)
{
	for(int i=0;i<numNonTerminal;i++)
	{
		if(strcmp(string,nonTerminalString[i])==0)
			return i;
	}

	return -1;

}

int getIndexTerminal(char* string)
{

	for(int i=0;i<numTerminal;i++)
	{
		if(strcmp(string,TerminalString[i])==0)
			return i;
	}

	return -1;
}

int main()
{

	FILE* ptr = fopen("grammar.txt","r");

	if (ptr==NULL) 
    { 
        printf("no such file."); 
        return 0; 
    } 

 	Production P[MAX];

 	int totalRules = 0;

 	char line[MAX];

 	while(fgets(line, MAX, ptr)!=NULL)
 	{
 		line[strlen(line)-2] = '\0';
 		
 		char* parsed[MAX];
 		int size = parseWithSpaces(line,parsed);

 		P[totalRules] = (Production)malloc(sizeof(struct production));

 		P[totalRules]->head = NULL;

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
 						P[totalRules]->lhs = ind;
 					}
 					else
 					{
 						// RHS
 						RhsNode r = (RhsNode)malloc(sizeof(struct rhsNode));
  						RhsNode temp = P[totalRules]->head;

  						if(temp == NULL){

  							P[totalRules]->head = r;
							(r->term).nt = ind;
	  				    	r->id = nonterminal;
	  				    	r->next = NULL; 

  						}
  						else
  						{

  						while(temp->next != NULL)
  						{
  							temp = temp->next;
  						}

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
  					RhsNode temp = P[totalRules]->head;
  						
  						if(temp == NULL){

  							P[totalRules]->head = r;
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

 		   //printf("Lhs = %d \n",P[0]->lhs);

   // RhsNode temp = P[0]->head;
   // while(temp!=NULL)
   // {

   // 	printf(" %d_%d",(temp->term).nt,temp->id);
   // 	temp = temp->next;
   // }


 		printf("%d -- ",ruleNum);
 		printf("%d ",P[totalRules]->lhs);

 		RhsNode temp = P[totalRules]->head;

 		while(temp!=NULL)
 		{
 			if(temp->id == nonterminal)
 				printf("%d ",(temp->term).t);

 			else
 				printf("%d ",(temp->term).nt);

 			temp = temp->next;
 		}
 		printf("\n");
 		ruleNum++;
 		totalRules++;

 	}



 	

	return 0;
}