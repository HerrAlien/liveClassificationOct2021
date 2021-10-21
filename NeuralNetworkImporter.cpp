#include "NeuralNetworkImporter.h"

#include <fstream>

// external libraries
#include "json.hpp"

NeuralNet NeuralNetworkImporter::FromJson(const std::string& path)
{
    std::ifstream inputFile(path);
    if (!inputFile)
        throw "Invalid file path";

    using nlohmann::json;
    json neuralNetJson;
    inputFile >> neuralNetJson;

    uint32_t numberOfLayers = neuralNetJson["n_layers"];
    std::vector<uint32_t> neuronsPerLayer = neuralNetJson["layer_sizes"];
    std::string activationFunctionType = neuralNetJson["activation"];
    std::string outputActivationFunctionType = neuralNetJson["out_activation"];
    std::vector<NeuralNet::Matrix> weights = neuralNetJson["weights"];
    std::vector<std::vector<float>> biases = neuralNetJson["biases"];

    // initialize neural net
    NeuralNet neuralNet;
    neuralNet.SetWeights(std::move(weights));
    neuralNet.SetBiases(std::move(biases));

    return neuralNet;
}
