
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <dirent.h>
#include "Parser.h"
using namespace std;

int indent = 0;
vector<string> keywords = { "constructor", "function", "method", "field", "static", "var", "int",
					"char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if",
					"else", "while", "return","class"};
vector<string> symbols = {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};
vector<string> opr = {"+","-","*","/","&","|","<",">","="};
vector<string> statm = {"let","if","while","do","return"};
vector<string> cmd;
tokens token;

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
bool isOpr(string s)
{
	for(auto ch:opr)
	{
		if(ch==s)
			return true;
	}
	return false;
}
bool isstatment(string s)
{
	for(auto ch:statm)
	{
		if(ch==s)
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
void compileClass()
{
	singleTagBegin("class");
	writeKey();
	token.next();
	writeIdentifier();
	token.next();
	writeSymbol();
	token.next();

	while(token.get() == "field" || token.get() == "static")
	{
		compileClassVarDec();
	}
	while(token.get() == "constructor" || token.get() == "function" || token.get() == "method")
	{
		compileSubroutine();
	}

	writeSymbol();
	singleTagEnd("class");
}
void compileClassVarDec()
{
	singleTagBegin("classVarDec");
	writeKey();
	token.next();
	writeType();
	token.next();
	writeIdentifier();
	token.next();
	while(token.get()==",")
	{
		writeSymbol();
		token.next();
		writeIdentifier();
		token.next();
	}
	writeSymbol();
	singleTagEnd("classVarDec");
	token.next();
}
void compileSubroutine()
{
	singleTagBegin("subroutineDec");

	writeKey();
	token.next();
	writeType();
	token.next();
	writeIdentifier();
	token.next();
	writeSymbol();
	token.next();
	compileParameterlist();
	writeSymbol();
	token.next();
	compileSubroutinebody();
	singleTagEnd("subroutineDec");

}
void compileParameterlist()
{
	singleTagBegin("parameterList");
	if(!isSymbol(token.get()))
	{
		writeType();
		token.next();
		writeIdentifier();
		token.next();
		while(token.get() == ",")
		{
			writeSymbol();
			token.next();
			writeType();
			token.next();
			writeIdentifier();
			token.next();
		}
	}
	singleTagEnd("parameterList");
}

void compileSubroutinebody()
{
	singleTagBegin("subroutineBody");
	writeSymbol();
	token.next();
	while(token.get() == "var")
		compileVarDec();
	compileStatements();
	writeSymbol();
	singleTagEnd("subroutineBody");
	token.next();
}
void compileVarDec()
{
	singleTagBegin("varDec");
	writeKey();
	token.next();
	writeType();
	token.next();
	writeIdentifier();
	token.next();
	while(token.get() == ",")
	{
		writeSymbol();
		token.next();
		writeIdentifier();
		token.next();
	}
	writeSymbol();
	singleTagEnd("varDec");
	token.next();
}
void compileStatements()
{
	singleTagBegin("statements");
	while(token.get()=="let"||token.get()=="do"||token.get()=="while"||token.get()=="if"||token.get()=="return")
	{
		if(token.get()=="let")
			compileLet();
		else if(token.get()=="do")
			compileDo();
		else if(token.get()=="while")
			compileWhile();
		else if(token.get()=="if")
			compileIf();
		else if(token.get()=="return")
			compileReturn();
	}

	singleTagEnd("statements");

}
void compileLet()
{
	singleTagBegin("letStatement");
	writeKey();
	token.next();
	writeIdentifier();
	token.next();
	if(token.get() == "[")
	{
		writeSymbol();
		token.next();
		compileExpression();
		writeSymbol();
		token.next();
	}
	writeSymbol();
	token.next();
	compileExpression();
	writeSymbol();
	singleTagEnd("letStatement");
	token.next();
}
void compileDo()
{
	singleTagBegin("doStatement");
	writeKey();
	token.next();
	compileSubroutineCall();
	writeSymbol();

	singleTagEnd("doStatement");
	token.next();
}

void compileWhile()
{
	singleTagBegin("whileStatement");
	writeKey();
	token.next();
	writeSymbol();
	token.next();
	compileExpression();
	writeSymbol();
	token.next();
	writeSymbol();
	token.next();
	compileStatements();
	writeSymbol();

	singleTagEnd("whileStatement");
	token.next();
}
void compileIf()
{
	singleTagBegin("ifStatement");
	writeKey();
	token.next();
	writeSymbol();
	token.next();
	compileExpression();
	writeSymbol();
	token.next();
	writeSymbol();
	token.next();
	compileStatements();
	writeSymbol();
	token.next();

	if(token.get()=="else")
	{
		writeKey();
		token.next();
		writeSymbol();
		token.next();
		compileStatements();
		writeSymbol();
		token.next();
	}
	singleTagEnd("ifStatement");
}


void compileExpression()
{
	singleTagBegin("expression");
	compileTerm();
	while(isOpr(token.get()))
	{
		writeSymbol();
		token.next();
		compileTerm();
	}
	string test = token.get();
	singleTagEnd("expression");
}
void compileExpressionList()
{
	singleTagBegin("expressionList");
	if(!isSymbol(token.get())||token.get() == "(")
		compileExpression();
	while(token.get()==",")
	{
		writeSymbol();
		token.next();
		compileExpression();
	}
	singleTagEnd("expressionList");
}
void compileSubroutineCall()
{
	writeIdentifier();
	token.next();
	if(token.get() == "(")
	{
		writeSymbol();
		token.next();
		compileExpressionList();
		writeSymbol();
	}
	else
	{
		writeSymbol();
		token.next();
		writeIdentifier();
		token.next();
		writeSymbol();
		token.next();
		compileExpressionList();
		writeSymbol();
	}
	token.next();
}
void compileTerm()
{
	singleTagBegin("term");
	string id;
	string tmp;
	if(isKey(token.get()))
	{
		writeType();
	}
	else if(isNumber(token.get()))
	{
		writeType();
	}
	else if(isString(token.get()))
	{
		writeType();
	}
	else if(isSymbol(token.get()))
	{
		if(token.get() == "(")
		{
			writeSymbol();
			token.next();
			compileExpression();
			writeSymbol();
		}
		else
		{
			writeSymbol();
			token.next();
			compileTerm();
			singleTagEnd("term");
			return ;

		}
	}
	else
	{
		id = token.get();
		token.next();
		if(token.get()=="[")
		{
			tmp = token.get();
			token.set(id);
			writeIdentifier();
			token.set(tmp);
			writeSymbol();
			token.next();
			compileExpression();
			writeSymbol();
			token.next();
		}
		else if(token.get()=="."||token.get()=="(")
		{
			compileSubroutineCall(id);
		}
		else
		{
			tmp = token.get();
			token.set(id);
			writeIdentifier();
			token.set(tmp);

		}
		singleTagEnd("term");
		return;
	}
	token.next();
	singleTagEnd("term");
}
void compileSubroutineCall(string name)
{
	string tmp = token.get();
	token.set(name);
	writeIdentifier();
	token.set(tmp);

	if(token.get() == "(")
	{
		writeSymbol();
		token.next();
		compileExpressionList();
		writeSymbol();
	}
	else
	{
		writeSymbol();
		token.next();
		writeIdentifier();
		token.next();
		writeSymbol();
		token.next();
		compileExpressionList();
		writeSymbol();
	}
	token.next();
}
void compileReturn()
{
	singleTagBegin("returnStatement");
	writeKey();
	token.next();
	if(!isSymbol(token.get())||(isSymbol(token.get())&&token.get()!=";"))
		compileExpression();
	writeSymbol();
	singleTagEnd("returnStatement");
	token.next();
}
void writeType()
{
	string type = writeindent();
	if(isKey(token.get()))
	{
		type+="<keyword> "+ token.get() + " </keyword>";
	}
	else if(isNumber(token.get()))
	{
		type+="<integerConstant> "+ token.get() + " </integerConstant>";
	}
	else if(isString(token.get()))
	{
		type+="<stringConstant> "+ token.get().substr(1,token.get().length()-2) + " </stringConstant>";
	}
	else
	{
		type+="<identifier> "+ token.get() + " </identifier>";
	}
	cmd.push_back(type);
}

void writeSymbol()
{
	string sym = writeindent();
	if(token.get() == "<")
	{
		sym+="<symbol> &lt; </symbol>";
	}
	else if(token.get() == "&")
	{
		sym+="<symbol> &amp; </symbol>";
	}
	else if(token.get() == ">")
	{
		sym+="<symbol> &gt; </symbol>";
	}
	else
	{
		sym += "<symbol> ";
		sym += token.get();
		sym+= " </symbol>";
	}
	cmd.push_back(sym);
}

void writeIdentifier()
{
	string id = writeindent();
	id += "<identifier> " + token.get() + " </identifier>";
	cmd.push_back(id);
}
void writeKey()
{
	string key = writeindent();
	key+="<keyword> " + token.get() + "</keyword>";
	cmd.push_back(key);

}
string writeindent()
{
	string res;
	for(int i = 0;i<indent;i++)
		res.push_back('	');
	return res;
}
void singleTagBegin(string s)
{
	string line = writeindent();
	line += "<" + s + ">";
	cmd.push_back(line);
	indent++;
}
void singleTagEnd(string s)
{
	indent--;
	string line = writeindent();
	line += "</" + s + ">";
	cmd.push_back(line);
}




//file related
void read(string path)
{
	char buffer[256];
	fstream in;
	in.open(path,ios::in);

	while(!in.eof())
	{
		in.getline(buffer,256,'\n');
		string line = buffer;
		line.pop_back();
		if(line.length()==0)
			continue;
		if(line == "\r")
			continue;
		for(int i=0;i<line.length();i++)
		{
			if(line[i] == '	')//erase tab
			{
				line.erase(i,1);
				i--;
			}
			else if(line[i]=='/')
			{
				if(i+1==line.length())
					continue;
				else if(line[i+1] == '/')
					line.erase(i);
			}

		}
		if(line.length()==0)
			continue;
		while(line[0]==' ' || line[0]=='	')
		{
			line.erase(0,1);
		}
		while(line.back()==' ' || line.back()=='	')
		{
			line.pop_back();
		}
		if(line.length()!=0)
		{
			if(line[0]=='/'||line[0]=='*')
				continue;
			seperate(line);
		}
	}
	in.close();
}
void save(string folder,string name)
{
	ofstream out;
	string outpath = folder + "/Myresult/" + name.substr(0,name.length()-5)+".xml";
	out.open(outpath,ios::trunc);

	for(string s:cmd)
		out<<s<<endl;

	out.close();
}

//check whether the dir exist
int IsFolderExist(string path)
{
    DIR *dp;
    const char *p = path.c_str();
    if ((dp = opendir(p)) == NULL)
    {
        return 0;
    }

    closedir(dp);
    return -1;
}
void seperate(string line)
{
	vector<string> res;
	int start = 0;
	bool isstring = false;
	for(int i = 0;i<line.length();i++)
	{
		if(line[i]=='"')
		{
			isstring = !isstring;
		}
		if(line[i] == ' '&&!isstring)
		{
			res.push_back(line.substr(start,i-start));
			start = i+1;
		}
	}
	res.push_back(line.substr(start));
	for(auto s:res)
	{
		start = 0;
		for(int i = 0;i<s.length();i++)
		{

			string what = "";
			what.push_back(s[i]);
			if(s[i] == '"')
			{
					i++;
				while(s[i]!='"')
					i++;
				string isstring = s.substr(start,i - start + 1);
				token.push(isstring);
				start = i + 1;
				continue;
			}
			if(isSymbol(what))
			{
				string tmp = s.substr(start,i-start);
				if(tmp.length()!=0)
					token.push(tmp);
				token.push(what);
				start = i + 1;
				continue;
			}
			if(i == s.length()-1)
			{
				token.push(s.substr(start,i - start + 1));
				continue;
			}
		}
	}
}


//main function
int main(int argc, char *argv[])
{
	string root_path = argv[1];
	if(root_path.back()=='/')
		root_path.pop_back();
	if(!IsFolderExist(root_path+"/Myresult/"))
	{
		string newpath = "mkdir "+root_path+"/Myresult";
		const char *command = newpath.c_str();
		system(command);
	}
	while(!isalnum(root_path.back()))
		root_path.pop_back();
	//is a folder
	if(root_path.length()<=3 || root_path[root_path.length()-3]!='.')
	{

		DIR *dp;
		struct dirent *dirp;
		int i = 0;
		regex reg_obj(".*\.jack", regex::icase);
		if((dp = opendir(root_path.c_str())) == NULL)
			cout << "Can't open " << root_path << endl;
		vector<string> allfiles;

		//all the files that end with .jack
		while((dirp = readdir(dp)) != NULL)
			if(regex_match(dirp->d_name, reg_obj))
				allfiles.push_back(dirp->d_name);

		closedir(dp);
		for(string s:allfiles)
		{
			cmd.clear();
			read(root_path+"/"+s);
			compileClass();
			save(root_path,s);
			token.clear();
			indent = 0;
		}
	}
	return 0;
}