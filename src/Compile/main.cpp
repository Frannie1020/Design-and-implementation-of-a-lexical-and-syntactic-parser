#include"lexical.h"
#include"parser.h"

int main(int argc, char *argv[])
{
	//初始化词法分析器并进行词法分析
	if(lexicalInit(argc, argv))
	{
		while(LexAnalyze()){}
		lexicalFinal();
	}
	//初始化语法分析器并进行语法分析
	if (parserInit(argc, argv))
	{
		Program();    //从根开始进行递归下降分析
		parserFinal();    //分析结束，进行结束处理
	}

	return 0;
}
