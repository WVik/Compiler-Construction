#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "typeChecker.h"


int getGlobalIndex(char* varName)
{
  for(int i=0;i<currGlobalIndex;i++)
    if(strcmp(varName,globalTable[i].symName)==0)
      return i;
  return -1;
}

int getVarIndex(int funcIndex,char* string)
{
  SymTableEntry t = functionTable[funcIndex]->symTable;
  int symbolTableLen = functionTable[funcIndex]->currIndex;
  for(int i=0;i<symbolTableLen;i++)
    if(strcmp(string,t[i].symName)==0)
      return i;
  return -1;
}

int getType(int functionIndex,TreeNode root)
{
  int symIndex = getVarIndex(functionIndex,root->leafInfo->lexeme);
  int globalIndex = getGlobalIndex(root->leafInfo->lexeme);

  if(symIndex!=-1)
    return functionTable[functionIndex]->symTable[symIndex].type;
  else if(globalIndex!=-1)
    return globalTable[globalIndex].type;

  printf("ERROR: Variable %s not declared before use at line %d",root->leafInfo->lexeme,root->leafInfo->lineNumber);
  return -1;
}



char* getTypeString(int type)
{
  if(type == 0)
    return "integer";
  else if(type == 1)
    return "real";
  else
    return recordTable[type-2].recordName;
}

void checkFunctionSemantics(TreeNode root, int functionIndex)
{
  if(root->nt == mainFunction)
    root = root->children;
  else
    root = root->children->next;

  while(root->nt != assignmentStmt)
  {
      root = root->next;
      if(root==NULL)
        return;
  }
  if(root == NULL)
    return;

  checkAssignmentStatement(root,functionIndex);
}


void checkAssignmentStatement(TreeNode root, int functionIndex)
{
    SymTableEntry symbolTable = functionTable[functionIndex]->symTable;
    int symTableLen = functionTable[functionIndex]->currIndex;
    TreeNode lhs = root->children;

    int lhsType = -1,rhsType = -1;

    //Not a record field
    if(lhs->next->t == eps)
      {
        lhsType = getType(functionIndex,lhs);
      }
    else{
      int recIndex = getRecordIndex(lhs->leafInfo->lexeme);
      for(int i=0;i<recordTable[recIndex].numFields;i++)
        if(strcmp(recordTable[recIndex].recordFields[i].symName,lhs->next->leafInfo->lexeme) == 0)
          lhsType = recordTable[recIndex].recordFields[i].type;

      if(lhsType == -1)
        printf("ERROR: Invalid record use at line %d\n",lhs->leafInfo->lineNumber);

      }

      if(lhsType == -1)
      {
        rhsType = evalArithmeticExpressionType(lhs->next->next,functionIndex);
        return;
      }

      int symTableIndex = getVarIndex(functionIndex,lhs->leafInfo->lexeme);
      if(symTableIndex == -1)
      {
        int globalTableIndex = getGlobalIndex(lhs->leafInfo->lexeme);
        globalTable[globalTableIndex].varChanged = 1;
      }
      else
        symbolTable[symTableIndex].varChanged = 1;


      rhsType = evalArithmeticExpressionType(lhs->next->next->children,functionIndex);

      if(rhsType == -1)
      {
        printf("ERROR: Incorrect arithmetic expression at line %d\n", lhs->leafInfo->lineNumber);
        return;
      }

      if(lhsType!=rhsType)
      {
        printf("Line %d: Type mismatch: Error assigning type %s to %s of type %s\n",lhs->leafInfo->lineNumber,getTypeString(rhsType),lhs->leafInfo->lexeme,getTypeString(lhsType));
        return;
      }

      return;

}



