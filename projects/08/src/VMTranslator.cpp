#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include<dirent.h>
#include <regex>

using namespace std;
int ID = 0;
string function_name = "";
string static_name = "";
set<string> math_set = { "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"};
set<string> mem_set = { "push", "pop"};
set<string> prog_set = {"label", "goto", "if-goto"};
set<string> func_set = {"function", "call", "return"};
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
			cmd.push_back("@"+static_name + arg2);
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
			cmd.push_back("@"+static_name + arg2);
			cmd.push_back("M=D");
		}
	}
}



void prog_flow_command(string command, string arg1, vector<string> &cmd){
	if (command == "label"){
		cmd.push_back("(" + function_name +"$" + arg1 + ")" );
	}
	else if (command == "goto"){
		cmd.push_back("@" + function_name + "$" + arg1 );
		cmd.push_back("0;JMP" );
	}
	else if (command == "if-goto"){

		cmd.push_back("@SP" );
		cmd.push_back("AM=M-1" );
		cmd.push_back("D=M" );

		cmd.push_back("@" + function_name + "$" + arg1 );
		cmd.push_back("D;JNE" );
		
	}

}


void func_calling_command(string command, string arg1, string arg2, vector<string> &cmd){
	if (command == "function"){
		//sets the function name for any labels that are generated while in the function
		function_name = arg1;

		cmd.push_back("("  + arg1 + ")" );
		for (int i = 0; i < stoi(arg2); i++){
			cmd.push_back("@0" );
			cmd.push_back("D=A" );
			cmd.push_back("@SP" );
			cmd.push_back("A=M" );
			cmd.push_back("M=D" );
			cmd.push_back("@SP" );
			cmd.push_back("M=M+1" );
		}
	}
	else if (command == "call"){
		//call
		//push return address to stack
		cmd.push_back("@RETURN_ADDRESS_" + arg1 + "_" + to_string(ID) );
		cmd.push_back("D=A" );
		cmd.push_back("@SP" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );
		cmd.push_back("@SP" );
		cmd.push_back("M=M+1" );
		
		//push LCL
		cmd.push_back("@LCL" );
		cmd.push_back("D=M" );
		cmd.push_back("@SP" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );
		cmd.push_back("@SP" );
		cmd.push_back("M=M+1" );

		//push ARG
		cmd.push_back("@ARG" );
		cmd.push_back("D=M" );
		cmd.push_back("@SP" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );
		cmd.push_back("@SP" );
		cmd.push_back("M=M+1" );

		//push THIS
		cmd.push_back("@THIS" );
		cmd.push_back("D=M" );
		cmd.push_back("@SP" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );
		cmd.push_back("@SP" );
		cmd.push_back("M=M+1" );

		//push THAT
		cmd.push_back("@THAT" );
		cmd.push_back("D=M" );
		cmd.push_back("@SP" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );
		cmd.push_back("@SP" );
		cmd.push_back("M=M+1" );

		//ARG = SP-n-5 ( n = number of arguments)
		cmd.push_back("@SP" );
		cmd.push_back("D=M" );
		cmd.push_back("@" + arg2 );
		cmd.push_back("D=D-A" );
		cmd.push_back("@5" );
		cmd.push_back("D=D-A" );
		cmd.push_back("@ARG" );
		cmd.push_back("M=D" );

		//LCL = SP
		cmd.push_back("@SP" );
		cmd.push_back("D=M" );
		cmd.push_back("@LCL" );
		cmd.push_back("M=D" );
		
		cmd.push_back("@" + arg1 );
		cmd.push_back("0;JMP" );
		
		cmd.push_back("(RETURN_ADDRESS_" + arg1 + "_" + to_string(ID) + ")" );
		ID++;

	}
	else if (command == "return")
	{

		//returns program execution back to return-address with the computed value on the top of the stack
		cmd.push_back("@LCL" );
		cmd.push_back("D=M" );
		cmd.push_back("@R13" );
		cmd.push_back("M=D" );

		cmd.push_back("@R13" );
		cmd.push_back("D=M" );
		cmd.push_back("@5" );
		cmd.push_back("A=D-A" );
		cmd.push_back("D=M" );
		cmd.push_back("@R14" );
		cmd.push_back("M=D" );

		cmd.push_back("@SP" );
		cmd.push_back("AM=M-1" );
		cmd.push_back("D=M" );
		cmd.push_back("@ARG" );
		cmd.push_back("A=M" );
		cmd.push_back("M=D" );

		cmd.push_back("@ARG" );
		cmd.push_back("D=M+1" );
		cmd.push_back("@SP" );
		cmd.push_back("M=D" );
		
		cmd.push_back("@R13" );
		cmd.push_back("A=M-1" );
		cmd.push_back("D=M" );
		cmd.push_back("@THAT" );
		cmd.push_back("M=D" );

		cmd.push_back("@R13" );
		cmd.push_back("D=M" );
		cmd.push_back("@2" );
		cmd.push_back("A=D-A" );
		cmd.push_back("D=M" );
		cmd.push_back("@THIS" );
		cmd.push_back("M=D" );

		cmd.push_back("@R13" );
		cmd.push_back("D=M" );
		cmd.push_back("@3" );
		cmd.push_back("A=D-A" );
		cmd.push_back("D=M" );
		cmd.push_back("@ARG" );
		cmd.push_back("M=D" );

		cmd.push_back("@R13" );
		cmd.push_back("D=M" );
		cmd.push_back("@4" );
		cmd.push_back("A=D-A" );
		cmd.push_back("D=M" );
		cmd.push_back("@LCL" );
		cmd.push_back("M=D" );

		cmd.push_back("@R14" );
		cmd.push_back("A=M" );
		cmd.push_back("0;JMP" );
	}


}

