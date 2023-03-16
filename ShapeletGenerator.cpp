#include <unordered_map>
#include <charconv>
#include "src/Shapelets.hpp"
#include "src/SeriesActions.h"

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
    auto split =  GenerateShapelets(series, windows);

    printf("Before split probabilities:\n");
    PrintProbabilities(GetCount(series));
    auto splitValues = split.attribute->Split(series, split.shapelet);
    printf("After probabilities:\n");
    PrintProbabilities(GetCount(splitValues.first), GetCount(splitValues.second));

    WriteCSV("out.tsv", {split.shapelet});
    return 0;
}