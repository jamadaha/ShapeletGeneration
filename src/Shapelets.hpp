#ifndef SHAPELETGENERATION_SHAPELETS_HPP
#define SHAPELETGENERATION_SHAPELETS_HPP

#include <unordered_map>
#include <optional>
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "Types.hpp"
#include "InformationGain.hpp"
#include "FileHandler.h"
#include "SlidingWindows.hpp"
#include "attributes/Attribute.h"
#include "attributes/Frequency.h"

namespace ShapeletGeneration {
    static Shapelet GenerateShapelets(const std::vector<LabelledSeries> &series,
                                      const std::vector<Window> &windows) {
        printf("---Generating Shapelets---\n");
        std::vector<Attribute*> attributes {
            new Frequency(0.1),
            new Frequency(0.2),
            new Frequency(0.4),
            new Frequency(0.8),
            new Frequency(1.6),
        };

        const double priorEntropy = CalculateEntropy(series);
        std::array<uint, maxClasses> counts { 0 };
        for (const auto &s : series)
            counts[s.label]++;

        std::optional<Shapelet> bestShapelet;
        double bestScore = 0;
        Attribute *bestAttribute = nullptr;

        for (int i = 0; i < windows.size(); ++i) {
            if (i % 10000 == 0)
                printf("Checking window %d/%zu\n", i, windows.size());
            const auto& window = windows.at(i);
            for (const auto &attribute: attributes) {
                const auto eval = attribute->EvaluateWindow(priorEntropy, bestScore, counts, series, window);
                if (!bestShapelet.has_value() || eval > bestScore) {
                    bestShapelet = window;
                    bestScore = eval;
                    bestAttribute = attribute;
                }
            }
        }

        attributes.clear();
        printf("Best gain: %f\n", bestScore);
        printf("---Finish Generating Shapelets---\n");
        return bestShapelet.value();
    }
}

#endif //SHAPELETGENERATION_SHAPELETS_HPP
