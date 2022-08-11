#pragma once
#include <iostream>
#include "Lexer.h"
#include <unordered_map>
#include "ARITHMETICPARSING.h"
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>

float e = 2.718281828459045235360287471352662497757247093f;

float Sigmoid(float x)
{
    return 1 / (1 + powf(e, -x));
}

unsigned int seed = time(NULL);
int RandomNum(int max, int min)
{
    srand(seed);
	int randomnum = rand();
	int result = randomnum % (max + 1 - min) + min;
	seed += randomnum;
    return result;
}

struct Neuron
{
    public:
    float value = 0;
    float bias = 0;
    vector<float> weights;
    Neuron()
    {

    }
    Neuron(vector<float> WEIGHTS)
    {
        weights = WEIGHTS;
    }
    Neuron(vector<float> WEIGHTS, float BIAS)
    {
        weights = WEIGHTS;
        bias = BIAS;
    }
    Neuron(int prevLayerCount)
    {
        vector<float> WEIGHTS;
        int UpperLimit = 1000000;
        int LowerLimit = -1000000;
        for(int i = 0; i < prevLayerCount; i++)
        {
            WEIGHTS.push_back(RandomNum(UpperLimit, LowerLimit) / 1000000.0f);
        }
        bias = RandomNum(UpperLimit, LowerLimit) / 1000000.0f * 10;
        weights = WEIGHTS;
    }

    void MutateNeuron(float mutationRate)
    {
        for(int i = 0; i < weights.size(); i++)
        {
            weights[i] += RandomNum(1000000, -1000000) / 1000000.0f * mutationRate;
        }
        bias += RandomNum(1000000, -1000000) / 1000000.0f * mutationRate;
    }

    float CalculateValue(vector<Neuron> prevLayer)
    {
        float result = bias;
        for(int i = 0; i < prevLayer.size(); i++)
        {
            result += prevLayer[i].value * weights[i];
        }
        value = Sigmoid(result);
        return value;
    }
};

struct NeuralNetwork
{
    public:
    vector<vector<Neuron>> neurons;

    NeuralNetwork(vector<int> layout)
    {
        vector<vector<Neuron>> neurs = {
            vector<Neuron>()
        };
        for(int i = 0; i < layout[0]; i++)
        {
            neurs[0].push_back(Neuron());
        }
        for(int i = 1; i < layout.size(); i++)
        {
            neurs.push_back(vector<Neuron>());
            for(int j = 0; j < layout[i]; j++)
            {
                neurs[i].push_back(Neuron(neurs[i - 1].size()));
            }
        }
        neurons = neurs;
    }

    NeuralNetwork(string path)
    {
        fstream lib;
		lib.open(path, ios::in);
		string text = "";
		if (lib.is_open())
		{
			string line;
			while (getline(lib, line))
			{
				text += line + '\n';
			}
			lib.close();
		}
        else
        {
            return;
        }
        vector<int> layout;
        string temp = "";
        int cursor = 0;
        while(cursor < text.size())
        {
            if(text[cursor] == ':') break;
            if(text[cursor] == ',')
            {
                layout.push_back(ParseInt(temp));
                temp = "";
            }
            else
            {
                temp += text[cursor];
            }
            cursor++;
        }
        cursor++;
        vector<vector<Neuron>> Neurons = {
            vector<Neuron> ()
        };
        for(int i = 0; i < layout[0]; i++)
        {
            Neurons[0].push_back(Neuron());
        }
        for(int i = 1; i < layout.size(); i++)
        {
            Neurons.push_back(vector<Neuron>());
            for(int j = 0; j < layout[i]; j++)
            {
                vector<float> weights;
                for(int k = 0; k < layout[i - 1]; k++)
                {
                    while(cursor < text.size())
                    {
                        if(text[cursor] == ',')
                        {
                            weights.push_back(ParseFloat(temp));
                            temp = "";
                            cursor++;
                            break;
                        }
                        temp += text[cursor];
                        cursor++;
                    }
                }
                float bias = 0;
                while(cursor < text.size())
                {
                    if(text[cursor] == ',')
                    {
                        bias = ParseFloat(temp);
                        temp = "";
                        cursor++;
                        break;
                    }
                    temp += text[cursor];
                    cursor++;
                }
                Neurons[i].push_back(Neuron(weights, bias));
            }
        }
        neurons = Neurons;
    }

