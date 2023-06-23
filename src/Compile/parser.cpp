#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include"characterOperation.h"
#include"fileOperation.h"
#include"parser.h"

//定义全局文件名
char parserGlobalFilename[MAX_COUNT] = "";

//types是支持数据类型的集合
typedef enum { integer } types;
//记录变量信息的结构体
typedef struct {
	char vname[17];
	char vproc[17];
	bool vkind;
	types vtype;
	int vlev;
	int vadr;
} varRecord;
//记录过程信息的结构体
typedef struct {
	char pname[17];
	types ptype;
	int plev;
	int varNum;
	int fadr;
	int ladr;
	int parameter;
	bool parameterIsDefined;
} proRecord;

char input[MAX_COUNT][17];//存放输入文件所有符号的数组
int kind[MAX_COUNT];
int inputCount;//输入符号的数量
int pToken;//指向当前输入符号
int pChar;//指向当前输入符号中的当前字符

varRecord currentVar;//存放当前变量的信息
proRecord currentPro;//存放当前过程的信息

int lineNum;//当前行号

varRecord var[MAX_COUNT];//存放变量名表项数组
proRecord pro[MAX_COUNT];//存放过程名表项数组
int varCount;//变量的数量
int proCount;//过程的数量

FILE* inFile;//输入文件句柄
FILE* outFile;//输出文件句柄
FILE* errFile;//错误文件句柄
FILE* varFile;//变量文件句柄
FILE* proFile;//过程文件句柄

