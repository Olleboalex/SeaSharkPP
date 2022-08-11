#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include "StandardLibrary.h"
#include "FileWriter.h"
#include "OpenGLSeaSharkLibrary.h"
#include <memory>
#include "SeaSharkML.h"
#include "SeaSharkTimelib.h"
#include "seasharkmath.h"
//#include "irrKlangSeaShark.h"

#define TOK vector<Token>

using namespace std;

unordered_set<string> MathIDS{
	"PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD", "LPAR", "RPAR", "RCURLBRACK", "LCURLBRACK",
	"EQUALS", "NOTEQUALS", "BIGEQUALS", "SMALLEQUALS", "SMALLER", "BIGGER", "INCREMENT",
	"DECREMENT", "PLUSEQUALS", "MINUSEQUALS", "MULTIPLYEQUALS", "DIVIDEEQUALS", "NOT"
};

unordered_map<string, method> listMethods{
	make_pair("append", method("append", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &append, true)),
	make_pair("appendmultiple", method("appendmultiple", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &appendmultiple, true)),
	make_pair("get", method("get", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &get, true)),
	make_pair("set", method("set", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &set, true)),
	make_pair("remove", method("remove", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &remove, true)),
	make_pair("size", method("size", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &size, true)),
	make_pair("contains", method("contains", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &contains, true))
};

unordered_map<string, method> METHODS;

map<string, Token> VARIABLES;

unordered_map<string, Properties> props{
	make_pair("LIST", Properties(listMethods, map<string, Token>())),
	make_pair("STRING", Properties(listMethods, map<string, Token>()))
};

unordered_map<string, unordered_map<string, method>> ContainedLibraries{
	make_pair("System", SystemMETHODS),
	make_pair("FileWriter", FileWriterMETHODS),
	make_pair("OpenGL", OpenGLSSMethods),
	make_pair("ML", SSMLMethods),
	make_pair("Time", TimeMethods),
	make_pair("Mathematics", mathMethods)
	//make_pair("IrrKlang", irrKlangMethods)
};

Token UniversalStructSet(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token structToken = MethodCall.EvalStatement[0][0];
	Token IdToken = MethodCall.EvalStatement[1][0];
	Token setVal = MethodCall.EvalStatement[2][0];
	if(IdToken.ID == "STRING")
	{
		if(structToken.structVars.count(IdToken.stringVal))
		{
			structToken.structVars[IdToken.stringVal] = setVal;
			return structToken;
		}
		else
		{
			cout << structToken.ID;
			return ErrorToken("Property does not exist in struct");
		}
	}
	else
	{
		return ErrorToken("First parameter in set() call must be of type string");
	}
}

/*
Function parses elements in methodcall parameters and if none are of type error it calls the appropriate function.
If function call contains incorrect number of parameters returns error here.
*/
Token SystemMethod(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	if ((*methods).count(MethodCall.NAME))
	{
		if ((*methods)[MethodCall.NAME].SystemMethod)
		{
			for (int i = 0; i < MethodCall.EvalStatement.size(); i++)
			{
				Token result = Parse(MethodCall.EvalStatement[i], methods, Variables);
				if (result.ID == "ERROR") return result;
				MethodCall.EvalStatement[i] = vector<Token>{ result };
			}
			return (*methods)[MethodCall.NAME].func(MethodCall, methods, Variables);
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "This is not a built in function";
			return errorToken;
		}
	}
	return Token();
}

