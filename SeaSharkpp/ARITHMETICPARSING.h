#pragma once
#include "Lexer.h"
#include <unordered_map>
#include "Parser.h"

#define TOK vector<Token>

using namespace std;

Token Parse(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables);

Token ParseArithmeticPLUSMINUS(vector<Token> tokens)
{
	string ID = "INT";
	float result = 0;
	if (tokens[0].ID == "INT")
	{
		result = tokens[0].intVal;
	}
	else
	{
		result = tokens[0].floatVal;
		ID = "FLOAT";
	}
	int cursor = 1;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "PLUS")
		{
			if (tokens[cursor + 1].ID == "INT")
			{
				result += tokens[cursor + 1].intVal;
			}
			else
			{
				result += tokens[cursor + 1].floatVal;
			}
		}
		else if (tokens[cursor].ID == "MINUS")
		{
			if (tokens[cursor + 1].ID == "INT")
			{
				result -= tokens[cursor + 1].intVal;
			}
			else
			{
				result -= tokens[cursor + 1].floatVal;
			}
		}
		else if (tokens[cursor].ID == "FLOAT")
		{
			ID = "FLOAT";
		}
		cursor++;
	}
	Token token = Token(0);
	token.ID = ID;
	if (ID == "INT")
	{
		token.intVal = static_cast<int>(result);
	}
	else
	{
		token.floatVal = result;
	}
	return token;
}

Token ParseArithmeticMultiplyDivide(vector<Token> tokens)
{
	vector<Token> resultTokens;
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (cursor < tokens.size() - 1)
		{
			if (tokens[cursor + 1].ID == "MULTIPLY" || tokens[cursor + 1].ID == "DIVIDE")
			{
				string ID = "INT";
				float result = 0;
				if (tokens[cursor].ID == "INT")
				{
					result = tokens[cursor].intVal;
				}
				else
				{
					result = tokens[cursor].floatVal;
					ID = "FLOAT";
				}
				cursor++;
				while (cursor < tokens.size() - 1)
				{
					if (tokens[cursor].ID == "MULTIPLY")
					{
						if (tokens[cursor + 1].ID == "FLOAT") ID = "FLOAT";
						result *= tokens[cursor + 1].GetTokenValueAsFloat();
					}
					else if (tokens[cursor].ID == "DIVIDE")
					{
						if (tokens[cursor + 1].ID == "FLOAT") ID = "FLOAT";
						result /= tokens[cursor + 1].GetTokenValueAsFloat();
					}
					else
					{
						break;
					}
					cursor += 2;
				}
				Token token = Token(0);
				token.ID = ID;
				if (ID == "INT")
				{
					token.intVal = static_cast<int>(result);
				}
				else
				{
					token.floatVal = result;
				}
				resultTokens.push_back(token);
			}
			if (cursor < tokens.size())
			{
				resultTokens.push_back(tokens[cursor]);
			}
		}
		else
		{
			resultTokens.push_back(tokens[cursor]);
		}
		cursor++;
	}
	return ParseArithmeticPLUSMINUS(resultTokens);
}

Token ParseArithmeticModulus(vector<Token> tokens)
{
	vector<Token> resultTokens;
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (cursor < tokens.size() - 1)
		{
			if (tokens[cursor + 1].ID == "MOD")
			{
				string ID = "INT";
				int result = 0;
				result = static_cast<int>(tokens[cursor].GetTokenValueAsFloat()) % static_cast<int>(tokens[cursor + 2].GetTokenValueAsFloat());
				Token token = Token(0);
				token.ID = ID;
				token.intVal = result;
				resultTokens.push_back(token);
				cursor += 2;
			}
			else
			{
				resultTokens.push_back(tokens[cursor]);
			}
		}
		else
		{
			resultTokens.push_back(tokens[cursor]);
		}
		cursor++;
	}
	return ParseArithmeticMultiplyDivide(resultTokens);
}

Token ParseArithmeticPARAN(vector<Token> tokens)
{
	vector<Token> resultTokens;
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "LPAR")
		{
			cursor++;
			vector<Token> tempTokens;
			while (cursor < tokens.size())
			{
				if (tokens[cursor].ID == "RPAR") break;
				tempTokens.push_back(tokens[cursor]);
				cursor++;
			}
			resultTokens.push_back(ParseArithmeticModulus(tempTokens));
		}
		else
		{
			resultTokens.push_back(tokens[cursor]);
		}
		cursor++;
	}
	return ParseArithmeticModulus(resultTokens);
}

Token ParseArithmetic(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	vector<Token> resultTokens;
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "VAR")
		{
			Token tok = (*Variables)[tokens[cursor].NAME];
			if (tok.ID != "INT" && tok.ID != "FLOAT")
			{
				Token errorTok;
				errorTok.ID = "ERROR";
				errorTok.NAME = "Cant do arithmetic on strings and numbers";
				return errorTok;
			}
			resultTokens.push_back(tok);
		}
		else if (tokens[cursor].ID == "METHOD")
		{
			resultTokens.push_back(Parse((*methods)[tokens[cursor].NAME].ExecutionStatements, methods, Variables));
		}
		else
		{
			resultTokens.push_back(tokens[cursor]);
		}
		cursor++;
	}
	return ParseArithmeticPARAN(resultTokens);
}

bool ParseBool(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (tokens.size() == 1 && tokens[0].ID == "BOOL") return tokens[0].boolVal;
	bool Equals = true;
	string comparisor;
	TOK LSide;
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "NOTEQUALS")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		else if (tokens[cursor].ID == "EQUALS")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		else if (tokens[cursor].ID == "BIGEQUALS")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		else if (tokens[cursor].ID == "SMALLEQUALS")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		else if (tokens[cursor].ID == "BIGGER")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		else if (tokens[cursor].ID == "SMALLER")
		{
			comparisor = tokens[cursor].ID;
			break;
		}
		LSide.push_back(tokens[cursor]);
		cursor++;
	}
	cursor += 1;
	TOK RSide;
	while (cursor < tokens.size())
	{
		RSide.push_back(tokens[cursor]);
		cursor++;
	}

	if (comparisor == "EQUALS")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() == ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}
	else if (comparisor == "NOTEQUALS")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() != ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}
	else if (comparisor == "BIGEQUALS")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() >= ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}
	else if (comparisor == "SMALLEQUALS")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() <= ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}
	else if (comparisor == "BIGGER")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() > ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}
	else if (comparisor == "SMALLER")
	{
		return ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() < ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat();
	}

	return false;
}