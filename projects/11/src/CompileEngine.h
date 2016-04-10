#ifndef _COMPILEENGINE_
#define _COMPILEENGINE_
#include <iostream>
#include <vector>
#include "VMWriter.h"
#include "SymbolTable.h"
#include "Token.h"
#include "Assist.h"
#include <stack>
#include <assert.h>
using namespace std;
class CompileEngine
{
private:
	tokens token;
	SymbolTable symboltable;
	VMWriter writer;
	string className, subroutineName, currentSubroutineType;
	size_t Argc;
	size_t LabelNum;
public:
	// Constructor
	CompileEngine(tokens t, VMWriter v, SymbolTable s):token(t),symboltable(s),writer(v){}; 
	void startCompile()
	{
		 compileClass();
		writer.save();
	}
	void compileClass()
	{
		assert(token.get() == "class");
		token.next();
		className = token.get();

		token.next();
		assert(token.get() == "{");

		token.next();

		while (token.get() == "field" || token.get() == "static")
			compileClassVarDec();
		while (token.get() == "constructor" || token.get() == "function" ||token.get() == "method")
			compileSubroutine();
		assert(token.get() == "}");
	}
	void compileClassVarDec()
	{

		string name, type, kind;
		kind = token.get();
		token.next();
		if (isKey(token.get()))
		{
			string keyword = token.get();
			assert(keyword == "int" || keyword == "char" || keyword == "boolean");
			type = keyword;
		}
		else
			type = token.get();
		

		token.next();
		name = token.get();
		symboltable.define(name, type, kind);

		while (1)
		{
			token.next();
			if (token.get() == ";")
			{
				break;
			}
			else
			{
				assert(token.get() == ",");
			}
			token.next();
			name = token.get();
			symboltable.define(name, type, kind);
		}

		token.next();
	}

	void compileSubroutine()
	{  
		symboltable.startSubroutine();

		currentSubroutineType = token.get();

		token.next(); // type

		token.next(); //name of subroutine

		subroutineName = token.get();

		token.next(); 

		assert(token.get() == "(");

		token.next();
		if (currentSubroutineType == "method") {
			symboltable.define("this", className, "arg");
		}
		compileParameterList();

		assert(token.get() == ")");

		token.next(); 

		assert(token.get() == "{");

		token.next(); 

		while (token.get() == "var")
			compileVarDec();

		writer.writeFunction(className + "." + subroutineName, symboltable.varCount("var"));
		if (currentSubroutineType == "method")
		{
			writer.writePush("argument", 0);
			writer.writePop("pointer", 0);
		}
		else if(currentSubroutineType == "constructor") {
			writer.writePush("constant", symboltable.varCount("field"));
			writer.writeCall("Memory.alloc", 1);
			writer.writePop("pointer", 0);
		}
		if(token.get() == "let" || token.get() == "if" || token.get() == "else" || token.get() == "while" || token.get() == "do" || token.get() == "return")
		compileStatements();

		assert(token.get() == "}");

		token.next();
	}

	void compileParameterList()
	{  
		string name, type, kind = "arg";


		if (isSymbol(token.get()))
		{
			return;
		}
		if (isKey(token.get()))
		{
			assert(token.get() == "int" || token.get() == "char" ||
				   token.get() == "boolean");
			type = token.get();
		}
		else
		{
			type = token.get();
		}

		token.next(); 

		name = token.get();

		symboltable.define(name, type, kind);

		while (1)
		{
			token.next(); 

			assert(token.get() == "," || token.get() == ")");
			if (token.get() == ")")
			{
				break;
			}
			else
			{
				token.next(); // type
				if (isKey(token.get()))
				{
					assert(token.get() == "int" || token.get() == "char" ||
						   token.get() == "boolean");
					type = token.get();
				}
				else
				{
					type = token.get();
				}
				token.next(); // variable name
				name = token.get();
				symboltable.define(name, type, kind);
			}
		}
	}

	void compileVarDec()
	{   
		string name, type, kind = "var";
		assert(token.get() == "var");

		token.next(); // type
		if (isKey(token.get())) {
			assert(token.get() == "int" || token.get() == "char" ||
				   token.get() == "boolean");
			type = token.get();
		} else {
			type = token.get();
		}

		token.next(); // variable name
		name = token.get();
		symboltable.define(name, type, kind);

		while (true) {
			token.next(); 
			assert(token.get() == "," || token.get() == ";");
			if (token.get() == ";")
			{
				break;
			}
			else
			{ // ,
				token.next(); // variable name
				name = token.get();
				symboltable.define(name, type, kind);
			}
		}

		token.next();
	}

