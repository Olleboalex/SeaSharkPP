#include <iostream>
#include "Tokens.h"
#include "Lexer.h"
#include "Parser.h"
#include <fstream>
#include <string>
#include "ARITHMETICPARSING.h"
#include <unordered_map>

using namespace std;

string codePath = "D:\\VisualStudioProjects\\SeaSharkPLUSPLUS\\SeaSharkPP\\SeaSharkpp\\x64\\Debug\\code.ss";

int main()
{
	fstream file;
	file.open(codePath, ios::in);
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

	//cout << tokens.size();

	Token runToken = Parse(tokens, &METHODS, &VARIABLES);

	if (runToken.ID == "ERROR")
	{
		cout << "Error: " + runToken.NAME;
	}
}