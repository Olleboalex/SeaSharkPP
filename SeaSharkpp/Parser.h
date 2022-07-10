#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>

#define TOK vector<Token>

using namespace std;

unordered_set<string> MathIDS{
	"PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD", "LPAR", "RPAR", "RCURLBRACK", "LCURLBRACK",
	"EQUALS", "NOTEQUALS", "BIGEQUALS", "SMALLEQUALS", "SMALLER", "BIGGER", "INCREMENT",
	"DECREMENT", "PLUSEQUALS", "MINUSEQUALS", "MULTIPLYEQUALS", "DIVIDEEQUALS"
};

unordered_map<string, method> METHODS{
	make_pair("print", method("print", vector<vector<Token>> {vector<Token>()}, vector<Token>(), true)),
	make_pair("printline", method("printline", vector<vector<Token>> {vector<Token>()}, vector<Token>(), true)),
	make_pair("read", method("read", vector<vector<Token>>(), vector<Token>(), true)),
	make_pair("int", method("int", vector<vector<Token>> {vector<Token>()}, vector<Token>(), true)),
	make_pair("float", method("float", vector<vector<Token>> {vector<Token>()}, vector<Token>(), true)),
	make_pair("random", method("random", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), true)),
	make_pair("append", method("append", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), true)),
	make_pair("get", method("get", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), true)),
	make_pair("size", method("size", vector<vector<Token>> {vector<Token>()}, vector<Token>(), true))
};
unordered_map<string, Token> VARIABLES;

