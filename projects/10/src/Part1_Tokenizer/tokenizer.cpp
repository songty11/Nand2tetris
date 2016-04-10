
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <dirent.h>
#include "tokenizer.h"
using namespace std;
vector<string> keywords = { "constructor", "function", "method", "field", "static", "var", "int",
					"char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if",
					"else", "while", "return","class"};
vector<string> symbols = {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};
vector<string> cmd;
tokens token;
//check whether it is a keywords
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


void writeType()
{
	string type = "";
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
	string sym = "";
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
		sym += "<symbol> " + token.get() + " </symbol>";
	}
	cmd.push_back(sym);
}
void makeTokenizer()
{
	while(!token.end())
	{
		string t = token.get();
		if(isSymbol(t))
		{
			writeSymbol();
		}
		else
		{
			writeType();
		}
		token.next();
	}
}
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
void save(string folder,string name)
{
	ofstream out;
	string outpath = folder + "/Myresult/" + name.substr(0,name.length()-5)+"T.xml";
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
			cmd.push_back("<tokens>");
			read(root_path+"/"+s);
			makeTokenizer();
			cmd.push_back("</tokens>");
			save(root_path,s);
			token.clear();
		}
	}
	return 0;
}