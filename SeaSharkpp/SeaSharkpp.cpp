#include <iostream>
#include "Tokens.h"
#include "Lexer.h"
#include "Parser.h"
#include <fstream>
#include <string>
#include "ARITHMETICPARSING.h"
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

string codePath = ".\\code.ss";

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

	Token runToken = Parse(tokens, &METHODS, &VARIABLES);

	if (runToken.ID == "ERROR")
	{
		cout << endl << "Error: " + runToken.NAME;
		cin.get();
	}
}