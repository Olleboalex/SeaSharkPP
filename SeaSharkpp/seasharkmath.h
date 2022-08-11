#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>

/*
Returns the sine of the given float which is in radians
*/
Token sssin(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = sin(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in sin() call must be of type int or float");
    }
}

/*
Returns the cosine of the given float which is in radians
*/
Token sscos(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = cos(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in cos() call must be of type int or float");
    }
}

/*
Returns the tangent of the given float which is in radians
*/
Token sstan(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = tan(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in tan() call must be of type int or float");
    }
}

/*
Returns asin of given float, output is in radians
*/
Token ssasin(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = asin(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in asin() call must be of type int or float");
    }
}

/*
Returns acos of given float, output is in radians
*/
Token ssacos(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = acos(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in acos() call must be of type int or float");
    }
}

/*
Returns atan of given float, output is in radians
*/
Token ssatan(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token angleToken = MethodCall.EvalStatement[0][0];
    if(angleToken.ID == "INT" || angleToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = atan(angleToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in atan() call must be of type int or float");
    }
}

/*
Returns pi
*/
Token ssPI(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token result;
    result.ID = "FLOAT";
    result.floatVal = M_PIf;
    return result;
}

/*
Returns e
*/
Token ssEulersNumber(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token result;
    result.ID = "FLOAT";
    result.floatVal = M_Ef;
    return result;
}

/*
Returns first float given raised to the power of the second float
*/
Token ssPow (Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token baseToken = MethodCall.EvalStatement[0][0];
    Token exponentToken = MethodCall.EvalStatement[1][0];

    if(baseToken.ID == "INT" || baseToken.ID == "FLOAT")
    {
        if(exponentToken.ID == "INT" || exponentToken.ID == "FLOAT")
        {
            Token resultToken;
            resultToken.ID = "FLOAT";
            resultToken.floatVal = powf(baseToken.GetTokenValueAsFloat(), exponentToken.GetTokenValueAsFloat());
            return resultToken;
        }
        else
        {
            return ErrorToken("Second parameter in pow() call must be of type int or float");
        }
    }
    else
    {
        return ErrorToken("First parameter in pow() call must be of type int or float");
    }

}

/*
Returns the square root of the given float
*/
Token sssqrt(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token inputToken = MethodCall.EvalStatement[0][0];
    if(inputToken.ID == "INT" || inputToken.ID == "FLOAT")
    {
        Token resultToken;
        resultToken.ID = "FLOAT";
        resultToken.floatVal = sqrtf(inputToken.GetTokenValueAsFloat());
        return resultToken;
    }
    else
    {
        return ErrorToken("First parameter in sqrt() call must be of type int or float");
    }
}

unordered_map<string, method> mathMethods
{
	make_pair("sin", method("sin", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &sssin, true)),
    make_pair("cos", method("cos", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &sscos, true)),
    make_pair("tan", method("tan", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &sstan, true)),
    make_pair("asin", method("asin", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ssasin, true)),
    make_pair("acos", method("acos", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ssacos, true)),
    make_pair("atan", method("atan", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ssatan, true)),
    make_pair("pi", method("pi", vector<vector<Token>>(), vector<Token>(), &ssPI, true)),
    make_pair("e", method("e", vector<vector<Token>>(), vector<Token>(), &ssEulersNumber, true)),
    make_pair("pow", method("pow", vector<vector<Token>>{vector<Token>(), vector<Token>()}, vector<Token>(), &ssPow, true)),
    make_pair("sqrt", method("sqrt", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &sssqrt, true)),
};
