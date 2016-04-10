#ifndef _SYMBOLTABLE_
#define _SYMBOLTABLE_
#include <string>
#include <map>
#include "Assist.h"
using namespace std;


class SymbolTable
{

private:
	struct VariableInfo {
		string type;
		string kind;
		int index;
	};
	map<string, VariableInfo> classSymbolTable;
	map<string, VariableInfo> subroutineSymbolTable;
	int staticIdx, fieldIdx, argIdx, varIdx;
public:		
	SymbolTable():staticIdx(0), fieldIdx(0), argIdx(0), varIdx(0){};

	void startSubroutine() 
	{
		subroutineSymbolTable.clear();
		argIdx = 0;
		varIdx = 0;
	}
	void define(string name, string type, string kind) 
	{
		VariableInfo vi;
		if (kind == "static") {
			VariableInfo v = { type, kind, staticIdx++ };
			vi = v;
		} else if(kind ==  "field") {
			VariableInfo v = { type, kind, fieldIdx++ };
			vi = v;
		} else if(kind ==  "arg") {
			VariableInfo v = { type, kind, argIdx++ };
			vi = v;
		} else if(kind == "var") {
			VariableInfo v = { type, kind, varIdx++ };
			vi = v;
		}
		if (kind == "static" || kind == "field") {
			classSymbolTable[name] = vi;
		} else {
			subroutineSymbolTable[name] = vi;
		}
	}
	int varCount(string kind) 
	{
		int variableCount = 0;
		map<string, VariableInfo>::iterator pos;
		if (kind == "static" || kind ==  "field") 
		{
			for (pos = classSymbolTable.begin(); pos != classSymbolTable.end(); pos++) 
			{
				if (pos->second.kind == kind) {
					variableCount++;
				}
			}
		} else if(kind == "arg" || kind == "var") {
			for (pos = subroutineSymbolTable.begin(); pos != subroutineSymbolTable.end(); pos++) {
				if (pos->second.kind == kind) {
					variableCount++;
				}
			}
		}
		return variableCount;
	}
	string kindOf(string name) 
	{
		string kind = "none";
		map<string, VariableInfo>::iterator pos;
		pos = subroutineSymbolTable.find(name) ;
		if(pos != subroutineSymbolTable.end()) {
			kind = pos->second.kind;
		} else {
			pos = classSymbolTable.find(name);
			if(pos != classSymbolTable.end()) {
				kind = pos->second.kind;
			}
		}
		return kind;
	}
	string typeOf(string name) 
	{
		string str = "none";
		map<string, VariableInfo>::iterator pos;
		pos = subroutineSymbolTable.find(name) ;
		if(pos != subroutineSymbolTable.end()) {
			str = pos->second.type;
		} else {
			pos = classSymbolTable.find(name);
			if(pos != classSymbolTable.end()) {
				str = pos->second.type;
			}
		}
		return str;
	}
	int indexOf(string name) 
	{
		int index = 0;
		map<string, VariableInfo>::iterator pos;
		pos = subroutineSymbolTable.find(name) ;
		if(pos != subroutineSymbolTable.end()) {
			index = pos->second.index;
		} else {
			pos = classSymbolTable.find(name);
			if(pos != classSymbolTable.end()) {
				index = pos->second.index;
			}
		}
		return index;
	}

};
#endif