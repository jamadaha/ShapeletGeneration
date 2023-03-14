#ifndef SHAPELETGENERATION_TYPES_HPP
#define SHAPELETGENERATION_TYPES_HPP

#include <array>
#include <vector>
#include <cassert>

namespace ShapeletGeneration {
    using uint = unsigned int;
    using Series = std::vector<double>;//std::vector<double>;
    using Window = std::vector<double>;
    using Shapelet = Window;
}

template<> struct std::hash<std::vector<double>> {
    std::size_t operator()(std::vector<double> const& vec) const noexcept {
        std::size_t seed = vec.size();

        for(auto& i : vec)
            seed ^= ((std::size_t) i * 100) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};

#endif //SHAPELETGENERATION_TYPES_HPP
