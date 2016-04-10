#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int ID = 0;


// Math command translation
void math_command(string command, vector<string> &cmd){
	//use a ID when creating any label in a function to make it unique to that function

	//result = x + y;
	if (command == "add")
	{

		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=M+D");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = x - y;
	else if (command == "sub")
	{

		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=M-D");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = (x & y); bitwise and
	else if (command == "and")
	{

		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=M&D");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = (x | y); bitwise or
	else if (command == "or")
	{
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=M|D");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = -y;
	else if (command == "neg")
	{
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=-M");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = !y
	else if (command == "not")
	{

		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("M=!M");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
	}

	//result = (x == y); (1 if equal, 0 if not)
	else if (command == "eq")
	{

		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M-D");
		cmd.push_back("M=0");
		cmd.push_back("@EQ" + to_string(ID));
		cmd.push_back("D;JEQ");
		cmd.push_back("@END" + to_string(ID));
		cmd.push_back("0;JMP");
		cmd.push_back("(EQ" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("A=M");
		cmd.push_back("M=!M");
		cmd.push_back("(END" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
		ID++;
	}

	//result = (x > y); true if x > y, else false
	else if (command == "gt")
	{
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M-D");
		cmd.push_back("M=0");
		cmd.push_back("@GT" + to_string(ID));
		cmd.push_back("D;JGT");
		cmd.push_back("@END" + to_string(ID));
		cmd.push_back("0;JMP");
		cmd.push_back("(GT" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("A=M");
		cmd.push_back("M=!M");
		cmd.push_back("(END" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
		ID++;
	}

	//result = (x < y); true if x < y, else false
	else if (command == "lt"){
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M");
		cmd.push_back("@SP");
		cmd.push_back("AM=M-1");
		cmd.push_back("D=M-D");
		cmd.push_back("M=0");
		cmd.push_back("@LT" + to_string(ID));
		cmd.push_back("D;JLT");
		cmd.push_back("@END" + to_string(ID));
		cmd.push_back("0;JMP");
		cmd.push_back("(LT" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("A=M");
		cmd.push_back("M=!M");
		cmd.push_back("(END" + to_string(ID)+")");
		cmd.push_back("@SP");
		cmd.push_back("M=M+1");
		ID++;
	}
}

//Memory access command translation
void mem_access_command(string command, string arg1, string arg2, vector<string> &cmd){

	//push command
	if (command == "push")
	{
		if (arg1 == "constant")
		{

			//push constant
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");

		}
		else if (arg1 == "local")
		{

			//push local
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@LCL");
			cmd.push_back("A=M+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}

		else if (arg1 == "argument")
		{

			//push argument 
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@ARG");
			cmd.push_back("A=M+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
		else if (arg1 == "this")
		{

			//push this
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@THIS");
			cmd.push_back("A=M+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
		else if (arg1 == "that")
		{

			//push that;
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@THAT");
			cmd.push_back("A=M+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
		else if (arg1 == "temp")
		{

			//push temp
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@R5");
			cmd.push_back("A=A+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
		else if (arg1 == "pointer")
		{

			//push pointer
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@R3");
			cmd.push_back("A=A+D");
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
		else if (arg1 == "static")
		{

			//Get variable stored at that static address
			cmd.push_back("@TianyuSong." + arg2);
			cmd.push_back("D=M");
			cmd.push_back("@SP");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
			cmd.push_back("@SP");
			cmd.push_back("M=M+1");
		}
	}

	//pop command
	if (command == "pop")
	{
		if (arg1 == "local")
		{

			//Pop local
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@LCL");
			cmd.push_back("D=M+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");

			
			
		}
		else if (arg1 == "argument")
		{

			//pop argument 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@ARG");
			cmd.push_back("D=M+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
		}
		else if (arg1 == "this")
		{

			//pop this 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@THIS");
			cmd.push_back("D=M+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");

		}
		else if (arg1 == "that")
		{

			//pop that 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@THAT");
			cmd.push_back("D=M+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
		}
		else if (arg1 == "temp")
		{

			//pop temp 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@R5");
			cmd.push_back("D=A+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");

			
		}
		else if (arg1 == "pointer")
		{

			//pop pointer 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@R15");
			cmd.push_back("M=D");
			cmd.push_back("@" + arg2);
			cmd.push_back("D=A");
			cmd.push_back("@R3");
			cmd.push_back("D=A+D");
			cmd.push_back("@R14");
			cmd.push_back("M=D");
			cmd.push_back("@R15");
			cmd.push_back("D=M");
			cmd.push_back("@R14");
			cmd.push_back("A=M");
			cmd.push_back("M=D");
		}
		else if (arg1 == "static")
		{

			//pop static 
			cmd.push_back("@SP");
			cmd.push_back("AM=M-1");
			cmd.push_back("D=M");
			cmd.push_back("@TianyuSong." + arg2);
			cmd.push_back("M=D");
		}
	}
}

// Decide the type
string cmdType(string line)
{
	for(int i= 0;i<line.length();i++)
	{
		if(line[i] == ' ')
			return "Mem_Access_Command";
	}
	return "Math_Command";
}

// Translate
void makeTranslate(string line,vector<string> &cmd)
{
	if(cmdType(line) == "Math_Command")
	{
		math_command(line,cmd);
	}
	else
	{
		int b1,b2;
		for(b1 = 0;b1<line.length();b1++)
		{
			if(line[b1] == ' ')
				break;
		}
		for(b2 = b1+1;b2<line.length();b2++)
		{
			if(line[b2] == ' ')
				break;
		}
		mem_access_command(line.substr(0,b1),line.substr(b1+1,b2-b1-1), line.substr(b2+1),cmd);
	}
}

//Read file 
void read(string path,vector<string> &cmd)
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
		if(line.length()!=0)
		{
			makeTranslate(line,cmd);
		}
	}
	in.close();
}

//save file
void save(string path,vector<string> cmd)
{
	ofstream out;
	string outpath = path.substr(0,path.length()-2)+"asm";
	out.open(outpath,ios::trunc);

	for(string s:cmd)
		out<<s<<endl;

	out.close();
}
int main(int argc, char *argv[])
{

	for(int i = 1;i<argc;i++)
	{
		string path = argv[i];
		vector<string> cmd;
		read(path,cmd);
		save(path,cmd);
	}
	
	return 0;
}