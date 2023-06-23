#include<stdio.h>
#include<string.h>
#include<Windows.h>

//词法分析器错误类型定义
#define ILLEGAL_CHAR_ERR 1    //非法字符错误
#define UNKNOWN_OPERATOR_ERR 2    //未知运算符错误

int reserve(char* token);    //返回保留字对应的种别
int sysmbol();    //返回标识符的对应种别
int constant();    //返回常数对应的种别
void output(const char* token, int kindNum);    //按照格式输出单词符号和种别
bool error(int lineNum, int errNum);    //根据行号和错误码输出错误
bool LexAnalyze();    //词法分析函数，每调用一次识别一个符号
bool lexicalInit(int argc, char *argv[]);    //词法分析初始化函数，接收输入文件地址，并打开输入、输出、错误文件，将标准输入重定向到输入文件，将标准输出重定向到输出文件，将标准错误重定向到错误文件
void lexicalFinal();    //结束处理函数，有关于词法分析结束关闭文件，显示输出路径