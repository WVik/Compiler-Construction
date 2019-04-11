#include<stdio.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"

int getVarIndex(int funcIndex,char* string)
{
  SymTableEntry t = functionTable[funcIndex]->symTable;
  int symbolTableLen = functionTable[funcIndex]->currIndex;
  for(int i=0;i<symbolTableLen;i++)
    if(strcmp(string,t[i].symName)==0)
      return i;
  return -1;
}


void checkAssignmentStatement(TreeNode root, int functionIndex)
{
    SymTableEntry symbolTable = functionTable[functionIndex]->symTable;
    int symTableLen = functionTable[functionIndex]->currIndex;
    TreeNode lhs = root->children;

    int lhsType,rhsType;
    if(lhs->next->nt == eps)
      {

        int symTableIndex = getVarIndex(functionIndex,lhs->leafInfo->lexeme)
        if(symTableIndex!=-1)
          {
            lhsType = symbolTable[symTableIndex].type;
            symbolTable[symTableIndex].varChanged = 1;
          }
        else
        {
          printf("ERROR: Variable %s not declared before use at line %d",lhs->leafInfo->lexeme,lhs->leafInfo->lineNumber);
          rhsType = evalArithmeticExpressionType(lhs->next->next->children);
          return;
        }

      }

}



int evalArithmeticExpressionType(TreeNode root,int functionIndex)
{
  if(root->id == t)
  {
    if(root->t == TK_ID)
    {
      int symIndex = getVarIndex(functionIndex,root->leafInfo->lexeme);
      if(symIndex!=-1)
        return functionTable[functionindex]->symTable[symIndex].type;

      printf("ERROR: Variable %s not declared before use at line %d",root->leafInfo->lexeme,root->leafInfo->lineNumber);
      return -1;
    }

    if(root->t == TK_NUM)
      return 0;

    if(root->t == TK_RNUM)
      return 1;

    if(root->t == TK_PLUS || root->t == TK_MINUS)
      {
        int leftType = evalArithmeticExpressionType(root->children);
        int rightType = evalArithmeticExpressionType(root->children->next);

        if(leftType == rightType && (leftType == 0 || leftType == 1))
          return leftType;

        if(leftType == rightType && leftType >= 2)
          return leftType;

        return -1;

      }

      if(root->t == TK_MUL || root->t == TK_DIV)
        {
          int leftType = evalArithmeticExpressionType(root->children);
          int rightType = evalArithmeticExpressionType(root->children->next);

          if(leftType == rightType && (leftType == 0 || leftType == 1))
            return leftType;

          if(leftType == rightType && leftType >= 2)
            return leftType;
          //TODO record types can't be mult or divd

          return -1;

        }
    //TODO Minus, DIV, MULT

  }

  if(root->nt == temp)
  {
    int index = getVarIndex(functionIndex,root->children->leafInfo->lexeme);
    if(index == -1)
      {
        //TODO error
        return -1;
      }
    int recordIndex = functionTable[functionIndex]->symTable[index]->type-2;
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
