#ifndef _ASSIST_
#define _ASSIST_
#include <vector>
using namespace std;
vector<string> keywords = { "constructor", "function", "method", "field", "static", "var", "int",
					"char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if",
					"else", "while", "return","class"};
vector<string> symbols = {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};
vector<string> opr = {"+","-","*","/","&","|","<",">","="};
vector<string> statm = {"let","if","while","do","return"};

bool isKey(string c)
{
	for(auto ch:keywords)
	{
		if(ch==c)
			return true;
	}
	return false;
}
bool isSymbol(string s)
{
	for(auto str:symbols)
	{
		if(str == s)
			return true;
	}
	return false;
}
//if is a number
bool isNumber(string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i]>'9'|| s[i]<'0')
			return false;
	}
	return true;
}
bool isString(string s)
{
	if(s[0]=='"')
		return true;
	return false;
}
bool isIdentifier(string s)
{
	if(isKey(s)||isSymbol(s)||isNumber(s)||isString(s))
		return false;
	return true;
}
string segment(string seg)
{
	if (seg == "static")
	{
		return "static";
	} else if (seg == "field") {
		return "this";
	} else if (seg ==  "arg") {
		return "argument";
	} else if (seg == "var") {
		return"local";
	}
}
#endif