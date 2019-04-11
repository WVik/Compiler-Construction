void checkFunctionSemantics(TreeNode root, int functionIndex);
int evalArithmeticExpressionType(TreeNode root,int functionIndex);
void checkAssignmentStatement(TreeNode root, int functionIndex);
int getGlobalIndex(char* varName);
int getVarIndex(int funcIndex,char* string);
int getType(int functionIndex,TreeNode root);
char* getTypeString(int type);
void checkFunctionCallSemantics(TreeNode root, int functionIndex);
