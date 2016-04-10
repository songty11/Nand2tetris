#ifndef _TOKEN_
#define _TOKEN_

#include <vector>
#include <fstream>
using namespace std;


class tokens
{
public:
	vector<string> data;
	int index;
	int size;
	void clear()
	{
		data.clear();
		size = 0;
		index = 0;
	}
	void push(string s)
	{
		data.push_back(s);
		size++;

	}
	void next()
	{
		index++;

	}
	string get()
	{
		return data[index];
	}
	void set(string s)
	{
		data[index] = s;
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
				push(isstring);
				start = i + 1;
				continue;
			}
			if(isSymbol(what))
			{
				string tmp = s.substr(start,i-start);
				if(tmp.length()!=0)
					push(tmp);
				push(what);
				start = i + 1;
				continue;
			}
			if(i == s.length()-1)
			{
				push(s.substr(start,i - start + 1));
				continue;
			}
		}
		}
	}
	tokens(string path)
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
		index = 0;
		size = 0;
	}

	tokens()
	{
		index = 0;
		size = 0;
	}
};


#endif

