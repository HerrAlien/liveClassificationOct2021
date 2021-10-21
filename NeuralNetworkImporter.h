#pragma once

#include "NeuralNetwork.h"

class NeuralNetworkImporter
{
public:
    // read neural net from json
    static NeuralNet FromJson(const std::string& path);
};

