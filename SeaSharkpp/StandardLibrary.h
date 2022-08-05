#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>

bool compareTokens(Token a, Token b)
{
	if (a.ID == b.ID)
	{
		if (a.ID == "STRING")
		{
			return a.stringVal == b.stringVal;
		}
		else if (a.ID == "INT")
		{
			return a.intVal == b.intVal;
		}
		else if (a.ID == "FLOAT")
		{
			return a.floatVal == b.floatVal;
		}
		else if (a.ID == "BOOL")
		{
			return a.boolVal == b.boolVal;
		}
		else if (a.ID == "LIST")
		{
			if (a.EvalStatement.size() == b.EvalStatement.size())
			{
				for (int i = 0; i < a.EvalStatement.size(); i++)
				{
					if (!compareTokens(a.EvalStatement[i][0], b.EvalStatement[i][0]))
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

Token print(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
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
Token printline(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
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
Token read(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token tok;
	tok.ID = "STRING";
	cin >> tok.stringVal;
	return tok;
}
Token Int(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
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
Token Float(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
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
Token String(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token tok = MethodCall.EvalStatement[0][0];
	Token result;
	result.ID = "STRING";

	if (tok.ID == "STRING")
	{
		return tok;
	}
	else if (tok.ID == "INT")
	{
		result.stringVal = to_string(tok.intVal);
	}
	else if (tok.ID == "FLOAT")
	{
		result.stringVal = to_string(tok.floatVal);
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Parameter in string() method call must be of type string, int or float";
		return errorToken;
	}

	return result;
}
Token random(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
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
Token append(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		listToken.EvalStatement.push_back(vector<Token>{MethodCall.EvalStatement[1][0]});
		return listToken;
	}
	else if (listToken.ID == "STRING")
	{
		Token appendToken = MethodCall.EvalStatement[1][0];
		if (appendToken.ID == "STRING")
		{
			listToken.stringVal += appendToken.stringVal;
			return listToken;
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "append value to string must be of type string";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in append() must be of type list or string";
		return errorToken;
	}
}
Token appendmultiple(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		Token appendToken = MethodCall.EvalStatement[1][0];
		if(appendToken.ID == "LIST")
		{
			for (int i = 0; i < appendToken.EvalStatement.size(); i++)
			{
				listToken.EvalStatement.push_back(appendToken.EvalStatement[i]);
			}
			return listToken;
		}
		else
		{
			return ErrorToken("Second parameter in appendmultiple() call must be of type list");
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in appendmultiple() must be of type list";
		return errorToken;
	}
}
Token get(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];

	if (listToken.ID == "LIST")
	{
		Token indexToken = MethodCall.EvalStatement[1][0];
		if (indexToken.ID == "INT")
		{
			if (indexToken.intVal < listToken.EvalStatement.size())
			{
				return listToken.EvalStatement[indexToken.intVal][0];
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant access index that doesn't exist in list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Second parameter in get() call must be of type int";
			return errorToken;
		}
	}
	else if (listToken.ID == "STRING")
	{
		Token indexToken = MethodCall.EvalStatement[1][0];
		if (indexToken.ID == "INT")
		{
			if (indexToken.intVal < listToken.stringVal.size())
			{
				Token returnTok;
				returnTok.ID = "STRING";
				returnTok.stringVal = listToken.stringVal[indexToken.intVal];
				return returnTok;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant access index that doesn't exist in string";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Second parameter in get() call must be of type int";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in get() call must be of type list";
		return errorToken;
	}

}
Token remove(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		Token indexToken = MethodCall.EvalStatement[1][0];
		if (indexToken.ID == "INT")
		{
			if (indexToken.intVal < listToken.EvalStatement.size())
			{
				listToken.EvalStatement.erase(listToken.EvalStatement.begin() + indexToken.intVal);
				return listToken;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant access index in list that does not exist";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Second parameter in remove() call must be of type int";
			return errorToken;
		}
	}
	else if (listToken.ID == "STRING")
	{
		Token indexToken = MethodCall.EvalStatement[1][0];
		if (indexToken.ID == "INT")
		{
			if (indexToken.intVal < listToken.stringVal.size())
			{
				listToken.stringVal.erase(listToken.stringVal.begin() + indexToken.intVal);
				return listToken;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant access index in string that does not exist";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Second parameter in remove() call must be of type int";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in remove() call must be of type list or string";
		return errorToken;
	}
}
Token set(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		Token indexToken = MethodCall.EvalStatement[1][0];
		if (indexToken.ID == "INT")
		{
			if (indexToken.intVal < listToken.EvalStatement.size())
			{
				listToken.EvalStatement[indexToken.intVal] = vector<Token>{ MethodCall.EvalStatement[2][0] };
				return listToken;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Cant access index that does not exist in list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Second parameter in set() call must be of type int";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in set() call must be of type list";
		return errorToken;
	}
}
Token size(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		Token result;
		result.ID = "INT";
		result.intVal = listToken.EvalStatement.size();
		return result;
	}
	else if (listToken.ID == "STRING")
	{
		Token result;
		result.ID = "INT";
		result.intVal = listToken.stringVal.size();
		return result;
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in size() call must be of type list or string";
		return errorToken;
	}
}
Token contains(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token listToken = MethodCall.EvalStatement[0][0];
	if (listToken.ID == "LIST")
	{
		Token checkToken = MethodCall.EvalStatement[1][0];
		for (int i = 0; i < listToken.EvalStatement.size(); i++)
		{
			if (compareTokens(listToken.EvalStatement[i][0], checkToken))
			{
				return Token(true);
			}
		}
		return Token(false);
	}
	else if (listToken.ID == "STRING")
	{
		Token charToken = MethodCall.EvalStatement[1][0];
		if (charToken.ID == "STRING")
		{
			if (charToken.stringVal.size() == 1)
			{
				for (int i = 0; i < listToken.stringVal.size(); i++)
				{
					if (listToken.stringVal[i] == charToken.stringVal[0])
					{
						return Token(true);
					}
				}
				return Token(false);
			}
			else
			{
				return ErrorToken("Second parameter in contains() call when string must be a single character");
			}
		}
		else
		{
			return ErrorToken("Cant compare string values to any other type");
		}
	}
	else
	{
		return ErrorToken("First parameter in contains() call must be of type list or string");
	}
}

unordered_map<string, method> SystemMETHODS
{
	make_pair("print", method("print", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &print, true)),
	make_pair("printline", method("printline", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &printline, true)),
	make_pair("read", method("read", vector<vector<Token>>(), vector<Token>(), &read, true)),
	make_pair("int", method("int", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &Int, true)),
	make_pair("float", method("float", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &Float, true)),
	make_pair("string", method("string", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &String, true)),
	make_pair("random", method("random", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &random, true)),
	make_pair("append", method("append", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &append, true)),
	make_pair("appendmultiple", method("appendmultiple", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &appendmultiple, true)),
	make_pair("get", method("get", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &get, true)),
	make_pair("set", method("set", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &set, true)),
	make_pair("remove", method("remove", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &remove, true)),
	make_pair("size", method("size", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &size, true)),
	make_pair("contains", method("contains", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &contains, true))
};