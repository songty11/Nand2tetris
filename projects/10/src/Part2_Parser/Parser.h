#include <vector>
using namespace std;
class tokens
	{
	public:
		vector<string> data;
		int i;
		int size;
		void clear()
		{
			data.clear();
			size = 0;
			i = 0;
		}
		void push(string s)
		{
			data.push_back(s);
			size++;

		}
		void next()
		{
			i++;

		}
		string get()
		{
			return data[i];
		}
		void set(string s)
		{
			data[i] = s;
		}
		tokens()
		{
			i = 0;
			size = 0;
		}
	};
//check whether it is a keywords
bool isKey(string c);
bool isSymbol(string s);
bool isOpr(string s);
bool isstatment(string s);
bool isNumber(string s);
bool isString(string s);
void compileClass();
void compileClassVarDec();
void compileSubroutine();
void compileParameterlist();
void compileSubroutinebody();
void compileVarDec();
void compileStatements();
void compileLet();
void compileDo();
void compileWhile();
void compileIf();
void compileExpression();
void compileTerm();
void compileExpressionList();
void compileSubroutineCall();
void compileSubroutineCall(string name);
void compileReturn();
void writeType();
void writeSymbol();
void writeIdentifier();
void writeKey();
string writeindent();
void singleTagBegin(string s);
void singleTagEnd(string s);
void read(string path);
void save(string folder,string name);
int IsFolderExist(string path);
void seperate(string line);
