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
#include <irrKlang/irrKlang.h>

using namespace std;

string codePath = "D:\\VisualStudioProjects\\SeaSharkPLUSPLUS\\SeaSharkPP\\SeaSharkpp\\x64\\Debug\\code.ss";


int main(int argc, char** argv)
{
	if (argc >= 2)
	{
		codePath = argv[1];
		string temp = "";
		int val = 0;
		for (int i = string(argv[1]).size() - 1; i >= 0; i--)
		{
			if (argv[1][i] == '\\')
			{
				val = i;
				break;
			}
		}
		for (int i = 0; i <= val; i++)
		{
			temp += argv[1][i];
		}
		libPath = temp;
	}

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