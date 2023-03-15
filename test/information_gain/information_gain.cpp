#include <cstdlib>
#include <stdexcept>
#include <unordered_map>
#include <sstream>

#include "InformationGain.hpp"

using namespace ShapeletGeneration;

std::unordered_map<int, int> Convert(std::string input) {
    std::unordered_map<int, int> values;

    std::stringstream iss(input);

    int tempNum;
    while (iss >> tempNum)
        values[tempNum]++;

    return values;
}

int GetTotal(std::unordered_map<int, int> values) {
    int total = 0;

    for (const auto &pair : values)
        total += pair.second;

    return total;
}

int main(int, char* argv[]) {
    const auto counts = Convert(argv[1]);
    const double expectedGain = std::atof(argv[3]);
    const int total = GetTotal(counts);


    const double actualEntropy = std::round(ShapeletGeneration::CalculateEntropy(total, counts) * 100) / 100;

    if (expectedGain == actualEntropy)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected entropy | Expected " +
                               std::to_string(expectedGain) + " Received " + std::to_string(actualEntropy));
}
