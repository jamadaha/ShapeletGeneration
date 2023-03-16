#ifndef SHAPELETGENERATION_DISTANCE_H
#define SHAPELETGENERATION_DISTANCE_H

#include "Attribute.h"

namespace ShapeletGeneration {
    class Distance : public Attribute {
        const double dist; // Maximum distance between two points
    public:
        Distance(double minValue, double maxValue) : dist(std::abs(maxValue - minValue)) {}

    private:
        [[nodiscard]] double CalculateDistance(const Series &series, uint offset, const Window &window) const {
            double tempDist = 0;
            const double yOffset = series[offset];

            for (uint i = 1; i < window.size(); i++)
                tempDist += series[i + offset] - window[i] - yOffset;

            return tempDist;
        }

        [[nodiscard]] double GenerateValue(const Series &series, const Window &window) const final {
            assert(window.size() <= series.size());

            const double maxDist = dist * (double) (window.size() - 1);
            double minDist = 1;

            for (uint i = 0; i < series.size() - window.size(); ++i) {
                double tempDist = CalculateDistance(series, i, window);
                if (tempDist < minDist)
                    minDist = tempDist;
            }

            return (double) minDist / maxDist;
        }
    };
}

#endif //SHAPELETGENERATION_DISTANCE_H
