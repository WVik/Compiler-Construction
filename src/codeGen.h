void reserveMemory(FILE* fp);
void generateStmtCode(TreeNode root,FILE *fp);
void generateStmtCodeHelper(TreeNode root,FILE* fp);
void generateAssignmentStmt(TreeNode root,FILE* fp);
int generateArithmeticStmt(TreeNode root,int offset,FILE* fp);

int labelCount;