bool parserInit(int argc, char* argv[])
{
	printf("\n开始语法分析\n");
	if (argc != 2)
	{
		return false;
	}
	else
	{
		char* inFilename = argv[1];
		//char* inFilename = "source.dyd";
		char outFilename[MAX_COUNT] = "";
		char errFilename[MAX_COUNT] = "";
		char varFilename[MAX_COUNT] = "";
		char proFilename[MAX_COUNT] = "";
		char filename[MAX_COUNT] = "";
		char path[MAX_COUNT] = "";
		//获得文件名（不包括扩展名）和路径
		getFilename(inFilename, filename);
		getPath(inFilename, path);

		//设置全局文件名
		strcpy(parserGlobalFilename, filename);

		//重新生成输入文件全部路径
		strcpy(inFilename, filename);
		strcat(inFilename, ".dyd");

		//生成输出文件全部路径
		strcat(outFilename, path);
		strcat(outFilename, filename);
		strcat(outFilename, ".dys");

		//生成错误文件全部路径
		strcat(errFilename, path);
		strcat(errFilename, filename);
		strcat(errFilename, ".err");

		//生成变量文件全部路径
		strcat(varFilename, path);
		strcat(varFilename, filename);
		strcat(varFilename, ".var");

		//生成过程文件全部路径
		strcat(proFilename, path);
		strcat(proFilename, filename);
		strcat(proFilename, ".pro");
		//打开文件句柄
		printf("读取到文件：%s\n", inFilename);
		printf("正在进行语法分析\n");
		if ((inFile = fopen(inFilename, "r")) && (outFile = fopen(outFilename, "w")) && (errFile = fopen(errFilename, "w")) && (varFile = fopen(varFilename, "w")) && (proFile = fopen(proFilename, "w")))
		{
			//初始化单词指针、字符指针、行号、层次
			inputCount = 0;
			pToken = 0;
			pChar = 0;
			lineNum = 1;//当前行号
			//level = 0;//当前层次
			//varCountInPro = 0;
			strcpy(currentPro.pname, "");
			currentPro.plev = 0;
			currentPro.varNum = 0;
			currentPro.parameter = -1;

			varCount = 0;
			proCount = 0;
			//读取输入文件内容，初始化input数组
			while (!feof(inFile))
			{
				char stringOfLine[MAX_COUNT];
				if (fgets(stringOfLine, MAX_COUNT, inFile))
				{
					char lineString[20] = "";
					strncpy(lineString, stringOfLine, 19);
					char* kindString = strrchr(lineString, ' ');
					kind[inputCount] = atoi(kindString + 1);

					char string[17] = "";
					strncpy(string, stringOfLine, 16);
					char* lastString = strrchr(string, ' ');
					strcpy(input[inputCount], lastString + 1);

					inputCount++;
				}
			}
			return true;
		}
		else
		{
			fclose(inFile);
			fclose(outFile);
			fclose(errFile);
			fclose(varFile);
			fclose(proFile);
			return false;
		}


	}
}
bool parserFinal()
{
	for (int i = 0; i < varCount; i++)
	{
		int vkind = var[i].vkind ? 1 : 0;
		char* vtype = (var[i].vtype == integer) ? "integer" : "";
		fprintf(varFile, "%16s %16s %d %s %d %d\n", var[i].vname, var[i].vproc, vkind, vtype, var[i].vlev, var[i].vadr);
	}
	for (int i = 0; i < proCount; i++)
	{
		char* ptype = (pro[i].ptype == integer) ? "integer" : "";
		fprintf(proFile, "%16s %s %d %d %d\n", pro[i].pname, ptype, pro[i].plev, pro[i].fadr, pro[i].ladr);
	}
	if (fseek(inFile, 0, 0) == 0)
	{
		while (!feof(inFile))
			fputc(fgetc(inFile), outFile);
	}

	bool val;
	val = fclose(inFile);
	val = fclose(outFile);
	val = fclose(errFile);
	val = fclose(varFile);
	val = fclose(proFile);
	printf("语法分析完成！\n");
	printf("语法分析结果已写入：%s.dys\n", parserGlobalFilename);
	printf("变量表已写入：%s.var\n", parserGlobalFilename);
	printf("过程表已写入：%s.pro\n", parserGlobalFilename);
	printf("语法分析错误已写入：%s.err\n", parserGlobalFilename);
	return val;
}
bool error(int errNum, const char* symbol)
{
	char* errInfo;
	switch (errNum)
	{
	case SIGN_UNDEFINED_ERR:
		fprintf(errFile, "LINE:%d  %s符号无定义\n", lineNum, input[pToken]);
		break;
	case SIGN_REDEFINED_ERR:
		fprintf(errFile, "LINE:%d  %s符号重定义\n", lineNum, input[pToken]);
		break;
	case SIGN_EXECUTE_ERR:
		fprintf(errFile, "LINE:%d  %s处不能匹配执行语句\n", lineNum, input[pToken]);
		break;
	case NO_SIGN_ERR:
		fprintf(errFile, "LINE:%d  %s处缺少%s\n", lineNum, input[pToken], symbol);
		break;
	case SIGN_RESERVE_ERR:
		errInfo = "以保留字开头";
		break;
	case NO_PARA_ERR:
		//fprintf(errFile, "LINE:%d  缺少形参%s的声明\n", lineNum, symbol);
		break;
	default:
		errInfo = "未知错误";
	}
	return true;
}

bool nextToken()
{
	pToken++;
	pChar = 0;
	if (strcmp(input[pToken], "EOF") == 0)
	{
		return true;
	}
	while (strcmp(input[pToken], "EOLN") == 0)
	{
		pToken++;
		lineNum++;
	}
	return false;
}
bool nextChar()
{
	if (input[pToken][pChar] == '\0')
	{
		//nextToken();
		return true;
	}
	pChar++;
	return false;

}
bool isVarExisted(char* vname, char* vproc, bool vkind)
{
	for (int i = 0; i < varCount; i++)
	{
		if ((strcmp(vname, var[i].vname) == 0) && (strcmp(vproc, var[i].vproc) == 0) && (var[i].vkind == vkind))
			return true;
	}
	for (int i = 0; i < proCount; i++)
	{
		if (strcmp(vname, pro[i].pname) == 0)
			return true;
	}
	return false;
}
bool isProExisted(char* vname)
{
	for (int i = 0; i < varCount; i++)
	{
		if (strcmp(vname, var[i].vname) == 0)
			return true;
	}
	for (int i = 0; i < proCount; i++)
	{
		if (strcmp(vname, pro[i].pname) == 0)
			return true;
	}
	return false;
}
int getNextToken()
{
	int pNextToken = pToken + 1;
	while (strcmp(input[pNextToken], "EOLN") == 0)
	{
		pNextToken++;
	}
	return pNextToken;
}