int evalArithmeticExpressionType(TreeNode root,int functionIndex)
{
  if(root->id == terminal)
  {
    if(root->t == TK_ID)
    {
      int symIndex = getVarIndex(functionIndex,root->leafInfo->lexeme);
      if(symIndex!=-1)
        return functionTable[functionIndex]->symTable[symIndex].type;

      printf("ERROR: Variable %s not declared before use at line %d",root->leafInfo->lexeme,root->leafInfo->lineNumber);
      return -1;
    }

    if(root->t == TK_NUM)
      return 0;

    if(root->t == TK_RNUM)
      return 1;

    if(root->t == TK_PLUS || root->t == TK_MINUS)
      {
        int leftType = evalArithmeticExpressionType(root->children,functionIndex);
        int rightType = evalArithmeticExpressionType(root->children->next,functionIndex);

        if(leftType == rightType && (leftType == 0 || leftType == 1))
          return leftType;

        if(leftType == rightType && leftType >= 2)
          return leftType;

        return -1;

      }

      if(root->t == TK_MUL || root->t == TK_DIV)
        {
          int leftType = evalArithmeticExpressionType(root->children,functionIndex);
          int rightType = evalArithmeticExpressionType(root->children->next,functionIndex);

          if(leftType == rightType && (leftType == 0 || leftType == 1))
            return leftType;

          if(leftType == rightType && leftType >= 2)
            {
              printf("ERROR: Records can't be multiplied or divided at line number %d\n",root->leafInfo->lineNumber);
              return -1;
            }

          return -1;

        }

  }

  if(root->nt == temp)
  {
    int index = getVarIndex(functionIndex,root->children->leafInfo->lexeme);
    if(index == -1)
      {
        //TODO error
        return -1;
      }
    int recordIndex = functionTable[functionIndex]->symTable[index].type-2;
    if(recordIndex < -1)
      return -1;

    for(int i=0;i<recordTable[recordIndex].numFields;i++)
    {
      if(strcmp(root->children->next->leafInfo->lexeme,recordTable[recordIndex].recordFields[i].symName) == 0)
        return recordTable[recordIndex].recordFields[i].type;
    }

    printf("ERROR: Record %s has no field named %s at line %d\n",recordTable[recordIndex].recordName, root->children->next->leafInfo->lexeme, root->children->next->leafInfo->lineNumber);
    return -1;
  }

}


void checkFunctionCallSemantics(TreeNode root, int functionIndex)
{
  TreeNode inputParNode = root->children->next;
  TreeNode outputParNode = root->children->next->next;
  int lineNumber = root->children->leafInfo->lineNumber;
  int calleeIndex = getFunctionIndex(root->children->leafInfo->lexeme);

  //Invalid call
  if(calleeIndex == -1)
  {
    printf("Line %d: No function named %s declared before use\n",lineNumber,root->children->leafInfo->lexeme);
    return;
  }

  //Called before declaration
  if(calleeIndex>functionIndex)
  {
    printf("Line %d: Function %s called before defining\n",lineNumber,root->children->leafInfo->lexeme);
    return;
  }

  //Count number of params
  int numInputParams = 0;
  TreeNode temp = inputParNode->children;
  while(temp!=NULL)
  {
    numInputParams++;
    temp = temp->next;
  }
  int numOutputParams = 0;
  temp = outputParNode->children;
  while(temp!=NULL)
  {
    numOutputParams++;
    temp = temp->next;
  }

  //Number mismatch
  if(numOutputParams != functionTable[calleeIndex]->numOutputParams)
  {
    printf("Line %d: Function %s returns %d parameters, recieved %d\n",lineNumber,root->children->leafInfo->lexeme,functionTable[calleeIndex]->numOutputParams,numOutputParams);
    return;
  }

  if(numInputParams != functionTable[calleeIndex]->numInputParams)
  {
    printf("Line %d: Number of actual and formal parameters of function %s don't match\n",lineNumber,root->children->leafInfo->lexeme);
    return;
  }


  temp = inputParNode->children;
  //Check types of input parameters
  for(int i=0;i<numInputParams;i++)
  {
    int actualType = getType(functionIndex, temp);
    int formalType = functionTable[calleeIndex]->inputParType[i];
    if(actualType == -1)
    {
      printf("Line %d: Invalid variable %s\n", temp->leafInfo->lineNumber, temp->leafInfo->lexeme);
      return;
    }
    //If formalType is -1, it means the error was already declared
    if(formalType == -1)
      return;

    if(formalType!=actualType)
    {
      printf("Line %d: Function type mismatch. Parameter number %d is expected to be of type %s\n",temp->leafInfo->lineNumber,i+1,getTypeString(formalType));
    }
    temp = temp->next;
  }

 //Check type for output parameters
 temp = outputParNode->children;
 for(int i=0;i<numOutputParams;i++)
 {
   int actualType = getType(functionIndex, temp);
   int formalType = functionTable[calleeIndex]->outputParType[i];
   if(actualType == -1)
   {
     printf("Line %d: Invalid variable %s\n", temp->leafInfo->lineNumber, temp->leafInfo->lexeme);
     return;
   }
   //If formalType is -1, it means the error was already declared
   if(formalType == -1)
     return;

   if(formalType!=actualType)
   {
     printf("Line %d: Function type mismatch. Parameter number %d is expected to be of type %s\n",temp->leafInfo->lineNumber,i+1,getTypeString(formalType));
   }

   temp = temp->next;
 }


}
