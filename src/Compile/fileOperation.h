#include<stdio.h>
#include<string.h>
#include<Windows.h>

#define MAX_COUNT 1024    // 文件名最大长度

char getNoBlankChar();    //从文件读入第一个非空白字符
void retract(char& character);    //回退一个字符
void getPath(char *in, char *out);    //获得路径
void getFilename(char *in, char *out);    //获得文件名，不包括扩展