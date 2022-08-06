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
#include "SeaSharkML.h"
//#include <irrKlang/irrKlang.h>

using namespace std;

string codePath = "test.ss";

string version = "2.2";


int main(int argc, char** argv)
{

	if (argc >= 2)
	{
		codePath = argv[1];
		string temp = "";
		int val = 0;
		for (int i = string(argv[1]).size() - 1; i >= 0; i--)
		{
			if (argv[1][i] == '/')
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
	else
	{
		cout << "SeaShark version " << version << endl;
		return 0;
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
	else
	{
		cout << "Failed to find file at: " << codePath << endl;
		return 0;
	}
	vector<Token> tokens = LexText(code);

	Token runToken = Parse(tokens, &METHODS, &VARIABLES);

	if (runToken.ID == "ERROR")
	{
		cout << endl << "Error: " + runToken.NAME;
		cin.get();
	}
}