    vector<float> Think(vector<Neuron> inputLayer)
    {
        neurons[0] = inputLayer;
        vector<float> result;
        for(int i = 1; i < neurons.size(); i++)
        {
            for(int j = 0; j < neurons[i].size(); j++)
            {
                float temp = neurons[i][j].CalculateValue(neurons[i - 1]);
                if(i = neurons.size() - 1)
                {
                    result.push_back(temp);
                }
            }
        }
        return result;
    }

    void Mutate(float mutationrate)
    {
        for(int i = 1; i < neurons.size(); i++)
        {
            for(int j = 0; j < neurons[i].size(); j++)
            {
                neurons[i][j].MutateNeuron(mutationrate);
            }
        }
    }
    void SaveNetwork(string path)
    {
        string result = "";
        for(int i = 0; i < neurons.size(); i++)
        {
            result += to_string(neurons[i].size());
            result += ',';
        }
        result += ':';
        for(int i = 1; i < neurons.size(); i++)
        {
            for(int j = 0; j < neurons[i].size(); j++)
            {
                for(int k = 0; k < neurons[i][j].weights.size(); k++)
                {
                    result += to_string(neurons[i][j].weights[k]);
                    result += ',';
                }
                result += to_string(neurons[i][j].bias);
                result += ',';
            }
        }
        ofstream file(path);
		file << result;
		file.close();
    }
};

vector<NeuralNetwork> neuralNetworks;


// Creates a network and returns an int with address to network in neuralNetworks vector
Token ml_CreateNetwork(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token listToken = MethodCall.EvalStatement[0][0];
    if(listToken.ID == "LIST")
    {
        vector<int> layout;
        for(int i = 0; i < listToken.EvalStatement.size(); i++)
        {
            Token elementToken = listToken.EvalStatement[i][0];
            if(elementToken.ID == "INT")
            {
                if(elementToken.intVal > 0)
                {
                    layout.push_back(elementToken.intVal);
                }
                else
                {
                    return ErrorToken("All elements in given list must be positive and non zero");
                }
            }
            else
            {
                return ErrorToken("All elements in given list must be of type int");
            }
        }
        if(layout.size() > 1)
        {
            neuralNetworks.push_back(NeuralNetwork(layout));
            Token tok;
            tok.ID = "INT";
            tok.intVal = neuralNetworks.size() - 1;
            return tok;
        }
        else
        {
            return ErrorToken("List length must be greater than 1");
        }
    }
    else
    {
        return ErrorToken("First parameter in ml_createNetwork() call must be of type LIST");
    }
}

// Creates a new instance of the network at given address and returns int with new address
Token ml_Clone(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token addressToken = MethodCall.EvalStatement[0][0];
    if(addressToken.ID == "INT")
    {
        if(addressToken.intVal < neuralNetworks.size() && addressToken.intVal >= 0)
        {
            neuralNetworks.push_back(neuralNetworks[addressToken.intVal]);
            Token tok;
            tok.ID = "INT";
            tok.intVal = neuralNetworks.size() - 1;
            return tok;
        }
        else
        {
            return ErrorToken("Given address must be smaller than collection size");
        }
    }
    else
    {
        return ErrorToken("First parameter in ml_cloneNetwork() call must be of type int");
    }
}

// Mutates network at given address with given rate of mutation
Token ml_mutateNetwork(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token addressToken = MethodCall.EvalStatement[0][0];
    Token mutationRateToken = MethodCall.EvalStatement[1][0];
    if(addressToken.ID == "INT")
    {
        if(addressToken.intVal < neuralNetworks.size() && addressToken.intVal >= 0)
        {
            if(mutationRateToken.ID == "INT" || mutationRateToken.ID == "FLOAT")
            {
                neuralNetworks[addressToken.intVal].Mutate(mutationRateToken.GetTokenValueAsFloat());
                return noReturnToken();
            }
            else
            {
                return ErrorToken("Second parameter in ml_mutateNetwork() call must be of type int or float");
            }
        }
        else
        {
            return ErrorToken("Given address must be smaller than collection size");
        }
    }
    else
    {
        return ErrorToken("First parameter in ml_mutateNetwork() call must be of type int");
    }
}

