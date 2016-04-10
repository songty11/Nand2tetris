#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

unordered_map <string,string> comp;
unordered_map <string,string> dest;
unordered_map <string,string> jump;
unordered_map <string,string> addr;
int numberofaddr = 16;

void mapInit()
{
	//addr map
	addr["SP"] = "0";
	addr["LCL"] = "1";
	addr["ARG"] = "2";
	addr["THIS"] = "3";
	addr["THAT"] = "4";
	addr["SCREEN"] = "16384";
	addr["KBD"] = "24576";
	addr["R0"] = "0";
	addr["R1"] = "1";
	addr["R2"] = "2";
	addr["R3"] = "3";
	addr["R4"] = "4";
	addr["R5"] = "5";
	addr["R6"] = "6";
	addr["R7"] = "7";
	addr["R8"] = "8";
	addr["R9"] = "9";
	addr["R10"] = "10";
	addr["R11"] = "11";
	addr["R12"] = "12";
	addr["R13"] = "13";
	addr["R14"] = "14";
	addr["R15"] = "15";


	//comp map
	comp["0"] = "0101010";
	comp["1"] = "0111111";
	comp["-1"] = "0111010";
	comp["D"] = "0001100";
	comp["A"] = "0110000";
	comp["!D"] = "0001101";
	comp["!A"] = "0110001";
	comp["-D"] = "0001111";
	comp["-A"]  ="0110011";
	comp["D+1"] = "0011111";
	comp["A+1"] = "0110111";
	comp["D-1"] = "0001110";
	comp["A-1"] = "0110010";
	comp["D+A"] = "0000010";
	comp["A+D"] = "0000010";
	comp["D-A"] = "0010011";
	comp["A-D"] = "0000111";
	comp["D&A"] = "0000000";
	comp["A&D"] = "0000000";
	comp["D|A"] = "0010101";
	comp["A|D"] = "0010101";
	comp["M"] = "1110000";
	comp["!M"] = "1110001";
	comp["-M"] = "1110011";
	comp["M+1"] = "1110111";
	comp["M-1"] = "1110010";
	comp["D+M"] = "1000010";
	comp["M+D"] = "1000010";
	comp["D-M"] = "1010011";
	comp["M-D"] = "1000111";
	comp["D&M"] = "1000000";
	comp["M&D"] = "1000000";
	comp["D|M"] = "1010101";
	comp["M|D"] = "1010101";

	//dest map
	dest["null"] = "000";
	dest["M"] = "001";
	dest["D"] = "010";
	dest["MD"] = "011";
	dest["DM"] = "011";
	dest["A"] = "100";
	dest["AM"] = "101";
	dest["MA"] = "101";
	dest["AD"] = "110";
	dest["DA"] = "110";
	dest["AMD"] = "111";

	//jump map
	jump["null"] = "000";
	jump["JGT"] = "001";
	jump["JEQ"] = "010";
	jump["JGE"] = "011";
	jump["JLT"] = "100";
	jump["JNE"] = "101";
	jump["JLE"] = "110";
	jump["JMP"] = "111";
}

//decide the command type
string commandType(string command)
{
	if(command[0] == '@')
		return "A_COMMAND";
	if(command[0] =='(')
		return "L_COMMAND";
	return "C_COMMAND";
}

//transfer a number in string into binary
string transBinary(string s)
{
	int num = stoi(s);
	string res;
	while(num)
	{
		res+=to_string(num%2);
		num/=2;
	}
	while(res.length()<15)
	{
		res+="0";
	}
	reverse(res.begin(),res.end());
	return res;
}
bool isNumber(string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i]>'9'|| s[i]<'0')
			return false;
	}
	return true;
}
string Ltransfer(string hack,int index)
{
	if(commandType(hack) == "L_COMMAND")
	{
		string variable = hack.substr(1,hack.length()-2);
		addr[variable] = to_string(index);
		return "";
	}
	else
		return hack;
}

string transfer(string hack)
{
	if(commandType(hack) == "A_COMMAND")
	{
		if(isNumber(hack.substr(1)))
			return "0"+transBinary(hack.substr(1));
		else
		{
			string variable = hack.substr(1);
			if(addr.find(variable)==addr.end())
			{
				if(numberofaddr==16384)
					addr[variable] = to_string(++numberofaddr);
				else if(numberofaddr==24576)
					addr[variable] = to_string(++numberofaddr);
				else
					addr[variable] = to_string(numberofaddr++);
			}
			return "0"+transBinary(addr[variable]);
		}
	}

	else if(commandType(hack) == "C_COMMAND")
	{
		string mydest,mycomp,myjump;
		auto equalpos = hack.find('=');
		auto semipos = hack.find(';');
		if(equalpos == string::npos)
		{
			mydest = "null";
			mycomp = hack.substr(0,semipos);
		}
		else
		{
			mydest = hack.substr(0,equalpos);
		}
		if(semipos == string::npos)
		{
			myjump = "null";
			mycomp = hack.substr(equalpos + 1);
		}
		else
		{
			myjump = hack.substr(semipos + 1);
		}
		if(mycomp.length()==0)
		{
			mycomp = hack.substr(equalpos+1,semipos-equalpos-1);
		}
		return "111"+comp[mycomp]+dest[mydest]+jump[myjump];
	}
}

vector<string> OnePass(vector<string> s)
{
	vector<string> res;
	int i = 0;
	for(string tmp:s)
	{
		string L = Ltransfer(tmp,i);
		if(L.length()!=0)
		{
			i++;
			res.push_back(L);
		}
	}
	return res;
}
vector<string> TwoPass(vector<string> s)
{
	vector<string> res;
	for(string tmp:s)
		res.push_back(transfer(tmp));
	return res;
}
vector<string> read(string path)
{
	vector<string> res;
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
			if(line[i] == ' '||line[i] == '	')
			{
				line.erase(i,1);
				i--;
			}
			else if(line[i]=='/')		//if there is a '/'
			{
				if(i+1==line.length())	//if it's the final character
					continue;
				else if(line[i+1] == '/') //if it's "//"
					line.erase(i);
			}

		}
		if(line.length()!=0)
		{
			res.push_back(line);
		}
	}
	in.close();
	return res;
}

void save(string path,vector<string> data)
{
	ofstream out;
	string outpath = path.substr(0,path.length()-3)+"hack";
	out.open(outpath,ios::trunc);

	for(string s:data)
		out<<s<<endl;

	out.close();
}
int main(int argc, char *argv[])
{
	string path = argv[1];
	mapInit();
	vector<string> data;
	data = read(path);
	data = OnePass(data);
	data = TwoPass(data);
	save(path,data);
	return 0;
}

