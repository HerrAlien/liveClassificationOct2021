#include "NeuralNetwork.h"

#include <algorithm>
#include <numeric>
#include <functional>

uint8_t NeuralNet::Predict(const Image& image) const
{
    auto argmax = [](const std::vector<float> & vector) {
        auto it = std::max_element(vector.begin(), vector.end());
        return it - vector.begin();
    };

    // softArgMax, or softmax conventionally in machine learning 
    // calculates the probability of a class, as the sum of all probabilities equals 1
    auto softArgMax = [](const std::vector<float> & vector) {
        float sumOfExponentials = std::accumulate(vector.begin(), vector.end(), 0.f,
            [](float partialSum, float value) {return partialSum + std::exp(value); }
        );

        std::vector<float> probabilities(vector.size());
        std::transform(vector.begin(), vector.end(), probabilities.begin(),
            [&sumOfExponentials](float value) { return value / sumOfExponentials; }
        );

        auto it = std::max_element(probabilities.begin(), probabilities.end());
        return it - probabilities.begin();
    };

    // TODO: (flexibility) allow selecting output activation function
    std::function<size_t(const std::vector<float>&)> outActivationFunction = softArgMax;

    const auto& result = feedForward(image.pixels);
    return static_cast<uint8_t>(outActivationFunction(result));
}

void NeuralNet::SetWeights(std::vector<Matrix>&& weights)
{
    // TODO: (optimization) transform the row major matrixes to column major matrixes, for easier multiplication later
    m_weights = std::move(weights);
}

void NeuralNet::SetBiases(std::vector<std::vector<float>>&& biases)
{
    m_biases = std::move(biases);
}

std::vector<float> NeuralNet::feedForward(const std::vector<uint8_t>& x) const
{
    if (m_weights.size() != m_biases.size())
        throw "Wieghts and biases size is not matching";

    // currently not used
    auto dotProductForColumnMajorMatrixes = [](const Matrix & matrix, const std::vector<float> & vector)
    {
        std::vector<float> result(matrix.size());
        std::transform(matrix.begin(), matrix.end(), result.begin(),
            [&vector](const std::vector<float> & row) {
                return std::inner_product(row.begin(), row.end(), vector.begin(), 0.f);
            }
        );

        return result;
    };

    auto dotProductForRowMajorMatrixes = [](const Matrix & matrix, const std::vector<float> & vector)
    {
        struct const_column_element_iterator
        {
            const_column_element_iterator(const Matrix& matrix, size_t columnIndex) : m_matrix(matrix), m_columnIndex(columnIndex), m_currentRow(0)
            {
            }

            void operator ++ ()
            {
                ++m_currentRow;
            }

            float operator * () const
            {
                return m_matrix[m_currentRow][m_columnIndex];
            }

        private:
            size_t m_columnIndex;
            size_t m_currentRow;
            const Matrix& m_matrix;
        };

        struct const_column_iterator
        {
            // required by certain algorithms from stl
            using iterator_category = std::input_iterator_tag;
            using value_type      = const_column_element_iterator;
            using difference_type = typename std::ptrdiff_t;
            using pointer         = const value_type*;
            using reference       = const value_type &;

            const_column_iterator(const Matrix& matrix, size_t currentColumn = 0) : m_matrix(matrix), m_currentColumn(currentColumn)
            {
            }

            void operator ++ ()
            {
                ++m_currentColumn;
            }

            const_column_element_iterator operator * () const
            {
                return const_column_element_iterator(m_matrix, m_currentColumn);
            }

            bool operator != (const const_column_iterator& other) const
            {
                return m_currentColumn != other.m_currentColumn;
            }

        private:
            size_t m_currentColumn;
            const Matrix& m_matrix;
        };

        size_t columnCount = matrix.front().size();
        std::vector<float> result(columnCount);
        std::transform(const_column_iterator(matrix), const_column_iterator(matrix, columnCount), result.begin(),
            [&vector](const const_column_element_iterator & columnElementIterator) {
                return std::inner_product(vector.begin(), vector.end(), columnElementIterator, 0.f);
            }
        );

        return result;
    };

    auto sigmoid = [](float& value) {
        value = 1.0f / (1.0f + std::exp(-value));
    };

    // TODO: (flexibility) allow selecting more activation functions (based on what is read from the file)
    std::function<void(float&)> activationFunction = sigmoid;

    std::vector<float> result(x.size());
    std::copy(x.begin(), x.end(), result.begin());

    size_t numberOfWeights = m_weights.size();
    for (size_t i = 0; i < numberOfWeights; ++i)
    {
        result = dotProductForRowMajorMatrixes(m_weights[i], result);
        std::transform(result.begin(), result.end(), m_biases[i].begin(), result.begin(), std::plus<float>());
        // TODO: (optimization) combine the above and below functions; maybe all three?
        std::for_each(result.begin(), result.end(), activationFunction);
    }

    return result;
}
