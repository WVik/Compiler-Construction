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
  if(root->t == TK_NUM)
    return 0;
  if(root->t==TK_RNUM)
    return 1;
  int symIndex = getVarIndex(functionIndex,root->leafInfo->lexeme);
  int globalIndex = getGlobalIndex(root->leafInfo->lexeme);

  if(symIndex!=-1)
    return functionTable[functionIndex]->symTable[symIndex].type;
  else if(globalIndex!=-1)
    return globalTable[globalIndex].type;

  printf("ERROR: Variable %s not declared before use at line %d\n",root->leafInfo->lexeme,root->leafInfo->lineNumber);
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

  if(root->nt == input_par)
    root = root->next;
  if(root->nt == output_par)
    root = root->next;
  if(root->nt == typeDefinitions)
    root = root->next;
  if(root->nt == declarations)
    root = root->next;

  checkStatementSemantics(root,functionIndex);

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
      int type = getType(functionIndex,lhs);
      if(type==-1)
      {
        return;
      }
      int recIndex = type-2;
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

          if(leftType == -1 || rightType == -1)
          {
            return -1;
          }

          if(leftType == rightType && (leftType == 0 || leftType == 1))
            return leftType;

          if(root->t == TK_MUL)
          {
            if(leftType>=2 && rightType<2)
              return leftType;

            if(rightType>=2 && leftType<2)
              return rightType;

            if(rightType+leftType >= 4)
              {
                printf("Line %d: Records can't be multipled\n",root->leafInfo->lineNumber);
              }

            return -1;
          }

          if(root->t == TK_DIV)
          {
            if(leftType>=2 && rightType<2)
              return leftType;
            if(rightType>=2)
            {
              printf("Line %d: Cannot divide by a record\n",root->leafInfo->lineNumber);
            }
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
  return -1;

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
      //printf("Line %d: Invalid variable %s\n", temp->leafInfo->lineNumber, temp->leafInfo->lexeme);
      break;
    }
    //If formalType is -1, it means the error was already declared
    if(formalType == -1)
      break;

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
     //printf("Line %d: Invalid variable %s\n", temp->leafInfo->lineNumber, temp->leafInfo->lexeme);
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


void checkBooleanSemantics(TreeNode root,int functionIndex)
{
  if(root->t == TK_LT || root->t == TK_LE || root->t == TK_EQ || root->t == TK_GT || root->t == TK_GE || root->t == TK_NE)
  {
    int lhsType = getType(functionIndex,root->children);
    int rhsType = getType(functionIndex,root->children->next);
    if(lhsType == -1 || rhsType == -1)
      return;
    else if(lhsType == rhsType)
      return;
    else
    {
      printf("Line %d : ERROR Type Mismatch -> Comparison of %s of type %s with %s of type %s\n",root->children->leafInfo->lineNumber,root->children->leafInfo->lexeme,getTypeString(lhsType),root->children->next->leafInfo->lexeme,getTypeString(rhsType));
      return;
    }
  }
  if(root->t == TK_AND || root->t == TK_OR)
  {
    checkBooleanSemantics(root->children,functionIndex);
    checkBooleanSemantics(root->children->next,functionIndex);
  }
  if(root->t == TK_NOT)
  {
    checkBooleanSemantics(root->children,functionIndex);
  }

}

void checkIterativeStatementSemantics(TreeNode root,int functionIndex)
{
  int startLine = root->children->children->leafInfo->lineNumber;
  int endLine = 0;
  TreeNode stmtNode = root->children->next;
  while(stmtNode->id!=terminal)
  {
    stmtNode = stmtNode->next;
  }
  endLine = stmtNode->leafInfo->lineNumber;
  if(isValidWhile(root->children->children,root->children->next) == 0)
    printf("Line %d-%d None of the variables participating in the iterations of the while loop gets updated \n",startLine,endLine);
  checkBooleanSemantics(root->children->children,functionIndex);
  checkStatementSemantics(root->children->next,functionIndex);
}


int checkIfVarUpdated(char* name,TreeNode stmtNode)
{
  while(stmtNode!=NULL)
  {
    if(stmtNode->nt == assignmentStmt)
      if(strcmp(stmtNode->children->leafInfo->lexeme,name)==0)
        return 1;
    if(stmtNode->nt == ioStmt)
      if(stmtNode->children->t == TK_READ)
        if(strcmp(stmtNode->children->next->leafInfo->lexeme,name)==0)
          return 1;
    stmtNode = stmtNode->next;
  }
  return 0;
}


int isValidWhile(TreeNode root,TreeNode stmtNode)
{
  if(root->t == TK_AND || root->t==TK_OR)
    return isValidWhile(root->children,stmtNode) || isValidWhile(root->children->next,stmtNode);
  if(root->t == TK_LT || root->t == TK_LE || root->t == TK_EQ || root->t == TK_GT || root->t == TK_GE || root->t == TK_NE)
    return isValidWhile(root->children,stmtNode) || isValidWhile(root->children->next,stmtNode);
  if(root->t == TK_NOT)
    return isValidWhile(root->children,stmtNode);
  if(root->t == TK_NUM || root->t == TK_RNUM)
    return 0;
  if(root->t == TK_ID)
    return checkIfVarUpdated(root->leafInfo->lexeme,stmtNode);
  return 0;
}

void checkConditionalStatementSemantics(TreeNode root,int functionIndex)
{
  checkBooleanSemantics(root->children->children,functionIndex);
  if(root->children->next->nt ==elsePart)
  {
    checkStatementSemantics(root->children->next->children,functionIndex);
    checkStatementSemantics(root->children->next->next,functionIndex);
    return;
  }
  checkStatementSemantics(root->children->next,functionIndex);
}


// void checkReturnSemantics(TreeNode root,functionIndex)
// {
//   temp = root->children;
//   int numOutputParams = functionTable[functionIndex]->numOutputParams;
//
//   for(int i=0;i<numOutputParams;i++)
//   {
//     int actualType = getType(functionIndex, temp);
//
//     int formalType = functionTable[functionIndex]->outputParType[i];
//     if(actualType == -1)
//     {
//       printf("Line %d: Invalid variable %s\n", temp->leafInfo->lineNumber, temp->leafInfo->lexeme);
//       return;
//     }
//     //If formalType is -1, it means the error was already declared
//     if(formalType == -1)
//       return;
//
//     if(formalType!=actualType)
//     {
//       printf("Line %d: Function type mismatch. Parameter number %d is expected to be of type %s\n",temp->leafInfo->lineNumber,i+1,getTypeString(formalType));
//     }
//
//     temp = temp->next;
//   }
// }


void checkStatementSemantics(TreeNode root,int functionIndex)
{
  while(root!=NULL || (root->id == terminal && root->t == TK_ENDWHILE))
  {
    // if(root->nt == ioStmt)
    //   checkIOStatementSemantics(root,functionIndex);
    if(root->nt == assignmentStmt)
      checkAssignmentStatement(root,functionIndex);
    else if(root->nt == conditionalStmt)
      checkConditionalStatementSemantics(root,functionIndex);
    else if(root->nt == iterativeStmt)
      checkIterativeStatementSemantics(root,functionIndex);
    else if(root->nt == funCallStmt)
      checkFunctionCallSemantics(root,functionIndex);
    // else if(root->nt == returnStmt)
    //   checkReturnSemantics(root,functionIndex);

    root = root->next;
    if(root == NULL)
      return;

  }
}
