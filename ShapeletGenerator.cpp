#include <unordered_map>
#include <charconv>
#include "src/Shapelets.hpp"
#include "src/SeriesActions.h"
#include "FileHandler.h"

using namespace ShapeletGeneration;

std::vector<Split> GenerateSplits(int depth, const std::vector<LabelledSeries> &series, int minWindowSize, int maxWindowSize) {
    if (series.size() < 2 || depth > 2)
        return {};
    const auto windows = GenerateWindows(series, minWindowSize, maxWindowSize);
    const auto counts = GetCount(series);
    std::vector<Split> splits;
    const auto split = GenerateShapelets(series, windows);
    const auto splitValues = split.attribute->Split(series, split.shapelet);
    splits.push_back(split);
    const std::vector<std::vector<LabelledSeries>> splitSeries { splitValues.first, splitValues.second };
    for (const auto &tempSeries : splitSeries) {
        const auto tempCounts = GetCount(tempSeries);
        bool harmonious = true;
        for (int i = 0; i < maxClasses; i++)
            if (tempCounts[i] != 0 && tempCounts[i] != counts[i]) {
                harmonious = false;
                break;
            }
        if (harmonious)
            continue;
        for (const auto &tempSplit: GenerateSplits(depth + 1, tempSeries, minWindowSize, maxWindowSize))
            splits.push_back(tempSplit);
    }
    return splits;
}

int main(int, char* argv[]) {
    const std::string path = argv[1];
    std::string delimiter = argv[2];
    if (delimiter.size() != 1) {
        if (delimiter.find('t') != -1)
            delimiter = '\t';
    }
    const int minWindowSize = std::atoi(argv[3]);
    const int maxWindowSize = std::atoi(argv[4]);

    const auto series = ReadCSV(path, delimiter);
    const auto windows = GenerateWindows(series, minWindowSize, maxWindowSize);

    std::vector<Split> splits = GenerateSplits(0, series, minWindowSize, maxWindowSize);
    WriteSplitValuesToFile(series, splits);
    printf("Split Count: %zu\n", splits.size());
    return 0;
}