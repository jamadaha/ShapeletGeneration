#ifndef SHAPELETGENERATION_SHAPELETS_HPP
#define SHAPELETGENERATION_SHAPELETS_HPP

#include <unordered_map>
#include <optional>
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include "Types.hpp"
#include "InformationGain.hpp"
#include "FileHandler.h"
#include "SlidingWindows.hpp"
#include "attributes/Attribute.h"
#include "attributes/Frequency.h"
#include "attributes/Distance.h"
#include "SeriesActions.h"

namespace ShapeletGeneration {
    struct Split {
        const Shapelet shapelet;
        const Attribute* attribute;
        const double gain;
        Split(Shapelet shapelet, Attribute *attribute, double gain) : shapelet(std::move(shapelet)), attribute(attribute), gain(gain) {};
    };

    static Split GenerateShapelets(const std::vector<LabelledSeries> &series,
                                      const std::vector<Window> &windows) {
        printf("---Generating Shapelet---\n");
        std::vector<Attribute*> attributes {
            new Frequency(0.4),
            new Distance(MinValue(series), MaxValue(series))
        };

        const double priorEntropy = CalculateEntropy(series);
        std::array<uint, maxClasses> counts = GetCount(series);

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

        printf("Best gain: %f\n", bestScore);
        printf("---Finish Generating Shapelets---\n");
        return Split(bestShapelet.value(), bestAttribute, bestScore);
    }
}

#endif //SHAPELETGENERATION_SHAPELETS_HPP
