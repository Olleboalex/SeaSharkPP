#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

int ParseInt(string val)
{
	int result = 0;
	int valSize = val.size();
	int multi = 1;
	if (val[0] == '-')
	{
		for (int i = 0; i < valSize - 1; i++)
		{
			result -= (val[valSize - 1 - i] - '0') * pow(10, i);
		}
		return result;
	}

	for (int i = 0; i < valSize; i++)
	{
		result += (val[valSize - 1 - i] - '0') * pow(10, i);
	}
	return result;
}

float ParseFloat(string val)
{
	float result = 0;
	string beforeDecimal = "";
	string afterDecimal = "";
	int i = 0;
	int sign = 1;
	// If number is negative ignore the first item
	if (val[0] == '-')
	{
		sign = -1;
		i = 1;
	}
	while (i < val.size())
	{
		if (val[i] == '.')
		{
			i++;
			break;
		}
		beforeDecimal += val[i];
		i++;
	}
	// Split the string by the decimal point
	while (i < val.size())
	{
		afterDecimal += val[i];
		i++;
	}
	// Parses the numbers after the decimal seperately to the ones before it
	for (int j = 0; j < beforeDecimal.size(); j++)
	{
		result += (beforeDecimal[j] - '0') * pow(10, beforeDecimal.size() - 1 - j);
	}
	for (int j = 0; j < afterDecimal.size(); j++)
	{
		result += (afterDecimal[j] - '0') * pow(10, -(j + 1));
	}
	return result * sign;
}

bool IsDigit(char c)
{
	return c >= '0' && c <= '9';
}

struct Token
{
	public:
		string ID;
		string varType;
		int intVal;
		float floatVal;
		bool boolVal;
		string stringVal;
		string NAME;
		vector<vector<Token>> EvalStatement;
		vector<Token> ExecStatement;

		// Input is single char operator
		// Constructor assigns values
		Token(char c)
		{
			switch (c)
			{
				case '+':
					ID = "PLUS";
					break;
				case '-':
					ID = "MINUS";
					break;
				case '*':
					ID = "MULTIPLY";
					break;
				case '/':
					ID = "DIVIDE";
					break;
				case '%':
					ID = "MOD";
					break;
				case '(':
					ID = "LPAR";
					break;
				case ')':
					ID = "RPAR";
					break;
				case '{':
					ID = "RCURLBRACK";
					break;
				case '}':
					ID = "LCURLBRACK";
					break;
				default:
					ID = "UNKNOWN";
					break;
			}
		}
		Token()
		{
			NAME = "";
			ID = "";
		}
		// Input is number represented as string
		// Constructor parses string and assigns value
		Token(string numText)
		{
			// Check for decimals
			// If one found then float
			// Otherwise int
			int decimalsfound = 0;
			bool Error = false;
			for (int i = 0; i < numText.size(); i++)
			{
				if (numText[i] == '.')
				{
					decimalsfound++;
				}
				else if (i != 0 && !IsDigit(numText[i]))
				{
					ID = "ERROR";
					NAME = "Cannot parse number";
					return;
				}
				else if (i == 0 && !IsDigit(numText[0]) && numText[0] != '-')
				{
					ID = "ERROR";
					NAME = "Cannot parse number";
					return;
				}
			}
			if (decimalsfound == 0)
			{
				ID = "INT";
				intVal = ParseInt(numText);
			}
			else if (decimalsfound == 1)
			{
				ID = "FLOAT";
				floatVal = ParseFloat(numText);
			}
			else
			{
				ID = "ERROR";
				NAME = "Cannot parse number";
			}
		}
		Token(int intval)
		{
			ID = "INT";
			intVal = intval;
		}
		Token(float floatval)
		{
			ID = "FLOAT";
			floatVal = floatval;
		}
		Token(bool boolval)
		{
			ID = "BOOL";
			boolVal = boolval;
		}
		Token(string id, string methodName)
		{
			ID = id;
			NAME = methodName;
		}
		Token(string id, vector<Token> evalToks)
		{
			ID = id;
			EvalStatement = vector<vector<Token>>{ evalToks };
		}		
		Token(string id, vector<Token> evalToks, vector<Token> execToks)
		{
			ID = id;
			EvalStatement = vector<vector<Token>>{ evalToks };
			ExecStatement = execToks;
		}
		float GetTokenValueAsFloat()
		{
			if (ID == "INT") return intVal;
			return floatVal;
		}
};

Token ErrorToken(string error)
{
	Token tok;
	tok.ID = "ERROR";
	tok.NAME = error;
	return tok;
}
Token noReturnToken()
{
	Token tok;
	tok.ID = "NORETURN";
	return tok;
}

struct method
{
	public:
		string Name;
		vector<vector<Token>> Parameters;
		vector<Token> ExecutionStatements;
		Token(*func)(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables);
		bool SystemMethod;
		bool fromLib;
		method(string name, vector<vector<Token>> params, vector<Token> exec)
		{
			Name = name;
			Parameters = params;
			ExecutionStatements = exec;
			SystemMethod = false;
			fromLib = false;
		}
		method(string name, vector<vector<Token>> params, vector<Token> exec, Token(*function)(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables), bool systemMethod)
		{
			Name = name;
			Parameters = params;
			ExecutionStatements = exec;
			SystemMethod = systemMethod;
			fromLib = false;
			func = function;
		}
		method(string name, vector<vector<Token>> params, vector<Token> exec, Token(*function)(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables), bool systemMethod, bool library)
		{
			Name = name;
			Parameters = params;
			ExecutionStatements = exec;
			SystemMethod = systemMethod;
			fromLib = library;
			func = function;
		}
		method(string name, vector<vector<Token>> params, vector<Token> exec, bool systemMethod, bool library)
		{
			Name = name;
			Parameters = params;
			ExecutionStatements = exec;
			SystemMethod = systemMethod;
			fromLib = library;
		}
		method()
		{
			Name = "";
			SystemMethod = false;
		}
};

/*
When calling on method in Properties first parameter supplied is always the object called on
*/
struct Properties
{
	public:
		unordered_map<string, method> METHODS;
		unordered_map<string, Token> VARIABLES;

		Properties(unordered_map<string, method> methods, unordered_map<string, Token> vars)
		{
			METHODS = methods;
			VARIABLES = vars;
		}
		Properties()
		{
			
		}
};