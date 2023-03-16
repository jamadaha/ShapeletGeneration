#ifndef SHAPELETGENERATION_SERIESACTIONS_H
#define SHAPELETGENERATION_SERIESACTIONS_H

#include <numeric>
#include <cstdio>
#include "Types.hpp"

namespace ShapeletGeneration {
    static void PrintProbabilities(const std::array<uint, maxClasses> &counts) {
        const uint total = std::accumulate(counts.begin(), counts.end(), (uint) 0);
        printf("Probabilities\n");
        for (int i = 0; i < maxClasses; i++) {
            if (counts.at(i) == 0)
                continue;
            printf("Class %d - %f%%\n", i, ((double) counts[i] / total) * 100);
        }
    }

    static void PrintProbabilities(const std::array<uint, maxClasses> &counts1, const std::array<uint, maxClasses> &counts2) {
        printf("Split\n");
        for (int i = 0; i < maxClasses; i++) {
            printf("Class %d: %d/%d\n", i, counts1[i], counts2[i]);
        }
    }

    static std::array<uint, maxClasses> GetCount(const std::vector<LabelledSeries> &series) {
        std::array<uint, maxClasses> counts { 0 };
        for (const auto &s : series)
            counts[s.label]++;
        return counts;
    }
}

#endif //SHAPELETGENERATION_SERIESACTIONS_H
