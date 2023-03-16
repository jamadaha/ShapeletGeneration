#ifndef SHAPELETGENERATION_ATTRIBUTE_H
#define SHAPELETGENERATION_ATTRIBUTE_H

#include <unordered_map>
#include "Types.hpp"
#include "InformationGain.hpp"

namespace ShapeletGeneration {
    class Attribute {
    public:
        // Calculates the information gained by splitting based upon a derived attributes perception? of the window
        double EvaluateWindow(double priorEntropy, double bestScore, const std::unordered_map<int, int> &classCounts,
                              const std::vector<LabelledSeries> &series, const Window &window) {
            std::map<double, std::array<uint, maxClasses>> valueCount; // At the given value, how many of each class

            std::array<uint, maxClasses> diff { 0 };
            for (const auto &cc : classCounts)
                diff[cc.first] = cc.second;
            for (const auto &s : series) {
                valueCount[GenerateValue(s.series, window)][s.label]++;
                diff.at(s.label)--;

                if (valueCount.size() < 2) // No split point at single value
                    continue;

                // Early entropy pruning
                const double optimalSplitPoint = GetOptimalSplitPoint(valueCount);
                const auto split = GetSplit(valueCount, optimalSplitPoint);
                for (int i = 0; i < maxClasses; i++) // Adds the rest of the values optimally
                    valueCount[split.first[i] > split.second[i]][i] += diff[i];

                const double optimalGain = CalculateInformationGain(valueCount, priorEntropy);
                if (optimalGain < bestScore)
                    return 0;
                for (int i = 0; i < maxClasses; i++)
                    valueCount[split.first[i] > split.second[i]][i] -= diff[i];
            }

            return CalculateInformationGain(valueCount, priorEntropy);
        }
    private:
        [[nodiscard]] virtual double GenerateValue(const Series &series, const Window &window) const = 0;
    };
}

#endif //SHAPELETGENERATION_ATTRIBUTE_H
