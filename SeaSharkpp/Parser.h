#pragma once
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>

#define TOK vector<Token>

using namespace std;

unordered_set<string> MathIDS{ "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD", "LPAR", "RPAR", "RCURLBRACK", "LCURLBRACK", "EQUALS", "NOTEQUALS"};

Token ParseLib(vector<Token> tokens, unordered_map<string, method>* methods)
{
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "DEFINITION")
		{
			method Method = method(tokens[cursor].NAME, tokens[cursor].EvalStatement, tokens[cursor].ExecStatement);
			(*methods)[Method.Name] = Method;
		}
		else if (tokens[cursor].ID == "USING")
		{
			fstream lib;
			string path = ".\\" + tokens[cursor].NAME + ".ssl";
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


Token Parse(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "PRINT")
		{
			Token result = Parse(tokens[cursor].EvalStatement, methods, Variables);
			if (result.ID == "FLOAT" || result.ID == "INT")
			{
				cout << result.GetTokenValueAsFloat() << endl;
			}
			else if(result.ID == "STRING")
			{
				cout << result.stringVal << endl;
			}
			else if (result.ID == "ERROR")
			{
				return result;
			}
		}
		else if (tokens[cursor].ID == "USING")
		{
			fstream lib;
			string path = ".\\" + tokens[cursor].NAME + ".ssl";
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
			if (ParseBool(tokens[cursor].EvalStatement, methods, Variables))
			{
				Token tok = Parse(tokens[cursor].ExecStatement, methods, Variables);
				if (tok.ID != "NORETURN") return tok;
			}
		}
		else if (tokens[cursor].ID == "WHILE")
		{
			bool run = ParseBool(tokens[cursor].EvalStatement, methods, Variables);
			while (run)
			{
				Token tok = Parse(tokens[cursor].ExecStatement, methods, Variables);
				if (tok.ID != "NORETURN") return tok;
				run = ParseBool(tokens[cursor].EvalStatement, methods, Variables);
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
			Token tok = Parse(tokens[cursor].EvalStatement, methods, Variables);
			(*Variables)[tokens[cursor - 1].NAME] = tok;
		}
		else if (tokens[cursor].ID == "METHOD")
		{
			if (!(*methods).count(tokens[cursor].NAME))
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Function does not exist";
				return errorToken;
			}
			if ((*methods)[tokens[cursor].NAME].Parameters.size() != tokens[cursor].EvalStatement.size())
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Incorrect number of parameters in function call";
				return errorToken;
			}
			unordered_map<string, Token> funcVariables = (*Variables);
			for (int i = 0; i < (*methods)[tokens[cursor].NAME].Parameters.size(); i++)
			{
				vector<Token> token = { tokens[cursor].EvalStatement[i] };
				funcVariables[(*methods)[tokens[cursor].NAME].Parameters[i].NAME] = Parse(token, methods, Variables);
			}
			Token tok = Parse((*methods)[tokens[cursor].NAME].ExecutionStatements, methods, &funcVariables);
			for (int i = 0; i < (*methods)[tokens[cursor].NAME].Parameters.size(); i++)
			{
				if ((*Variables).count((*methods)[tokens[cursor].NAME].Parameters[i].NAME))
				{
					funcVariables[(*methods)[tokens[cursor].NAME].Parameters[i].NAME] = (*Variables)[(*methods)[tokens[cursor].NAME].Parameters[i].NAME];
				}
				else
				{
					funcVariables.erase((*methods)[tokens[cursor].NAME].Parameters[i].NAME);
				}
			}
			(*Variables) = funcVariables;
			if(tokens.size() == 1) return tok;
		}
		else if (tokens[cursor].ID == "RETURN")
		{
			return Parse(tokens[cursor].EvalStatement, methods, Variables);
		}
		else if (tokens[cursor].ID == "FLOAT" || tokens[cursor].ID == "INT")
		{
			return ParseArithmetic(tokens, methods, Variables);
		}
		else if (tokens[cursor].ID == "BOOl")
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
		cursor++;
	}
	Token tokk = Token(0);
	tokk.ID = "NORETURN";
	return tokk;
}