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

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

class Shader
{
	public:
	GLFWwindow* window;
	unsigned int shaderProgram;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	string Name;

	Shader(const char* name)
	{
		Name = name;
	}
	Shader()
	{
		Name = "";
	}

	int Init(int width, int height)
	{
		window = glfwCreateWindow(width, height, Name.c_str(), NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 0;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return 0;
		}

		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetWindowShouldClose(window, false);

		return 1;
	}

	int CompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR when compiling shader" << std::endl << infoLog << std::endl;
			return 0;
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR when compiling shader" << std::endl << infoLog << std::endl;
			return 0;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR when linking program" << std::endl << infoLog << std::endl;
		}

		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return 1;
	}

	void SetVerticesAndIndices(vector<float> vertices, vector<unsigned int> indices)
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, &vertices[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void SetUniform(const char* id, int ival)
	{
		int location = glGetUniformLocation(shaderProgram, id);
		glUniform1i(location, ival);
	}
	void SetUniform(const char* id, float fval)
	{
		int location = glGetUniformLocation(shaderProgram, id);
		glUniform1f(location, fval);
	}
	void SetUniform(const char* id, float fval, float fval2)
	{
		int location = glGetUniformLocation(shaderProgram, id);
		glUniform2f(location, fval, fval2);
	}
	void SetUniform(const char* id, float fval, float fval2, float fval3)
	{
		int location = glGetUniformLocation(shaderProgram, id);
		glUniform3f(location, fval, fval2, fval3);
	}
	void SetUniform(const char* id, vector<float> fvals)
	{
		int location = glGetUniformLocation(shaderProgram, id);
		glUniform1fv(location, fvals.size(), &fvals[0]);
	}

	void Update()
	{
		glfwMakeContextCurrent(window);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
};

Token checkInput(string key)
{
	if (key.size() == 1)
	{
		if (key[0] >= 'A' && key[0] <= 'Z')
		{
			return Token(glfwGetKey(glfwGetCurrentContext(), key[0]) == GLFW_PRESS);
		}
		else if (key[0] >= '0' && key[0] <= '9')
		{
			return Token(glfwGetKey(glfwGetCurrentContext(), key[0]) == GLFW_PRESS);
		}
	}

	if (key == "ESCAPE")
	{
		return Token(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS);
	}
	else if (key == "SPACE")
	{
		return Token(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS);
	}
	
	return ErrorToken("That is not a valid key");
}

unordered_map<string, Shader> Shaders;

/*Method initializes opengl*/
Token gl_INIT(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Token tokk = Token(0);
	tokk.ID = "NORETURN";
	return tokk;
}

/*Method creates a new shader and adds it to Shaders map with name according to given input*/
Token gl_createShader(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		Shaders[nameToken.stringVal] = Shader(nameToken.stringVal.c_str());
		return nameToken;
	}
	else
	{
		Token errorTok;
		errorTok.ID = "ERROR";
		errorTok.NAME = "First parameter in gl_createShader() call must be of type string";
		return errorTok;
	}
}

/*Compiles the given vertex shader and fragment shader and creates a program in the given shader*/
Token gl_compileShader(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			Token vertexShader = MethodCall.EvalStatement[1][0];
			Token fragmentShader = MethodCall.EvalStatement[2][0];

			if (vertexShader.ID == "STRING" && fragmentShader.ID == "STRING")
			{
				if (Shaders[nameToken.stringVal].CompileShaders(vertexShader.stringVal.c_str(), fragmentShader.stringVal.c_str()))
				{
					Token tok;
					tok.ID = "NORETURN";
					return tok;
				}
				else
				{
					Token errorTok;
					errorTok.ID = "ERROR";
					errorTok.NAME = "Failure when compiling shaders";
					return errorTok;
				}
			}
			else
			{
				Token errorTok;
				errorTok.ID = "ERROR";
				errorTok.NAME = "Second and third parameters in gl_compileShader() call must be of type string";
				return errorTok;
			}
		}
		else
		{
			Token errorTok;
			errorTok.ID = "ERROR";
			errorTok.NAME = "Shader doesn't exist";
			return errorTok;
		}
	}
	else
	{
		Token errorTok;
		errorTok.ID = "ERROR";
		errorTok.NAME = "First parameter in gl_createShader() call must be of type string";
		return errorTok;
	}
}

