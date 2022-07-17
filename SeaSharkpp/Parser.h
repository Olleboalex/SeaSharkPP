#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include "StandardLibrary.h"
#include "FileWriter.h"

#define TOK vector<Token>

using namespace std;

unordered_set<string> MathIDS{
	"PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD", "LPAR", "RPAR", "RCURLBRACK", "LCURLBRACK",
	"EQUALS", "NOTEQUALS", "BIGEQUALS", "SMALLEQUALS", "SMALLER", "BIGGER", "INCREMENT",
	"DECREMENT", "PLUSEQUALS", "MINUSEQUALS", "MULTIPLYEQUALS", "DIVIDEEQUALS"
};

unordered_map<string, method> METHODS;

unordered_map<string, Token> VARIABLES;

unordered_map<string, unordered_map<string, method>> ContainedLibraries{
	make_pair("System", SystemMETHODS),
	make_pair("FileWriter", FileWriterMETHODS)
};


/*
Function parses elements in methodcall parameters and if none are of type error it calls the appropriate function.
If function call contains incorrect number of parameters returns error here.
*/
Token SystemMethod(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (METHODS.count(MethodCall.NAME))
	{
		if (METHODS[MethodCall.NAME].SystemMethod)
		{
			for (int i = 0; i < MethodCall.EvalStatement.size(); i++)
			{
				Token result = Parse(MethodCall.EvalStatement[i], methods, Variables);
				if (result.ID == "ERROR") return result;
				MethodCall.EvalStatement[i] = vector<Token>{ result };
			}
			return METHODS[MethodCall.NAME].func(MethodCall, methods, Variables);
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "This is not a built in function";
			return errorToken;
		}
	}
	return Token();
}

string libPath = "D:\\VisualStudioProjects\\SeaSharkPLUSPLUS\\SeaSharkPP\\SeaSharkpp\\x64\\Debug\\";

Token ParseLib(vector<Token> tokens, unordered_map<string, method>* methods)
{
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "DEFINITION")
		{
			method Method = method(tokens[cursor].NAME, tokens[cursor].EvalStatement, tokens[cursor].ExecStatement, false, true);
			(*methods)[Method.Name] = Method;
		}
		else if (tokens[cursor].ID == "USING")
		{
			if (ContainedLibraries.count(tokens[cursor].NAME))
			{
				for (pair<string, method> var : ContainedLibraries[tokens[cursor].NAME])
				{
					METHODS.insert(var);
				}
			}
			else
			{
				fstream lib;
				string path = libPath + tokens[cursor].NAME + ".ssl";
				lib.open(path, ios::in);
				string code = "";
				if (lib.is_open())
				{
					string line;
					while (getline(lib, line))
					{
						code += line + '\n';
					}
					lib.close();
				}
				else
				{
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Failed to load library " + tokens[cursor].NAME;
					return errorTok;
				}
				Token libToken = ParseLib(LexText(code), methods);
				if (libToken.ID == "ERROR") return libToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Failed to parse library";
			return errorToken;
		}
		cursor++;
	}
	return Token(0);
}

bool CheckIfNameIsContained(vector<vector<Token>> x, string name)
{
	for (int i = 0; i < x.size(); i++)
	{
		if (x[i][0].NAME == name) return true;
	}
	return false;
}

Token ParseMethodCall(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (!(*methods).count(MethodCall.NAME))
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Function does not exist";
		return errorToken;
	}
	if ((*methods)[MethodCall.NAME].Parameters.size() != MethodCall.EvalStatement.size())
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Incorrect number of parameters in function call";
		return errorToken;
	}

	if ((*methods)[MethodCall.NAME].SystemMethod)
	{
		Token tok = SystemMethod(MethodCall, methods, Variables);
		return tok;
	}
	else if ((*methods)[MethodCall.NAME].fromLib)
	{
		unordered_map<string, Token> funcVariables = VARIABLES;
		for (int i = 0; i < (*methods)[MethodCall.NAME].Parameters.size(); i++)
		{
			vector<Token> token = { MethodCall.EvalStatement[i] };
			funcVariables[(*methods)[MethodCall.NAME].Parameters[i][0].NAME] = Parse(token, methods, Variables);
		}
		Token tok = Parse((*methods)[MethodCall.NAME].ExecutionStatements, methods, &funcVariables);
		return tok;
	}
	else
	{
		unordered_map<string, Token> funcVariables = VARIABLES;
		for (int i = 0; i < (*methods)[MethodCall.NAME].Parameters.size(); i++)
		{
			vector<Token> token = { MethodCall.EvalStatement[i] };
			funcVariables[(*methods)[MethodCall.NAME].Parameters[i][0].NAME] = Parse(token, methods, Variables);
		}
		Token tok = Parse((*methods)[MethodCall.NAME].ExecutionStatements, methods, &funcVariables);
		for (pair<string, Token> element : funcVariables)
		{
			if (VARIABLES.count(element.first))
			{
				if (!CheckIfNameIsContained((*methods)[MethodCall.NAME].Parameters, element.first))
				{
					VARIABLES[element.first] = element.second;
				}
			}
		}
		return tok;
	}
}

