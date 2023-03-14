#ifndef SHAPELETGENERATION_SEQUENCEMATCHING_HPP
#define SHAPELETGENERATION_SEQUENCEMATCHING_HPP

#include <cassert>
#include "Types.hpp"

namespace ShapeletGeneration {
    constexpr double tolerance = 0.1;

    static bool ToleranceMatch(const Series &series, uint offset, const Window &window) {
        const double yOffset = series[offset] - window.at(0);

        for (uint i = 0; i < window.size(); i++)
            if (std::abs(series[i + offset] - yOffset - window.at(i)) > tolerance)
                return false;

        return true;
    }

    static double MatchFrequency(const Series &series, const Window &window) {
        assert(window.size() <= series.size());
        uint checked = 0;
        uint matchCount = 0;

        for (uint i = 0; i < series.size() - window.size(); ++i, ++checked)
            if (ToleranceMatch(series, i, window))
                ++matchCount;

        return (double) matchCount / checked;
    }
}




#endif //SHAPELETGENERATION_SEQUENCEMATCHING_HPP
