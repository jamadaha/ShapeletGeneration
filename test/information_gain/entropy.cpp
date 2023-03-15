#include <cstdlib>
#include <stdexcept>
#include <unordered_map>
#include <sstream>

#include "InformationGain.hpp"

using namespace ShapeletGeneration;

std::array<uint, maxClasses> Convert(std::string input) {
    std::array<uint, maxClasses> values { 0 };

    std::stringstream iss(input);

    int tempNum;
    while (iss >> tempNum)
        values[tempNum]++;

    return values;
}

uint GetTotal(std::array<uint, maxClasses> values) {
    uint total = 0;

    for (const auto &value : values)
        total += value;

    return total;
}

int main(int, char* argv[]) {
    const auto counts = Convert(argv[1]);
    const double expectedEntropy = std::atof(argv[2]);
    const uint total = GetTotal(counts);


    const double actualEntropy = std::round(ShapeletGeneration::CalculateEntropy(total, counts) * 100) / 100;

    if (expectedEntropy == actualEntropy)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected entropy | Expected " +
        std::to_string(expectedEntropy) + " Received " + std::to_string(actualEntropy));
}
