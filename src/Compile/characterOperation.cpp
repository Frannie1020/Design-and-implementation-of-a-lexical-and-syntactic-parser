#include<stdio.h>
#include<string.h>
#include<Windows.h>

//判断character是否为字母
bool letter(char character)
{
	if((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))    return true;
	else    return false;
}

//判断character是否为数字
bool digit(char character)
{
	if(character >= '0' && character <= '9')    return true;
	else    return false;
}