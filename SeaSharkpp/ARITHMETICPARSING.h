#pragma once
#include "Lexer.h"
#include <unordered_map>
#include "Parser.h"

#define TOK vector<Token>

using namespace std;

Token Parse(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables);

Token SystemMethod(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables);

Token ParseMethodCall(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables);

Token ParseArithmeticPLUSMINUS(vector<Token> tokens)
{
	int cursor = 1;
	string ID = "INT";
	float result = 0;
	if (tokens[0].ID == "INT")
	{
		result = tokens[0].intVal;
	}
	else if (tokens[0].ID == "MINUS")
	{
		if (tokens[1].ID == "INT")
		{
			result = -tokens[1].intVal;
		}
		else
		{
			result = -tokens[1].floatVal;
			ID = "FLOAT";
		}
		cursor++;
	}
	else
	{
		result = tokens[0].floatVal;
		ID = "FLOAT";
	}
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
			if(cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT")
			{
				vector<Token> dotTokens{
					tokens[cursor], tokens[cursor + 1]
				};
				cursor++;
				while(cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT")
				{
					dotTokens.push_back(tokens[cursor + 1]);
					cursor++;
				}
				Token tok = Parse(dotTokens, methods, Variables);
				if (tok.ID != "INT" && tok.ID != "FLOAT")
				{
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Cant do arithmetic on strings and numbers";
					return errorTok;
				}
				resultTokens.push_back(tok);
			}
			else 
			{
				Token tok = (*Variables)[tokens[cursor].NAME];
				if (tok.ID != "INT" && tok.ID != "FLOAT")
				{
					cout << tok.ID << endl;
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Cant do arithmetic on strings and numbers";
					return errorTok;
				}
				resultTokens.push_back(tok);
			}
		}
		else if (tokens[cursor].ID == "METHOD")
		{
			if (cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT")
			{
				vector<Token> dotTokens{
					tokens[cursor], tokens[cursor + 1]
				};
				cursor++;
				while (cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT")
				{
					dotTokens.push_back(tokens[cursor + 1]);
					cursor++;
				}
				Token tok = Parse(dotTokens, methods, Variables);
				if (tok.ID != "INT" && tok.ID != "FLOAT")
				{
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Cant do arithmetic on strings and numbers";
					return errorTok;
				}
				resultTokens.push_back(tok);
			}
			else
			{
				Token tok = ParseMethodCall(tokens[cursor], methods, Variables);
				if (tok.ID == "ERROR") return tok;
				resultTokens.push_back(tok);
			}
		}
		else
		{
			resultTokens.push_back(tokens[cursor]);
		}
		cursor++;
	}
	return ParseArithmeticPARAN(resultTokens);
}

Token ParseBool(vector<Token> tokens, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (tokens.size() == 1)
	{
		if (tokens[0].ID == "BOOL")
		{
			return Token(tokens[0].boolVal);
		}
		else if (tokens[0].ID == "VAR")
		{
			Token varTok = (*Variables)[tokens[0].NAME];
			if (varTok.ID == "BOOL")
			{
				return varTok;
			}
			else if(varTok.ID == "ERROR")
			{
				return varTok;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Could not parse bool";
				return errorToken;
			}
		}
		else if (tokens[0].ID == "METHOD")
		{
			Token metTok = Parse(vector<Token>{tokens[0]}, methods, Variables);
			if (metTok.ID == "BOOL")
			{
				return metTok;
			}
			else if (metTok.ID == "ERROR")
			{
				return metTok;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Could not parse bool";
				return errorToken;
			}
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Could not parse bool";
			return errorTok;
		}
	}
	else if (tokens.size() == 2)
	{
		if (tokens[0].ID == "NOT")
		{
			if (tokens[1].ID == "BOOL")
			{
				return Token(!tokens[1].boolVal);
			}
			else if (tokens[1].ID == "VAR")
			{
				Token varTok = (*Variables)[tokens[1].NAME];
				if (varTok.ID == "BOOL")
				{
					return Token(!varTok.boolVal);
				}
				else if (varTok.ID == "ERROR")
				{
					return varTok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Could not parse bool";
					return errorToken;
				}
			}
			else if (tokens[1].ID == "METHOD")
			{
				Token metTok = Parse(vector<Token>{tokens[1]}, methods, Variables);
				if (metTok.ID == "BOOL")
				{
					return Token(!metTok.boolVal);
				}
				else if (metTok.ID == "ERROR")
				{
					return metTok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Could not parse bool";
					return errorToken;
				}
			}
			else
			{
				Token errorTok;
				errorTok.ID = "ERROR";
				errorTok.NAME = "Could not parse bool";
				return errorTok;
			}
		}
	}

	if(tokens.size() <= 3)
	{
		if(tokens[tokens.size() - 1].ID == "DOT")
		{
			Token tok = Parse(vector<Token> {tokens[tokens.size() - 2], tokens[tokens.size() - 1]}, methods, Variables);
			if (tok.ID == "ERROR") return tok;
			tokens.erase(tokens.begin() + tokens.size() - 1);
			tokens.erase(tokens.begin() + tokens.size() - 1);
			tokens.push_back(tok);
			return ParseBool(tokens, methods, Variables);
		}
	}

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
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() == ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else if (comparisor == "NOTEQUALS")
	{
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() != ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else if (comparisor == "BIGEQUALS")
	{
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() >= ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else if (comparisor == "SMALLEQUALS")
	{
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() <= ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else if (comparisor == "BIGGER")
	{
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() > ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else if (comparisor == "SMALLER")
	{
		return Token(ParseArithmetic(LSide, methods, Variables).GetTokenValueAsFloat() < ParseArithmetic(RSide, methods, Variables).GetTokenValueAsFloat());
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "No comparisor";
		return errorToken;
	}

	return Token(false);
}