//If there are multiple files, then we should run following code
void code_init(vector<string> &cmd)
{
	cmd.push_back("@256");
	cmd.push_back("D=A");
	cmd.push_back("@SP");
	cmd.push_back("M=D");
	func_calling_command("call", "Sys.init", "0",cmd);
}
// Decide the type
string cmdType(string line)
{
	for(int i = 0;i<line.length();i++)
	{
		string tmp = line.substr(0,i+1);
		if(math_set.find(tmp)!=math_set.end())
			return "Math_Command";
		if(mem_set.find(tmp)!=mem_set.end())
			return "Mem_Access_Command";
		if(prog_set.find(tmp)!=prog_set.end())
			return "Prog_Flow_Command";
		if(func_set.find(tmp)!=func_set.end())
			return "Func_Calling_Command";
	}
}

// Translate
void makeTranslate(string line,vector<string> &cmd)
{
	string cmdtype = cmdType(line);
	if(cmdtype == "Math_Command")
	{
		math_command(line, cmd);

	}
	else if(cmdtype == "Prog_Flow_Command")
	{
		int b;
		for(b = 0;b<line.length();b++)
		{
			if(line[b] == ' ')
				break;
		}
		prog_flow_command(line.substr(0,b),line.substr(b+1),cmd);

	}
	else if(cmdtype == "Mem_Access_Command")
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
		if(b1==line.length()&&b2==b1+1)
			func_calling_command("return","","",cmd);
		else
			func_calling_command(line.substr(0,b1),line.substr(b1+1,b2-b1-1), line.substr(b2+1),cmd);
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
	//for each file (or folder)
	for(int i = 1;i<argc;i++)
	{
		vector<string> cmd;
		string root_path = argv[i];
		while(!isalnum(root_path.back()))
			root_path.pop_back();
		//is a folder
		if(root_path.length()<=3 || root_path[root_path.length()-3]!='.')
		{
			
			DIR *dp;
			struct dirent *dirp;
			int i = 0;
			regex reg_obj(".*\.vm", regex::icase);
			if((dp = opendir(root_path.c_str())) == NULL)
				cout << "Can't open " << root_path << endl;
			vector<string> allfiles;

			//all the files that end with .vm
			while((dirp = readdir(dp)) != NULL)
				if(regex_match(dirp->d_name, reg_obj))
					allfiles.push_back(root_path+"/"+dirp->d_name);

			closedir(dp);

			if(allfiles.size()>1)
				code_init(cmd);
			for(string s:allfiles)
			{
				static_name = "TianyuSong" + to_string(i++);

				//do the translation
				read(s,cmd);
			}
			save(root_path+".//",cmd);
		}
		//a single file
		else
		{
			static_name = "TianyuSong";
			string file_name = root_path;
			read(file_name, cmd);
			save(root_path,cmd);
		}
	}
	
	return 0;
}