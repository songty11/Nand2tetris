
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <dirent.h>
#include "VMWriter.h"
#include "SymbolTable.h"
#include "Token.h"
#include "Assist.h"
#include "CompileEngine.h"
using namespace std;



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
//main function
int main(int argc, char *argv[])
{
	string root_path = argv[1];
	if(root_path.back()=='/')
		root_path.pop_back();
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
			tokens newtoken(root_path+"/"+s);
			VMWriter writer(root_path,s);
			SymbolTable symboltable;
			CompileEngine compileengine(newtoken,writer,symboltable);
			compileengine.startCompile();
		}
	}
	return 0;
}