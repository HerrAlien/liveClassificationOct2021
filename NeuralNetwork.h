#pragma once
#include "Image.h"

class NeuralNet
{
public:
    using Matrix = std::vector<std::vector<float>>;

public:
    // Predict image label
    uint8_t Predict(const Image& image) const;

    // weight matrixes should use the row major (walking the matrix can be done row by row) representation
    void SetWeights(std::vector<Matrix>&& weights);

    void SetBiases(std::vector<std::vector<float>>&& biases);

    bool IsInitialized() { return !m_weights.empty(); }

private:
    std::vector<float> feedForward(const std::vector<uint8_t>& x) const;

private:
    std::vector<Matrix> m_weights;
    std::vector<std::vector<float>> m_biases;
};