/*Method intializes the given shader with the dimensions given*/
Token gl_InitShader(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			Token wTok = MethodCall.EvalStatement[1][0];
			Token hTok = MethodCall.EvalStatement[2][0];
			if (wTok.ID == "INT" && hTok.ID == "INT")
			{
				if (Shaders[nameToken.stringVal].Init(wTok.intVal, hTok.intVal))
				{
					Token tok;
					tok.ID = "NORETURN";
					return tok;
				}
				else
				{
					Token errorToken;
					errorToken.ID = "ERROR";
					errorToken.NAME = "Error when initializing shader";
					return errorToken;
				}
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Second and third parameter in gl_initShader() call must be of type int";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Shader doesn't exist";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in gl_initShader() call must be of type string";
		return errorToken;
	}
}

/*Method sets the vertices and indices in the given shader in that order*/
Token SetVertsInShader(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			Token vertsList = MethodCall.EvalStatement[1][0];
			Token indicesList = MethodCall.EvalStatement[2][0];
			if (vertsList.ID == "LIST" && indicesList.ID == "LIST")
			{
				/* Convert vertsList to vector<float> and indicesList to vector<unsigned int>*/

				vector<float> vertices;
				for (int i = 0; i < vertsList.EvalStatement.size(); i++)
				{
					Token Val = vertsList.EvalStatement[i][0];
					if (Val.ID == "INT" || Val.ID == "FLOAT")
					{
						/*In OpenGL all vertices are in normalized coordinates so they are only shown within the range -1 to 1.
						Therefore the precision loss during int to float type cast is irrelevant*/
						vertices.push_back(Val.GetTokenValueAsFloat());
					}
					else
					{
						Token errorToken;
						errorToken.ID = "ERROR";
						errorToken.NAME = "All elements in second parameter list in gl_setVerts() call must be of type int or float";
						return errorToken;
					}
				}

				vector<unsigned int> indices;

				for (int i = 0; i < indicesList.EvalStatement.size(); i++)
				{
					Token Val = indicesList.EvalStatement[i][0];
					if (Val.ID == "INT")
					{
						if (Val.intVal >= 0)
						{
							indices.push_back(Val.intVal);
						}
						else
						{
							Token errorToken;
							errorToken.ID = "ERROR";
							errorToken.NAME = "All elements in third parameter list in gl_setVerts() call must be non negative";
							return errorToken;
						}
					}
					else
					{
						Token errorToken;
						errorToken.ID = "ERROR";
						errorToken.NAME = "All elements in third parameter list in gl_setVerts() call must be of type int";
						return errorToken;
					}
				}
				Shaders[nameToken.stringVal].SetVerticesAndIndices(vertices, indices);

				Token tok;
				tok.ID = "NORETURN";
				return tok;
			}
			else
			{
				Token errorToken;
				errorToken.ID = "ERROR";
				errorToken.NAME = "Second and third parameters in gl_setVerts() call must be of type list";
				return errorToken;
			}
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Shader doesn't exist";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in gl_setVerts() call must be of type string";
		return errorToken;
	}
}

/*Method renders new frame in given shaders window*/
Token gl_updateShader(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			Shaders[nameToken.stringVal].Update();
			Token tok;
			tok.ID = "NORETURN";
			return tok;
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Shader doesn't exist";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in gl_Update() call must be of type string";
		return errorToken;
	}
}

/*Method terminates glfw*/
Token gl_Terminate(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	glfwTerminate();
	Token tok;
	tok.ID = "NORETURN";
	return tok;
}

