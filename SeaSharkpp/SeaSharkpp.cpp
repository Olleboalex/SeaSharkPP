#include <iostream>
#include "Tokens.h"
#include "Lexer.h"
#include "Parser.h"
#include <fstream>
#include <string>
#include "ARITHMETICPARSING.h"
#include <unordered_map>

using namespace std;

int main()
{
	fstream file;
	file.open("D:\\VisualStudioProjects\\SeaSharkpp\\x64\\Debug\\code.ss", ios::in);
	string code = "";
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			code += line + '\n';
		}
		file.close();
	}

	vector<Token> tokens = LexText(code);
	unordered_map<string, method> METHODS;
	unordered_map<string, Token> VARIABLES;
	Parse(tokens, &METHODS, &VARIABLES);
}