#include<stdio.h>
#include<string.h>
#include<Windows.h>

//从文件读入第一个非空白字符
char getNoBlankChar()
{
    char ch;
	ch = getchar();
	while(1)
	{
		if(ch == '\r' || ch == '\t' || ch == ' ')    ch = getchar();
		else    break;
	}
	return ch;
}
//回退一个字符
void retract(char& character)
{
	ungetc(character, stdin);
	character = NULL;
}
//获得路径
void getPath(char *in, char *out)
{
	char *name;
	name = strrchr(in, '\\');
	if(name != NULL)    strncpy(out, in, strlen(in) - strlen(name) + 1);
	else    strcpy(out, "");
}
//获得文件名，不包括扩展
void getFilename(char *in, char *out)
{
	char *fullName;
	char *extension;
	fullName = strrchr(in, '\\');
	extension = strrchr(in, '.');
	if(fullName != NULL)    strncpy(out, fullName + 1, strlen(fullName) - 1 - strlen(extension));
	else    strncpy(out, in, strlen(in) - strlen(extension));
}