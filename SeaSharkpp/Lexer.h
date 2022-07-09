#pragma once
#include <iostream>
#include <vector>
#include "Tokens.h"
#include <unordered_set>

using namespace std;

unordered_set<char> mathOperators {'+', '-', '*', '/', '%', '(', ')', '=', '!'};

vector<Token> LexText(string Text)
{
	vector<Token> tokens;
	int cursor = 0;
	while (cursor < Text.size())
	{
		if (IsDigit(Text[cursor]))
		{
			// Find the whole number
			string temp = "";
			while (cursor < Text.size())
			{
				if (!IsDigit(Text[cursor]) && Text[cursor] != '.') break;
				temp += Text[cursor];
				cursor++;
			}
			tokens.push_back(Token(temp));
			if (cursor >= Text.size()) break;
		}
		if (Text[cursor] != ' ' && Text[cursor] != '\n' && Text[cursor] != '\t')
		{
			/*if (cursor < Text.size() - 4 && Text[cursor] == 'p' && Text[cursor + 1] == 'r' && Text[cursor + 2] == 'i' && Text[cursor + 3] == 'n' && Text[cursor + 4] == 't')
			{
				cursor += 5;
				int parans = 1;
				while (Text[cursor] != '(')
				{
					cursor++;
				}
				cursor++;
				string evalText = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '(') parans++;
					if (Text[cursor] == ')') parans--;
					if (parans == 0) break;
					evalText += Text[cursor];
					cursor++;
				}
				Token tok = Token("METHOD", vector<Token> {LexText(evalText)});
				tok.NAME = "PRINT";
				tokens.push_back(tok);
			}*/
			if (cursor < Text.size() - 1 && Text[cursor] == 'i' && Text[cursor + 1] == 'f')
			{
				cursor += 2;
				int parans = 1;
				while (Text[cursor] != '(')
				{
					cursor++;
				}
				cursor++;
				string evalText = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '(') parans++;
					if (Text[cursor] == ')') parans--;
					if (parans == 0) break;
					evalText += Text[cursor];
					cursor++;
				}
				while (Text[cursor] != '{')
				{
					cursor++;
				}
				cursor++;
				int curlyBracks = 1;
				string execText = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '{') curlyBracks++;
					if (Text[cursor] == '}') curlyBracks--;
					if (curlyBracks == 0) break;
					execText += Text[cursor];
					cursor++;
				}
				tokens.push_back(Token("IF", LexText(evalText), LexText(execText)));
			}
			else if (cursor < Text.size() - 4 && Text[cursor] == 'w' && Text[cursor + 1] == 'h' && Text[cursor + 2] == 'i' && Text[cursor + 3] == 'l' && Text[cursor + 4] == 'e')
			{
				cursor += 4;
				while (Text[cursor] != '(')
				{
					cursor++;
				}
				cursor++;
				string evalText = "";
				int Paran = 1;
				while (cursor < Text.size())
				{
					if (Text[cursor] == '(') Paran++;
					if (Text[cursor] == ')') Paran--;
					if (Paran == 0) break;
					evalText += Text[cursor];
					cursor++;
				}
				while (Text[cursor] != '{')
				{
					cursor++;
				}
				cursor++;
				string execText = "";
				int CurlyBracks = 1;
				while (cursor < Text.size())
				{
					if (Text[cursor] == '{') CurlyBracks++;
					if (Text[cursor] == '}') CurlyBracks--;
					if (CurlyBracks == 0) break;
					execText += Text[cursor];
					cursor++;
				}

				Token tok = Token();
				tok.ID = "WHILE";
				tok.EvalStatement = vector<vector<Token>>{ LexText(evalText) };
				tok.ExecStatement = LexText(execText);
				tokens.push_back(tok);
			}
			else if (cursor < Text.size() - 3 && Text[cursor] == 't' && Text[cursor + 1] == 'r' && Text[cursor + 2] == 'u' && Text[cursor + 3] == 'e')
			{
				cursor += 3;
				tokens.push_back(Token(true));
			}
			else if (cursor < Text.size() - 4 && Text[cursor] == 'f' && Text[cursor + 1] == 'a' && Text[cursor + 2] == 'l' && Text[cursor + 3] == 's' && Text[cursor + 4] == 'e')
			{
				cursor += 4;
				tokens.push_back(Token(false));
			}
			else if (cursor < Text.size() - 1 && Text[cursor] == '!' && Text[cursor + 1] == '=')
			{
				cursor += 1;
				Token tok = Token(0);
				tok.ID = "NOTEQUALS";
				tokens.push_back(tok);
			}
			else if (cursor < Text.size() - 1 && Text[cursor] == '=' && Text[cursor + 1] == '=')
			{
				cursor += 1;
				Token tok = Token(0);
				tok.ID = "EQUALS";
				tokens.push_back(tok);
			}
			else if (cursor < Text.size() - 5 && Text[cursor] == 'r' && Text[cursor + 1] == 'e' && Text[cursor + 2] == 't' && Text[cursor + 3] == 'u' && Text[cursor + 4] == 'r' && Text[cursor + 5] == 'n')
			{
				cursor += 6;
				while (Text[cursor] == ' ')
				{
					cursor++;
				}
				string statementText = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '\n') break;
					statementText += Text[cursor];
					cursor++;
				}
				Token tok = Token("RETURN", LexText(statementText));
				tokens.push_back(tok);
			}
			else if (cursor < Text.size() - 2 && Text[cursor] == 'd' && Text[cursor + 1] == 'e' && Text[cursor + 2] == 'f')
			{
				cursor += 3;
				while (Text[cursor] == ' ')
				{
					cursor++;
				}
				string name = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '(') break;
					if (Text[cursor] == ' ') break;
					name += Text[cursor];
					cursor++;
				}
				while (Text[cursor] == ' ' || Text[cursor] == '(')
				{
					cursor++;
				}
				string paramText = "";
				vector<vector<Token>> params;
				while (cursor < Text.size())
				{
					if (Text[cursor] == ')') break;
					if (Text[cursor] == ',')
					{
						params.push_back(LexText(paramText));
						paramText = "";
					}
					else
					{
						if(Text[cursor] != ' ') paramText += Text[cursor];
					}
					cursor++;
				}
				if (paramText != "")
				{
					params.push_back(LexText(paramText));
				}
				while (Text[cursor] != '{')
				{
					cursor++;
				}
				cursor++;
				string execText = "";
				int curlyBracks = 1;
				while (cursor < Text.size())
				{
					if (Text[cursor] == '{') curlyBracks++;
					if (Text[cursor] == '}') curlyBracks--;
					if (curlyBracks == 0) break;
					execText += Text[cursor];
					cursor++;
				}
				Token tok = Token(0);
				tok.ID = "DEFINITION";
				tok.EvalStatement = params;
				tok.ExecStatement = LexText(execText);
				tok.NAME = name;
				tokens.push_back(tok);
			}
			else if (cursor < Text.size() - 4 && Text[cursor] == 'u' && Text[cursor + 1] == 's' && Text[cursor + 2] == 'i' && Text[cursor + 3] == 'n' && Text[cursor + 4] == 'g')
			{
				cursor += 5;
				while (Text[cursor] == ' ')
				{
					cursor++;
				}
				string LibraryName;
				while (cursor < Text.size())
				{
					if (Text[cursor] == ' ') break;
					if (Text[cursor] == '\n') break;
					LibraryName += Text[cursor];
					cursor++;
				}
				Token tok;
				tok.ID = "USING";
				tok.NAME = LibraryName;
				tokens.push_back(tok);
			}
			else if (Text[cursor] == '"')
			{
				cursor++;
				string text = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '"') break;
					text += Text[cursor];
					cursor++;
				}
				Token tok = Token(0);
				tok.ID = "STRING";
				tok.stringVal = text;
				tokens.push_back(tok);
			}
			else if (Text[cursor] == '=')
			{
				cursor++;
				string setText = "";
				while (cursor < Text.size())
				{
					if (Text[cursor] == '\n') break;
					setText += Text[cursor];
					cursor++;
				}
				Token tok = Token(0);
				tok.ID = "SETEQUALS";
				tok.EvalStatement = vector<vector<Token>>{ LexText(setText) };
				tokens.push_back(tok);
			}
			else if (Text[cursor] == '#')
			{
				while (cursor < Text.size())
				{
					if (Text[cursor] == '\n') break;
					cursor++;
				}
			}
			else
			{
				if (IsDigit(Text[cursor]) || mathOperators.count(Text[cursor]))
				{
					tokens.push_back(Token(Text[cursor]));
				}
				else
				{
					string name = "";
					bool IsMethod = false;
					while (cursor < Text.size())
					{
						if (Text[cursor] == '(')
						{
							IsMethod = true;
							break;
						}
						if (mathOperators.count(Text[cursor]))
						{
							cursor--;
							break;
						}
						if (Text[cursor] == '\n') break;
						if (Text[cursor] != ' ')
						{
							name += Text[cursor];
						}
						cursor++;
					}
					if (IsMethod)
					{
						cursor++;
						int parans = 1;
						string paramText = "";
						vector<vector<Token>> params;
						while (Text[cursor] == ' ')
						{
							cursor++;
						}
						while (cursor < Text.size())
						{
							if (Text[cursor] == '(') parans++;
							if (Text[cursor] == ')') parans--;
							if (parans == 0) break;
							if (Text[cursor] == ',')
							{
								params.push_back(LexText(paramText));
								paramText = "";
								cursor++;
								while (Text[cursor] == ' ')
								{
									cursor++;
								}
							}
							else
							{
								paramText += Text[cursor];
								cursor++;
							}
						}
						if (paramText != "")
						{
							params.push_back(LexText(paramText));
						}
						Token tok = Token(0);
						tok.ID = "METHOD";
						tok.NAME = name;
						tok.EvalStatement = params;
						tokens.push_back(tok);
					}
					else
					{
						Token tok = Token(0);
						tok.ID = "VAR";
						tok.NAME = name;
						tokens.push_back(tok);
					}
				}
			}
		}
		cursor++;
	}
	return tokens;
}