	void compileStatements()
	{	

		while (1)
		{
			if (isSymbol(token.get())) {
				assert(token.get() == "}");
				return;
			}
			string keyword = token.get();
			assert(keyword == "let" || keyword == "if" || keyword == "while" ||
				   keyword == "do"  || keyword == "return");
			if (keyword == "let") {
				compileLet();
			} else if (keyword == "if") {
				compileIf();
			} else if (keyword == "while") {
				compileWhile();
			} else if (keyword == "do") {
				compileDo();
			} else if (keyword == "return") {
				compileReturn();
			}
			if (keyword != "if") {
				token.next();
			}
		}
	}
	void compileDo()
	{
		assert(token.get() == "do");

		char previousSymbol;
		string identBeforeDot, identAfterDot;

		token.next();
		identBeforeDot = token.get();

		token.next();
		assert(token.get() == "(" || token.get() == ".");
		previousSymbol = token.get()[0];
		if (token.get() == "(")
		{
			if (currentSubroutineType == "constructor")
			{
				writer.writePush("pointer", 0);
			}
			else
			{
				writer.writePush("argument", 0);
			}
			token.next();
			compileExpressionList();
		}

		token.next();
		if (previousSymbol == '.') {
			identAfterDot = token.get();
		}
		else
		{
			assert(token.get() == ";");
			writer.writeCall(className + "." + identBeforeDot, Argc + 1);
			writer.writePop("temp", 0);
			Argc = 0;
			return;
		}

		token.next();

		assert(token.get() == "(");
		token.next();
		if (symboltable.kindOf(identBeforeDot) != "none") {
			writer.writePush(segment(symboltable.kindOf(identBeforeDot)), symboltable.indexOf(identBeforeDot));
		}
		compileExpressionList();

		assert(token.get() == ")");

		token.next();

		assert(token.get() == ";");

		if (symboltable.kindOf(identBeforeDot) == "none")
		{
			writer.writeCall(identBeforeDot + "." + identAfterDot, Argc);
		}
		else
		{
			writer.writeCall(symboltable.typeOf(identBeforeDot) + "." + identAfterDot, Argc + 1);
		}
		writer.writePop("temp", 0);
		Argc = 0;
	}

	void compileLet()
	{   // "let"
		assert(token.get() == "let");

		token.next(); // variable name
		string assignedVariable = token.get();
		bool isArray = false;
		token.next(); // '=' or '['

		assert(token.get() == "[" || token.get() == "=");
		if (token.get() == "[") {
			token.next(); // beginning of expression
			compileExpression(); // expression and ']'
			writer.writePush(segment(symboltable.kindOf(assignedVariable)),
							 symboltable.indexOf(assignedVariable));
			
			writer.writeArithmetic("add");
			isArray = true;
			token.next(); 
		}
		
		if(token.get()=="=")
		{ // '='
			token.next();
			compileExpression(); // expression and ';' or ")"
			if(token.get() == ")")
			{
				token.next();
			}
			if(isArray)
				writer.writePop("temp",0);
			
		}
		if(!isArray)
		{
			writer.writePop(segment(symboltable.kindOf(assignedVariable)),symboltable.indexOf(assignedVariable));
		}
		else
		{
			writer.writePop("pointer",1);
			writer.writePush("temp",0);
			writer.writePop("that",0);
		}
	}
	void compileWhile()
	{
		string labelTrue, labelFalse;
		labelTrue = "WHILETRUE" + to_string(LabelNum);
		labelFalse = "WHILEFALSE" + to_string(LabelNum);
		LabelNum++;
		assert(token.get() == "while");

		writer.writeLabel(labelTrue);
		// read a "("
		token.next();
		assert(token.get() == "(");

		// read an expression and ")"
		token.next();
		compileExpression();
		writer.writeArithmetic("not");
		writer.writeIf(labelFalse);

		assert(token.get() == ")");
		token.next();
		assert(token.get() == "{");

		token.next();
		compileStatements();
		assert(token.get() == "}");

		writer.writeGoto(labelTrue);
		writer.writeLabel(labelFalse);
	}
	void compileReturn()
	{
		assert(token.get() == "return");

		token.next();
		if(token.get() == ";")
		{
			writer.writePush("constant", 0);
			writer.writeReturn();
		}
		else
		{
			compileExpression();
			assert(token.get() == ";");
			writer.writeReturn();
		}
	}
	void compileIf()
	{
		string labelElse, labelContinue;
		labelElse = "IFFALSE" + to_string(LabelNum);
		labelContinue = "CONTINUE" + to_string(LabelNum);
		LabelNum++;
		token.next();
		assert(token.get() == "(");

		token.next();
		compileExpression();
		writer.writeArithmetic("not");
		writer.writeIf(labelElse);
		token.next();

		assert(token.get() == "{");

		// read statements and a closing brace.
		token.next();
		compileStatements();
		assert(token.get() == "}");
		writer.writeGoto(labelContinue);
		writer.writeLabel(labelElse);

		token.next();
		if ( token.get() == "else")
		{
			token.next();
			assert(token.get() == "{");
			token.next();
			compileStatements();
			assert(token.get() == "}");
			token.next();

		}

		writer.writeLabel(labelContinue);

	}