// Returns list with output from network at given address and with given inputs
Token ml_Think(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token addressToken = MethodCall.EvalStatement[0][0];
    Token inputsListToken = MethodCall.EvalStatement[1][0];
    if(addressToken.ID == "INT")
    {
        if(inputsListToken.ID == "LIST")
        {
            vector<Neuron> inputs;
            for(int i = 0; i < inputsListToken.EvalStatement.size(); i++)
            {
                Token listElement = inputsListToken.EvalStatement[i][0];
                if(listElement.ID == "FLOAT" || listElement.ID == "INT")
                {
                    Neuron neuron;
                    neuron.value = listElement.GetTokenValueAsFloat();
                    inputs.push_back(neuron);
                }
                else
                {
                    return ErrorToken("All elements of given list must be of type int or float");
                }
            }
            if(addressToken.intVal < neuralNetworks.size() && addressToken.intVal >= 0)
            {
                vector<float> result = neuralNetworks[addressToken.intVal].Think(inputs);
                Token resultToken;
                resultToken.ID = "LIST";
                for(int i = 0; i < result.size(); i++)
                {
                    Token elementToken;
                    elementToken.ID = "FLOAT";
                    elementToken.floatVal = result[i];
                    resultToken.EvalStatement.push_back(vector<Token>{elementToken});
                }
                return resultToken;
            }
            else
            {
                return ErrorToken("Given address must fit in collection");
            }
        }
        else
        {
            return ErrorToken("Second parameter in ml_Think() call must be of type list");
        }
    }
    else
    {
        return ErrorToken("First parameter in ml_Think() call must be of type int");
    }
}

// Saves neuralnetwork at given address to path specified
Token ml_Save(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token addressToken = MethodCall.EvalStatement[0][0];
    Token pathToken = MethodCall.EvalStatement[1][0];
    if(addressToken.ID == "INT")
    {
        if(addressToken.intVal < neuralNetworks.size() && addressToken.intVal >= 0)
        {
            if(pathToken.ID == "STRING")
            {
                neuralNetworks[addressToken.intVal].SaveNetwork(pathToken.stringVal);
                return noReturnToken();
            }
            else
            {
                return ErrorToken("Second parameter in ml_Save() call must be of type string");
            }
        }
        else
        {
            return ErrorToken("Address must fit into collection");
        }
    }
    else
    {
        return ErrorToken("First parameter in ml_Save() call must be of type int");
    }
}

// Loads neuralnetwork from given path and returns address
Token ml_Load(Token MethodCall, unordered_map<string, method>* methods, map<string, Token>* Variables)
{
    Token pathToken = MethodCall.EvalStatement[0][0];
    if(pathToken.ID == "STRING")
    {
        neuralNetworks.push_back(NeuralNetwork(pathToken.stringVal));
        Token tok;
        tok.ID = "INT";
        tok.intVal = neuralNetworks.size() - 1;
        return tok;
    }
    else
    {
        return ErrorToken("First parameter in ml_Load() call must be of type string");
    }
}

unordered_map<string, method> SSMLMethods
{
	make_pair("ml_createNetwork", method("ml_createNetwork", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ml_CreateNetwork, true)),
    make_pair("ml_cloneNetwork", method("ml_cloneNetwork", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ml_Clone, true)),
    make_pair("ml_mutateNetwork", method("ml_mutateNetwork", vector<vector<Token>>{vector<Token>(), vector<Token>()}, vector<Token>(), &ml_mutateNetwork, true)),
    make_pair("ml_Think", method("ml_Think", vector<vector<Token>>{vector<Token>(), vector<Token>()}, vector<Token>(), &ml_Think, true)),
    make_pair("ml_Save", method("ml_Save", vector<vector<Token>>{vector<Token>(), vector<Token>()}, vector<Token>(), &ml_Save, true)),
    make_pair("ml_Load", method("ml_Load", vector<vector<Token>>{vector<Token>()}, vector<Token>(), &ml_Load, true)),
};