void Program()
{
	SubProgram();
}
void SubProgram()
{
	if (strcmp(input[pToken], "begin") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "begin");
		if (strcmp(input[pToken], "integer") != 0)
		{
			nextToken();
		}
	}
	StatementList();
	if (strcmp(input[pToken], ";") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ";");
		if ((strcmp(input[pToken], "integer") != 0) && (strcmp(input[pToken], "read") != 0) && (strcmp(input[pToken], "write") != 0) && (kind[pToken] != 10))
		{
			nextToken();
		}
	}
	ExecutionStatement();
	if (strcmp(input[pToken], "end") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "end");
	}
}
void StatementList()
{
	Statement();
	StatementList_();
}
void StatementList_()
{

	if (strcmp(input[pToken], ";") == 0 && strcmp(input[getNextToken()], "integer") == 0)
	{
		nextToken();
		Statement();
		StatementList_();
	}
	else
	{
		if (strcmp(input[pToken], "integer") == 0)
		{
			error(NO_SIGN_ERR, ";");
			Statement();
			StatementList_();
		}
	}

}
void Statement()
{
	if (strcmp(input[pToken + 1], "function") == 0)
	{
		FunctionStatement();
	}
	else
	{
		VarStatement();
	}
}
void VarStatement()
{
	if (strcmp(input[pToken], "integer") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "integer");
		nextToken();
	}
	strcpy(currentVar.vname, input[pToken]);
	strcpy(currentVar.vproc, currentPro.pname);
	if (pToken == currentPro.parameter)
	{
		currentVar.vkind = true;
		currentPro.parameterIsDefined = true;
	}
	else
	{
		currentVar.vkind = false;
	}
	currentVar.vtype = integer;
	currentVar.vlev = currentPro.plev;
	currentVar.vadr = varCount;
	if (isVarExisted(input[pToken], currentPro.pname, currentVar.vkind))//如果存在变量
	{
		error(SIGN_REDEFINED_ERR, NULL);
	}
	else
	{
		if (currentPro.varNum == 0)//如果当前过程中变量数为0，则当前变量是当前过程的第一个变量
		{
			currentPro.fadr = currentVar.vadr;
		}
		currentPro.ladr = currentVar.vadr;//过程中最后一个变量在变量表中的位置
		currentPro.varNum++;//过程中变量数++
		var[varCount] = currentVar;//当前变量存入var数组
		varCount++;//变量数++
	}
	Var();
}
void Var()
{
	Identifier();
}
void Identifier()
{
	if (kind[pToken] == 10)
	{
		nextToken();
	}
}


