#ifndef SHAPELETGENERATION_INFORMATIONGAIN_HPP
#define SHAPELETGENERATION_INFORMATIONGAIN_HPP

#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <cmath>
#include <map>
#include "Types.hpp"

namespace ShapeletGeneration {
    static double CalculateEntropy(std::unordered_set<int> counts) {
        const int total = std::accumulate(counts.begin(), counts.end(), 0);
        double entropy = 0;
        for (const auto &c : counts) {
            const double prob = (double) c / total;
            entropy += prob * (std::log2(prob));
        }
        return -entropy;
    }

    static double CalculateEntropy(int total, const std::unordered_map<int, int> &counts) {
        double entropy = 0;
        for (const auto &cc : counts) {
            const double prob = (double) cc.second / total;
            entropy += prob * (std::log2(prob));
        }
        return -entropy;
    }

    static double CalculateInformationGain(const std::map<double, std::unordered_map<int, int>> &matchFrequency, double priorEntropy) {
        double bestGain = 0;

        for (auto iter = matchFrequency.begin(); iter != matchFrequency.end() && std::next(iter, 1) != matchFrequency.end(); iter++) {
            const double splitPoint = iter->first + (std::next(iter, 1)->first - iter->first) / 2;
            int lowerTotal = 0;
            int upperTotal = 0;
            std::unordered_map<int, int> lowerCount;
            std::unordered_map<int, int> upperCount;
            for (const auto &frequency : matchFrequency)
                if (frequency.first < splitPoint)
                    for (const auto &elem : frequency.second) {
                        lowerTotal += elem.second;
                        lowerCount[elem.first] += elem.second;
                    }
                else
                    for (const auto &elem : frequency.second) {
                        upperTotal += elem.second;
                        upperCount[elem.first] += elem.second;
                    }
            const int total = lowerTotal + upperTotal;
            const double lowerEntropy = CalculateEntropy(total, lowerCount);
            const double upperEntropy = CalculateEntropy(total, upperCount);

            const double lowerProb = (double) lowerTotal / total;
            const double upperProb = (double) upperTotal / total;

            const double infoGain = priorEntropy - (lowerEntropy * lowerProb + upperEntropy * upperProb);
            if (infoGain > bestGain)
                bestGain = infoGain;
        }

        return bestGain;
    }
}

#endif //SHAPELETGENERATION_INFORMATIONGAIN_HPP