string libPath = "./";

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
			if (ContainedLibraries.count(tokens[cursor].NAME))
			{
				for (pair<string, method> var : ContainedLibraries[tokens[cursor].NAME])
				{
					METHODS.insert(var);
				}
			}
			else
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
		}
		else if(tokens[cursor].ID == "STRUCT")
		{
			unordered_map<string, method> meths{
				make_pair("set", method("set", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(),& UniversalStructSet, true))
			};
			map<string, Token> vars;
			Token tok = Parse(tokens[cursor].ExecStatement, &meths, &vars);
			if (tok.ID == "ERROR") return tok;
			props[tokens[cursor].NAME] = Properties(meths, vars);
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

Token ParseMethodCall(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{

	if(props.count(MethodCall.NAME))
	{
		if (MethodCall.EvalStatement.size() > 1)
		{
			return ErrorToken("Invalid contructor arguments");
		}

		Token tok;
		tok.ID = MethodCall.NAME;
		tok.structVars = props[MethodCall.NAME].VARIABLES;

		if (MethodCall.EvalStatement.size() == 1)
		{
			Token listToken = Parse(MethodCall.EvalStatement[0], methods, Variables);

			if (listToken.EvalStatement.size() % 2 == 1)
			{
				return ErrorToken("Invalid constructor arguments");
			}

			if (listToken.ID == "ERROR") return listToken;

			for (int i = 0; i < listToken.EvalStatement.size(); i += 2)
			{
				if (listToken.EvalStatement[i][0].ID == "STRING")
				{
					if (tok.structVars.count(listToken.EvalStatement[i][0].stringVal))
					{
						tok.structVars[listToken.EvalStatement[i][0].stringVal] = listToken.EvalStatement[i + 1][0];
					}
					else
					{
						return ErrorToken("Invalid constructor arguments");
					}
				}
				else
				{
					return ErrorToken("Invalid constructor arguments");
				}
			}
		}

		return tok;
	}

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
	map<string, Token> funcVariables = VARIABLES;
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
	map<string, Token> funcVariables = VARIABLES;
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

Token Parse(vector<Token> tokens, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	int cursor = 0;
	while (cursor < tokens.size())
	{
		if (tokens[cursor].ID == "USING")
		{
			if (ContainedLibraries.count(tokens[cursor].NAME))
			{
				for (pair<string, method> var : ContainedLibraries[tokens[cursor].NAME])
				{
					METHODS.insert(var);
				}
			}
			else
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
		}
		else if (tokens[cursor].ID == "IF")
		{
			Token boolTok = ParseBool(tokens[cursor].EvalStatement[0], methods, Variables);
			vector<Token> result;

			Token ifToken = tokens[cursor];

			if (cursor < tokens.size() - 1)
			{
				if (tokens[cursor + 1].ID == "ELSE")
				{
					result = tokens[cursor + 1].ExecStatement;
					cursor++;
				}
				else if (tokens[cursor + 1].ID == "ELIF")
				{
					Token tok;
					tok.ID = "IF";
					tok.EvalStatement = tokens[cursor + 1].EvalStatement;
					tok.ExecStatement = tokens[cursor + 1].ExecStatement;
					result.push_back(tok);
					cursor += 2;
					while (cursor < tokens.size())
					{
						if (tokens[cursor].ID == "ELIF" || tokens[cursor].ID == "ELSE")
						{
							result.push_back(tokens[cursor]);
							cursor++;
						}
						else
						{
							cursor--;
							break;
						}
					}
				}
			}

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
				Token tok = Parse(ifToken.ExecStatement, methods, Variables);
				if (tok.ID != "NORETURN") return tok;
			}
			else
			{
				Token res = Parse(result, methods, Variables);
				if (res.ID != "NORETURN") return res;
			}
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
				if (tok.ID != "NORETURN") return tok;
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
			if (cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT") 
			{

			}
			else 
			{
				if (!(*Variables).count(tokens[cursor].NAME))
				{
					(*Variables)[tokens[cursor].NAME] = Token(0);
				}
				if (tokens.size() == 1) return (*Variables)[tokens[cursor].NAME];
			}
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
				errorToken.NAME = "Cant decrement when there is no variable";
				return errorToken;
			}
		}
		else if (tokens[cursor].ID == "METHOD")
		{
			if (!(cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT"))
			{
				Token tok = ParseMethodCall(tokens[cursor], methods, Variables);
				if (tok.ID == "ERROR") return tok;
				if (tokens.size() == 1) return tok;
			}
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
		else if(tokens[cursor].ID == "DOT")
		{
			if(cursor < tokens.size() - 1 && tokens[cursor + 1].ID == "DOT")
			{
			}
			else
			{
				if (cursor > 0)
				{
					Token objTok = tokens[cursor - 1];
					string relevantID = "";
					if (objTok.ID == "VAR")
					{
						if ((*Variables).count(objTok.NAME))
						{
							objTok = (*Variables)[objTok.NAME];
						}
						else
						{
							return ErrorToken("That variable does not exist");
						}
					}
					else if (objTok.ID == "METHOD")
					{
						objTok = ParseMethodCall(objTok, methods, Variables);
						if (objTok.ID == "ERROR") return objTok;
					}
					else if (objTok.ID == "DOT")
					{
						objTok = Parse(vector<Token>{tokens[cursor - 2], tokens[cursor - 1]}, methods, Variables);
						if (objTok.ID == "ERROR") return objTok;
					}

					if (props.count(objTok.ID))
					{
						if (tokens[cursor].ExecStatement.size() == 1)
						{
							Token propTok = tokens[cursor].ExecStatement[0];
							if (propTok.ID == "VAR")
							{
								if (objTok.structVars.count(propTok.NAME))
								{
									Token tok = objTok.structVars[propTok.NAME];
									if (tok.ID == "ERROR") return tok;
									if (tokens.size() == 2) return tok;
									int propertyholders = 0;
									bool check = true;
									for (int i = 0; i < tokens.size(); i++)
									{
										if (tokens[i].ID == "VAR" || tokens[i].ID == "METHOD")
										{
											propertyholders++;
											if (propertyholders > 1)
											{
												check = false;
												break;
											}
										}
										else if (tokens[i].ID != "DOT")
										{
											check = false;
										}
									}
									if (check) return tok;
								}
								else
								{
									return ErrorToken("That property does not exist");
								}
							}
							else if (propTok.ID == "METHOD")
							{
								if (props[objTok.ID].METHODS.count(propTok.NAME))
								{
									if (props[objTok.ID].METHODS[propTok.NAME].SystemMethod)
									{
										propTok.EvalStatement.insert(propTok.EvalStatement.begin(), vector<Token>{objTok});
									}
									unordered_map<string, method> appropMethods = *methods;

									for (pair <string, method> element : props[objTok.ID].METHODS)
									{
										appropMethods[element.first] = element.second;
									}
									
									Token tok = ParseMethodCall(propTok, &appropMethods, Variables);
									if (tok.ID == "ERROR") return tok;
									if (tokens.size() == 2) return tok;
									int propertyholders = 0;
									bool check = true;
									for (int i = 0; i < tokens.size(); i++)
									{
										if (tokens[i].ID == "VAR" || tokens[i].ID == "METHOD")
										{
											propertyholders++;
											if (propertyholders > 1)
											{
												check = false;
												break;
											}
										}
										else if (tokens[i].ID != "DOT")
										{
											check = false;
										}
									}
									if (check) return tok;
								}
								else
								{
									return ErrorToken("That property does not exist");
								}
							}
							else
							{
								return ErrorToken("That is an incorrect property call");
							}
						}
						else
						{
							return ErrorToken("That is an incorrect property call");
						}
					}
					else
					{
						return ErrorToken("That structure has no properties");
					}
				}
				else
				{
					return ErrorToken("Property call must be applied to an object");
				}
			}
		}
		else if(tokens[cursor].ID == "STRUCT")
		{
			unordered_map<string, method> meths{
				make_pair("set", method("set", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(),& UniversalStructSet, true))
			};
			map<string, Token> vars;
			Token tok = Parse(tokens[cursor].ExecStatement, &meths, &vars);
			if (tok.ID == "ERROR") return tok;
			props[tokens[cursor].NAME] = Properties(meths, vars);
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
			if (tokens.size() == 1) return tokens[cursor];
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
