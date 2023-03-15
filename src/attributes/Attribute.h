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

            std::unordered_map<int, int> diff = classCounts;
            for (const auto &s : series) {
                valueCount[GenerateValue(s.series, window)][s.label]++;
                diff.at(s.label)--;
                if (diff.at(s.label) == 0)
                    diff.erase(s.label);

                if (valueCount.size() < 2) // No split point at single value
                    continue;

                // Early entropy pruning
                const double optimalSplitPoint = GetOptimalSplitPoint(valueCount);
                const auto split = GetSplit(valueCount, optimalSplitPoint);
                for (const auto &d : diff) // Adds the rest of the values optimally
                    if (split.first[d.first] > split.second[d.first])
                        valueCount[0][d.first] += d.second;
                    else
                        valueCount[1][d.first] += d.second;

                const double optimalGain = CalculateInformationGain(valueCount, priorEntropy);
                if (optimalGain < bestScore)
                    return 0;
                for (int i = 0; i < maxClasses; i++) {
                    if (!diff.contains(i))
                        continue;
                    if (split.first[i] > split.second[i])
                        valueCount.at(0).at(i) -= diff.at(i);
                    else
                        valueCount.at(1).at(i) -= diff.at(i);
                }
            }

            return CalculateInformationGain(valueCount, priorEntropy);
        }
    private:
        [[nodiscard]] virtual double GenerateValue(const Series &series, const Window &window) const = 0;
    };
}

#endif //SHAPELETGENERATION_ATTRIBUTE_H
