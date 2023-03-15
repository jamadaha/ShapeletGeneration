#include <cstdlib>
#include <stdexcept>

#include <unordered_map>
#include "SlidingWindows.hpp"

using namespace ShapeletGeneration;

int main(int, char* argv[]) {
    const uint sLength = std::atoi(argv[1]);
    const uint wLength = std::atoi(argv[2]);
    const uint expectedWindowCount = std::atoi(argv[3]);

    Series series;
    for (uint i = 0; i < sLength; i++)
        series.push_back(i);

    auto windows = ShapeletGeneration::GenerateWindows(series, wLength);

    if (expectedWindowCount == windows.size())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected window count | Expected " +
        std::to_string(expectedWindowCount) + " Received " + std::to_string(windows.size()));
}
