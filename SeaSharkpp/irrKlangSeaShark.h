#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>

using namespace std;
using namespace irrklang;

ISoundEngine* SoundEngine;

Token klang_Init(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	SoundEngine = createIrrKlangDevice();
	return noReturnToken();
}

Token klang_play2D(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	Token linkToken = MethodCall.EvalStatement[0][0];
	if (linkToken.ID == "STRING")
	{
		Token loopToken = MethodCall.EvalStatement[1][0];
		if(loopToken.ID == "BOOL")
		{
			SoundEngine->play2D(linkToken.stringVal.c_str(), loopToken.boolVal);
			return noReturnToken();
		}
		else
		{
			return ErrorToken("Second parameter in klang_play2D() call must be of type bool");
		}
	}
	else
	{
		return ErrorToken("First parameter in klang_play2D() call must be of type string");
	}
}

Token klang_setVolume(Token MethodCall, unordered_map<string, method>* methods, unordered_map<string, Token>* Variables)
{
	Token volumeToken = MethodCall.EvalStatement[0][0];
	if(volumeToken.ID == "FLOAT")
	{
		SoundEngine->setSoundVolume(volumeToken.floatVal);
	}
	else if (volumeToken.ID == "INT")
	{
		SoundEngine->setSoundVolume(volumeToken.intVal);
	}
	else
	{
		return ErrorToken("First parameter in klang_setVolume() call must be of type int or float");
	}
	return noReturnToken();
}

unordered_map<string, method> irrKlangMethods
{
	make_pair("klang_Init", method("klang_Init", vector<vector<Token>>(), vector<Token>(), &klang_Init, true)),
	make_pair("klang_play2D", method("klang_play2D", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &klang_play2D, true)),
	make_pair("klang_setVolume", method("klang_setVolume", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &klang_setVolume, true))
};