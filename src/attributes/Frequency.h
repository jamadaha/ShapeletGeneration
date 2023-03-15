#ifndef SHAPELETGENERATION_FREQUENCY_H
#define SHAPELETGENERATION_FREQUENCY_H

#include "Attribute.h"

namespace ShapeletGeneration {
    class Frequency : public Attribute {
    public:
        const double tolerance;
        explicit Frequency(double tolerance) : tolerance(tolerance) {}

    private:
        [[nodiscard]] bool ToleranceMatch(const Series &series, uint offset, const Window &window) const {
            const double yOffset = series[offset] - window.at(0);

            for (uint i = 0; i < window.size(); i++)
                if (std::abs(series[i + offset] - yOffset - window.at(i)) > tolerance)
                    return false;

            return true;
        }

        [[nodiscard]] double GenerateValue(const Series &series, const Window &window) const final {
            assert(window.size() <= series.size());
            uint checked = 0;
            uint matchCount = 0;

            for (uint i = 0; i < series.size() - window.size(); ++i) {
                if (ToleranceMatch(series, i, window))
                    ++matchCount;
                ++checked;
            }

            return (double) matchCount / checked;
        }
    };
}

#endif //SHAPELETGENERATION_FREQUENCY_H