Token Parse(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "USING")
		{
			if (ContainedLibraries.count(tokens[cursor].NAME))
			{
				for (pair<string, method> var : ContainedLibraries[tokens[cursor].NAME])
				{
					METHODS.insert(var);
				}
			}
			else
			{
				fstream lib;
				string path = libPath + tokens[cursor].NAME + ".ssl";
				lib.open(path, ios::in);
				string code = "";
				if (lib.is_open())
				{
					string line;
					while (getline(lib, line))
					{
						code += line + '\n';
					}
					lib.close();
				}
				else
				{
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Failed to load library " + tokens[cursor].NAME;
					return errorTok;
				}
				Token libToken = ParseLib(LexText(code), methods);
				if (libToken.ID == "ERROR") return libToken;
			}
		}
		else if (tokens[cursor].ID == "IF")
		{
			Token boolTok = ParseBool(tokens[cursor].EvalStatement[0], methods, Variables);

			if (boolTok.ID == "ERROR")
			{
				return boolTok;
			}
			else if (boolTok.ID != "BOOL")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Input in if statement must be of type bool";
				return errorToken;
			}

			if (boolTok.boolVal)
			{
				Token tok = Parse(tokens[cursor].ExecStatement, methods, Variables);
				if (cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "ELSE") cursor++;
				if (tok.ID != "NORETURN") return tok;
			}
			else
			{
				if (cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "ELSE")
				{
					Token tok = Parse(tokens[cursor + 1].ExecStatement, methods, Variables);
					cursor++;
					if (tok.ID != "NORETURN") return tok;
				}
			}
		}
		else if (tokens[cursor].ID == "ELSE")
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Could not find if statement";
			return errorToken;
		}
		else if (tokens[cursor].ID == "WHILE")
		{
			Token run = ParseBool(tokens[cursor].EvalStatement[0], methods, Variables);

			if (run.ID == "ERROR")
			{
				return run;
			}
			else if (run.ID != "BOOL")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Input in if statement must be of type bool";
				return errorToken;
			}
			while (run.boolVal)
			{
				Token tok = Parse(tokens[cursor].ExecStatement, methods, Variables);
				//if (tok.ID != "NORETURN") return tok;
				if (tok.ID == "ERROR") return tok;
				run = ParseBool(tokens[cursor].EvalStatement[0], methods, Variables);
				if (run.ID == "ERROR")
				{
					return run;
				}
				else if (run.ID != "BOOL")
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Input in if statement must be of type bool";
					return errorToken;
				}
			}
		}
		else if (tokens[cursor].ID == "DEFINITION")
		{
			method Method = method(tokens[cursor].NAME, tokens[cursor].EvalStatement, tokens[cursor].ExecStatement);
			(*methods)[Method.Name] = Method;
		}
		else if (tokens[cursor].ID == "VAR")
		{
			if (!(*Variables).count(tokens[cursor].NAME))
			{
				(*Variables)[tokens[cursor].NAME] = Token(0);
			}
			if (tokens.size() == 1) return (*Variables)[tokens[cursor].NAME];
		}
		else if (tokens[cursor].ID == "SETEQUALS")
		{
			Token tok = Parse(tokens[cursor].EvalStatement[0], methods, Variables);

			if (tok.ID == "ERROR") return tok;

			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				(*Variables)[tokens[cursor - 1].NAME] = tok;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Assignment operator requires variable on left side";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "PLUSEQUALS")
		{
			Token tok = Parse(tokens[cursor].EvalStatement[0], methods, Variables);
			if (tok.ID == "ERROR") return tok;
			if (tok.ID != "FLOAT" && tok.ID != "INT")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Operator requires type int or float";
				return errorToken;
			}

			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal += tok.GetTokenValueAsFloat();
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					if (tok.ID == "FLOAT")
					{
						(*Variables)[tokens[cursor - 1].NAME].ID = "FLOAT";
						(*Variables)[tokens[cursor - 1].NAME].floatVal = (*Variables)[tokens[cursor - 1].NAME].intVal + tok.floatVal;
					}
					else
					{
						(*Variables)[tokens[cursor - 1].NAME].intVal += tok.intVal;
					}
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Operator requires type int or float";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Assignment operator requires variable on left side";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "MINUSEQUALS")
		{
			Token tok = Parse(tokens[cursor].EvalStatement[0], methods, Variables);
			if (tok.ID == "ERROR") return tok;
			if (tok.ID != "FLOAT" && tok.ID != "INT")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Operator requires type int or float";
				return errorToken;
			}

			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal -= tok.GetTokenValueAsFloat();
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					if (tok.ID == "FLOAT")
					{
						(*Variables)[tokens[cursor - 1].NAME].ID = "FLOAT";
						(*Variables)[tokens[cursor - 1].NAME].floatVal = (*Variables)[tokens[cursor - 1].NAME].intVal - tok.floatVal;
					}
					else
					{
						(*Variables)[tokens[cursor - 1].NAME].intVal -= tok.intVal;
					}
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Operator requires type int or float";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Assignment operator requires variable on left side";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "MULTIPLYEQUALS")
		{
			Token tok = Parse(tokens[cursor].EvalStatement[0], methods, Variables);
			if (tok.ID == "ERROR") return tok;
			if (tok.ID != "FLOAT" && tok.ID != "INT")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Operator requires type int or float";
				return errorToken;
			}

			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal *= tok.GetTokenValueAsFloat();
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					if (tok.ID == "FLOAT")
					{
						(*Variables)[tokens[cursor - 1].NAME].ID = "FLOAT";
						(*Variables)[tokens[cursor - 1].NAME].floatVal = (*Variables)[tokens[cursor - 1].NAME].intVal * tok.floatVal;
					}
					else
					{
						(*Variables)[tokens[cursor - 1].NAME].intVal *= tok.intVal;
					}
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Operator requires type int or float";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Assignment operator requires variable on left side";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "DIVIDEEQUALS")
		{
			Token tok = Parse(tokens[cursor].EvalStatement[0], methods, Variables);
			if (tok.ID == "ERROR") return tok;
			if (tok.ID != "FLOAT" && tok.ID != "INT")
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Operator requires type int or float";
				return errorToken;
			}

			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal /= tok.GetTokenValueAsFloat();
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					if (tok.ID == "FLOAT")
					{
						(*Variables)[tokens[cursor - 1].NAME].ID = "FLOAT";
						(*Variables)[tokens[cursor - 1].NAME].floatVal = (*Variables)[tokens[cursor - 1].NAME].intVal / tok.floatVal;
					}
					else
					{
						(*Variables)[tokens[cursor - 1].NAME].intVal /= tok.intVal;
					}
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Operator requires type int or float";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Assignment operator requires variable on left side";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "INCREMENT")
		{
			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					(*Variables)[tokens[cursor - 1].NAME].intVal++;
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal++;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Variable must be of type int or float when incrementing";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant increment when there is no variable";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "DECREMENT")
		{
			if (cursor > 0 && tokens[cursor - 1].ID == "VAR")
			{
				if ((*Variables)[tokens[cursor - 1].NAME].ID == "INT")
				{
					(*Variables)[tokens[cursor - 1].NAME].intVal--;
				}
				else if ((*Variables)[tokens[cursor - 1].NAME].ID == "FLOAT")
				{
					(*Variables)[tokens[cursor - 1].NAME].floatVal--;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Variable must be of type int or float when decrementing";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant increment when there is no variable";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "METHOD")
		{
			Token tok = ParseMethodCall(tokens[cursor], methods, Variables);
			if (tok.ID == "ERROR") return tok;
			if (tokens.size() == 1) return tok;
		}
		else if (tokens[cursor].ID == "LIST")
		{
			Token tok;
			tok.ID = "LIST";
			for (int i = 0; i < tokens[cursor].EvalStatement.size(); i++)
			{
				Token elementToken = Parse(tokens[cursor].EvalStatement[i], methods, Variables);
				if (elementToken.ID == "ERROR") return elementToken;
				tok.EvalStatement.push_back(vector<Token>{elementToken});
			}
			if (tokens.size() == 1)
			{
				return tok;
			}
		}
		else if (tokens[cursor].ID == "RETURN")
		{
			return Parse(tokens[cursor].EvalStatement[0], methods, Variables);
		}
		else if (tokens[cursor].ID == "FLOAT" || tokens[cursor].ID == "INT")
		{
			return ParseArithmetic(tokens, methods, Variables);
		}
		else if (tokens[cursor].ID == "BOOL")
		{
			return Token(ParseBool(tokens, methods, Variables));
		}
		else if (tokens[cursor].ID == "STRING")
		{
			return tokens[cursor];
		}
		else if (MathIDS.count(tokens[cursor].ID))
		{
			return ParseArithmetic(tokens, methods, Variables);
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Unable to parse token";
			return errorToken;
		}
		cursor++;
	}
	Token tokk = Token(0);
	tokk.ID = "NORETURN";
	return tokk;
}