/*Returns true if the given shaders window should close otherwise false*/
Token gl_shouldClose(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			Token result;
			result.ID = "BOOL";
			result.boolVal = glfwWindowShouldClose(Shaders[nameToken.stringVal].window);
			return result;
		}
		else
		{
			Token errorToken;
			errorToken.ID = "ERROR";
			errorToken.NAME = "Shader does not exist";
			return errorToken;
		}
	}
	else
	{
		Token errorToken;
		errorToken.ID = "ERROR";
		errorToken.NAME = "First parameter in gl_shouldClose() call must be of type string";
		return errorToken;
	}
}

/*Returns the dimensions of the given shaders window in a list with two elements*/
Token gl_windowSize(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameTok = MethodCall.EvalStatement[0][0];
	if (nameTok.ID == "STRING")
	{
		if (Shaders.count(nameTok.stringVal))
		{
			Token sizeX, sizeY;
			sizeX.ID = "INT";
			sizeY.ID = "INT";
			glfwGetWindowSize(Shaders[nameTok.stringVal].window, &(sizeX.intVal), &(sizeY.intVal));
			Token result;
			result.ID = "LIST";
			result.EvalStatement.push_back(vector<Token>{sizeX});
			result.EvalStatement.push_back(vector<Token>{sizeY});
			return result;
		}
		else
		{
			return ErrorToken("That shader does not exist");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_windowSize() call must be of type string");
	}
}

/*Returns float token with the time that the program has run in seconds*/
Token gl_getTime(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token result;
	result.ID = "FLOAT";
	result.floatVal = glfwGetTime();
	return result;
}

/*Sets up given shader window to close*/
Token gl_Close(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameToken = MethodCall.EvalStatement[0][0];
	if (nameToken.ID == "STRING")
	{
		if (Shaders.count(nameToken.stringVal))
		{
			glfwSetWindowShouldClose(Shaders[nameToken.stringVal].window, true);
			return noReturnToken();
		}
		else
		{
			return ErrorToken("That shader does not exist");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_Close() call must be of type string");
	}
}

/*Returns true token if given key is pressed otherwise returns false token*/
Token gl_isPressed(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token keyToken = MethodCall.EvalStatement[0][0];
	if (keyToken.ID == "STRING")
	{
		return checkInput(keyToken.stringVal);
	}
	else
	{
		return ErrorToken("First parameter in gl_isPressed() call must be of type string");
	}
}

/*Returns current mouse position in screen coordinates as a list with two elements*/
Token gl_mousePosition(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token result;
	result.ID = "LIST";

	double x, y;

	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

	Token xTok;
	xTok.ID = "FLOAT";
	xTok.floatVal = x;
	Token yTok;
	yTok.ID = "FLOAT";
	yTok.floatVal = y;

	result.EvalStatement.push_back(vector<Token> {xTok});
	result.EvalStatement.push_back(vector<Token> {yTok});

	return result;
}

/*Method sets the position of the mouse coordinates to the given ints*/
Token gl_setmousePosition(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token xTok = MethodCall.EvalStatement[0][0];
	Token yTok = MethodCall.EvalStatement[1][0];
	if (xTok.ID == "INT" && yTok.ID == "INT")
	{
		glfwSetCursorPos(glfwGetCurrentContext(), xTok.intVal, yTok.intVal);
		
		return noReturnToken();
	}
	else
	{
		return ErrorToken("First and second parameters in gl_setmousePosition() call must be of type int");
	}
}

/*Method sets the visibility of the cursor, input is bool*/
Token gl_setmouseVisibility(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token input = MethodCall.EvalStatement[0][0];
	if (input.ID == "BOOL")
	{
		if (input.boolVal)
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_setmouseVisibility() call must be of type bool");
	}
	return noReturnToken();
}

//Method enables or disables vsync according to bool input
Token gl_setVsync(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token input = MethodCall.EvalStatement[0][0];
	if (input.ID == "BOOL")
	{
		if (input.boolVal)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_setVsync() call must be of type bool");
	}
	return noReturnToken();
}

/*Method sets uniform values in the given shader*/
Token gl_uniform1f(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameTok = MethodCall.EvalStatement[0][0];
	if (nameTok.ID == "STRING")
	{
		Token idTok = MethodCall.EvalStatement[1][0];
		Token ValTok = MethodCall.EvalStatement[2][0];
		if (idTok.ID == "STRING")
		{
			if (ValTok.ID == "FLOAT" || ValTok.ID == "INT")
			{
				if (Shaders.count(nameTok.stringVal))
				{
					Shaders[nameTok.stringVal].SetUniform(idTok.stringVal.c_str(), ValTok.GetTokenValueAsFloat());
					return noReturnToken();
				}
				else
				{
					return ErrorToken("That shader does not exist");
				}
			}
			else
			{
				return ErrorToken("Third parameter in gl_uniform1f() call must be of type int or float");
			}
		}
		else
		{
			return ErrorToken("Second parameter in gl_uniform1f() call must be of type string");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_uniform1f() call must be of type string");
	}
}
Token gl_uniform2f(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameTok = MethodCall.EvalStatement[0][0];
	if (nameTok.ID == "STRING")
	{
		Token idTok = MethodCall.EvalStatement[1][0];
		Token ValTok1 = MethodCall.EvalStatement[2][0];
		Token ValTok2 = MethodCall.EvalStatement[3][0];
		if (idTok.ID == "STRING")
		{
			if (ValTok1.ID == "FLOAT" || ValTok1.ID == "INT")
			{
				if (ValTok2.ID == "FLOAT" || ValTok2.ID == "INT")
				{
					if (Shaders.count(nameTok.stringVal))
					{
						Shaders[nameTok.stringVal].SetUniform(idTok.stringVal.c_str(), ValTok1.GetTokenValueAsFloat(), ValTok2.GetTokenValueAsFloat());
						return noReturnToken();
					}
					else
					{
						return ErrorToken("That shader does not exist");
					}
				}
				else
				{
					return ErrorToken("Fourth parameter in gl_uniform2f() call must be of type int or float");
				}
			}
			else
			{
				return ErrorToken("Third parameter in gl_uniform2f() call must be of type int or float");
			}
		}
		else
		{
			return ErrorToken("Second parameter in gl_uniform2f() call must be of type string");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_uniform2f() call must be of type string");
	}
}
Token gl_uniform3f(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameTok = MethodCall.EvalStatement[0][0];
	if (nameTok.ID == "STRING")
	{
		Token idTok = MethodCall.EvalStatement[1][0];
		Token ValTok1 = MethodCall.EvalStatement[2][0];
		Token ValTok2 = MethodCall.EvalStatement[3][0];
		Token ValTok3 = MethodCall.EvalStatement[4][0];
		if (idTok.ID == "STRING")
		{
			if (ValTok1.ID == "FLOAT" || ValTok1.ID == "INT")
			{
				if (ValTok2.ID == "FLOAT" || ValTok2.ID == "INT")
				{
					if (ValTok3.ID == "FLOAT" || ValTok3.ID == "INT")
					{
						if (Shaders.count(nameTok.stringVal))
						{
							Shaders[nameTok.stringVal].SetUniform(idTok.stringVal.c_str(), ValTok1.GetTokenValueAsFloat(), ValTok2.GetTokenValueAsFloat(), ValTok3.GetTokenValueAsFloat());
							return noReturnToken();
						}
						else
						{
							return ErrorToken("That shader does not exist");
						}
					}
					else
					{
						return ErrorToken("Fifth parameter in gl_uniform3f() call must be of type int or float");
					}
				}
				else
				{
					return ErrorToken("Fourth parameter in gl_uniform3f() call must be of type int or float");
				}
			}
			else
			{
				return ErrorToken("Third parameter in gl_uniform3f() call must be of type int or float");
			}
		}
		else
		{
			return ErrorToken("Second parameter in gl_uniform3f() call must be of type string");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_uniform3f() call must be of type string");
	}
}

/*Method sets the given uniform id in the given shader to the list of floats given*/
Token gl_uniformarrayf(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
	Token nameTok = MethodCall.EvalStatement[0][0];
	if (nameTok.ID == "STRING")
	{
		Token idTok = MethodCall.EvalStatement[1][0];
		Token ValTok = MethodCall.EvalStatement[2][0];
		if (idTok.ID == "STRING")
		{
			if (ValTok.ID == "LIST")
			{
				if (Shaders.count(nameTok.stringVal))
				{
					vector<float> data;
					for (int i = 0; i < ValTok.EvalStatement.size(); i++)
					{
						if (ValTok.EvalStatement[i][0].ID == "FLOAT" || ValTok.EvalStatement[i][0].ID == "INT")
						{
							data.push_back(ValTok.EvalStatement[i][0].GetTokenValueAsFloat());
						}
						else
						{
							return ErrorToken("All list elements must be of type float or int");
						}
					}
					Shaders[nameTok.stringVal].SetUniform(idTok.stringVal.c_str(), data);
					return noReturnToken();
				}
				else
				{
					return ErrorToken("That shader does not exist");
				}
			}
			else
			{
				return ErrorToken("Third parameter in gl_uniformarrayf() call must be of type list");
			}
		}
		else
		{
			return ErrorToken("Second parameter in gl_uniformarrayf() call must be of type string");
		}
	}
	else
	{
		return ErrorToken("First parameter in gl_uniformarrayf() call must be of type string");
	}
}

unordered_map<string, method> OpenGLSSMethods
{
	make_pair("gl_Init", method("gl_Init", vector<vector<Token>> (), vector<Token>(), &gl_INIT, true)),
	make_pair("gl_createShader", method("gl_createShader", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_createShader, true)),
	make_pair("gl_compileShader", method("gl_compileShader", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_compileShader, true)),
	make_pair("gl_initShader", method("gl_initShader", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_InitShader, true)),
	make_pair("gl_setVerts", method("gl_setVerts", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &SetVertsInShader, true)),
	make_pair("gl_Update", method("gl_Update", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_updateShader, true)),
	make_pair("gl_Terminate", method("gl_Terminate", vector<vector<Token>> (), vector<Token>(), &gl_Terminate, true)),
	make_pair("gl_shouldClose", method("gl_shouldClose", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_shouldClose, true)),
	make_pair("gl_windowSize", method("gl_windowSize", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_windowSize, true)),
	make_pair("gl_getTime", method("gl_getTime", vector<vector<Token>> (), vector<Token>(), &gl_getTime, true)),
	make_pair("gl_Close", method("gl_Close", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_Close, true)),
	make_pair("gl_isPressed", method("gl_isPressed", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_isPressed, true)),
	make_pair("gl_mousePosition", method("gl_mousePosition", vector<vector<Token>> (), vector<Token>(), &gl_mousePosition, true)),
	make_pair("gl_setmousePosition", method("gl_setmousePosition", vector<vector<Token>> {vector<Token>(), vector<Token>()}, vector<Token>(), &gl_setmousePosition, true)),
	make_pair("gl_setmouseVisibility", method("gl_setmouseVisibility", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_setmouseVisibility, true)),
	make_pair("gl_setVsync", method("gl_setvSync", vector<vector<Token>> {vector<Token>()}, vector<Token>(), &gl_setVsync, true)),
	make_pair("gl_uniform1f", method("gl_uniform1f", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_uniform1f, true)),
	make_pair("gl_uniform2f", method("gl_uniform2f", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_uniform2f, true)),
	make_pair("gl_uniform3f", method("gl_uniform3f", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_uniform3f, true)),
	make_pair("gl_uniformarrayf", method("gl_uniformarrayf", vector<vector<Token>> {vector<Token>(), vector<Token>(), vector<Token>()}, vector<Token>(), &gl_uniformarrayf, true)),
};