#ifndef _VMWRITER_
#define _VMWRITER_

#include <fstream>
#include <vector>
#include "Assist.h"
using namespace std;

class VMWriter
{

public:
	vector<string> cmd;
	string folder,name;
	VMWriter(string a,string b):folder(a),name(b){};

	void writePush(string segment, int index)
	{
		cmd.push_back("push " + segment + " " + to_string(index));
	}

	void writePop(string segment, int index)
	{
		cmd.push_back("pop " + segment + " " + to_string(index));
	}

	void writeArithmetic(string command)
	{
		cmd.push_back(command);
	}

	void writeLabel(string label)
	{
		cmd.push_back("label " + label );
	}

	void writeGoto(string label)
	{
		cmd.push_back("goto " + label );
	}

	void writeIf(string label)
	{
		cmd.push_back("if-goto " + label );
	}

	void writeCall(string name, int nArgs)
	{
		cmd.push_back("call " + name + " " + to_string(nArgs));
	}

	void writeFunction(string name, int nLocals)
	{
		cmd.push_back("function " + name + " " + to_string(nLocals));
	}

	void writeReturn()
	{
		cmd.push_back("return");
	}
	void save()
	{
		ofstream out;
		string outpath = folder + "/" + name.substr(0,name.length()-5)+".vm";
		out.open(outpath,ios::trunc);

		for(string s:cmd)
			out<<s<<endl;

		out.close();
	}
};

#endif