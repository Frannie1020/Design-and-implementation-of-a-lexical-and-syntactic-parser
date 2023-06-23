#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include"characterOperation.h"
#include"fileOperation.h"
#include"lexical.h"

//定义全局文件名
char lexicalGlobalFilename[MAX_COUNT] = "";

//返回保留字对应的种别
int reserve(char* token)
{
	if(strcmp(token, "begin") == 0)    return 1;
	else if(strcmp(token, "end") == 0)    return 2;
	else if(strcmp(token, "integer") == 0)    return 3;
	else if(strcmp(token, "if") == 0)    return 4;
	else if(strcmp(token, "then") == 0)    return 5;
	else if(strcmp(token, "else") == 0)    return 6;
	else if(strcmp(token, "function") == 0)    return 7;
	else if(strcmp(token, "read") == 0)    return 8;
	else if(strcmp(token, "write") == 0)    return 9;
	else    return 0;
}
//返回标识符的对应种别
int sysmbol()
{
	return 10;
}
//返回常数对应的种别
int constant()
{
	return 11;
}
//按照格式输出单词符号和种别
void output(const char* token, int kindNum)
{
	printf("%16s %2d\n", token, kindNum);
}
//根据行号和错误码输出错误
bool error(int lineNum, int errNum)
{
	char* errInfo;
	switch(errNum)
	{
	case ILLEGAL_CHAR_ERR:
		errInfo = "出现字母表以外的非法字符";
		break;
	case UNKNOWN_OPERATOR_ERR:
		errInfo = "出现未知运算符";
		break;
	default:
		errInfo = "未知错误";
	}
	if(fprintf(stderr,"LINE:%d %s\n", lineNum, errInfo) > 0)    return true;
	else    return false;
}
//词法分析函数，每调用一次识别一个符号
bool LexAnalyze()
{
    static int lineNum = 1;
	char character;
	char token[17] = "";
	character = getNoBlankChar();
	switch (character)
	{
	case '\n':
		output("EOLN",24);
		lineNum++;
		break;
	case EOF:
		output("EOF",25);
		return false;//表示已经读到文件末尾
		break;
	case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : case 'g' : case 'h' : case 'i' : case 'j' : 
    case 'k' : case 'l' : case 'm' : case 'n' : case 'o' : case 'p' : case 'q' : case 'r' : case 's' : case 't' : 
    case 'u' : case 'v' : case 'w' : case 'x' : case 'y' : case 'z' : case 'A' : case 'B' : case 'C' : case 'D' : 
    case 'E' : case 'F' : case 'G' : case 'H' : case 'I' : case 'J' : case 'K' : case 'L' : case 'M' : case 'N' : 
	case 'O' : case 'P' : case 'Q' : case 'R' : case 'S' : case 'T' : case 'U' : case 'V' : case 'W' : case 'X' : 
    case 'Y' : case 'Z' ://大小写字符出错判断
	    while(letter(character) || digit(character))
		{
	    	char s[2] = {character};
	    	strcat(token,s);
	    	character = getchar();
	    }
	    retract(character);
	    int num;
	    num = reserve(token);
	    if(num != 0)
		{
	    	output(token, num);
	    }
	    else
		{
	    	int val;
	    	val = sysmbol();
	    	output(token, val);
	    }
	    break;
    case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' ://数字出错情况判断
	    while(digit(character))
		{
			char s[2] = {character};
			strcat(token,s);
			character = getchar();
		}
		retract(character);
		int val;
		val = constant();
		output(token, val);
		break;
	case '=' :
	    output("=", 12);
		break;
	case '<' :
	    character = getchar();
		if(character == '>')
		{
			output("<>", 13);
		}
		else if(character == '=')
		{
			output("<=", 14);
		}
		else
		{
			retract(character);
			output("<", 15);
		}
		break;
	case '>' :
	    character = getchar();
		if(character == '=')
		{
			output(">=", 16);
		}
		else
		{
			retract(character);
			output(">", 17);
		}
		break;
	case '-' :
	    output("-", 18);
		break;
	case '*' :
	    output("*", 19);
		break;
	case ':' :
	    character = getchar();
		if(character == '=')
		{
			output(":=", 20);
		}
		else
	    {
			error(lineNum, 2);//冒号出错判断
		}
		break;
	case '(' :
	    output("(", 21);
		break;
	case ')' :
	    output(")", 22);
		break;
	case ';' :
	    output(";", 23);
		break;
	default:
	    error(lineNum, 1);//标识符出错判断    
	}
    return true;
}
//词法分析初始化函数，接收输入文件地址，并打开输入、输出、错误文件，将标准输入重定向到输入文件，将标准输出重定向到输出文件，将标准错误重定向到错误文件
bool lexicalInit(int argc, char *argv[])
{
	if (argc != 2) {
		printf("输入参数错误！\n\t使用方法：compile.exe test.cpp    ——其中test.cpp为源文件名\n");
		exit(0);
	}
	else
	{
		printf("开始词法分析:\n");
		char *inFilename = argv[1];
		char outFilename[MAX_COUNT] = "";
		char errFilename[MAX_COUNT] = "";
		char filename[MAX_COUNT] = "";
		char path[MAX_COUNT] = "";
		printf("读取到文件：%s\n", inFilename);

		//获得文件名（不包括扩展名）和路径
		getFilename(inFilename, filename);
		getPath(inFilename, path);

		//设置全局文件名
		strcpy(lexicalGlobalFilename, filename);

		//生成输出文件的全部路径
		strcat(outFilename, filename);
		strcat(outFilename, ".dyd");
		strcat(errFilename, filename);
		strcat(errFilename, ".err");
		printf("正在进行词法分析\n");
		if(freopen(inFilename, "r", stdin) != NULL && freopen(outFilename, "w", stdout) != NULL && freopen(errFilename, "w", stderr) != NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
void lexicalFinal()
{
	//关闭打开的标准输入、输出和出错
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
	//恢复重定向
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	printf("词法分析完成！\n");
	printf("词法分析结果已写入：%s.dyd\n", lexicalGlobalFilename);
	printf("词法分析错误已写入：%s.err\n", lexicalGlobalFilename);
}