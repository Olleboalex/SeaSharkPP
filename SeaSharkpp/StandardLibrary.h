#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>

Token print(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
		return errorTok;
	}
	Token tokk = Token(0);
	tokk.ID = "NORETURN";
	return tokk;
}
Token printline(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
	Token tokk = Token(0);
	tokk.ID = "NORETURN";
	return tokk;
}
Token read(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	Token tok;
	tok.ID = "STRING";
	cin >> tok.stringVal;
	return tok;
}
Token Int(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
Token Float(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
Token random(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
Token append(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
Token get(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
					else if (varTok.ID == "ERROR")
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
Token remove(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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
						tok.EvalStatement.erase(tok.EvalStatement.begin() + MethodCall.EvalStatement[1][0].intVal);
						return tok;
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
							tok.EvalStatement.erase(tok.EvalStatement.begin() + varTok.intVal);
							return tok;
						}
						else
						{
							Token errorToken;
							errorToken.ID = "ERROR";
							errorToken.NAME = "Cant access index that does not exist";
							return errorToken;
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
						errorToken.NAME = "Second parameter in remove() call must be of type int";
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
					errorToken.NAME = "Second parameter in remove() call must be of type int";
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
				errorToken.NAME = "First input in remove() must be of type list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "First input in remove() must be of type list";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Incorrect input in remove() method call";
		return errorToken;
	}
}
Token set(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	if (MethodCall.EvalStatement[0].size() == 1 && MethodCall.EvalStatement[1].size() == 1 && MethodCall.EvalStatement[2].size() == 1)
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
						if (MethodCall.EvalStatement[2][0].ID == "VAR")
						{
							Token varTok = (*Variables)[MethodCall.EvalStatement[2][0].NAME];
							if (varTok.ID != "ERROR")
							{
								tok.EvalStatement[MethodCall.EvalStatement[1][0].intVal] = vector<Token>{ varTok };
								return tok;
							}
							else
							{
								return varTok;
							}
						}
						else if (MethodCall.EvalStatement[2][0].ID == "ERROR")
						{
							return MethodCall.EvalStatement[2][0];
						}
						else
						{
							tok.EvalStatement[MethodCall.EvalStatement[1][0].intVal] = vector<Token>{ MethodCall.EvalStatement[2][0] };
							return tok;
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
							if (MethodCall.EvalStatement[2][0].ID == "VAR")
							{
								Token varTok = (*Variables)[MethodCall.EvalStatement[2][0].NAME];
								if (varTok.ID != "ERROR")
								{
									tok.EvalStatement[MethodCall.EvalStatement[1][0].intVal] = vector<Token>{ varTok };
									return tok;
								}
								else
								{
									return varTok;
								}
							}
							else if (MethodCall.EvalStatement[2][0].ID == "ERROR")
							{
								return MethodCall.EvalStatement[2][0];
							}
							else
							{
								tok.EvalStatement[MethodCall.EvalStatement[1][0].intVal] = vector<Token>{ MethodCall.EvalStatement[2][0] };
								return tok;
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
					else if (varTok.ID == "ERROR")
					{
						return varTok;
					}
					else
					{
						Token errorToken;
						errorToken.ID = "ERROR";
						errorToken.NAME = "Second parameter in set() call must be of type int";
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
					errorToken.NAME = "Second parameter in set() call must be of type int";
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
				errorToken.NAME = "First input in set() must be of type list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "First input in set() must be of type list";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Incorrect input in set() method call";
		return errorToken;
	}
}
Token size(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
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

unordered_map<string, method> SystemMETHODS
{
	make_pair("print", method("print", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &print, true)),
	make_pair("printline", method("printline", vector<vector<Token>> {vector<Token>()}, vector<Token>(),& printline, true)),
	make_pair("read", method("read", vector<vector<Token>>(), vector<Token>(), &read, true)),
	make_pair("int", method("int", vector<vector<Token>> {vector<Token>()}, vector<Token>(),& Int, true)),
	make_pair("float", method("float", vector<vector<Token>> {vector<Token>()}, vector<Token>(),& Float, true)),
	make_pair("random", method("random", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(),& random, true)),
	make_pair("append", method("append", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(),& append, true)),
	make_pair("get", method("get", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(),& get, true)),
	make_pair("set", method("set", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(),& set, true)),
	make_pair("remove", method("remove", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(),& remove, true)),
	make_pair("size", method("size", vector<vector<Token>> {vector<Token>()}, vector<Token>(),& size, true))
};