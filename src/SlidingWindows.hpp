#ifndef SHAPELETGENERATION_SLIDINGWINDOWS_HPP
#define SHAPELETGENERATION_SLIDINGWINDOWS_HPP

#include <unordered_set>
#include "Types.hpp"

namespace ShapeletGeneration {
        static std::vector<Window> GenerateWindows(const Series &series, uint length) {
            std::vector<Window> windows;

            if (series.empty() || length == 0)
                return windows;

            for (uint i = 0; i + length <= series.size(); ++i) {
                Window window;
                const double yOffset = series[i]; // Always start a window at 0
                for (uint offset = 0; offset < length; ++offset)
                    window.push_back(series[i + offset] - yOffset);
                windows.push_back(window);
            }

            return windows;
        }

        static std::vector<Window> GenerateWindows(const std::vector<Series> &series, uint length) {
            std::vector<Window> windows;

            for (const auto &s : series) {
                auto tWindows = GenerateWindows(s, length);
                for (const auto &window : tWindows)
                    windows.push_back(window);
            }

            return windows;
        }

        static std::vector<Window> GenerateWindows(const std::vector<Series> &series, uint minLength, uint maxLength) {
            std::vector<Window> windows;

            for (uint i = minLength; i < maxLength; i++) {
                auto tWindows = GenerateWindows(series, i);
                for (const auto &window : tWindows)
                    windows.push_back(window);
            }

            return windows;
        }

        static std::unordered_map<int, std::vector<Window>>
        GenerateWindows(const std::unordered_map<int, std::vector<Series>> &series, uint minLength, uint maxLength) {
            printf("---Generating Windows---\n");
            std::unordered_map<int, std::vector<Window>> windows;
            uint totalWindows = 0;

            for (const auto &seriesSet : series) {
                const auto tempWindows = GenerateWindows(seriesSet.second, minLength, maxLength);
                totalWindows += tempWindows.size();
                windows.emplace(seriesSet.first, tempWindows);
            }

            printf("Total Windows: %d\n", totalWindows);
            printf("---Finish Generating Windows---\n");
            return windows;
        }
}


#endif //SHAPELETGENERATION_SLIDINGWINDOWS_HPP