void FunctionStatement()
{
	proRecord proBackup = currentPro;//备份当前过程，在匹配完G过程后恢复
	if (strcmp(input[pToken], "integer") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "integer");
		if (strcmp(input[pToken], "function") != 0)
		{
			nextToken();
		}
	}
	if (strcmp(input[pToken], "function") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "function");
		if (kind[pToken] != 10)
		{
			nextToken();
		}
	}
	strcpy(currentPro.pname, input[pToken]);
	currentPro.ptype = integer;
	currentPro.plev++;
	currentPro.varNum = 0;
	currentPro.parameterIsDefined = false;
	if (isProExisted(input[pToken]))
	{
		error(SIGN_REDEFINED_ERR, NULL);
	}
	Identifier();
	if (strcmp(input[pToken], "(") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "(");
		if (kind[pToken] != 10)
		{
			nextToken();
		}
	}
	currentPro.parameter = pToken;
	Parameter();
	if (strcmp(input[pToken], ")") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ")");
		if (strcmp(input[pToken], ";") != 0)
		{
			nextToken();
		}
	}
	if (strcmp(input[pToken], ";") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ";");
		if (strcmp(input[pToken], "begin") != 0)
		{
			nextToken();
		}
	}
	Function();
	currentPro = proBackup;//匹配完G过程后恢复原过程
}
void Parameter()
{
	Var();
}
void Function()
{
	if (strcmp(input[pToken], "begin") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "begin");
		if (strcmp(input[pToken], "integer") != 0)
		{
			nextToken();
		}
	}
	StatementList();
	if (!currentPro.parameterIsDefined)
	{
		error(NO_PARA_ERR, input[currentPro.parameter]);
	}
	pro[proCount] = currentPro;//在这里而不是在J()函数最后把currentPro加入pro数组是因为M中可能会使用当前过程(递归)
	proCount++;
	if (strcmp(input[pToken], ";") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ";");
		if ((strcmp(input[pToken], "integer") != 0) && (strcmp(input[pToken], "read") != 0) && (strcmp(input[pToken], "write") != 0) && (kind[pToken] != 10))
		{
			nextToken();
		}
	}
	ExecutionStatement();
	if (strcmp(input[pToken], "end") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "end");
		if ((strcmp(input[pToken], ";") != 0) && (strcmp(input[pToken], "end") != 0))
		{
			nextToken();
		}
	}
}
void ExecutionStatement()
{
	Execution();
	ExecutionStatement_();
}
void ExecutionStatement_()
{
	if (strcmp(input[pToken], ";") == 0)
	{
		nextToken();
		Execution();
		ExecutionStatement_();
	}
	else
	{
		if ((strcmp(input[pToken], "end") != 0) && (strcmp(input[pToken], "EOF") != 0))
		{
			error(NO_SIGN_ERR, ";");
			Execution();
			ExecutionStatement_();
		}
	}
}
void Execution()
{
	if (strcmp(input[pToken], "read") == 0)
	{
		Read();
	}
	else if (strcmp(input[pToken], "write") == 0)
	{
		Write();
	}
	else if (strcmp(input[pToken], "if") == 0)
	{
		Integer();
	}
	else if (kind[pToken] == 10)
	{
		Equal();
	}
	else
	{
		error(SIGN_EXECUTE_ERR, NULL);
		nextToken();
	}
}
void Read()
{
	if (strcmp(input[pToken], "read") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "read");
		if (strcmp(input[pToken], "(") != 0)
		{
			nextToken();
		}
	}
	if (strcmp(input[pToken], "(") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "(");
		if (kind[pToken] != 10)
		{
			nextToken();
		}
	}
	if (!isVarExisted(input[pToken], currentPro.pname, false) && !isVarExisted(input[pToken], currentPro.pname, true))
	{
		error(SIGN_UNDEFINED_ERR, NULL);
	}
	Var();
	if (strcmp(input[pToken], ")") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ")");
		if ((strcmp(input[pToken], ";") != 0) && (strcmp(input[pToken], "end") != 0))
		{
			nextToken();
		}
	}
}
void Write()
{
	if (strcmp(input[pToken], "write") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "write");
		if (strcmp(input[pToken], "(") != 0)
		{
			nextToken();
		}
	}
	if (strcmp(input[pToken], "(") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "(");
		if (kind[pToken] != 10)
		{
			nextToken();
		}
	}
	if (!isVarExisted(input[pToken], currentPro.pname, false) && !isVarExisted(input[pToken], currentPro.pname, true))
	{
		error(SIGN_UNDEFINED_ERR, NULL);
	}
	Var();
	if (strcmp(input[pToken], ")") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ")");
		if ((strcmp(input[pToken], ";") != 0) && (strcmp(input[pToken], "end") != 0))
		{
			nextToken();
		}
	}
}
void Equal()
{
	if (!isVarExisted(input[pToken], currentPro.pname, false) && !isVarExisted(input[pToken], currentPro.pname, true) && !isProExisted(input[pToken]))
	{
		error(SIGN_UNDEFINED_ERR, NULL);
	}
	Var();
	if (strcmp(input[pToken], ":=") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ":=");
		if ((kind[pToken] != 10) && (kind[pToken] != 11))
		{
			nextToken();
		}
	}
	Expression();
}
void Expression()
{
	Item();
	Expression_();
}
void Expression_()
{
	if (strcmp(input[pToken], "-") == 0)
	{
		nextToken();
		Item();
		Expression_();
	}
	else
	{
		if ((kind[pToken] == 10) || (kind[pToken] == 11))
		{
			Item();
			Expression_();
		}
	}
}
void Item()
{
	Factor();
	Item_();
}
void Item_()
{
	if (strcmp(input[pToken], "*") == 0)
	{
		nextToken();
		Factor();
		Item_();
	}
	else
	{
		if ((kind[pToken] == 10) || (kind[pToken] == 11))
		{
			Factor();
			Item_();
		}
	}
}
void Factor()
{
	if (input[pToken][pChar] >= '0' && input[pToken][pChar] <= '9')
	{
		Constant();
	}
	else if (strcmp(input[getNextToken()], "(") == 0)
	{
		FunctionCall();
	}
	else
	{
		if (!isVarExisted(input[pToken], currentPro.pname, false) && !isVarExisted(input[pToken], currentPro.pname, true))
		{
			error(SIGN_UNDEFINED_ERR, NULL);
		}
		Var();
	}
}
void Constant()
{
	if (kind[pToken] == 11)
	{
		nextToken();
	}
}