	void compileExpression()
	{
		char op;
		compileTerm();
		while (true)
		{
			if (!isSymbol(token.get()))
			{
				break;
			}
			op = token.get()[0];
			if (op != '+' && op != '-' && op != '*' && op != '/' && op != '&' &&
				op != '|' && op != '<' && op != '>' && op != '=') {
				break;
			}
			token.next();
			compileTerm();
			switch(op)
			{
				case '+':
					writer.writeArithmetic("add");
					break;
				case '-':
					writer.writeArithmetic("sub");
					break;
				case '*':
					writer.writeCall("Math.multiply",2);
					break;
				case '/':
					writer.writeCall("Math.divide",2);
					break;
				case '&':
					writer.writeArithmetic("and");
					break;
				case '|':
					writer.writeArithmetic("or");
					break;
				case '<':
					writer.writeArithmetic("lt");
					break;
				case '>':
					writer.writeArithmetic("gt");
					break;
				case '=':
					writer.writeArithmetic("eq");
					break;
				case '~':
					writer.writeArithmetic("not");
					break;
			}
		}
	}

	void compileTerm()
	{
		if (isNumber(token.get())) {
			writer.writePush("constant", stoi(token.get()));
		} else if (isString(token.get())) {
			writer.writePush("constant", token.get().length()-2);
			writer.writeCall("String.new", 1);
			for(size_t i = 1; i < token.get().length()-1; i++)
			{
				writer.writePush("constant", token.get()[i]);
				writer.writeCall("String.appendChar", 2);
			}
		}
		else if (isKey(token.get()))
		{
			assert(token.get() == "true" || token.get() == "false" ||
				   token.get() == "null" || token.get() == "this");

			if (token.get() == "this")
			{
				writer.writePush("pointer", 0);
			} else if (token.get() == "true") {
				writer.writePush("constant", 1);
				writer.writeArithmetic("neg");
			} else if (token.get() == "false" || token.get() == "null") {
				writer.writePush("constant", 0);
			}
		} else if (isIdentifier(token.get())) {
			string ident = token.get();
			token.next();

			if (token.get() == "[") {
				writer.writePush(segment(symboltable.kindOf(ident)), symboltable.indexOf(ident));
				token.next();
				compileExpression();
				writer.writeArithmetic("add");
				writer.writePop("pointer", 1);
				writer.writePush("that", 0);
			}
			else if (token.get() == "(")
			{
				token.next();
				compileExpressionList();
				writer.writeCall(ident, Argc);
			}
			else if (token.get() == ".")
			{
				token.next(); // function name

				string functionName = token.get();
				token.next(); // "("

				token.next();
				compileExpressionList(); // expression list and ")"
				if (symboltable.kindOf(ident) == "none") {
					writer.writeCall(ident + "." + functionName, Argc);
				} else
				{
					writer.writePush("this", 1);
					writer.writeCall(symboltable.typeOf(ident) + "." + functionName, Argc + 1);
				}
			} else
			{
				writer.writePush(segment(symboltable.kindOf(ident)), symboltable.indexOf(ident));
				return;
			}
		}
		else if (isSymbol(token.get()))
		{
			if (token.get() == "(")
			{
				token.next();
				compileExpression();
			}
			else {
				assert(token.get() == "-" || token.get() == "~");
				char binaryOp = token.get()[0];
				token.next();
				compileTerm();
				if (binaryOp == '-') {
					writer.writeArithmetic("neg");
				}
				else
				{ 
					writer.writeArithmetic("not");
				}
				return;
			}
		}

		token.next();
	}

	void compileExpressionList()
	{
		Argc = 0;

		if (token.get() == ")") {
			return;
		}

		compileExpression();
		while(1) 
		{
			Argc++;
			if (token.get() == ")")
			{
				return;
			}
			else
			{ 
				assert(token.get() == ",");
				token.next();
				compileExpression();
			}
		}
	}
};
#endif