Token SystemMethod(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (MethodCall.NAME == "print")
	{
		if (MethodCall.EvalStatement.size() == 1)
		{
			Token result = Parse(MethodCall.EvalStatement[0], methods, Variables);
			if (result.ID == "FLOAT" || result.ID == "INT")
			{
				cout << result.GetTokenValueAsFloat();
			}
			else if (result.ID == "STRING")
			{
				cout << result.stringVal;
			}
			else if (result.ID == "ERROR")
			{
				return result;
			}
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Incorrect number of inputs in print() function call";
		}
	}
	else if (MethodCall.NAME == "printline")
	{
		if (MethodCall.EvalStatement.size() == 1)
		{
			Token result = Parse(MethodCall.EvalStatement[0], methods, Variables);
			if (result.ID == "FLOAT" || result.ID == "INT")
			{
				cout << result.GetTokenValueAsFloat() << endl;
			}
			else if (result.ID == "STRING")
			{
				cout << result.stringVal << endl;
			}
			else if (result.ID == "ERROR")
			{
				return result;
			}
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Incorrect number of inputs in print() function call";
		}
	}
	else if (MethodCall.NAME == "read")
	{
		Token tok;
		tok.ID = "STRING";
		cin >> tok.stringVal;
		return tok;
	}
	else if (MethodCall.NAME == "int")
	{
		if (MethodCall.EvalStatement.size() != 1 || MethodCall.EvalStatement[0].size() != 1)
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Incorrect input in int() method call";
		}
		Token tok = MethodCall.EvalStatement[0][0];
		if (MethodCall.EvalStatement[0][0].ID == "VAR")
		{
			tok = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
		}
		else if (MethodCall.EvalStatement[0][0].ID == "METHOD")
		{
			tok = Parse(MethodCall.EvalStatement[0], methods, Variables);
		}

		if (tok.ID == "STRING")
		{
			Token token = Token(tok.stringVal);
			token.intVal = static_cast<int>(token.GetTokenValueAsFloat());
			token.ID = "INT";
			return token;
		}
		else if (tok.ID == "FLOAT" || tok.ID == "INT")
		{
			Token token;
			token.ID = "INT";
			token.intVal = static_cast<int>(tok.GetTokenValueAsFloat());
			return token;
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Input must be of type string or float";
			return errorTok;
		}
	}
	else if (MethodCall.NAME == "float")
	{
		if (MethodCall.EvalStatement.size() != 1 || MethodCall.EvalStatement[0].size() != 1)
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Incorrect input in float() method call";
		}
		Token tok = MethodCall.EvalStatement[0][0];
		if (MethodCall.EvalStatement[0][0].ID == "VAR")
		{
			tok = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
		}
		else if (MethodCall.EvalStatement[0][0].ID == "METHOD")
		{
			tok = Parse(MethodCall.EvalStatement[0], methods, Variables);
		}

		if (tok.ID == "STRING")
		{
			Token token = Token(tok.stringVal);
			token.floatVal = token.GetTokenValueAsFloat();
			token.ID = "FLOAT";
			return token;
		}
		else if (tok.ID == "INT" || tok.ID == "FLOAT")
		{
			Token token;
			token.ID = "FLOAT";
			token.floatVal = tok.GetTokenValueAsFloat();
			return token;
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Input must be of type string, int or float";
			return errorTok;
		}
	}
	else if (MethodCall.NAME == "random")
	{
		if (MethodCall.EvalStatement.size() == 2)
		{
			Token LowerLimit = Parse(MethodCall.EvalStatement[0], methods, Variables);
			Token UpperLimit = Parse(MethodCall.EvalStatement[1], methods, Variables);

			if (LowerLimit.ID == "INT" && UpperLimit.ID == "INT")
			{
				if (LowerLimit.intVal < UpperLimit.intVal)
				{
					srand(time(NULL));
					int result = rand() % (UpperLimit.intVal + 1 - LowerLimit.intVal) + LowerLimit.intVal;
					Token tok;
					tok.ID = "INT";
					tok.intVal = result;
					return tok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "random() lower limit must be lower than upper limit";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Parameters in random() must be of type int";
				return errorToken;
			}

		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Incorrect number of inputs in random() function call";
			return errorToken;
		}
	}
	else if (MethodCall.NAME == "append")
	{
		if (MethodCall.EvalStatement[0].size() == 1 && MethodCall.EvalStatement[1].size() == 1)
		{
			if (MethodCall.EvalStatement[0][0].ID == "VAR")
			{
				if ((*Variables).count(MethodCall.EvalStatement[0][0].NAME))
				{
					Token varTok = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
					if (varTok.ID == "LIST")
					{
						if (MethodCall.EvalStatement[1][0].ID == "VAR")
						{
							Token secVarTok = (*Variables)[MethodCall.EvalStatement[1][0].NAME];
							if (secVarTok.ID == "ERROR")
							{
								return secVarTok;
							}
							else
							{
								Token tok = varTok;
								tok.EvalStatement.push_back(vector<Token>{secVarTok});
								return tok;
							}
						}
						else if (MethodCall.EvalStatement[1][0].ID == "ERROR")
						{
							return MethodCall.EvalStatement[1][0];
						}
						else
						{
							Token tok = varTok;
							tok.EvalStatement.push_back(vector<Token>{MethodCall.EvalStatement[1][0]});
							return tok;
						}
					}
					else if (varTok.ID == "ERROR")
					{
						return varTok;
					}
					else
					{
						Token errorToken;
						errorToken.ID = "ERROR";
						errorToken.NAME = "First input in append() must be of type list";
						return errorToken;
					}
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Variable does not exist";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "First input in append() must be of type list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Incorrect input in append() method call";
			return errorToken;
		}
	}
	else if (MethodCall.NAME == "get")
	{
		if (MethodCall.EvalStatement[0].size() == 1 && MethodCall.EvalStatement[1].size() == 1)
		{
			if (MethodCall.EvalStatement[0][0].ID == "VAR")
			{
				Token tok = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
				if (tok.ID == "LIST")
				{
					if (MethodCall.EvalStatement[1][0].ID == "INT")
					{
						if (tok.EvalStatement.size() > MethodCall.EvalStatement[1][0].intVal)
						{
							vector<Token> temp = tok.EvalStatement[MethodCall.EvalStatement[1][0].intVal];
							if (temp.size() == 1 && temp[0].ID != "ERROR")
							{
								return temp[0];
							}
							else
							{
								Token errorToken;
								errorToken.ID = "ERROR";
								errorToken.NAME = "Error parsing list element";
								return errorToken;
							}
						}
						else
						{
							Token errorToken;
							errorToken.ID = "ERROR";
							errorToken.NAME = "Cant access index that does not exist";
							return errorToken;
						}
					}
					else if (MethodCall.EvalStatement[1][0].ID == "VAR")
					{
						Token varTok = (*Variables)[MethodCall.EvalStatement[1][0].NAME];
						if (varTok.ID == "INT")
						{
							if (tok.EvalStatement.size() > varTok.intVal)
							{
								vector<Token> temp = tok.EvalStatement[varTok.intVal];
								if (temp.size() == 1 && temp[0].ID != "ERROR")
								{
									return temp[0];
								}
								else
								{
									Token errorToken;
									errorToken.ID = "ERROR";
									errorToken.NAME = "Error parsing list element";
									return errorToken;
								}
							}
							else
							{
								Token errorToken;
								errorToken.ID = "ERROR";
								errorToken.NAME = "Cant access index that does not exist";
								return errorToken;
							}
						}
						else if(varTok.ID == "ERROR")
						{
							return varTok;
						}
						else
						{
							Token errorToken;
							errorToken.ID = "ERROR";
							errorToken.NAME = "Second parameter in get() call must be of type int";
							return errorToken;
						}
					}
					else if (MethodCall.EvalStatement[1][0].ID == "ERROR")
					{
						return MethodCall.EvalStatement[1][0];
					}
					else
					{
						Token errorToken;
						errorToken.ID = "ERROR";
						errorToken.NAME = "Second parameter in get() call must be of type int";
						return errorToken;
					}
				}
				else if (tok.ID == "ERROR")
				{
					return tok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "First input in get() must be of type list";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "First input in get() must be of type list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Incorrect input in get() method call";
			return errorToken;
		}
	}
	else if (MethodCall.NAME == "size")
	{
		if (MethodCall.EvalStatement.size() == 1 && MethodCall.EvalStatement[0].size() == 1)
		{
			if (MethodCall.EvalStatement[0][0].ID == "LIST")
			{
				Token tok;
				tok.ID = "INT";
				tok.intVal = MethodCall.EvalStatement[0][0].EvalStatement.size();
				return tok;
			}
			else if (MethodCall.EvalStatement[0][0].ID == "VAR")
			{
				Token varTok = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
				if (varTok.ID == "LIST")
				{
					Token tok;
					tok.ID = "INT";
					tok.intVal = varTok.EvalStatement.size();
					return tok;
				}
				else if (varTok.ID == "ERROR")
				{
					return varTok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Input in size() must be of type LIST";
					return errorToken;
				}
			}
			else if (MethodCall.EvalStatement[0][0].ID == "METHOD")
			{
				Token methodTok = Parse(vector<Token> {MethodCall.EvalStatement[0][0]}, methods, Variables);

				if (methodTok.ID == "LIST")
				{
					Token tok;
					tok.ID = "INT";
					tok.intVal = methodTok.EvalStatement.size();
					return tok;
				}
				else if (methodTok.ID == "ERROR")
				{
					return methodTok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Input in size() must be of type LIST";
					return errorToken;
				}

			}
			else if (MethodCall.EvalStatement[0][0].ID == "ERROR")
			{
				return MethodCall.EvalStatement[0][0];
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Input in size() must be of type LIST";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Incorrect parameter size in size() method call";
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