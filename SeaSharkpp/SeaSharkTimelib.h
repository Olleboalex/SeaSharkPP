#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>

using namespace chrono;

auto StartTime = high_resolution_clock::now();

Token Time(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    duration<float> elapsed = high_resolution_clock::now() - StartTime;
    Token result;
    result.ID = "FLOAT";
    result.floatVal = elapsed.count();
    return result;
}

unordered_map<string, method> TimeMethods
{
	make_pair("Time", method("Time", vector<vector<Token>>(), vector<Token>(), &Time, true))
};