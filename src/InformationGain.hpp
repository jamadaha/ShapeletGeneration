#ifndef SHAPELETGENERATION_INFORMATIONGAIN_HPP
#define SHAPELETGENERATION_INFORMATIONGAIN_HPP

#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <cmath>
#include <map>
#include "Types.hpp"
#include "SeriesActions.h"

namespace ShapeletGeneration {
    static double CalculateEntropy(uint total, const std::array<uint, maxClasses> &counts) {
        double entropy = 0;
        for (int i = 0; i < maxClasses; i++) {
            const double prob = (double) counts.at(i) / total;
            if (prob > 0)
                entropy += prob * (std::log2(prob));
        }
        return -entropy;
    }

    static inline double CalculateEntropy(const std::vector<LabelledSeries> &series) {
        return CalculateEntropy(series.size(), GetCount(series));
    }

    // Splits given values according to those below and above
    static std::pair<std::array<uint, maxClasses>, std::array<uint, maxClasses>> GetSplit
    (const std::map<double, std::array<uint, maxClasses>> &values, double splitPoint) {
        std::array<uint, maxClasses> lowerCount { 0 };
        std::array<uint, maxClasses> upperCount { 0 };
        for (const auto &frequency : values)
            if (frequency.first < splitPoint)
                for (int i = 0; i < maxClasses; i++)
                    lowerCount[i] += frequency.second[i];
            else
                for (int i = 0; i < maxClasses; i++)
                    upperCount[i] += frequency.second[i];
        return std::make_pair(lowerCount, upperCount);
    }

    static double CalculateSplitEntropy(const std::map<double, std::array<uint, maxClasses>> &values, double splitPoint) {
        const auto split = GetSplit(values, splitPoint);
        uint lowerTotal = 0;
        uint upperTotal = 0;
        for (const auto &v : split.first)
            lowerTotal += v;
        for (const auto &v : split.second)
            upperTotal += v;
        const uint total = lowerTotal + upperTotal;
        const double lowerEntropy = CalculateEntropy(total, split.first);
        const double upperEntropy = CalculateEntropy(total, split.second);

        const double lowerProb = (double) lowerTotal / total;
        const double upperProb = (double) upperTotal / total;

        return lowerEntropy * lowerProb + upperEntropy * upperProb;
    }

    static double GetOptimalSplitPoint(const std::map<double, std::array<uint, maxClasses>> &matchFrequency) {
        std::optional<double> bestPoint;
        double bestEntropy;

        for (auto iter = matchFrequency.begin(); iter != matchFrequency.end() && std::next(iter, 1) != matchFrequency.end(); iter++) {
            const double splitPoint = iter->first + (std::next(iter, 1)->first - iter->first) / 2;
            const double splitEntropy = CalculateSplitEntropy(matchFrequency, splitPoint);

            if (!bestPoint.has_value() || splitEntropy < bestEntropy) {
                bestPoint = splitPoint;
                bestEntropy = splitEntropy;
            }
        }

        return bestPoint.value();
    }

    static double CalculateInformationGain(const std::map<double, std::array<uint, maxClasses>> &matchFrequency, double priorEntropy) {
        double bestGain = 0;

        for (auto iter = matchFrequency.begin(); iter != matchFrequency.end() && std::next(iter, 1) != matchFrequency.end(); iter++) {
            const double splitPoint = iter->first + (std::next(iter, 1)->first - iter->first) / 2;
            const double splitEntropy = CalculateSplitEntropy(matchFrequency, splitPoint);
            const double infoGain = priorEntropy - splitEntropy;

            if (infoGain > bestGain)
                bestGain = infoGain;
        }

        return bestGain;
    }

    static double GetBestSplitPoint(const std::map<double, std::array<uint, maxClasses>> &matchFrequency) {
        std::optional<double> bestPoint;
        double bestSplitEntropy = 0;

        for (auto iter = matchFrequency.begin(); iter != matchFrequency.end() && std::next(iter, 1) != matchFrequency.end(); iter++) {
            const double splitPoint = iter->first + (std::next(iter, 1)->first - iter->first) / 2;
            const double splitEntropy = CalculateSplitEntropy(matchFrequency, splitPoint);

            if (!bestPoint.has_value() || splitEntropy < bestSplitEntropy) {
                bestPoint = splitPoint;
                bestSplitEntropy = splitEntropy;
            }
        }

        return bestPoint.value();
    }
}

#endif //SHAPELETGENERATION_INFORMATIONGAIN_HPP
