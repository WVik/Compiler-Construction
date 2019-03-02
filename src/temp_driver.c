  #include "parser.h"

  # define MAX 500
  # define numNonTerminal 51
  # define numTerminal 54
  #define numRules 100


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

  void getGrammar(fp)
  {

  	
  	if (fp==NULL) 
      { 
          printf("no such file."); 
          return 0; 
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
   		ruleIndex++;
   	}
  }


  //Print Grammar

      //printf("%d ",P[ruleIndex]->lhs);

      // RhsNode temp = P[ruleIndex]->head;

      // while(temp!=NULL)
      // {
      //  if(temp->id == nonterminal)
      //    printf("%d ",(temp->term).t);

      //  else
      //    printf("%d ",(temp->term).nt);

      //  temp = temp->next;
      // }
      // printf("\n");



  // void removeSpaces(char *str) 
  // { 
  //     // To keep track of non-space character count 
  //     int count = 0; 
    
  //     // Traverse the given string. If current character 
  //     // is not space, then place it at index 'count++' 
  //     for (int i = 0; str[i]; i++) 
  //         if (str[i] != ' ') 
  //             str[count++] = str[i]; // here count is 
  //                                    // incremented 
  //     str[count] = '\0'; 
  // } 
