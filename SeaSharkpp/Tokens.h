#pragma once
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int ParseInt(string val)
{
	int result = 0;
	int valSize = val.size();
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
	return result;
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
		vector<Token> EvalStatement;
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
			for (int i = 0; i < numText.size(); i++)
			{
				if (numText[i] == '.')
				{
					decimalsfound++;
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
				ID = "UNKNOWN";
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
			EvalStatement = evalToks;
		}		
		Token(string id, vector<Token> evalToks, vector<Token> execToks)
		{
			ID = id;
			EvalStatement = evalToks;
			ExecStatement = execToks;
		}
		float GetTokenValueAsFloat()
		{
			if (ID == "INT") return intVal;
			return floatVal;
		}
};

struct method
{
	public:
		string Name;
		vector<Token> Parameters;
		vector<Token> ExecutionStatements;
		method(string name, vector<Token> params, vector<Token> exec)
		{
			Name = name;
			Parameters = params;
			ExecutionStatements = exec;
		}
		method()
		{
			Name = "";
		}
};