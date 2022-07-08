#pragma once
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"

#define TOK vector<Token>

using namespace std;

unordered_set<string> MathIDS{ "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD", "LPAR", "RPAR", "RCURLBRACK", "LCURLBRACK", "EQUALS", "NOTEQUALS"};

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
			unordered_map<string, Token> funcVariables = (*Variables);
			for (int i = 0; i < (*methods)[tokens[cursor].NAME].Parameters.size(); i++)
			{
				vector<Token> token = { tokens[cursor].EvalStatement[i] };
				funcVariables[(*methods)[tokens[cursor].NAME].Parameters[i].NAME] = Parse(token, methods, Variables);
			}
			Token tok = Parse((*methods)[tokens[cursor].NAME].ExecutionStatements, methods, &funcVariables);
			for (int i = 0; i < (*methods)[tokens[cursor].NAME].Parameters.size(); i++)
			{
				funcVariables[(*methods)[tokens[cursor].NAME].Parameters[i].NAME] = Token(0);
			}
			(*Variables) = funcVariables;
			return tok;
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