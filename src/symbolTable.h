#include "symbolTableDef.h"
#include "lexer.h"
#include "parser.h"

struct recTable;

struct symTableEntry{
  TreeNode ASTNode;
  char* symName;
  int type;
  int width;
  short varChanged;
  //Indexing into record table is done by type-2
};

typedef struct symTableEntry* SymTableEntry;

struct funcTable{
  char* funcName;
  int* inputParType;
  int numInputParams;
  int* outputParType;
  int numOutputParams;
  int tableSize;
  SymTableEntry symTable;
  int currIndex;
  int callFlag;
};
typedef struct funcTable* FuncTable;


// Table for records
struct recordTable{
  char* recordName;
  SymTableEntry recordFields;
  int numFields;
  int recordWidth;
  int recordType;
};

typedef struct recordTable* RecTable;



//Hastable for records
struct recordHashNode
{
  char* recName;
  int recType;
  struct recordHashNode* next;
};
typedef struct recordHashNode* RecordHashNode;


//Global function table
FuncTable* functionTable;
RecTable recordTable;
SymTableEntry globalTable;

//Hashtable for recordtypes
RecordHashNode recordHashTable;