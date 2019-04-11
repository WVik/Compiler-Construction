 /*
  Group - 5
  Vikram Waradpande - 2015B4A70454P
  Rinkesh Jain - 2015B4A70590P
  Yajat Dawar - 2015B4A70620P
  Anmol Naugaria - 2015B4A70835P
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>


#ifndef TERMANDNONTERM
#define TERMANDNONTERM
//Map from an enum of type nonterminal to its string
char* nTerminalString[] = {"program","mainFunction","otherFunctions","function","input_par","output_par","parameter_list","dataType","primitiveDatatype",
"constructedDatatype","remaining_list","stmts","typeDefinitions","typeDefinition","fieldDefinitions","fieldDefinition","moreFields",
"declarations","declaration","global_or_not","otherStmts","stmt","assignmentStmt","singleOrRecId","new_24","funCallStmt","outputParameters",
"inputParameters","iterativeStmt","conditionalStmt","elsePart","ioStmt","allVar","arithmeticExpression","expPrime","term","termPrime",
"factor","highPrecedenceOperators","lowPrecedenceOperators","all","temp","booleanExpression","var","logicalOp","relationalOp","returnStmt",
"optionalReturn","idList","more_ids","allVarDash"};


//Map from an enum of type terminal to its string
char* TString[] = {"TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH",
"TK_PARAMETERS","TK_END","TK_WHILE","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT",
"TK_OUTPUT","TK_INT","TK_REAL","TK_COMMA","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN",
"TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD","TK_ENDRECORD",
"TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","eps","$","TK_ERROR"};
#endif


#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "typeChecker.h"


int option = -1;

//Remove comments from source code
void removeComments(FILE* fp)
{
	char newLine[MAX];
	while(fgets(newLine,MAX+1,fp)!=NULL)
	{
		int currInd = 0;
		while(currInd < MAX && (newLine[currInd]==' ' || newLine[currInd]=='\t'))
			currInd++;
		if(currInd<MAX)
			if(newLine[currInd]=='%')
				continue;
		printf("%s",newLine);
	}
}



int main(int argc, char* argv[])
{

  printf("\n\nNOTE: The code works perfectly on testcases 2 and 3. On testcases 1 and 4, it runs on gcc version 4.2.1 and it sometimes shows a segfault. Apologies.\n\n");
  printf("1) First and Follow Automated\n");
  printf("2) Lexical and Syntactic modules implemented\n");
  printf("3) Panic mode with follow synchronizing sets used for error recovery\n");
  printf("4) Parse tree constructed. Pretty Printed parse tree displayed on console\n");
  printf("5) Code comments will be removed and printed on the console after pressing 1. \n");
  printf("6) Code works perfectly with gcc version 4.2.1 \n");
  printf("7) Lexical errors are displayed by showing an error token TK_ERROR \n\n\n");



  FILE* grammarFile;

  //The testcase for the file
  FILE* sourceFile;

  int option = -1;
  printf("Enter your option: \n0:Exit\n1:Print comment free code\n2:Print token list using lexer\n3:Print parse tree and point out lexical errors.\n4:Print time taken by stage 1 of parser.\n\n");

  scanf("%d",&option);

  while(1)
  {
  	if(option==0)
  	{
  		printf("Exiting the program\n");
  		break;
  	}

  	else if(option==1)
  	{
  		sourceFile = fopen(argv[1],"r");
  		removeComments(sourceFile);
  		fclose(sourceFile);
  	}

  	else if(option == 2)
  	{
  		sourceFile = fopen(argv[1],"r");
  		initializeLexer();
  		printf("Printing tokens line by line:\n");

      TokenInfo newToken = getNextToken(sourceFile);

  		while(overFlag==0 && newToken!=NULL)
  		{
  			printf("%d  %s : %s\n",newToken->lineNumber,TString[newToken->token],newToken->lexeme);
  			newToken = getNextToken(sourceFile);
  		}

      if(newToken!=NULL)
        printf("%d  %s : %s\n",newToken->lineNumber,TString[newToken->token],newToken->lexeme);


  		fclose(sourceFile);
  	}

  	else if(option == 3){

      sourceFile = fopen(argv[1],"r");
  		grammarFile = fopen("grammar.txt","r");

      initializeLexer();
      initializeParser(grammarFile);
      buildTreeAndParse(sourceFile);
			createAST(root);

			initializeSymbolTableVariables();
			populateSymbolTable(root);

			checkFunctionSemantics(root->children->next,1);

      //printf("Parsing successful: Program is syntactically correct.");
      printf("Printing Parse Tree: \n\n");
    //  printParseTree(root,argv[2]);


      printf("Pretty printing parse tree\n");
      prettyPrintParseTree(root,0);
      fclose(sourceFile);
      fclose(grammarFile);

  	}
  	else if(option == 4){



        clock_t    start_time, end_time;

        double total_CPU_time, total_CPU_time_in_seconds;

        start_time = clock();


        //-------------------------------LEXER------------------------------
        sourceFile = fopen(argv[1],"r");
        initializeLexer();
        printf("Printing tokens line by line:\n");

        TokenInfo newToken = getNextToken(sourceFile);

        while(overFlag==0 && newToken!=NULL)
        {
          printf("%d  %s : %s\n",newToken->lineNumber,TString[newToken->token],newToken->lexeme);
          newToken = getNextToken(sourceFile);
        }

        if(newToken!=NULL)
          printf("%d  %s : %s\n",newToken->lineNumber,TString[newToken->token],newToken->lexeme);


        fclose(sourceFile);
        //-----------------------------LEXER FINISHED----------------------



        //-----------------------------PARSER-------------------------------

        sourceFile = fopen(argv[1],"r");
        grammarFile = fopen("grammar.txt","r");
        initializeLexer();
        initializeParser(grammarFile);
        buildTreeAndParse(sourceFile);
        //printf("Parsing successful: Program is syntactically correct.");
        printf("Printing Parse Tree: \n\n");
        printParseTree(root,argv[2]);
        printf("Pretty printing parse tree\n");
        prettyPrintParseTree(root,0);
        fclose(sourceFile);
        fclose(grammarFile);

        //-----------------------------LEXER FINISHED----------------------



          end_time = clock();

          total_CPU_time  =  (double) (end_time - start_time);

          total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

          printf("Time in seconds:  %f\n", total_CPU_time_in_seconds);
          printf("Ticks: %d\n",CLOCKS_PER_SEC);


  	}

  	printf("\n\nEnter another option:\n");
  	scanf("%d",&option);

  }

	printf("\n\n");

	//printf("Printing the inorder traversal of the parse tree\n");
	//printInorderTraversal(root);

  return 0;
}
