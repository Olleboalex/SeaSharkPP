#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>


Token readFile(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	if (MethodCall.EvalStatement[0].size() == 1)
	{
		if (MethodCall.EvalStatement[0][0].ID == "STRING")
		{
			fstream lib;
			string path = MethodCall.EvalStatement[0][0].stringVal;
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
				errorTok.NAME = "Failed to find file";
				return errorTok;
			}
			Token tok;
			tok.ID = "STRING";
			tok.stringVal = code;
			return tok;
		}
		else if (MethodCall.EvalStatement[0][0].ID == "VAR")
		{
			Token result = (*Variables)[MethodCall.EvalStatement[0][0].NAME];
			if (result.ID == "STRING")
			{
				fstream lib;
				string path = result.stringVal;
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
					errorTok.NAME = "Failed to find file";
					return errorTok;
				}
				Token tok;
				tok.ID = "STRING";
				tok.stringVal = code;
				return tok;
			}
			else if(result.ID == "ERROR")
			{
				return result;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Input in readFile method call must be of type string";
				return errorToken;
			}
		}
		else if (MethodCall.EvalStatement[0][0].ID == "METHOD")
		{
			Token result = Parse((*methods)[MethodCall.EvalStatement[0][0].NAME].ExecutionStatements, methods, Variables);
			if (result.ID == "STRING")
			{
				fstream lib;
				string path = result.stringVal;
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
					errorTok.NAME = "Failed to find file";
					return errorTok;
				}
				Token tok;
				tok.ID = "STRING";
				tok.stringVal = code;
				return tok;
			}
			else if (result.ID == "ERROR")
			{
				return result;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Input in readFile method call must be of type string";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Input in readFile method call must be of type string";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "Input in readFile method call is incorrect";
		return errorToken;
	}
}

Token writeFile(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	if (MethodCall.EvalStatement[0][0].ID == "STRING")
	{
		if (MethodCall.EvalStatement[1][0].ID == "STRING")
		{
			ofstream file(MethodCall.EvalStatement[0][0].stringVal);
			file << MethodCall.EvalStatement[1][0].stringVal;
			file.close();
		}
		else if (MethodCall.EvalStatement[1][0].ID == "INT" || MethodCall.EvalStatement[1][0].ID == "FLOAT")
		{
			ofstream file(MethodCall.EvalStatement[0][0].stringVal);
			file << MethodCall.EvalStatement[1][0].GetTokenValueAsFloat();
			file.close();
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "writeFile() method call requires string, int or float as first parameter";
			return errorToken;
		}
		return noReturnToken();
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "writeFile() method call requires string as first parameter";
		return errorToken;
	}
}

unordered_map<string, method> FileWriterMETHODS
{
	make_pair("readFile", method("readFile", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &readFile, true)),
	make_pair("writeFile", method("writeFile", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &writeFile, true))
};