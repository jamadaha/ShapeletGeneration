#ifndef SHAPELETGENERATION_TYPES_HPP
#define SHAPELETGENERATION_TYPES_HPP

#include <array>
#include <vector>
#include <cassert>

namespace ShapeletGeneration {
    constexpr uint maxClasses = 62;
    using uint = unsigned int;
    using Series = std::vector<double>;
    struct LabelledSeries {
        const int label;
        const Series series;
        LabelledSeries(int label, const Series series) : label(label), series(series) {}
    };
    using Window = std::vector<double>;
    using Shapelet = Window;

}

#endif //SHAPELETGENERATION_TYPES_HPP
