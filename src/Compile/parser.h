#include<stdio.h>
#include<string.h>
#include<Windows.h>

//语法分析器错误类型定义
#define SIGN_UNDEFINED_ERR 1    //符号未定义错误
#define SIGN_REDEFINED_ERR 2    //符号重定义错误
#define SIGN_EXECUTE_ERR 3    //不能匹配执行语句错误
#define NO_SIGN_ERR 4    //缺少配对符号错误
#define SIGN_RESERVE_ERR 5    //以保留字开头错误
#define NO_PARA_ERR 6    //缺少形参声明错误

bool parserInit(int argc, char* argv[]);    //初始化函数：从输入文件读取数据，建立各个文件，初始化全局变量
bool parserFinal();    //结束处理函数，将var和pro数组中的元素输出到相应文件，填充输出文件
bool error(int lineNum, int errNum, const char* sign);    //错误处理函数，参数分别为行号、错误码和错误符号
bool nextToken();    //获得下一符号,true表示已到队尾，false表示还未到队尾
bool nextChar();    //获得当前符号的下一字符,true表示已到'\0'
bool isVarExisted(char* vname, char* vproc, bool vkind);    //判断变量是否已存在
bool isProExisted(char* vname);    //判断过程是否已存在，参数为过程名
int getNextToken();    //获得下一符号，指针不变

//以下是递归下降分析器中的函数
void Program();//程序
void SubProgram();//子程序
void StatementList();//说明语句表
void StatementList_();//说明与句表'
void Statement();//说明语句
void VarStatement();//变量说明
void Var();//变量
void Identifier();//标识符
void FunctionStatement();//函数说明
void Parameter();//参数
void Function();//函数体
void ExecutionStatement();//执行语句表
void ExecutionStatement_();//执行语句表’
void Execution();//执行语句
void Read();//读语句
void Write();//写语句
void Equal();//赋值语句
void Expression();//算术表达式
void Expression_();//算术表达式'
void Item();//项	
void Item_();//项'
void Factor();//因子
void Constant();//常数
void Integer();//无符号整数
void Integer_();//无符号整数’
void RelationalOperator();//关系运算符
void FunctionCall();//函数调用