void Integer()
{
	if (strcmp(input[pToken], "if") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "if");
		if ((kind[pToken] != 10) && (kind[pToken] != 11))
		{
			nextToken();
		}
	}
	Integer_();
	if (strcmp(input[pToken], "then") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "then");
		if ((strcmp(input[pToken], "integer") != 0) && (strcmp(input[pToken], "read") != 0) && (strcmp(input[pToken], "write") != 0) && (kind[pToken] != 10))
		{
			nextToken();
		}
	}
	Execution();
	if (strcmp(input[pToken], "else") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "else");
		if ((strcmp(input[pToken], "integer") != 0) && (strcmp(input[pToken], "read") != 0) && (strcmp(input[pToken], "write") != 0) && (kind[pToken] != 10))
		{
			nextToken();
		}
	}
	Execution();
}
void Integer_()
{
	Expression();
	RelationalOperator();
	Expression();
}
void RelationalOperator()
{
	if (strcmp(input[pToken], "<") == 0 || strcmp(input[pToken], "<=") == 0 || strcmp(input[pToken], ">") == 0 || strcmp(input[pToken], ">=") == 0 || strcmp(input[pToken], "=") == 0 || strcmp(input[pToken], "<>") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "关系运算符");
		if ((kind[pToken] != 10) && (kind[pToken] != 11))
		{
			nextToken();
		}
	}
}
void FunctionCall()
{
	if (!isProExisted(input[pToken]))
	{
		error(SIGN_UNDEFINED_ERR, NULL);
	}
	Identifier();
	if (strcmp(input[pToken], "(") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, "(");
		if ((kind[pToken] != 10) && (kind[pToken] != 11))
		{
			nextToken();
		}
	}
	Expression();
	if (strcmp(input[pToken], ")") == 0)
	{
		nextToken();
	}
	else
	{
		error(NO_SIGN_ERR, ")");
		if ((strcmp(input[pToken], "-") != 0) && (strcmp(input[pToken], "*") != 0) && (strcmp(input[pToken], ";") != 0) && (strcmp(input[pToken], "end")))
		{
			nextToken();
		}
	}
}