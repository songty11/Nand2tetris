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
		bool end()
		{
			if(i==size)
				return true;
			return false;
		}
		tokens()
		{
			i = 0;
			size = 0;
		}

	};
bool isKey(string);
bool isSymbol(string);
bool isNumber(string);
bool isString(string);
void writeType();
void writeSymbol();
void makeTokenizer();
void read(string);
void save(string,string);
int IsFolderExist(string);
void seperate(string line);