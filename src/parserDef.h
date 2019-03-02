#include<stdio.h>

# define MAX 500
# define numNonTerminals 51
# define numTerminals 54
# define MAX 500
#define numRules 92


typedef enum nonTerminal {program,mainFunction,otherFunctions,function,input_par,output_par,parameter_list,dataType,primitiveDatatype,
constructedDatatype,remaining_list,stmts,typeDefinitions,typeDefinition,fieldDefinitions,fieldDefinition,moreFields,
declarations,declaration,global_or_not,otherStmts,stmt,assignmentStmt,singleOrRecId,new_24,funCallStmt,outputParameters,
inputParameters,iterativeStmt,conditionalStmt,elsePart,ioStmt,allVar,arithmeticExpression,expPrime,term,termPrime,
factor,highPrecedenceOperators,lowPrecedenceOperators,all,temp,booleanExpression,var,logicalOp,relationalOp,returnStmt,
optionalReturn,idList,more_ids,allVarDash} NonTerminal;

//enum for terminals
// Union Name should be Term

typedef enum terminal {TK_ASSIGNOP,TK_COMMENT,TK_FIELDID,TK_ID,TK_NUM,TK_RNUM,TK_FUNID,TK_RECORDID,TK_WITH,
TK_PARAMETERS,TK_END,TK_WHILE,TK_TYPE,TK_MAIN,TK_GLOBAL,TK_PARAMETER,TK_LIST,TK_SQL,TK_SQR,TK_INPUT,
TK_OUTPUT,TK_INT,TK_REAL,TK_COMMA,TK_SEM,TK_COLON,TK_DOT,TK_ENDWHILE,TK_OP,TK_CL,TK_IF,TK_THEN,
TK_ENDIF,TK_READ,TK_WRITE,TK_RETURN,TK_PLUS,TK_MINUS,TK_MUL,TK_DIV,TK_CALL,TK_RECORD,TK_ENDRECORD,
TK_ELSE,TK_AND,TK_OR,TK_NOT,TK_LT,TK_LE,TK_EQ,TK_GT,TK_GE,TK_NE,eps} Terminal;

typedef enum termOrNonTerm {nonterminal,terminal} TermOrNonTerm;


char* nonTerminalString[] = {"program","mainFunction","otherFunctions","function","input_par","output_par","parameter_list","dataType","primitiveDatatype",
"constructedDatatype","remaining_list","stmts","typeDefinitions","typeDefinition","fieldDefinitions","fieldDefinition","moreFields",
"declarations","declaration","global_or_not","otherStmts","stmt","assignmentStmt","singleOrRecId","new_24","funCallStmt","outputParameters",
"inputParameters","iterativeStmt","conditionalStmt","elsePart","ioStmt","allVar","arithmeticExpression","expPrime","term","termPrime",
"factor","highPrecedenceOperators","lowPrecedenceOperators","all","temp","booleanExpression","var","logicalOp","relationalOp","returnStmt",
"optionalReturn","idList","more_ids","allVarDash"};

char* TerminalString[] = {"TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH",
"TK_PARAMETERS","TK_END","TK_WHILE","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT",
"TK_OUTPUT","TK_INT","TK_REAL","TK_COMMA","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN",
"TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD","TK_ENDRECORD",
"TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","eps"};

typedef union term {
	NonTerminal nt;
	Terminal t;
}Term;


struct rhsNode{
	Term term;
	TermOrNonTerm id; 
	struct rhsNode* next;
};

typedef struct rhsNode* RhsNode;


struct production{
	NonTerminal lhs;
	RhsNode head;
};

typedef struct production* Production;
typedef Production* Grammar;
Grammar g;

struct firstFollowNode{
	RhsNode head;
};

typedef struct firstFollowNode FirstFollowNode;

typedef FirstFollowNode* First;

First Ft;

// struct firstAndFollow{
// 	First f;
// };

// typedef struct firstAndFollow* FirstAndFollow;

// FirstAndFollow FF;

