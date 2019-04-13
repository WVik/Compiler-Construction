#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "typeChecker.h"
#include "codeGen.h"


void reserveMemory(FILE* fp)
{
  fprintf(fp,"section .bss\n");
  for(int i=0;i<functionTable[0]->currIndex;i++)
    fprintf(fp,"%s resd 1\n",functionTable[0]->symTable[i].symName);

  for(int i=0;i<currGlobalIndex;i++)
    fprintf(fp,"%s resd 1\n",globalTable[i].symName);
}


void generateStmtCode(TreeNode root, FILE* fp)
{

  // TreeNode temp = root->children;
  // while(temp!=NULL)
  //   {
  //     generateStmtCode(temp,fp);
  //     temp = temp->next;
  //   }
  generateStmtCodeHelper(root->children->children,fp);
}


void generateStmtCodeHelper(TreeNode root, FILE* fp)
{
  if(root->nt == typeDefinition)
    root = root->next;

  if(root->nt == declarations)
    root = root->next;

  if(root->nt == assignmentStmt)
    generateAssignmentStmt(root,fp);
}


void generateAssignmentStmt(TreeNode root,FILE* fp)
{
  fprintf(fp, "mov ecx localBuff\n");
  int prevOffset = 0;
  int offset  = generateArithmeticStmt(root->children->next->next->children,0,fp);
  int type = getType(0,root->children);
  root = root->children;
  if(type>=2)
  {
    for(int i=0;i<type-2;i++)
    {
      fprintf(fp,"mov eax ecx[%d]\n",4*i);
      fprintf(fp,"mov [%s.%s] eax\n",root->leafInfo->lexeme,root->next->leafInfo->lexeme);
    }
  }
  else
  {
    fprintf(fp,"mov eax [ecx]\n");
    fprintf(fp,"mov [%s] eax\n",root->leafInfo->lexeme);
  }


}

int generateArithmeticStmt(TreeNode root,int offset,FILE* fp)
{
  if(root->id == terminal)
  {

    // if(root->t == TK_NUM || root->t == TK_RNUM)
    // {
    //
    // }
    //
    if(root->t == TK_ID && getType(0,root)<2)
    {
        fprintf(fp, "mov edx [%s]\n",root->leafInfo->lexeme);
        fprintf(fp, "mov [ecx+%d] edx\n",offset);
        return offset+4;
    }
    else if(root->t == TK_ID)
    {
        int prevOffset = offset;
        int type = getType(0,root);
        for(int i=0;i<recordTable[type-2].numFields;i++)
        {
          fprintf(fp, "mov edx [%s.%s]",root->leafInfo->lexeme,recordTable[type-2].recordFields[i].symName);
          fprintf(fp, "mov [ecx+%d] edx\n",offset);
          offset+=4;
        }
        return offset;
    }

    if(root->t == TK_PLUS)
    {
      int prevOffset = offset;
      offset = generateArithmeticStmt(root->children,offset,fp);
      offset = generateArithmeticStmt(root->children->next,offset,fp);

      for(int i=0;i<(offset-prevOffset)/2;i+=4)
      {
        fprintf(fp,"mov edx [ecx+%d]\n",(prevOffset+offset)/2+i);
        fprintf(fp,"add [ecx+%d] edx\n",prevOffset+i);
      }
      return (prevOffset+offset)/2;
    }

    else if(root->t == TK_MINUS)
    {
      int prevOffset = offset;
      offset = generateArithmeticStmt(root->children,offset,fp);
      offset = generateArithmeticStmt(root->children->next,offset,fp);

      for(int i=0;i<(offset-prevOffset)/2;i+=4)
      {
        fprintf(fp,"mov edx [ecx+%d]\n",(prevOffset+offset)/2+i);
        fprintf(fp,"sub [ecx+%d] edx\n",prevOffset+i);
      }
      return (offset+prevOffset)/2;
    }

    else if(root->t == TK_DIV)
    {
      int prevOffset = offset;
      offset = generateArithmeticStmt(root->children,offset,fp);
      int recOrIdOffset = offset;
      offset = generateArithmeticStmt(root->children->next,offset,fp);

      fprintf(fp,"mov edx [ecx+%d]\n",recOrIdOffset);

      for(int i=prevOffset;i<recOrIdOffset;i+=4)
        fprintf(fp,"div [ecx+%d] edx",i);

      return recOrIdOffset;
    }

    else if(root->t == TK_MUL)
    {
      int prevOffset = offset;
      offset = generateArithmeticStmt(root->children,offset,fp);
      int recOrIdOffset = offset;
      offset = generateArithmeticStmt(root->children->next,offset,fp);

      fprintf(fp,"mov edx [ecx+%d]\n",recOrIdOffset);

      for(int i=prevOffset;i<recOrIdOffset;i+=4)
        fprintf(fp,"div [ecx+%d] edx",i);

      return recOrIdOffset;
    }

  }
  else{
        root = root->children;
        fprintf(fp, "mov edx [%s.%s]\n",root->leafInfo->lexeme,root->next->leafInfo->lexeme);
        fprintf(fp, "mov [ecx+%d] edx\n",offset);
        return offset+4;
  }
  return 0;
}
