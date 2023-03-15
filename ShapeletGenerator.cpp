#include <unordered_map>
#include <charconv>
#include "src/Shapelets.hpp"

using namespace ShapeletGeneration;


int main(int, char* argv[]) {
    const std::string path = argv[1];
    std::string delimiter = argv[2];
    if (delimiter.size() != 1) {
        if (delimiter.find('t') != -1)
            delimiter = '\t';
    }
    const int minWindowSize = std::atoi(argv[3]);
    const int maxWindowSize = std::atoi(argv[4]);

    auto series = ReadCSV(path, delimiter);
    auto windows = GenerateWindows(series, minWindowSize, maxWindowSize);
    auto shapelets =  GenerateShapelets(series, windows);

    ShapeletGeneration::WriteCSV("Out.tsv", shapelets, delimiter);
    return 0;
}